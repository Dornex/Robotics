const int redPin = 9, greenPin = 10, bluePin = 11;
const int redPot = A0, greenPot = A1, bluePot = A2;
int analogRedValue, analogGreenValue, analogBlueValue;

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  analogRedValue = analogRead(redPot);
  analogGreenValue = analogRead(greenPot);
  analogBlueValue = analogRead(bluePot);
  
  setColor(analogRedValue, analogGreenValue, analogBlueValue);
}

void setColor(int red, int green, int blue) {
  analogWrite(redPin, map(red, 0, 1023, 0 , 255));
  analogWrite(greenPin, map(green, 0, 1023, 0 , 255));
  analogWrite(bluePin, map(blue, 0, 1023, 0 , 255));
}