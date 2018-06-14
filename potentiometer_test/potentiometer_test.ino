int potPin = 2;
int potVal = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("The test has begun.");
}

void loop() {
  potVal = analogRead(potPin);
  Serial.print("potentiometer value: ");
  Serial.println(potVal);
}
