const int pinA = 12;
const int pinB = 8;
const int pinC = 5;
const int pinD = 3;
const int pinE = 2;
const int pinF = 11;
const int pinG = 6;
const int pinDP = A5;
const int pinD1 = 7;
const int pinD2 = 9;
const int pinD3 = 10;
const int pinD4 = 13;
const int pinX = A1;
const int pinY = A0;
const int pinSW = 4;
const int segSize = 8, noOfDisplays = 4, noOfDigits = 10;
const int minThreshold = 300, maxThreshold = 700;

int lastSwitchValue;
int switchValue, xValue, yValue;
int currentNumber = 0, currentSelectedDigit;

byte xAxisTilted, yAxisTilted, digitNotSelected = true;

int number[noOfDisplays] = {0, 0, 0, 0};

int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

int digits[noOfDisplays] = {
  pinD1, pinD2, pinD3, pinD4
 };   
 
byte digitMatrix[noOfDigits][segSize - 1] = {
// a  b  c  d  e  f  g
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

void displayDigit(byte digit, byte decimalPoint) {
  digitalWrite(segments[segSize - 1], decimalPoint);
  
  for (int i = 0; i < segSize - 1; i++) {
   digitalWrite(segments[i], digitMatrix[digit][i]);
  }  
}

void showDigit(int num) {
  for (int i = 0; i < noOfDisplays; i++) {
    digitalWrite(digits[i], HIGH);
  }
  
  digitalWrite(digits[num], LOW);
}

void setup() {
  pinMode(pinDP, OUTPUT);   
  pinMode(pinSW, INPUT_PULLUP);
  for (int i = 0; i < segSize - 1; i++) {
    pinMode(segments[i], OUTPUT);  
  }
  for (int i = 0; i < noOfDisplays; i++) {
    pinMode(digits[i], OUTPUT);  
  }
  Serial.begin(9600);
}

void loop() {
  int digit, lastDigit;
  switchValue = !digitalRead(pinSW);
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  joystickControl(xValue, yValue, switchValue, lastSwitchValue);
  displayNumber(); 
  
  lastSwitchValue = switchValue;
}

void displayNumber() {
  for(int i = 0; i < 4; i++) {
    showDigit(i);
    if(i != currentSelectedDigit)
      displayDigit(number[i], LOW); 
    else
      displayDigit(number[i], HIGH); 
    delay(5);
  }
}

void joystickControl(int xValue, int yValue, int switchValue, int lastSwitchValue) {
  if(switchValue != lastSwitchValue && switchValue == 1) 
    digitNotSelected = !digitNotSelected;  
    
  if(digitNotSelected == true) {
    if(xValue < minThreshold && xAxisTilted == false) {
      if(currentSelectedDigit == 3) currentSelectedDigit = 0;
      else currentSelectedDigit++;
      xAxisTilted = true;
    }
    else if(xValue > maxThreshold && xAxisTilted == false) {
      if(currentSelectedDigit == 0) currentSelectedDigit = 3;
      else currentSelectedDigit--;
      xAxisTilted = true;
    }
    else if(xValue < maxThreshold && xValue > minThreshold)
      xAxisTilted = false;
  }
  else {
    currentNumber = number[currentSelectedDigit];
    
    if(yValue > maxThreshold && yAxisTilted == false) {
      if(currentNumber == 9) currentNumber = 0;
      else currentNumber++;
      yAxisTilted = true;
    }
    else if(yValue < minThreshold && yAxisTilted == false) {
      if(currentNumber == 0) currentNumber = 9;
      else currentNumber--;
      yAxisTilted = true;
    }
    else if(yValue < maxThreshold && yValue > minThreshold) 
      yAxisTilted = false;
      
    number[currentSelectedDigit] = currentNumber;
  }
}
