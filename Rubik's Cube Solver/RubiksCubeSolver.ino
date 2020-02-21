#include <Servo.h>

#define LOCKED_CUBE_ANGLE 153
#define PUSH_FIRST_PHASE 120
#define LATERAL_PUSH_FIRST_PHASE 131
#define PUSH_SECOND_PHASE 135
#define PUSH_START 170
#define PUSH_END 175

Servo armServo;
Servo cubeServo;

const int cubePin = 11;
const int armPin = 6;
int cubeAngle = 90;
String response = "";
bool ok = true; 
char terminator = '.';

void setup() {
  cubeServo.attach(cubePin);
  armServo.attach(armPin);
  cubeServo.write(cubeAngle);
  armServo.write(175);
  delay(3000);
  Serial.begin(9600);
  while(!Serial);
}

void loop() {
  if(Serial.available()) {
    delay(500);
    String incomingMessage = Serial.readStringUntil(terminator); 
    if(incomingMessage.compareTo("push") == 0) {
      decidePushModeAndPush(1);
      Serial.println("next");
    }
    else if(incomingMessage.compareTo("rotateLeft") == 0) {
      rotateToLeft();
      Serial.println("next");
    }
    else if(incomingMessage.compareTo("rotateRight") == 0) {
      rotateToRight();
      Serial.println("next");
    }
    else if(incomingMessage.compareTo("center") == 0) {
      rotateToCenter();
      Serial.println("next");
    }
    else if(incomingMessage.compareTo("solve") == 0) {
      back();
    }
    else if(validMove(incomingMessage)){
      decideMove(incomingMessage);
      Serial.println("done");
    }
  }
}

bool validMove(String incomingMessage) {
  return ((incomingMessage.compareTo("R") == 0) or
         (incomingMessage.compareTo("Rr") == 0) or
         (incomingMessage.compareTo("R2") == 0) or
         (incomingMessage.compareTo("L") == 0) or
         (incomingMessage.compareTo("Lr") == 0) or
         (incomingMessage.compareTo("L2") == 0) or
         (incomingMessage.compareTo("F") == 0) or
         (incomingMessage.compareTo("Fr") == 0) or
         (incomingMessage.compareTo("F2") == 0) or
         (incomingMessage.compareTo("B") == 0) or
         (incomingMessage.compareTo("Br") == 0) or
         (incomingMessage.compareTo("B2") == 0) or
         (incomingMessage.compareTo("U") == 0) or
         (incomingMessage.compareTo("Ur") == 0) or
         (incomingMessage.compareTo("U2") == 0) or
         (incomingMessage.compareTo("D") == 0) or
         (incomingMessage.compareTo("Dr") == 0) or
         (incomingMessage.compareTo("D2") == 0));
}

void pushCubeLateral(int firstAngle = LATERAL_PUSH_FIRST_PHASE) {
  for(int i = PUSH_START; i >= firstAngle; i--) {
    armServo.write(i);
    delay(8);
  }
  delay(200);
  for(int i = PUSH_FIRST_PHASE; i <= PUSH_END; i++) {
    armServo.write(i);
    delay(10);
  }
}

void clearSerialBuffer() {
  while(Serial.available() > 0)
    Serial.read();
}

void rotateToCenter() {
  cubeServo.write(90);
  cubeAngle = 90;
  delay(500);
}

void decideMove(String move) {
  if(!move.compareTo("L"))
    left();
  else if(!move.compareTo("Lr"))
    leftRev();
  else if(!move.compareTo("L2"))
    leftTwice();
  else if(!move.compareTo("R"))
    right();
  else if(!move.compareTo("Rr"))
    rightRev();
  else if(!move.compareTo("R2"))
    rightTwice();
  else if(!move.compareTo("F"))
    front();
  else if(!move.compareTo("Fr"))
    frontRev();
  else if(!move.compareTo("F2"))
    frontTwice();
  else if(!move.compareTo("B"))
    back(); 
  else if(!move.compareTo("Br"))
    backRev();
  else if(!move.compareTo("B2"))
    backTwice();
  else if(!move.compareTo("U"))
    top();
  else if(!move.compareTo("Ur"))
    topRev();
  else if(!move.compareTo("U2"))
    topTwice();
  else if(!move.compareTo("D"))
    bottom();
  else if(!move.compareTo("Dr"))
    bottomRev();
  else if(!move.compareTo("D2"))
    bottomTwice();
}

void decidePushModeAndPush(int numberOfPushes) {
  if(cubeAngle == 180 || cubeAngle == 0)
    multiplePushCube(numberOfPushes, LATERAL_PUSH_FIRST_PHASE);
  else 
    multiplePushCube(numberOfPushes, PUSH_FIRST_PHASE);
}

void front() {
  decidePushModeAndPush(3);
  rotateCubeCounterClockwise();  
}

void frontRev() {
  decidePushModeAndPush(3);
  rotateCubeClockwise();
}

void frontTwice() {
  decidePushModeAndPush(3);
  rotateCubeCounterClockwise();
  rotateCubeCounterClockwise();
}

void top() {
  decidePushModeAndPush(2);
  rotateCubeCounterClockwise();
}

void topRev() {
  decidePushModeAndPush(2);
  rotateCubeClockwise();
}

void topTwice() {
  decidePushModeAndPush(2);
  rotateCubeCounterClockwise();
  rotateCubeCounterClockwise();
}

void back() {
  decidePushModeAndPush(1);
  rotateCubeCounterClockwise();
}

void backRev() {
  decidePushModeAndPush(1);
  rotateCubeClockwise();
}

void backTwice() {
  decidePushModeAndPush(1);
  rotateCubeCounterClockwise();
  rotateCubeCounterClockwise();
}

void bottom() {
  rotateCubeCounterClockwise();
}

void bottomRev() {
  rotateCubeClockwise();
}

void bottomTwice() {
  rotateCubeCounterClockwise();
  rotateCubeCounterClockwise();
}

void left() {
  rotateToLeft();
  back();
}

void leftRev() {
  rotateToLeft();
  backRev();
}

void leftTwice() {
  rotateToLeft();
  backTwice();
}

void right() {
  rotateToRight();
  back();
}

void rightRev() {
  rotateToRight();
  backRev();
}

void rightTwice() {
  rotateToRight();
  backTwice();
}

void rotateToRight() {
  cubeServo.write(180);
  cubeAngle = 180;
  delay(1000);
}

void rotateToLeft() {
  cubeServo.write(0);
  cubeAngle = 0;
  delay(1000);
}

void lockCube() {
  for(int i = 175; i >= LOCKED_CUBE_ANGLE; i--) {
    armServo.write(i);
    delay(10);
  }
}

void multiplePushCube(int numberOfPushes, int firstAngle) {
  for(int i = 0; i < numberOfPushes; i++) {
    if(firstAngle == LATERAL_PUSH_FIRST_PHASE)
      pushCubeLateral(firstAngle);
    else
      pushCube(firstAngle);
    delay(700);
  }
}

void pushCube(int firstAngle) {
  for(int i = PUSH_START; i >= firstAngle; i--) {
    armServo.write(i);
    delay(8);
  }
  delay(350);
  for(int i = PUSH_FIRST_PHASE; i <= PUSH_END; i++) {
    armServo.write(i);
    delay(20);
  }
}

void rotateCubeClockwise() {
    resetArm();
    for(int i = 0; i <= 175; i++) {
      cubeServo.write(i);
      delay(2);
    }   
    cubeAngle = 175;
    delay(200);
    lockCube();
    delay(200);
    for(int i = cubeAngle; i >= 82; i--) {
      cubeServo.write(i);
      delay(5);
    }  
    cubeAngle = 85;
    delay(400);
    resetArm();   
    cubeServo.write(90);
    cubeAngle = 90;
    delay(300);
}

void rotateCubeCounterClockwise() {
    resetArm();
    for(int i = 175; i >= 0; i--) {
      cubeServo.write(i);
      delay(2);
    }
    cubeAngle = 0;
    delay(200);
    lockCube();
    delay(200);
    for(int i = cubeAngle; i <= 102; i++) {
      cubeServo.write(i);
      delay(5);
    }
    cubeAngle = 102;
    delay(400);
    resetArm();  
    cubeServo.write(90);
    cubeAngle = 90;
    delay(300);
}

void resetArm() {
  armServo.write(175);
  delay(300);
}
