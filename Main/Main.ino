#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <PID_v1.h>

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
double Setpoint, Input, Output;
double duration, distance;
double Kp=4, Ki=0, Kd=6;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);// set up Serial library at 9600 bps
  pinMode(button,INPUT);
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");
  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Input = distance;
  Setpoint = 50;
  myPID.SetMode(AUTOMATIC);
  
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
  setMotorSpeeds(-255, 100);
  delay(2000);
  setMotorSpeeds(-150, -150);
  delay(500);
}
void loop() {
  digitalWrite(trigPin, LOW); // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); 
  distance = (duration/2) / 29.1;
  Input = distance;
  int direct=1;
  myPID.SetControllerDirection(DIRECT);
  if (distance>Setpoint) {
    myPID.SetControllerDirection(REVERSE);
    direct=-1;
  }
  myPID.Compute();
  Serial.print(Output);Serial.print(" "); Serial.println(distance);
  Output*=0.4;
  setMotorSpeeds(100-Output*direct, 100+Output*direct);
  if (digitalRead(button)){
      ++count;
      turn();
  }
  /*if (count < 1) {
    setMotorSpeeds(255,250);
  }
  else {
    setMotorSpeeds(255,150);
    delay(1000);
    setMotorSpeeds(255, 230);
    delay(4500);
    setMotorSpeeds(-255, -250);
    delay(1500);
    setMotorSpeeds(150, 255);
    delay(1500);
    setMotorSpeeds(255,150);
    delay(1500);
    setMotorSpeeds(150, 100);
    delay(100000);
  }
  if (digitalRead(button)){
      ++count;
      turn();
  }*/
}
