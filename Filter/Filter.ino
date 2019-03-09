int count=0;

void setup() {
  Serial.begin(9600);
  pinMode(10, OUTPUT);
  pinMode(A0, INPUT);
}

void loop() {
  if (count==0) {
    
  }
  float v=2.46;
  analogWrite(10, v*255.0/4.95);
  float voltage = analogRead(A0)*4.95/1023.0;
  Serial.println(voltage);
  delay(300);
}
