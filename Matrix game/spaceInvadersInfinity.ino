#include "LedControl.h"
#include <LiquidCrystal.h>
#include <LiquidMenu.h>

LiquidLine press_line(1, 1, "Press to enter");

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


#define  LCD_ROWS  2
#define  LCD_COLS  16
const byte startingScreen = 1;

// All used pins
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

// Variables used
const int maxThreshold = 700, minThreshold = 300;
int playerPos = 4;
int xValue, yValue;
const int movementDelay = 200, shootDelay = 400, bulletDelay = 100, invulnDelay = 6000, gameOverNextScreen = 10000, refreshRate = 60;
unsigned long movementTime, shootTime, lastInvulnTime, gameOverTime, lastRefresh;
byte swValue, buttonValue, lastSwValue, yAxisNotMoved;
const int noOfBullets = 10, noOfLevels = 5;
int levelNumber = -1;
int numberOfEnemies = 0, noOfEnemies = 0;
long randNumber;
int lives;
int score;

struct Enemie {
  unsigned long createdTime, movementTime;
  int movementSpeed;
  int posX, posY;
  int spawnTime;
  bool spawned = false;
}; 

// Numbers of enemies generated each level
int level[noOfLevels] = {8, 14, 20, 27, 35}; 

struct Bullet {
  int posX;
  int posY;
  int movementDelay;
  bool spawned;
} bullets[noOfBullets];

LedControl lc = LedControl(din, clk, load, 2);
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);
LiquidMenu menu(lcd, startingScreen);
LiquidMenu game_menu(lcd, startingScreen);
LiquidSystem menus(1);

void setup() {
  randomSeed(analogRead(A5));
  lcd.begin(16, 2);

  play_line.attach_function(1, playGame);
  
  menu.add_screen(start_screen);
  menu.add_screen(play_screen);
  menu.add_screen(settings_screen);
  menu.add_screen(highscore_screen);
  menu.update();

  game_menu.add_screen(game_screen);

  menus.add_menu(menu);
  menus.add_menu(game_menu);
  menus.update();
 
  for(int i = 0; i < noOfBullets; i++)
    bullets[i].spawned = false;
    
  pinMode(joySW, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);
  
  for(int i = 0; i < lc.getDeviceCount(); i++) {
    lc.setIntensity(i, 2);
    lc.shutdown(i, false);
    lc.clearDisplay(i);
  }
  
  analogWrite(contrast, 90);
}

void loop() {
  navigateMenu();
}

void navigateMenu() {
  xValue = analogRead(joyX);
  yValue = analogRead(joyY);
  swValue = !digitalRead(joySW);
  buttonValue = digitalRead(buttonPin);
 
  if(yValue < minThreshold && yAxisNotMoved) {
    menu.previous_screen();
    menu.switch_focus();
    yAxisNotMoved = 0;
  }  
  else if(yValue > maxThreshold && yAxisNotMoved) {
    menu.next_screen();
    menu.switch_focus();
    yAxisNotMoved = 0;
  }
  else if(yValue < maxThreshold && yValue > minThreshold)
    yAxisNotMoved = 1;

  if(swValue == 1 && swValue != lastSwValue) {
    menu.call_function(1); 
  }
  
  lastSwValue = swValue;
  
}

void checkBoundaries() {
  if(playerPos == 0) playerPos = 1;
  else if(playerPos == 7) playerPos = 6;
}

void playGame() {
  Enemie *enemies = new Enemie;
  menus.change_menu(game_menu);
  menus.update();
  lives = 3;
  score = 0;
  
  while(1) {
    lcd.setCursor(10, 0);
    lcd.print(score);
    lcd.setCursor(6, 1);
    lcd.print(lives);
    lcd.setCursor(15, 1);
    lcd.print(levelNumber);
    lcd.setCursor(0, 0);
    lcd.print(numberOfEnemies);

    if(millis() - lastRefresh > refreshRate) {
      lastRefresh = millis();
      lcd.clear();
      game_menu.update();
    }
    if(numberOfEnemies == 0)
       enemies = createLevel();
    showPlayer();
    getPlayerMovement();
    updateBullets();
    showBullets();
    showEnemies(enemies);
    updateEnemies(enemies);
    checkPlayerCollision(enemies);
    checkBulletEnemiesCollision(enemies);
    
    if(checkGameOver()) {
      gameOverTime = millis();
      break;
    }   
  } 
  gameOver();
}

void checkBulletEnemiesCollision(Enemie *enemies) {
  for(int i = 0; i < noOfBullets; i++) 
    for(int j = 0; j < noOfEnemies; j++) {
      if(bullets[i].spawned = true && enemies[j].spawned == true) {
        if((bullets[i].posX == enemies[j].posX && bullets[i].posY == enemies[j].posY) || 
            (bullets[i].posX == enemies[j].posX - 1 && bullets[i].posY == enemies[j].posY - 1) ||
            (bullets[i].posX == enemies[j].posX + 1 && bullets[i].posY == enemies[j].posY - 1)) {
              if(bullets[i].posY > 7)
                lc.setLed(1, bullets[i].posY % 8, bullets[i].posX, false);
              else 
                lc.setLed(0, bullets[i].posY, bullets[i].posX, false);
                
              if(enemies[j].posY < 8) {
                lc.setLed(0, enemies[j].posY - 1, enemies[j].posX - 1, false);
                lc.setLed(0, enemies[j].posY, enemies[j].posX, false);
                lc.setLed(0, enemies[j].posY - 1, enemies[j].posX + 1, false);
              }
              else {
                lc.setLed(1, enemies[j].posY % 8 - 1, enemies[j].posX - 1, false);
                lc.setLed(1, enemies[j].posY % 8, enemies[j].posX, false);
                lc.setLed(1, enemies[j].posY % 8 - 1, enemies[j].posX + 1, false);
              }
              enemies[j].spawned = false;
              bullets[i].spawned = false;
              score += levelNumber * 10;
              numberOfEnemies--;
            }
      }
    }
}

void checkPlayerCollision(Enemie *enemies) {
  for(int i = 0; i < noOfEnemies; i++) {
    if(((enemies[i].posY == 15 && enemies[i].posX == playerPos) ||
       (enemies[i].posY == 15 && enemies[i].posX == playerPos - 1) ||
       (enemies[i].posY == 15 && enemies[i].posX == playerPos + 1)) && 
       millis() - lastInvulnTime > invulnDelay) {
          lastInvulnTime = millis();
          enemies[i].posX = -1;
          lives--;
       }   
  }
}

bool checkGameOver() {
  if(lives == 0)
    return 1;
  return 0;
}

void gameOver() {
  lcd.clear();
  while(1) {
    lcd.setCursor(0, 0);
    lcd.print("Game Over!");
  }
}

void showEnemies(Enemie *enemies) {
  for(int i = 0; i < noOfEnemies; i++) {
    if(enemies[i].posY == 16)
      enemies[i].spawned = false;
        
    if(millis() - enemies[i].createdTime > enemies[i].spawnTime && enemies[i].spawned == true) {
      enemies[i].spawned = true;
      if(enemies[i].posY < 8) {
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
  for(int i = 0; i < noOfEnemies; i++) {
    if(millis() - enemies[i].createdTime > enemies[i].spawnTime) {
      if(millis() - enemies[i].movementTime > enemies[i].movementSpeed && enemies[i].spawned == true) {
        if(enemies[i].posY < 8) {
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
}

Enemie *createLevel() {
  levelNumber++;
  const int levelNumberOfEnemies = level[levelNumber];
  
  numberOfEnemies = levelNumberOfEnemies;
  noOfEnemies = levelNumberOfEnemies;
  
  Enemie *enemies = new Enemie[levelNumberOfEnemies];
  for(int i = 0; i < levelNumberOfEnemies; ++i) {
    enemies[i].posY = 0;
    enemies[i].posX = random(1, 6);
    enemies[i].movementTime = millis();
    enemies[i].movementSpeed = 200;
    enemies[i].spawnTime = random(400, 4000);
    enemies[i].createdTime = millis();
    enemies[i].spawned = false;
  }
  return enemies;
}

void updateBullets() {
  for(int i = 0; i < noOfBullets; i++) {
    if(bullets[i].spawned == true && millis() - bullets[i].movementDelay > bulletDelay) {
      if(bullets[i].posY > 7)
        lc.setLed(1, bullets[i].posY % 8, bullets[i].posX, false);
      else 
        lc.setLed(0, bullets[i].posY, bullets[i].posX, false);
      bullets[i].movementDelay = millis();
      bullets[i].posY--; 
    }
  }
}

void showBullets() {
  for(int i = 0; i < noOfBullets; i++) {
    if(bullets[i].posY == -1)
      bullets[i].spawned = false;
    if(bullets[i].spawned == true)
      if(bullets[i].posY > 7)
        lc.setLed(1, bullets[i].posY % 8, bullets[i].posX, true);
      else lc.setLed(0, bullets[i].posY, bullets[i].posX, true);
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
  xValue = analogRead(joyX);
  yValue = analogRead(joyY);
  swValue = !digitalRead(joySW);
  buttonValue = digitalRead(buttonPin);

  if(xValue < minThreshold && millis() - movementTime >= movementDelay) {
    playerPos++;
    movementTime = millis();
  }  
  else if(xValue > maxThreshold && millis() - movementTime >= movementDelay) {
    playerPos--;
    movementTime = millis();
  }

  if(swValue == 1 && millis() - shootTime > shootDelay) {
    playerShoot();
    shootTime = millis();
  }
}

void playerShoot() {
  Bullet bullet;
  bullet.posX = playerPos;
  bullet.posY = 15;
  bullet.spawned = true;
  bullet.movementDelay = millis();
  for(int i = 0; i < noOfBullets; i++)
    if(bullets[i].spawned == false) {
      bullets[i] = bullet;
      break;
    }
}
