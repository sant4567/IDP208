#include <Wire.h>
#include <Adafruit_MotorShield.h>
#define button 2
#define trigPin 13
#define echoPin 12
// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *L_motor = AFMS.getMotor(1);
// You can also make another motor on port M2
Adafruit_DCMotor *R_motor = AFMS.getMotor(2);
int count = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);// set up Serial library at 9600 bps
  pinMode(button,INPUT);
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");
  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
}
void setMotorSpeed(Adafruit_DCMotor *motor, int motor_speed) {
  motor->setSpeed(abs(motor_speed));
  if (motor_speed >= 0) {
    motor->run(FORWARD);
  }
  else if (motor_speed < 0) {
    motor->run(BACKWARD);
  }
}

void setMotorSpeeds(int L_speed, int R_speed) {
  setMotorSpeed(L_motor, L_speed);
  setMotorSpeed(R_motor, R_speed);
}

void turn(void){
// Turn
  setMotorSpeeds(-255, -50);
  delay(2000);
}
void loop() {
  /*
  // Ultrasound
  double duration, distance;
  digitalWrite(trigPin, LOW); // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  int L = 200+(distance-12)*8;
  if (L>255) {
    L = 255;
  }
  setMotorSpeeds(L, 200);
  if (digitalRead(button)){
    turn();
  }
  Serial.println(distance);
  */
  if (count < 3) {
    setMotorSpeeds(255,250);
  }
  else {
    setMotorSpeeds(255,245);
  }
  if (digitalRead(button)){
    ++count;
    turn();
  }
}
