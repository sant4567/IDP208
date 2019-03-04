#define IRSMALL A0
void setup() {
  Serial.begin (9600);
  pinMode(IRSMALL, INPUT);
}
void loop() {
  if (analogRead(IRSMALL) > 10) {
    int iter = 0;
    while (iter < 20 and analogRead(IRSMALL) > 10) {
      iter+=1;
      Serial.println("Test");
    }
    if (iter>19) {
      Serial.println("Block Detected");
    }
    }
  }
  Serial.println(analogRead(IRSMALL));
}
