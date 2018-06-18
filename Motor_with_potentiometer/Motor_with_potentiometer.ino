// connection pins for motor; digital and pwm pins.
int enA = 5;
int in3 = 8;
int in4 = 9;
int motorSpeed = 0;

// connection pin for potentiometer; analog pin.
int potPin = 2;
int potVal = 0;

void setup() {
  Serial.begin(9600);

  // set motor direction
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  // set all motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // set potentiometer pin to input
  pinMode(potPin, INPUT);
}

void loop() {
  potVal = analogRead(potPin);
  
  // map potentiometer value to motor speed value
  motorSpeed = potVal/4;

  // set motor speed
  analogWrite(enA, motorSpeed);
  
  // for debugging
  Serial.print("Potentiometer value: ");
  Serial.println(potVal);

  Serial.print("motorSpeed value: ");
  Serial.println(motorSpeed);
}
