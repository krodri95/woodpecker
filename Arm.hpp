#ifndef ARM_HPP
#define ARM_HPP

// Include the servo library to allow for connection and control of servo motors
#include <Servo.h>
#include "Arduino.h"

#define L1_EXTENSION 60 //in degrees approx
#define EN_DEBUG 1

class Arm {
  public:
    Arm(){}
    ~Arm(){}

    void draw() {

      //extend the arm
      pwmL1 = 1400;
      servoL1.writeMicroseconds(pwmL1);
      delay(500);
      
#if EN_DEBUG
        Serial.print("Draw arm : ");
        Serial.print("L1 PWM = ");
        Serial.println(pwmL1);
#endif

      //retract the arm
      pwmL1 = 495;
      servoL1.writeMicroseconds(pwmL1);
      delay(500);
      
#if EN_DEBUG
        Serial.print("Retract arm : ");
        Serial.print("L1 PWM = ");
        Serial.println(pwmL1);
#endif

    }

    // Servo sg90
    Servo servoL1; //pwm 500-2384 for 0-170 degrees

  private:
    int pwmL1;

};
#endif