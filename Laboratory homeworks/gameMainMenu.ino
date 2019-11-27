#include <LiquidCrystal.h>
#include <EEPROM.h>
const int RS = 12;
const int enable = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;
const int pinSW = 8;
const int pinJoyX = A0;
const int pinJoyY = A1;
const int minThreshold = 300, maxThreshold = 700, clearDelay = 500;
const int printDelay = 500, gameOverDelay = 10000, increaseScoreDelay = 5000, noOfOptions = 4;
unsigned long clearTime, gameOverTimer, settingsTimer, highscoreTimer, increaseScoreTimer;
int highscoreAddr = 0;
int menuIndex, valueX, valueY, startingLevelValue = 1, highscore = 0;
byte buttonValue, lastButtonValue, joyNotMoved, inSubmenu, buttonPressed, inGameOver, levelChanged;
String menuOptions[noOfOptions] = {String("Start"), String("Settings"), String("Highscore"), String("Exit")};
int signPosition[noOfOptions] = {0, 7, 16, 27};

LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

void setup() {
  pinMode(pinSW, INPUT_PULLUP);
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop() {
  buttonValue = !digitalRead(pinSW);
  valueX = analogRead(pinJoyX);
  valueY = analogRead(pinJoyY);

  if(inSubmenu == false)
    showMainMenu();

  if(checkButtonPress() && inSubmenu == false) {
    lcd.clear();
    buttonPressed = !buttonPressed;
    gameOverTimer = millis();
    settingsTimer = millis();
    highscoreTimer = millis();
    increaseScoreTimer = millis();
  }

  if(buttonPressed == false)
    navigateMainMenu();

  if(buttonPressed == true)
    showSelectedMenu(menuIndex);
  
  clearScreen();
  lastButtonValue = buttonValue;
}

void showMainMenu() {
  lcd.setCursor(1, 0);
  lcd.print(menuOptions[0]);
  lcd.setCursor(8, 0);
  lcd.print(menuOptions[1]);
  lcd.setCursor(1, 1);
  lcd.print(menuOptions[2]);
  lcd.setCursor(12, 1);
  lcd.print(menuOptions[3]);
}

void navigateMainMenu() {
  if(signPosition[menuIndex] < 16 && buttonPressed == false) {
    lcd.setCursor(signPosition[menuIndex], 0);
    lcd.print(">");
  }
  else if(signPosition[menuIndex] >= 16 && buttonPressed == false){
    lcd.setCursor(signPosition[menuIndex] % 16, 1);
    lcd.print(">");
  }

  joystickMovedUpMenu(); 
  joystickMovedDownMenu();
    
}

void showSelectedMenu(int index) {
  inSubmenu = true;
  if(index == 0)
    showStart();
  else if(index == 1)
    showSettings();
  else if(index == 2)
    showHighscore();
  else
    showExit();
}

void showStart() {
  if(inGameOver == false) {
    lcd.setCursor(1, 0);
    lcd.print("Lives:3");
    lcd.setCursor(9, 0);
    lcd.print(String("Level:" + String(startingLevelValue)));
    lcd.setCursor(1, 1);
    lcd.print(String("Score:" + String(startingLevelValue * 3)));
  }
 
  if(millis() - gameOverTimer > gameOverDelay) {
    int score;
    EEPROM.get(highscoreAddr, score);
    Serial.println(score);
    if(score < startingLevelValue * 3) {
      EEPROM.put(highscoreAddr, startingLevelValue * 3);
    }
    showGameOver();
  }

  if(millis() - increaseScoreTimer > increaseScoreDelay) {
    startingLevelValue++;
    increaseScoreTimer = millis();
  }
}

void showGameOver() {
  inGameOver = true;
  lcd.setCursor(0, 0);
  lcd.print("Game Over!");
  lcd.setCursor(0, 1);
  lcd.print("Press button!");

  if(checkButtonPress()) {
    inGameOver = false;
    inSubmenu = false;
    buttonPressed = false;
  }
}

void showSettings() {
  lcd.setCursor(1, 0);
  lcd.print("Level:" + String(startingLevelValue));
  lcd.setCursor(1, 1);
  lcd.print("Name: Eduardo");
  lcd.setCursor(11, 0);
  lcd.print(">Exit");
  joystickMovedUpSettings();
  joystickMovedDownSettings();

  if(checkButtonPress() && millis() - settingsTimer > 50) {
    lcd.clear();
    levelChanged = false;
    inSubmenu = false;
    buttonPressed = false;
  }
}

void showHighscore() {
  lcd.setCursor(0, 0);
  lcd.print("Highscore");
  lcd.setCursor(0, 1);
  int score;
  EEPROM.get(highscoreAddr, score);
  lcd.print("Eduardo: " + String(score));
  lcd.setCursor(11, 0);
  lcd.print(">Exit");

  if(checkButtonPress() && millis() - highscoreTimer > 50) {
    lcd.clear();
    inSubmenu = false;
    buttonPressed = false;
  }
}

void showExit() {
  lcd.setCursor(0, 0);
  lcd.print("Thank you for");
  lcd.setCursor(0, 1);
  lcd.print("playing! :)");
}

void joystickMovedUpSettings() {
  if(valueY > maxThreshold && joyNotMoved) {
    joyNotMoved = false;
    lcd.clear();
    startingLevelValue++;
  }
  else if(valueY < maxThreshold && valueY > minThreshold) 
    joyNotMoved = true;
}

void joystickMovedDownSettings() {
  if(valueY < minThreshold && joyNotMoved) {
    joyNotMoved = false;
    lcd.clear();
    if(startingLevelValue - 1 < 1) startingLevelValue = 1;
    else startingLevelValue --;
  }
  else if(valueY < maxThreshold && valueY > minThreshold) 
    joyNotMoved = true;
}

void joystickMovedUpMenu() {
  if(valueY > maxThreshold && joyNotMoved) {
    joyNotMoved = false;
    lcd.clear();
    if(menuIndex + 1 == 4) menuIndex = 0;
    else menuIndex++;
  }
  else if(valueY < maxThreshold && valueY > minThreshold) 
    joyNotMoved = true;
}

void joystickMovedDownMenu() {
  if(valueY < minThreshold && joyNotMoved) {
    joyNotMoved = false;
    lcd.clear();
    if(menuIndex - 1 == -1) menuIndex = 3;
    else menuIndex--;
  }
  else if(valueY < maxThreshold && valueY > minThreshold) 
    joyNotMoved = true;
}

void clearScreen() {
  if(millis() - clearTime > clearDelay) {
    clearTime = millis();
    lcd.clear();
  }
}

byte checkButtonPress() {
  if(buttonValue != lastButtonValue && buttonValue == 1)
    return 1;
  else
    return 0;
}
