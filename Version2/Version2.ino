#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Servo.h>
#define button 2
#define side_button 3
#define trigPin 13
#define echoPin 12
#define IRSMALL A0
//#define amberPin N

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Select ports
Adafruit_DCMotor *L_motor = AFMS.getMotor(1);
Adafruit_DCMotor *R_motor = AFMS.getMotor(2);
Adafruit_DCMotor *S_motor = AFMS.getMotor(3);
int count = 0;
int row_count = 0;
double duration, distance;
int amber =0;
int iter = 0;

// Initiate servo
int servo_pin = 9;
Servo servo;
int open_angle = 70;
int closed_angle = 150;

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

void setSweeperSpeed(int S_speed) {
  setMotorSpeed(S_motor, S_speed);
}

void turn(void){
// Turn
/* Original Turn Mechanism, redundant now
  setMotorSpeeds(-255, 50);
  delay(800);
  setMotorSpeeds(200,200);
  delay(900);
  setMotorSpeeds(210,255);
  delay(500);
  */
  // Updated turn
  /*
  setMotorSpeeds(-250,0);
  delay(1500);
  setMotorSpeeds(200,250);
  delay(1400);
  setMotorSpeeds(-220,0);
  delay(1900);
  setMotorSpeeds(-200,-250);
  delay(1200);
  setMotorSpeeds(-200,-200);
  delay(1000);
  setMotorSpeeds(250,200);
  delay(1500);
  //setMotorSpeeds(0,0);
  //delay(49000);
  */
  setMotorSpeeds(-200,0);
  delay(3800);
  setMotorSpeeds(0,200);
  delay(2500);
  setMotorSpeeds(-230,-250);
  delay(3200);
  setMotorSpeeds(190,0);
  delay(2500);
  setMotorSpeeds(-200,-180);
  delay(2500);
  setMotorSpeeds(255,220);
  delay(500);
}
void turn_180(void){
  if (row_count%2) {
    setMotorSpeeds(-250,0);
    delay(3800);
    setMotorSpeeds(-200,-200);
    delay(1000);
    ++row_count;
  }
  else {
    setMotorSpeeds(0,-250);
    delay(3800);
    setMotorSpeeds(-200,-200);
    delay(1000);
    ++row_count;
  }
}
void detectHES(void) {
  // take HES reading
  if (not count%2) {
    block_load();
  }
  else {
    setMotorSpeeds(100,100);
    delay(1000);
  }
  ++count;
  /*if (not port is high i.e. not magnetic) {
    // potentially reverse
      block_load();
  }*/
}
void detectIR(void) {
  /*if (analogRead(IRSMALL) > 10) {
    setMotorSpeeds(0,0);
    int IR = 0;
    while (IR < 10 and analogRead(IRSMALL) > 10) {
      IR+=1;
      Serial.println("positive reading");
    }
    if (IR>9) {
      Serial.println("Block Detected");
      detectHES();
    }
  }*/
}
void shelf_park(void) {
  setMotorSpeeds(-150, -145);
  delay(2000);
  setMotorSpeeds(125, 200);
  delay(2000);
  setMotorSpeeds(200,125);
  delay(2000);
  /*while (!digitalRead(side_button)){
    setMotorSpeeds(150, 100);
  }
  delay(100);
  while (digitalRead(side_button)){
    setMotorSpeeds(-110,-100);
  }
  setMotorSpeeds(100,100);
  // activate servo 
  delay(2000);//driving forwards while cubes slide onto shelf
  //cubes now deposited on shelf
  //servo lowers
  //robot drives forward past shelf towards start zone
  while (!digitalRead(button));
    setMotorSpeeds(250,250);
  //robot reaches final wall next to start zone and begins turning
  setMotorSpeeds(-200,0);
  delay(1000);
  setMotorSpeeds(100,100);
  delay(200);
  setMotorSpeeds(0,0);
  delay(10000);//robot finished in start zone*/
}
void block_load(void) {
  setSweeperSpeed(-100);
  delay(1500);
  setMotorSpeeds(100,100);
  delay(1000);
  setMotorSpeeds(0,0);
  setSweeperSpeed(255);
  delay(550);
  setSweeperSpeed(-100);
  delay(2500);
  setSweeperSpeed(100);
  delay(550);
  setSweeperSpeed(0);
}
void park(void) {
  //cubes now deposited on shelf
  //servo lowers
  //robot drives forward past shelf towards start zone
  Serial.println("PARK");
  setMotorSpeeds(253,250);
  while (true) {
    if (digitalRead(button)) {
      setMotorSpeeds(-200, 0);
      delay(3700);
      setMotorSpeeds(0, -200);
      delay(800);
      setMotorSpeeds(200, 0);
      delay(1600);
      setMotorSpeeds(0, 200);
      delay(1450);
      setMotorSpeeds(-150, -150);
      delay(1000);
      setMotorSpeeds(0,0);
      break;
    }
  }
  delay(100000);
}
void servo1(void) {
  setMotorSpeeds(255,255);
  delay(1000000);
  servo.write(145);
  setMotorSpeeds(100, 100);
  delay(1000);
  setMotorSpeeds(0, 0);
  servo.write(150);
  setSweeperSpeed(255);
  delay(1000);
  setSweeperSpeed(-200);
  delay(2000);
  setSweeperSpeed(100);
  delay(550);
  setSweeperSpeed(0);
  for (int i=0; i<=1000; i++) {
    servo.write(150-i/10);
    delay(1);
  }
  setMotorSpeeds(110,100);
  delay(10000);
  while (true) {
    servo.write(50);
    Serial.println(servo.read());
    delay(2000);
    servo.write(150);
    Serial.println(servo.read());
    delay(2000);
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);// set up Serial library at 9600 bps
  pinMode(button,INPUT);
  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(IRSMALL, INPUT);
  setMotorSpeeds(0, 0);
  setSweeperSpeed(-100);
  delay(2500);
  setSweeperSpeed(100);
  delay(550);
  setSweeperSpeed(0);
  servo.attach(servo_pin);
  servo.write(150);
  delay(1000);
  servo.write(148);
  delay(2000);
}
void loop() {
  servo1();
  setMotorSpeeds(255,240);
  detectIR();
  iter++;
  if (iter>500) {
    iter=0;
    amber=not amber;
    //digitalWrite(amberPin, amber);
  }
  if (digitalRead(button)){
    if (count<3){
      turn();
    }
    else if (count==3) {
      shelf_park();
    }
    ++count;
  }
}
