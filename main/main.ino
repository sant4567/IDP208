/*  The main loop continuously scans for an infrared signal,
    whilst controlling the action of the progress variable count,
    blinking the amber led and setting the motor speeds appropriately
     
    The route is as follows:
      - Follow the outside wall on the initial run, collecting known blocks
      - Search the area in strips parallel to the shelf on the second run
      - Deposit blocks after each run
*/

// preamble : libraries, arduino pins
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Servo.h>
#define button 2
#define side_button 3
#define amberled 6
#define greenled 7
#define reference 11 // reference for hall sensor
#define infrared A1
#define hall_1 A2 // positive comparator output
#define hall_2 A3 // negative comparator output

// motor shield object with default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *L_motor = AFMS.getMotor(1);
Adafruit_DCMotor *R_motor = AFMS.getMotor(2);
Adafruit_DCMotor *S_motor = AFMS.getMotor(3);

// initialise variables
int count = 0; // progress variable (see loop)
int row_count = 0; // scout mode turn_180 counts
int amber = true; // state of amber led
int iter = 0; // iterations for amber led blink

// initialise servo
int servo_pin = 9;
Servo servo;

void setMotorSpeed(Adafruit_DCMotor *motor, int motor_speed) {
  // framework for various motors
  motor->setSpeed(abs(motor_speed));
  if (motor_speed >= 0) {
    motor->run(FORWARD);
  }
  else if (motor_speed < 0) {
    motor->run(BACKWARD);
  }
}

void setMotorSpeeds(int L_speed, int R_speed) {
  // function for ease of motor access
  setMotorSpeed(L_motor, L_speed);
  setMotorSpeed(R_motor, R_speed);
}

void setSweeperSpeed(int S_speed) {
  // function for ease of sweeper access
  setMotorSpeed(S_motor, S_speed);
}

void turn(void){
  // corner turn : longer but more robust to different surfaces on table
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
  // scout mode turns
  // second turn
  if (row_count%2) {
    setMotorSpeeds(-250,0);
    delay(3800);
    setMotorSpeeds(-200,-200);
    delay(2500);
    row_count++;
  }
  // first and third turns
  else {
    setMotorSpeeds(0,-250);
    delay(3800);
    setMotorSpeeds(-200,-200);
    delay(2500);
    row_count++;
  }
}

void detectHES(void) {
  int magnetic=0; // block non-magnetic
  int HE=0; // hall-effect counter
  for (int HE=0; HE<=100; HE++){
    if (magnetic) {break;} // exit if magnetic confirmed at any point
    setMotorSpeeds(20, 20); // travel forwards slowly whilst scanning block
    if (analogRead(hall_2) > 500 or analogRead(hall_1) > 500) {
      setMotorSpeeds(0, 0); // stop and test if hall reading high
      int itercount = 0;
      while (itercount < 10 and (analogRead(hall_2) > 500 or analogRead(hall_1) > 500)) {
        ++itercount;
        delay(20);
      }
      // requires 10 consecutive high readings to prevent anomalies
      if (itercount >= 10){
        magnetic=1; // magnetic block confirmed
      }
    }
    delay(10); // allows sufficient scanning time/space
  }
  // non-magnetic blocks loaded
  if (not magnetic) {
    setMotorSpeeds(-20, -20);
    delay(2000);
    setMotorSpeeds(0,0);
    delay(2000);
    block_load();
  }
  // magnetic blocks rejected
  else if (magnetic) {
    delay(2000);
    setMotorSpeeds(100, 100);
    delay(1000);
    digitalWrite(greenled, LOW); // reset green led
  }
}

void detectIR(void) {
  if (analogRead(infrared) > 2) {
    setMotorSpeeds(0,0); // one (perhaps anomalous) instance of block detection stops motors
    int IR = 0;
    while (IR < 10 and analogRead(infrared) > 2) {
      IR+=1;
    }
    // requires 10 continuous high infrared readings
    if (IR>9) {
      digitalWrite(greenled, HIGH); // block detection confirmed
      detectHES(); // hall effect sensor detection sequence called
    }
  }
}

void shelf_park(void) {
  // park alongside shelf + deposits blocks + parks in start zone
  setMotorSpeeds(-150, -145); // reverse, hugging wall
  delay(2000);
  setMotorSpeeds(125, 200); // curve round shelf
  delay(1600);
  setMotorSpeeds(200,125);
  delay(2000);
  // wait for back-right side button
  while (!digitalRead(side_button)){
    setMotorSpeeds(130, 100);
  }
  delay(100); // allow debounce
  // reverse once side button pressed until side button released
  while (digitalRead(side_button)){
    setMotorSpeeds(-110,-100);
  }
  setMotorSpeeds(100,100); // travel forward alongside shelf
  delay(500);
  setMotorSpeeds(0,0);
  servo.write(150); // sweeper ensures blocks remain on platform
  setSweeperSpeed(-255); 
  delay(2000);
  setSweeperSpeed(255);
  delay(2000);
  setSweeperSpeed(-255);
  delay(2000);
  servo.write(45); // deposit blocks
  setMotorSpeeds(100,100);
  delay(4000);
  servo.write(130);
  delay(1000);
  // wait for far wall detection
  while (!digitalRead(button));
    setMotorSpeeds(230,230);
  // reach wall and turn
  setMotorSpeeds(-200,0);
  delay(2500);
  setMotorSpeeds(100,100);
  delay(200);
  setMotorSpeeds(0,0);
  // reset sweeper
  setSweeperSpeed(-200);
  delay(500);
  setSweeperSpeed(100);
  delay(550);
  setSweeperSpeed(0);
  if (count==10) {
    delay(10000000); // ends program temporarily
  }
  // otherwise, return to loop : count = 4
}

void block_load(void) {
  setSweeperSpeed(-100); // open sweeper to accept
  delay(1500);
  setMotorSpeeds(100,100);
  delay(1000);
  servo.write(150); // forces platform to ground level
  delay(100);
  servo.write(150); // twice to minimise risk of bugs
  delay(100);
  setMotorSpeeds(0,0);
  setSweeperSpeed(255); // sweeps blocks onto platform
  delay(700);
  setSweeperSpeed(-100); // resets sweeper
  delay(2500);
  setSweeperSpeed(100);
  delay(550);
  setSweeperSpeed(0);
  servo.write(130); // prevents friction
  digitalWrite(greenled, LOW); // reset green led
  setMotorSpeeds(-100, -100); // reverse to initial position : counters build up of blocks in close proximity
  delay(400);
  setMotorSpeeds(0,0); // calms down appearance of motion/strain on motors!
  delay(400);
}

void setup() {
  // initialise motorshield and motors
  AFMS.begin();
  setMotorSpeeds(0, 0);

  // initialise pins
  pinMode(button, INPUT);
  pinMode(infrared, INPUT);
  pinMode(hall_2, INPUT);
  pinMode(hall_1, INPUT);
  pinMode(reference, OUTPUT);
  pinMode(amberled, OUTPUT);
  pinMode(greenled, OUTPUT);

  // initialise leds
  digitalWrite(greenled, LOW);
  digitalWrite(amberled, LOW);

  // write constant reference to hall sensor circuit : 5V on 255 bit scale
  analogWrite(reference, 123);

  // sweeper to start position (reject blocks)
  setSweeperSpeed(-100);
  delay(2500);
  setSweeperSpeed(100);
  delay(550);
  setSweeperSpeed(0);

  // servo to start position (
  servo.attach(servo_pin);
  servo.write(150); // tight to floor angle (block-loading)
  delay(1000);
  servo.write(130); // above floor angle (roaming)
  delay(2000);
}

void loop() {
  if (count>4){setMotorSpeeds(205,200);} // travel straight in scout mode
  else {setMotorSpeeds(220,200);} // hug wall during first lap

  // continuous block detection
  detectIR();

  // blinking amber led
  iter++;
  if (iter>100) {
    iter=0;
    amber=not amber;
    digitalWrite(amberled, amber);
  }

  // front button press records progress
  if (digitalRead(button)){
    /*
                    RESULT OF FRONT BUTTON PRESSES
       COUNT   :                ACTION               : UPDATED COUNT    
     count = 0 : corner turn                         : count = 1
     count = 1 : corner turn + sweeper               : count = 2
     count = 2 : corner turn                         : count = 3
     count = 3 : deposit blocks + park in start zone : count = 4
     count = 4 : half turn                           : count = 5
     count = 5 : right shoulder turn 180             : count = 6
     count = 6 : left shoulder turn 180              : count = 7
     count = 7 : right shoulder turn 180 + 90        : count = 8
     count = 8 : corner turn                         : count = 9
     count = 9 : deposit blocks + park in start zone : count = 10
     */
    if (count==0 or count==2 or count==8) {
      turn();
    }
    else if (count==1){
      // sweeper ensures blocks remain on platform
      setMotorSpeeds(0,0);
      servo.write(150);
      setSweeperSpeed(255);
      delay(700);
      setSweeperSpeed(-100);
      delay(1500);
      setSweeperSpeed(100);
      delay(1000);
      setSweeperSpeed(0);
      servo.write(130);
      delay(200); // apparent fix to servo bugs
      turn();
    }
    else if (count==3 or count==9) {
      shelf_park();
    }
    else if (count==4) {
      // turn 90
      setMotorSpeeds(-200,0);
      delay(4000);
    }
    else if (count==5 or count==6) {
      turn_180();
    }
    count++;
  }
}
