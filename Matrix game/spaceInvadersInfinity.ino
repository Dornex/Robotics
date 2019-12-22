#include "LedControl.h"
#include <LiquidCrystal.h>
#include <LiquidMenu.h>
#include <EEPROM.h>

#define  LCD_ROWS  2
#define  LCD_COLS  16
#define  BULLET_DESTROYED -3
#define  ENEMIE_DESTROYED -7

// Used pins
const int RS = 2;
const int enable = 5;
const int d4 = 4;
const int d5 = 7;
const int contrast = 3;
const int d6 = 8;
const int d7 = 12;
const int din = 13;
const int clk = 9;
const int load = 6;
const int buttonPin = 10;
const int joyX = A1;
const int joyY = A0;
const int joySW = 11;

//LiquidMenu library
/*
    To sum up what this library does is:
    You create LiquidLines, which are text lines that show on LCD
    You create LiquidScreens made of LiquidLines, which are different screens you can cycle through
    You create LiquidMenus made of LiquidScreens, which are different menus you can change
    You create LiquidSystem made of LiquidMenus, which contains multiple menus
    Easier said, this LiquidLines are the words in a book, LiquidScreens the pages, LiquidMenus are the books and
    LiquidSystems are the bookshelves
*/
const byte startingScreen = 1;

LedControl lc = LedControl(din, clk, load, 2);
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

LiquidLine press_line(1, 1, "Press to enter");

/*
   I'll comment this example:
   I create two lines that when put together in a Screen,
   will show the name of the game.
   In setup I will put all of these Screens in a menu, so I can
   cycle through them using the joystick.
*/
LiquidLine game_line(1, 0, "Space Invaders");
LiquidLine scroll_line(1, 1, "Infinity");
LiquidScreen start_screen(game_line, scroll_line);

LiquidLine play_line(3, 0, "Play Game");
LiquidScreen play_screen(play_line, press_line);

LiquidLine settings_line(4, 0, "Settings");
LiquidScreen settings_screen(settings_line, press_line);

LiquidLine highscore_line(3, 0, "Highscores");
LiquidScreen highscore_screen(highscore_line, press_line);

LiquidLine score_line(3, 0, "Score:");
LiquidLine level_line(0, 1, "Lives:");
LiquidLine lives_line(8, 1, "Level:");
LiquidScreen game_screen(score_line, level_line, lives_line);

LiquidLine highscore_line2(0, 0, "#1: ");
LiquidLine highscore_line3(0, 1, "#2: ");
LiquidScreen highscore_screen2(highscore_line2, highscore_line3);

/*
   This should have been the Settings and Info menu,
   but because this library is memory heavy, enabling
   these lines would result in the game breaking because of
   little memory available. I didn't know this before using this library,
   but I'll fix this once I get my hand on the project.

  LiquidLine settings_line2(0, 0, "Name:");
  LiquidLine settings_line3(0, 1, "Starting Level:");
  LiquidScreen settings_screen2(settings_line2, settings_line3);

  LiquidLine name_line(0, 0, "Dorneanu Eduard");
  LiquidLine unibuc_line(16, 0, "@UnibucRobotics");
  LiquidLine git_line(0, 1, "git:Dornex");
  LiquidLine gameName_line(12, 1, "Space Invaders Infinity");
  LiquidScreen info_screen2(name_line, git_line, gameName_line, unibuc_line);

  LiquidLine info_line(5, 0, "Info");
  LiquidScreen info_screen(info_line, press_line);
  LiquidMenu settings_menu(lcd, startingScreen);
  LiquidMenu info_menu(lcd, startingScreen);
*/

LiquidMenu menu(lcd, startingScreen);
LiquidMenu game_menu(lcd, startingScreen);
LiquidMenu highscore_menu(lcd, startingScreen);
LiquidSystem menus(1);

// Variables used

const int maxThreshold = 700, minThreshold = 300;

// delays
const int movementDelay = 100, shootDelay = 400, bulletDelay = 3, invulnDelay = 500;
const int gameOverNextScreen = 10000, refreshRate = 60, enemyCreateDelay = 1000, debounceDelay = 100;

// millis holding variables
unsigned long movementTime, shootTime, lastInvulnTime, gameOverTime, lastRefresh, enemyCreateTime, lastDebounceTime;

bool swValue, buttonValue, lastSwValue, yAxisNotMoved, xAxisNotMoved;
bool lastButtonValue, isGameOver, noDamageTakenCurrentLevel = true;

//EEPROM addresses
const int highscoreAddress1 = 0, highscoreAddress2 = 32;

int playerPos = 4, xValue, yValue, levelNumber = -1, noOfEnemies = 0;
int lives, enemyCounter, enemiesMovementSpeed;
int maxNumberOfEnemies = 6, enemiesDefeated = 0;
unsigned int score;
const int noOfBullets = 5, noOfLevels = 6;
long randNumber;
char playerName[8];

// Numbers of enemies generated each level
int level[noOfLevels] = {5, 10, 15, 20, 25, 0};

// custom LCD symbol
byte leftInf[8] = {
  B00000,
  B01100,
  B10010,
  B10001,
  B10001,
  B10010,
  B01100,
  B00000
};

byte rightInf[8] = {
  B00000,
  B00110,
  B01001,
  B10001,
  B10001,
  B01001,
  B00110,
  B00000
};

struct Enemie {
  unsigned long createdTime, movementTime;
  int posX, posY;
  bool created;
};

struct Bullet {
  unsigned long movementDelay;
  int posX;
  int posY;
} bullets[noOfBullets];

struct highscore {
  int score;
  char name[8];
};

void setup() {
  strcpy(playerName, "Unknown");

  //Used to generate randomNumbers based on the noise in Analog Pin 5
  randomSeed(analogRead(A5));

  lcd.begin(16, 2);
  lcd.createChar(0, leftInf);
  lcd.createChar(1, rightInf);
  for (int i = 0; i < lc.getDeviceCount(); i++) {
    lc.setIntensity(i, 2);
    lc.shutdown(i, false);
    lc.clearDisplay(i);
  }

  /* You can attach functions to LiquidLines,
      which I trigger when I press a button.
      Each function sends me to another menu(info/settings/highscore).

    info_line.attach_function(1, viewInfo);
    settings_line.attach_function(1, viewSettings);
  */

  play_line.attach_function(1, playGame);
  highscore_line.attach_function(1, viewHighscore);

  //  menu.add_screen(info_screen);
  //  menu.add_screen(settings_screen);
  menu.add_screen(start_screen);
  menu.add_screen(play_screen);
  menu.add_screen(highscore_screen);
  menu.update();

  //  info_menu.add_screen(info_screen2);
  //  info_menu.update();
  //  settings_menu.add_screen(settings_screen2);
  //  settings_menu.update();
  game_menu.add_screen(game_screen);
  game_menu.update();
  highscore_menu.add_screen(highscore_screen2);
  highscore_menu.update();


  //  menus.add_menu(settings_menu);
  //  menus.add_menu(info_menu);
  menus.add_menu(menu);
  menus.add_menu(game_menu);
  menus.add_menu(highscore_menu);
  menus.update();

  for (int i = 0; i < noOfBullets; i++)
    bullets[i].posX = BULLET_DESTROYED;

  pinMode(joySW, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);

  analogWrite(contrast, 90);
}

void loop() {
  navigateMenu();
}

void navigateMenu() {
  readJoystickValues();

  // Cyclying through LiqidScreens using joystick
  if (yValue < minThreshold && yAxisNotMoved) {
    menu.previous_screen();
    menu.set_focusedLine(0);
    yAxisNotMoved = 0;
  }
  else if (yValue > maxThreshold && yAxisNotMoved) {
    menu.next_screen();
    menu.set_focusedLine(0);
    yAxisNotMoved = 0;
  }
  else if (yValue < maxThreshold && yValue > minThreshold)
    yAxisNotMoved = 1;

  if ((swValue == 1 || buttonValue == 1) && (swValue != lastSwValue || buttonValue != lastButtonValue))
    lastDebounceTime = millis();

  //On button press I call the function assigned to the selected Line
  if (millis() - lastDebounceTime > debounceDelay)
    if (buttonValue != lastButtonValue || swValue != lastSwValue)
      menu.call_function(1);

  lastButtonValue = buttonValue;
  lastSwValue = swValue;
}

void viewHighscore() {
  menus.change_menu(highscore_menu);
  menus.update();

  highscore high1;
  highscore high2;
  EEPROM.get(highscoreAddress1, high1);
  EEPROM.get(highscoreAddress2, high2);

  while (1) {
    xValue = analogRead(joyX);

    //If joystick moved to left, exit
    if (xValue > maxThreshold)
      break;

    lcd.setCursor(3, 0);
    lcd.print(high1.name);
    lcd.setCursor(11, 0);
    lcd.print(high1.score);
    lcd.setCursor(3, 1);
    lcd.print(high2.name);
    lcd.setCursor(11, 1);
    lcd.print(high2.score);
  }
}

void viewInfo() {
  //  menus.change_menu(info_menu);
  menus.update();
  while (1) {
    xValue = analogRead(joyX);
    if (xValue > maxThreshold)
      break;
    lcd.scrollDisplayLeft();
    delay(250);
  }
}

//void viewSettings() {
//  menus.change_menu(settings_menu);
//  menus.update();
//  int level = 1;
//  int index = 0;
//  unsigned long blinkTime = 0, refreshLCD = 0;
//  const int blinkDelay = 500, refreshRate = 800;
//  lcd.setCursor(6, 0);
//  lcd.print(playerName);
//
//  bool selectedBlink = 1;
//  bool blinking = true;
//  while (1) {
//
//    yValue = analogRead(joyY);
//    swValue = !digitalRead(joySW);
//    buttonValue = !digitalRead(buttonPin);
//
//    if (yValue < minThreshold && yAxisNotMoved) {
//      selectedBlink = !selectedBlink;
//      yAxisNotMoved = 0;
//    }
//    else if (yValue > maxThreshold && yAxisNotMoved) {
//      selectedBlink = !selectedBlink;
//      yAxisNotMoved = 0;
//    }
//    else if (yValue < maxThreshold && yValue > minThreshold)
//      yAxisNotMoved = 1;
//
//    if (selectedBlink == 0) {
//      if (millis() - blinkTime > blinkDelay) {
//        blinkTime = millis();
//        if (blinking == 0) {
//          for (int i = 6; i < 13; i++) {
//            lcd.setCursor(i, 0);
//            lcd.print(playerName[i - 6]);
//          }
//        }
//        else {
//          for (int i = 6; i < 13; i++) {
//            lcd.setCursor(i, 0);
//            lcd.print(" ");
//          }
//        }
//        blinking = !blinking;
//      }
//    }
//    else {
//      lcd.setCursor(6, 0);
//      lcd.print(playerName);
//    }
//
//    if (selectedBlink == 1) {
//      if (millis() - blinkTime > blinkDelay) {
//        blinkTime = millis();
//        if (blinking == 0) {
//          lcd.setCursor(15, 1);
//          lcd.print(level);
//        } else {
//          lcd.setCursor(15, 1);
//          lcd.print(" ");
//        }
//      }
//      blinking = !blinking;
//    }
//    else {
//      lcd.setCursor(15, 1);
//      lcd.print(level);
//    }
//
//    if (millis() - refreshLCD > refreshRate) {
//      lcd.clear();
//      settings_menu.update();
//      refreshLCD = millis();
//    }
//
//    lastButtonValue = buttonValue;
//    lastSwValue = swValue;
//  }
//}

void readJoystickValues() {
  xValue = analogRead(joyX);
  yValue = analogRead(joyY);
  swValue = !digitalRead(joySW);
  buttonValue = !digitalRead(buttonPin);
}

void checkBoundaries() {
  if (playerPos == 0) playerPos = 1;
  else if (playerPos == 7) playerPos = 6;
}

void printGameLCD() {
  lcd.setCursor(10, 0);
  lcd.print(score);
  lcd.setCursor(6, 1);
  lcd.print(lives);
  lcd.setCursor(15, 1);
  if (levelNumber + 1 > noOfLevels) {
    lcd.setCursor(14, 1);
    lcd.write(byte(0));
    lcd.setCursor(15, 1);
    lcd.write(byte(1));
  }
  else lcd.print(levelNumber + 1);
}

void refreshLCD() {
  if (millis() - lastRefresh > refreshRate) {
    lastRefresh = millis();
    lcd.clear();
  }
}

void refreshGameLCD() {
  if (millis() - lastRefresh > refreshRate) {
    lastRefresh = millis();
    lcd.clear();
    game_menu.update();
  }
}

bool checkLevelOver(Enemie *enemies) {
  int numberOfDeadEnemies = 0;
  for (int i = 0; i < noOfEnemies; i++)
    if (enemies[i].posX == ENEMIE_DESTROYED)
      numberOfDeadEnemies++;

  if (numberOfDeadEnemies == noOfEnemies) {
    delete[] enemies;
    return 1;
  }
  return 0;
}

void showEnemies(Enemie *enemies) {
  for (int i = 0; i < noOfEnemies; i++) {
    if (enemies[i].posY == 16) {
      enemies[i].posY = 18;
      enemies[i].posX = ENEMIE_DESTROYED;
    }

    if (millis() - enemyCreateTime > enemyCreateDelay) {
      enemies[enemyCounter].created = true;
      enemyCounter++;
      enemyCreateTime = millis();
    }

    if (enemies[i].created == true && enemies[i].posX != ENEMIE_DESTROYED) {
      if (enemies[i].posY < 8) {
        lc.setLed(0, enemies[i].posY - 1, enemies[i].posX - 1, true);
        lc.setLed(0, enemies[i].posY, enemies[i].posX, true);
        lc.setLed(0, enemies[i].posY - 1, enemies[i].posX + 1, true);
      }
      else {
        lc.setLed(1, enemies[i].posY % 8 - 1, enemies[i].posX - 1, true);
        lc.setLed(1, enemies[i].posY % 8, enemies[i].posX, true);
        lc.setLed(1, enemies[i].posY % 8 - 1, enemies[i].posX + 1, true);
      }
    }
  } 
}

void updateEnemies(Enemie *enemies) {
  for (int i = 0; i < noOfEnemies; i++) {
    if (enemies[i].created == true && enemies[i].posX != ENEMIE_DESTROYED)
      if (millis() - enemies[i].movementTime > enemiesMovementSpeed) {
        if (enemies[i].posY < 8) {
          lc.setLed(0, enemies[i].posY - 1, enemies[i].posX - 1, false);
          lc.setLed(0, enemies[i].posY, enemies[i].posX, false);
          lc.setLed(0, enemies[i].posY - 1, enemies[i].posX + 1, false);
        }
        else {
          lc.setLed(1, enemies[i].posY % 8 - 1, enemies[i].posX - 1, false);
          lc.setLed(1, enemies[i].posY % 8, enemies[i].posX, false);
          lc.setLed(1, enemies[i].posY % 8 - 1, enemies[i].posX + 1, false);
        }
        enemies[i].movementTime = millis();
        enemies[i].posY++;
      }
  }
}

Enemie *createLevel() {
  levelNumber++;
  enemyCounter = 0;
  noDamageTakenCurrentLevel = true;
  enemiesMovementSpeed = (7 - levelNumber) * 25;
  
  const int levelNumberOfEnemies = level[levelNumber];
  noOfEnemies = levelNumberOfEnemies;
  Enemie *enemies = new Enemie[levelNumberOfEnemies];
  
  for (int i = 0; i < levelNumberOfEnemies; ++i) {
    enemies[i].posY = -2;
    enemies[i].posX = random(1, 6);
    enemies[i].movementTime = millis();
    enemies[i].createdTime = millis();
    enemies[i].created = false;
  }
  return enemies;
}

void updateBullets() {
  for (int i = 0; i < noOfBullets; i++)
    if (bullets[i].posX != BULLET_DESTROYED && millis() - bullets[i].movementDelay > bulletDelay) {
      if (bullets[i].posY > 7)
        lc.setLed(1, bullets[i].posY % 8, bullets[i].posX, false);
      else
        lc.setLed(0, bullets[i].posY, bullets[i].posX, false);
      bullets[i].movementDelay = millis();
      bullets[i].posY--;
    }
}

void showBullets() {

  for (int i = 0; i < noOfBullets; i++) {
    if (bullets[i].posY == 0)
      bullets[i].posX = BULLET_DESTROYED;
    if (bullets[i].posX != BULLET_DESTROYED)
      if (bullets[i].posY > 7)
        lc.setLed(1, bullets[i].posY % 8, bullets[i].posX, true);
      else
        lc.setLed(0, bullets[i].posY, bullets[i].posX, true);
  }
}

void showPlayer() {
  checkBoundaries();
  lc.setLed(1, 7, playerPos, true);
  lc.setLed(1, 7, playerPos - 1, true);
  lc.setLed(1, 7, playerPos + 1, true);
  lc.setLed(1, 6, playerPos - 1, true);
  lc.setLed(1, 6, playerPos + 1, true);

  lc.setLed(1, 7, playerPos, false);
  lc.setLed(1, 7, playerPos - 1, false);
  lc.setLed(1, 7, playerPos + 1, false);
  lc.setLed(1, 6, playerPos - 1, false);
  lc.setLed(1, 6, playerPos + 1, false);
}

void getPlayerMovement() {
  readJoystickValues();

  if (xValue < minThreshold && millis() - movementTime >= movementDelay) {
    playerPos++;
    movementTime = millis();
  }
  else if (xValue > maxThreshold && millis() - movementTime >= movementDelay) {
    playerPos--;
    movementTime = millis();
  }

  if ((swValue == 1 || buttonValue == 1) && millis() - shootTime > shootDelay) {
    playerShoot();
    shootTime = millis();
  }
}

void playerShoot() {
  Bullet bullet;
  bullet.posX = playerPos;
  bullet.posY = 15;
  bullet.movementDelay = millis();
  for (int i = 0; i < noOfBullets; i++)
    if (bullets[i].posX == BULLET_DESTROYED) {
      bullets[i] = bullet;
      break;
    }
}

void playGame() {
  Enemie *enemies = new Enemie;
  isGameOver = false;
  menus.change_menu(game_menu);
  menus.update();
  lives = 3;
  score = 0;
  levelNumber = -1;
  for (int i = 0; i < lc.getDeviceCount(); i++)
    lc.clearDisplay(i);

  while (1) {
    printGameLCD();
    refreshGameLCD();

    if (levelNumber == -1)
      enemies = createLevel();

    if (checkLevelOver(enemies)) {
      //If I finished the last level without dying
      //break and call infinityLevel()
      if (levelNumber == noOfLevels - 1)
        break;
      else {
        if (noDamageTakenCurrentLevel == true)
          score += 200;
        shop();
        enemies = createLevel();
      }
    }

    showPlayer();
    getPlayerMovement();

    updateBullets();
    showBullets();

    showEnemies(enemies);
    updateEnemies(enemies);

    checkPlayerCollision(enemies);
    checkBulletEnemiesCollision(enemies);

    if (checkGameOver()) {
      isGameOver = true;
      break;
    }
  }

  if (isGameOver == true)
    gameOver();
  else
    infinityLevel();
}

void checkFreeEnemieSpace(Enemie *enemies) {
  for (int i = 0; i < maxNumberOfEnemies; i++)
    if (enemies[i].posX == ENEMIE_DESTROYED) {
      Enemie enemie;
      enemie.posY = -2;
      enemie.posX = random(1, 6);
      enemie.movementTime = millis();
      enemie.createdTime = millis();
      enemie.created = false;
      enemies[i] = enemie;
    }
}

void infinityStory() {
  /*
     All the delays used here are for scrolling speed or for
     waiting a bit between texts. They do not harm the game in any way.
  */
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Congrats! You reached INFINITY!");
  for (int i = 0; i < 24; i++) {
    lcd.scrollDisplayLeft();
    delay(250);
  }

  delay(2000);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Everything will get harder as you play");
  for (int i = 0; i < 26; i++) {
    lcd.scrollDisplayLeft();
    delay(250);
  }

  delay(2000);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Every 50 enemies defeated => life shop");
  for (int i = 0; i < 25; i++) {
    lcd.scrollDisplayLeft();
    delay(250);
  }
  delay(2000);
  lcd.clear();
}

void infinityLevel() {
  infinityStory();
  levelNumber++;

  menus.change_menu(game_menu);
  menus.update();

  Enemie *enemies = new Enemie[maxNumberOfEnemies];
  noOfEnemies = maxNumberOfEnemies;
  enemiesDefeated = 0;

  for (int i = 0; i < maxNumberOfEnemies; ++i) {
    enemies[i].posY = -2;
    enemies[i].posX = random(1, 6);
    enemies[i].movementTime = millis();
    enemies[i].createdTime = millis();
    enemies[i].created = false;
  }

  while (1) {
    printGameLCD();
    refreshGameLCD();

    showPlayer();
    getPlayerMovement();

    updateBullets();
    showBullets();

    checkPlayerCollision(enemies);
    checkBulletEnemiesCollision(enemies);

    checkFreeEnemieSpace(enemies);
    showEnemies(enemies);
    updateEnemies(enemies);

    if (enemiesDefeated % 50 == 0)
      shop();

    if (enemyCounter == 6) 
      enemyCounter = 0;

    if (checkGameOver())
      break;
  }
  gameOver();
}

void increaseDifficulty() {
  if (enemiesDefeated % 15 == 0)
    enemiesMovementSpeed -= 5;

  if (enemiesMovementSpeed < 15)
    enemiesMovementSpeed = 15;
}

//void refreshSettingsLCD() {
//  if (millis() - lastRefresh > refreshRate) {
//    lastRefresh = millis();
//    lcd.clear();
//    settings_menu.update();
//  }
//}

void printShopLCD(int currentPos) {
  lcd.setCursor(0, 0);
  lcd.print("lives++ for 1000");
  lcd.setCursor(0, 1);
  lcd.print("Score:    Yes No");
  lcd.setCursor(6, 1);
  lcd.print(score);
  lcd.setCursor(currentPos, 1);
  lcd.print(">");
}

void shop() {
  lcd.clear();

  int yesPos = 9, noPos = 13;
  int currentPos = yesPos;

  printShopLCD(currentPos);
  enemiesDefeated++;

  // Continue only when both buttons are not pressed
  while (swValue == 1 || buttonValue == 1) {
    swValue = !digitalRead(joySW);
    buttonValue = !digitalRead(buttonPin);
  }

  while (1) {
    printShopLCD(currentPos);
    readJoystickValues();

    if (xValue < minThreshold && xAxisNotMoved) {
      if (currentPos == yesPos)
        currentPos = noPos;
      else currentPos = yesPos;
      xAxisNotMoved = 0;
    }
    else if (xValue > maxThreshold && xAxisNotMoved) {
      if (currentPos == yesPos)
        currentPos = noPos;
      else currentPos = yesPos;
      xAxisNotMoved = 0;
    }
    else if (xValue < maxThreshold && xValue > minThreshold)
      xAxisNotMoved = 1;

    if ((swValue == 1 || buttonValue == 1) && (swValue != lastSwValue || buttonValue != lastButtonValue)) {
      if (currentPos == yesPos) {
        if (score < 1000) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Not enough score!");
          lcd.setCursor(0, 1);
          lcd.print("Next level coming");
          //Show a text, wait for the player to read and continue with the game.
          delay(3000);
          break;
        }
        lives++;
        score -= 1000;
      }
      break;
    }

    lastButtonValue = buttonValue;
    lastSwValue = swValue;
  }
}

void checkBulletEnemiesCollision(Enemie *enemies) {
  for (int i = 0; i < noOfBullets; i++)
    for (int j = 0; j < noOfEnemies; j++) {
      if (bullets[i].posX != BULLET_DESTROYED && enemies[j].posX != ENEMIE_DESTROYED) {
        if ((bullets[i].posX == enemies[j].posX && bullets[i].posY == enemies[j].posY) ||
            (bullets[i].posX == enemies[j].posX - 1 && bullets[i].posY == enemies[j].posY - 1) ||
            (bullets[i].posX == enemies[j].posX + 1 && bullets[i].posY == enemies[j].posY - 1) ||
            (bullets[i].posX == enemies[j].posX && bullets[i].posY == enemies[j].posY - 1) ||
            (bullets[i].posX == enemies[j].posX - 1 && bullets[i].posY == enemies[j].posY - 2) ||
            (bullets[i].posX == enemies[j].posX + 1 && bullets[i].posY == enemies[j].posY - 2)) {
          if (bullets[i].posY > 7)
            lc.setLed(1, bullets[i].posY % 8, bullets[i].posX, false);
          else
            lc.setLed(0, bullets[i].posY, bullets[i].posX, false);

          if (enemies[j].posY < 8) {
            lc.setLed(0, enemies[j].posY - 1, enemies[j].posX - 1, false);
            lc.setLed(0, enemies[j].posY, enemies[j].posX, false);
            lc.setLed(0, enemies[j].posY - 1, enemies[j].posX + 1, false);
          }
          else {
            lc.setLed(1, enemies[j].posY % 8 - 1, enemies[j].posX - 1, false);
            lc.setLed(1, enemies[j].posY % 8, enemies[j].posX, false);
            lc.setLed(1, enemies[j].posY % 8 - 1, enemies[j].posX + 1, false);
          }
          enemies[j].posX = ENEMIE_DESTROYED;
          bullets[i].posX = BULLET_DESTROYED;
          score += random(0, (levelNumber + 2) * 15);
          enemiesDefeated++;

          if (levelNumber + 1 > noOfLevels)
            increaseDifficulty();
        }
      }
    }
}

void checkPlayerCollision(Enemie *enemies) {
  for (int i = 0; i < noOfEnemies; i++)
    if (((enemies[i].posY == 15 && enemies[i].posX == playerPos) ||
         (enemies[i].posY == 15 && enemies[i].posX == playerPos - 1) ||
         (enemies[i].posY == 15 && enemies[i].posX == playerPos + 1)) &&
        millis() - lastInvulnTime > invulnDelay) {
      lastInvulnTime = millis();
      enemies[i].posX = ENEMIE_DESTROYED;
      lives--;
      noDamageTakenCurrentLevel = false;
    }
}

bool checkGameOver() {
  if (lives == 0)
    return 1;
  return 0;
}

void gameOver() {
  lcd.clear();
  highscore high1;
  highscore high2;
  EEPROM.get(highscoreAddress1, high1);
  EEPROM.get(highscoreAddress2, high2);

  unsigned long nextScreenTime = millis();
  const int nextScreenDelay = 10000;

  //New highest score
  if (score > high1.score) {
    highscore newHigh;
    newHigh.score = score;
    strcpy(newHigh.name, playerName);
    EEPROM.put(highscoreAddress2, high1);
    EEPROM.put(highscoreAddress1, newHigh);

    lcd.clear();
    while (1) {
      buttonValue = !digitalRead(buttonPin);
      swValue = !digitalRead(joySW);

      if (millis() - nextScreenTime > nextScreenDelay)
        break;
      if ((swValue == 1 || buttonValue == 1) && (swValue != lastSwValue || buttonValue != lastButtonValue))
        break;

      lcd.setCursor(0, 0);
      lcd.print("Congrats, you");
      lcd.setCursor(0, 1);
      lcd.print("are number one!");

      lastSwValue = swValue;
      lastButtonValue = buttonValue;
    }
  }
  //Second highest score
  else if (score > high2.score) {
    highscore newHigh;
    newHigh.score = score;
    strcpy(newHigh.name, playerName);
    EEPROM.put(highscoreAddress2, newHigh);
    lcd.clear();

    while (1) {
      buttonValue = !digitalRead(buttonPin);
      swValue = !digitalRead(joySW);

      if (millis() - nextScreenTime > nextScreenDelay)
        break;
      if ((swValue == 1 || buttonValue == 1) && (swValue != lastSwValue || buttonValue != lastButtonValue))
        break;

      lcd.setCursor(0, 0);
      lcd.print("Congrats, you");
      lcd.setCursor(0, 1);
      lcd.print("are number two!");

      lastSwValue = swValue;
      lastButtonValue = buttonValue;
    }
  }
  else {
    lcd.clear();
    while (1) {
      buttonValue = !digitalRead(buttonPin);
      swValue = !digitalRead(joySW);

      if (millis() - nextScreenTime > nextScreenDelay)
        break;
      if ((swValue == 1 || buttonValue == 1) && (swValue != lastSwValue || buttonValue != lastButtonValue))
        break;

      lcd.setCursor(0, 0);
      lcd.print("Game over, wanna");
      lcd.setCursor(0, 1);
      lcd.print("try again?");

      lastSwValue = swValue;
      lastButtonValue = buttonValue;
    }
  }

  int currentPos = 0;
  int tryPos = 0;
  int backPos = 11;
  lcd.clear();
  while (1) {
    xValue = analogRead(joyX);
    buttonValue = !digitalRead(buttonPin);
    swValue = !digitalRead(joySW);

    if (xValue < minThreshold && xAxisNotMoved) {
      if (currentPos == tryPos)
        currentPos = backPos;
      else currentPos = tryPos;
      xAxisNotMoved = 0;
    }
    else if (xValue > maxThreshold && xAxisNotMoved) {
      if (currentPos == tryPos)
        currentPos = backPos;
      else currentPos = tryPos;
      xAxisNotMoved = 0;
    }
    else if (xValue < maxThreshold && xValue > minThreshold)
      xAxisNotMoved = 1;
    if ((swValue == 1 || buttonValue == 1) && (swValue != lastSwValue || buttonValue != lastButtonValue)) {
      if (currentPos == tryPos)
        playGame();
      else
        break;
    }

    lastButtonValue = buttonValue;
    lastSwValue = swValue;

    lcd.setCursor(3, 0);
    lcd.print("Score:");
    lcd.setCursor(9, 0);
    lcd.print(score);
    lcd.setCursor(1, 1);
    lcd.print("Play again");
    lcd.setCursor(12, 1);
    lcd.print("Back");
    lcd.setCursor(currentPos, 1);
    lcd.print(">");
    refreshLCD();
  }
  navigateMenu();
}

