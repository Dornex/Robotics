#include "LedControl.h"

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

LedControl lc = LedControl(din, clk, load, 2);

const int joyX = A1;
const int joyY = A0;
const int joySW = 11;
const int maxThreshold = 700, minThreshold = 300;
int playerPos = 4;
int xValue, yValue;
const int movementDelay = 400, shootDelay = 400, bulletDelay = 200;
unsigned long movementTime, shootTime;
byte swValue, buttonValue;
const int noOfBullets = 10, noOfLevels = 5;
int levelNumber = -1;
int numberOfEnemies = 0;
long randNumber;

struct Enemie {
  int movementSpeed, shootSpeed;
  int posX, posY;
  int spawnTime;
  unsigned long createdTime, movementTime, shootTime;
}; 

int level[noOfLevels] = {8, 14, 20, 27, 35}; 

struct Bullet {
  int posX;
  int posY;
  int movementDelay;
} bullets[noOfBullets];

void setup() {
  //bullet with posX = -1 doesn't "exist"
  for(int i = 0; i < noOfBullets; i++)
    bullets[i].posX = -1;
    
  randomSeed(analogRead(A5));
  Serial.begin(9600);
  pinMode(joySW, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);
  
  lc.clearDisplay(0);// clear screen
  for(int i = 0; i < lc.getDeviceCount(); i++) {
    lc.setIntensity(i, 2);
    lc.shutdown(i, false);
  }
          
}

void loop() {
  playGame();
}

void checkBoundaries() {
  if(playerPos == 0) playerPos = 1;
  else if(playerPos == 7) playerPos = 6;
}

void playGame() {
  Enemie *enemies = new Enemie;
  while(1) {
    if(numberOfEnemies == 0)
       enemies = createLevel();
    showPlayer();
    getPlayerMovement();
    updateBullets();
    showBullets();
    showEnemies(enemies);
    updateEnemies(enemies);
  }
}

void showEnemies(Enemie *enemies) {
  for(int i = 0; i < numberOfEnemies; i++) {
    if(enemies[i].posY == 16)
        enemies[i].posX = -1;
    if(millis() - enemies[i].createdTime > enemies[i].spawnTime && enemies[i].posX != -1)
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

void updateEnemies(Enemie *enemies) {
  for(int i = 0; i < numberOfEnemies; i++) {
    if(millis() - enemies[i].createdTime > enemies[i].spawnTime) {
      if(millis() - enemies[i].movementTime > enemies[i].movementSpeed) {
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
  const int noOfEnemies = level[levelNumber];
  numberOfEnemies = noOfEnemies;
  Enemie *enemies = new Enemie[noOfEnemies];
  for(int i = 0; i < noOfEnemies; ++i) {
    enemies[i].posY = 0;
    enemies[i].posX = random(1, 6);
    enemies[i].movementSpeed = 700;
    enemies[i].shootSpeed = levelNumber * 200;
    enemies[i].spawnTime = random(400, 9000);
    enemies[i].movementTime = 0;
    enemies[i].shootTime = 0;
    enemies[i].createdTime = millis();
  }
  for(int i = 0; i < noOfEnemies; ++i) {
    for(int j = 0; j < noOfEnemies; ++j) {
      if(abs(enemies[i].spawnTime - enemies[j].spawnTime) < 2000 && i != j) 
        enemies[j].spawnTime += 2000; 
    }
  }
  return enemies;
}

void updateBullets() {
  for(int i = 0; i < noOfBullets; i++) {
    if(bullets[i].posX != -1 && millis() - bullets[i].movementDelay > bulletDelay) {
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
      bullets[i].posX = -1;
    if(bullets[i].posX != -1)
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
  buttonValue = !digitalRead(buttonPin);

  if(xValue < minThreshold && millis() - movementTime >= movementDelay) {
    playerPos++;
    movementTime = millis();
  }  
  else if(xValue > maxThreshold && millis() - movementTime >= movementDelay) {
    playerPos--;
    movementTime = millis();
  }

  if((swValue == 1 || buttonValue == 1) && millis() - shootTime > shootDelay) {
    playerShoot();
    shootTime = millis();
  }
}

void playerShoot() {
  Bullet bullet;
  bullet.posX = playerPos;
  bullet.posY = 15;
  bullet.movementDelay = millis();
  for(int i = 0; i < noOfBullets; i++)
    if(bullets[i].posX == -1) {
      bullets[i] = bullet;
      break;
    }
}
