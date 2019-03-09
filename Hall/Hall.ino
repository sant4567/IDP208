#define refpin 9
int hcount=0;
int reference;
float rvoltage;
int* p;

void setup() {
 Serial.begin(9600);
 pinMode(refpin, OUTPUT);
 pinMode(A0, INPUT);
 pinMode(10, OUTPUT);
}

void loop() {
  if (hcount==0) {
    reference = analogRead(A0);
    p=&reference;
    ++hcount;
  }
  reference = *p;
  float rvoltage=reference*4.95/1023.0;
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (4.95 / 1023.0);
  Serial.print("Voltage: ");Serial.print(voltage);Serial.print(" Reference: ");Serial.println(rvoltage);
  
  analogWrite(10, reference/4.01176471);
  //  
  //  Serial.println(voltage);
  //  analogWrite(refpin, sensorValue/4.01176471);
  delay(300);
}
