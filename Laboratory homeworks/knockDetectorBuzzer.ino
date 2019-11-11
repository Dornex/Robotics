const int knockSensor = A0, buttonPin = 8, activeBuzzerPin = 3;
const int threshold = 0, debounceDelay = 50;
unsigned long debounceTimer, knockTime;
int knockValue, buttonState = LOW, lastState = 0, currentState, knocked;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(activeBuzzerPin, OUTPUT);
  pinMode(knockSensor, INPUT);
  Serial.begin(9600);
}

void loop() {
  muteIfNotKnocked();
  
  currentState = !digitalRead(buttonPin);
  if(currentState != lastState)
    debounceTimer = millis();

  knockValue = analogRead(knockSensor);
  if(knockValue > threshold) {
    knockTime = millis();
    knocked = 1;
  }
  
  if(millis() - knockTime > 5000 && knocked == 1) {
    playSound();
    checkButtonPress();
  }

  lastState = currentState;
}

void checkButtonPress() {
  if((millis() - debounceTimer) > debounceDelay)
    if(currentState != buttonState)
      knocked = 0;
}

void playSound() {
    analogWrite(activeBuzzerPin, 5);    
}

void muteIfNotKnocked() {
  if(knocked == 0)
    analogWrite(activeBuzzerPin, 0);
}
