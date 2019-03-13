#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Servo.h>
#define button 2
#define side_button 3
#define IRSMALL A1
#define gled 7
#define aled 6
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
int amber =true;
int iter = 0;
int hcount=0;
int reference;
float rvoltage;
int* p;
int blockcount=0;

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
  delay(4000);
  setMotorSpeeds(0,200);
  delay(3500);
  setMotorSpeeds(-230,-250);
  delay(3200);
  setMotorSpeeds(190,0);
  delay(2900);
  setMotorSpeeds(-200,-180);
  delay(2500);
  setMotorSpeeds(255,200);
  delay(800);
}
void turn_180(void){
  if (row_count%2) {
    setMotorSpeeds(-250,0);
    delay(3800);
    setMotorSpeeds(-200,-200);
    delay(1500);
    ++row_count;
  }
  else {
    setMotorSpeeds(0,-250);
    delay(3800);
    setMotorSpeeds(-200,-200);
    delay(1500);
    ++row_count;
  }
}
void detectHES(void) {
  int magnetic=0;
  int HE=0;
  for (int HE=0; HE<=100; HE++){
    if (magnetic) {break;}
    setMotorSpeeds(20, 20);
    Serial.println("Speeds 10");
    if (analogRead(A3) > 500 or analogRead(A2) > 500) {
      Serial.println("Speeds 0");
      setMotorSpeeds(0, 0);
      int itercount = 0;
      while (itercount < 20 and (analogRead(A3) > 500 or analogRead(A2) > 500)) {
        Serial.println(itercount);
        ++itercount;
        delay(20);
      }
      if (itercount >= 19){
        Serial.println("Magnetic");
        magnetic=1;
      }
    }
    Serial.println(HE);
    delay(10);
  }
  if (not magnetic) {
    Serial.println("block_load()");
    setMotorSpeeds(-20, -20);
    delay(2000);
    setMotorSpeeds(0,0);
    delay(2000);
    block_load();
  }
  else if (magnetic) {
    delay(2000);
    setMotorSpeeds(100, 100);
    delay(1000);
    digitalWrite(gled,LOW);
  }

}
void detectIR(void) {
  if (analogRead(IRSMALL) > 2) {
    setMotorSpeeds(0,0);
    int IR = 0;
    while (IR < 10 and analogRead(IRSMALL) > 0) {
      IR+=1;
    }
    if (IR>9) {
      digitalWrite(gled, HIGH);
      Serial.println("IR detected");
      detectHES();
    }
  }
}
void shelf_park(void) {
  setMotorSpeeds(-150, -145);
  delay(2000);
  setMotorSpeeds(125, 200);
  delay(1600);
  setMotorSpeeds(200,125);
  delay(2000);
  while (!digitalRead(side_button)){
    setMotorSpeeds(130, 100);
  }
  delay(100);
  while (digitalRead(side_button)){
    setMotorSpeeds(-110,-100);
  }
  setMotorSpeeds(100,100);
  delay(500);
  setMotorSpeeds(0,0);
  setSweeperSpeed(-255);
  delay(700);
  servo.write(45);
  setMotorSpeeds(100,100);
  delay(4000);
  servo.write(130);
  delay(1000);
  // activate servo
  while (!digitalRead(button));
    setMotorSpeeds(200,200);
  //robot reaches final wall next to start zone and begins turning
  setMotorSpeeds(-200,0);
  delay(2000);
  setMotorSpeeds(100,100);
  delay(200);
  setMotorSpeeds(0,0);
  setSweeperSpeed(-100);
  delay(2500);
  setSweeperSpeed(100);
  delay(550);
  setSweeperSpeed(0);
  if (count==10) {
    delay(1000000);
  }
}
void block_load(void) {
  setSweeperSpeed(-100);
  delay(1500);
  setMotorSpeeds(100,100);
  delay(1000);
  servo.write(150);
  delay(100);
  servo.write(150);
  delay(100);
  setMotorSpeeds(0,0);
  setSweeperSpeed(255);
  delay(700);
  setSweeperSpeed(-100);
  delay(2500);
  setSweeperSpeed(100);
  delay(550);
  setSweeperSpeed(0);
  servo.write(130);
  digitalWrite(gled,LOW);
  setMotorSpeeds(-100, -100);
  delay(400);
  setMotorSpeeds(0,0);
  delay(400);
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
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);// set up Serial library at 9600 bps
  pinMode(button,INPUT);
  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  pinMode(IRSMALL, INPUT);
  pinMode(A3, INPUT);
  pinMode(A2, INPUT);
  pinMode(A1, INPUT);
  pinMode(A0, INPUT);
  pinMode(10, OUTPUT);
  pinMode(aled, OUTPUT);
  pinMode(gled, OUTPUT);
  digitalWrite(gled,LOW);
  reference = analogRead(A0);
  analogWrite(11, 123);
  setMotorSpeeds(0, 0);
  setSweeperSpeed(-100);
  delay(2500);
  setSweeperSpeed(100);
  delay(550);
  setSweeperSpeed(0);
  servo.attach(servo_pin);
  servo.write(150);
  delay(1000);
  servo.write(130);
  delay(2000);
}
void loop() {
  if (count>4){setMotorSpeeds(200,200);}
  else {setMotorSpeeds(220,200);}
  detectIR();
  iter++;
  if (iter>100) {
    iter=0;
    amber=not amber;
    digitalWrite(aled, amber);
  }
  if (digitalRead(button)){
    if (count==0 or count==2 or count==9) {
      turn();
    }
    else if (count==1){
      setMotorSpeeds(0,0);
      servo.write(150);
      setSweeperSpeed(255);
      delay(700);
      setSweeperSpeed(-100);
      delay(2500);
      setSweeperSpeed(100);
      delay(1000);
      setSweeperSpeed(0);
      servo.write(130);
      delay(200);
      turn();
    }
    else if (count==3 or count==10) {
      shelf_park();
    }
    else if (count==4) {
      setMotorSpeeds(-200,0);
      delay(4000);
    }
    else if (count>=5 and count<=7) {
      turn_180();
    }
    else if (count==8){
      turn_180();
      setMotorSpeeds(200, 0);
      delay(3500);
    }
    count++;
  }
}
