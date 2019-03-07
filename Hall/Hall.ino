#define refpin 9

void setup() {
 Serial.begin(9600);
 pinMode(refpin, OUTPUT);
}

void loop() {
  int sensorValue = analogRead(A3);
  float voltage = sensorValue * (5.0 / 1023.0);
  Serial.println(voltage);
  analogWrite(refpin, sensorValue/4.01176471);
  delay(300);
}
