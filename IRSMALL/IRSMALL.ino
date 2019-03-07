#define IRSMALL A0
void setup() {
  Serial.begin (9600);
  pinMode(IRSMALL, INPUT);
}
void loop() {
  Serial.println(analogRead(IRSMALL));
  delay(300);
}
