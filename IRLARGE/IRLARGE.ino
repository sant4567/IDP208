#define sensor A0 // Sharp IR GP2Y0A41SK0F (4-30cm, analog)

void setup() {
  Serial.begin(9600); // start the serial port
}

void loop() {
  
  // 5v
  double volts = analogRead(sensor);  // value from sensor
  double distance = ((6700/(volts-3)) - 4);// best fit line calculated from graph
  delay(500); // slow down serial port 
  
  if (distance >= 0){
    Serial.println(distance);   // print the distance
  }
}
