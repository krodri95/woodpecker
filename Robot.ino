// #include "IR.hpp"
#include "Motor.hpp"
#include "Arm.hpp"
#include "Robot.hpp"

Robot *R = new Robot();

///*
// Include the servo library to allow for connection and control of servo motors
#include <Servo.h>

const int pinServoL1 = 4;
Servo servoL1; //pwm 500-2384 for 0-170 degrees

const int pinA = 18;
const int pinB = 19;
volatile long enc_count;

//*/
void setup()
{

  // "attach"  servos L1 and L2 to the defined pin
  servoL1.attach(pinServoL1);
  servoL1.writeMicroseconds(495); //0 degree
  R->marker->servoL1 = servoL1;

  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  attachInterrupt(digitalPinToInterrupt(pinA), channelA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinB), channelB, CHANGE);
  enc_count=0;

  //Initialize the serial port
  Serial.begin(9600);

}

void loop()
{
  // Serial.print("IR 1:");
  // float dist1 = R->ir1->find_distance();

  // Serial.print("IR 2:");
  // float dist2 = R->ir2->find_distance();

  // int dist0 = R->usonic->find_distance();

  ///*
  rot(180);
  R->adjust(NULL, NULL);
  int dist0 = R->usonic->find_distance();

  while(dist0 >= 18) {
    R->fw();
    dist0 = R->usonic->find_distance();
  }
  R->stop();
  delay(1000);

  rot(90);
  R->adjust(NULL, NULL);
  dist0 = R->usonic->find_distance();
  while(dist0 >= 10) {
    R->fw();
    dist0 = R->usonic->find_distance();
  }
  R->stop();

  R->adjust(NULL, NULL);
  R->marker->draw();
  dist0 = R->usonic->find_distance();
  while(dist0 <= 15) {
    R->bw();
    dist0 = R->usonic->find_distance();
  }
  R->stop();
  delay(1000);

  rot(90);
  R->adjust(NULL, NULL);
  dist0 = R->usonic->find_distance();
  while(dist0 >= 18) {
    R->fw();
    dist0 = R->usonic->find_distance();
  }
  R->stop();

  rot(90);
  R->adjust(NULL, NULL);
  dist0 = R->usonic->find_distance();
  while(dist0 >= 18) {
    R->fw();
    dist0 = R->usonic->find_distance();
  }
  R->stop();
  //*/
  // rot(180);
  delay(50000);

}

void rot(const float angle) { //rotate on the spot, left by an angle in degrees

  /*calculate rotational time
  32 pulses per revolution
  distance travelled by the wheel
  */
  long desired_enc_count = 2375*(6+7)/(4*360)*angle; //4600

  enc_count = 0;
  R->m1->bw();
  R->m2->fw();
  R->m3->bw();
  R->m4->fw();

  while(enc_count < desired_enc_count) {
    //Serial.print("enc_count = ");
    //Serial.println(enc_count);
  }
  R->stop();
  delay(1000);
  enc_count = 0;

}

void channelA(void) {

  bool outA = digitalRead(pinA);
  bool outB = digitalRead(pinB);

  if(outA) {
    if(outB == 0) {
      enc_count++;
    } else {
      enc_count--;
    }
  } else {
    if(outB == 1) {
      enc_count++;
    } else {
      enc_count--;
    }
  }

}

void channelB(void) {

  bool outA = digitalRead(pinA);
  bool outB = digitalRead(pinB);

  if(outB) {
    if(outA == 1) {
      enc_count++;
    } else {
      enc_count--;
    }
  } else {
    if(outA == 0) {
      enc_count++;
    } else {
      enc_count--;
    }
  }

}
