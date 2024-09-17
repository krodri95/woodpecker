#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "Motor.hpp"
#include "IR.hpp"
#include "Arm.hpp"
#include "Ultrasonic.hpp"

#define EN_DEBUG 1

// Define the elements in lookup table 1, front sensor
float LuT_0_xElements[] = {80, 84, 88, 93, 101, 108, 120, 131, 143, 159, 182, 217, 266, 343, 500, 552, 603};
float LuT_0_yElements[] = {80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, 9, 8};

// Define the elements in lookup table 1, right front
float LuT_1_xElements[] = {80, 84, 88, 93, 101, 108, 120, 131, 143, 159, 182, 217, 266, 343, 500, 552, 603};
float LuT_1_yElements[] = {80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, 9, 8};

// Define the elements in lookup table 1, right rear
float LuT_2_xElements[] = {80, 84, 88, 93, 101, 108, 120, 131, 143, 159, 182, 217, 266, 343, 500, 552, 603};
float LuT_2_yElements[] = {80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, 9, 8};

// // Define the elements in lookup table 1
float LuT_xElements[] = {173, 206, 252, 322, 347, 373, 397, 427, 483, 526, 570};
float LuT_yElements[] = {30, 25, 20, 15, 14, 13, 12, 11, 10, 9, 8};

class Robot {
  public:
    Robot() {}
    ~Robot(){}

    void fw() {
      
      m1->fw();
      m2->fw();
      m3->fw();
      m4->fw();
    }

    void bw() {
      
      m1->bw();
      m2->bw();
      m3->bw();
      m4->bw();
    }

    void stop() {
      m1->stop();
      m2->stop();
      m3->stop();
      m4->stop();
    }

    void rot(const bool dir, const float angle) { //rotate on the spot, left : dir = 0; right : dir = 1

      /*calculate rotational time
      w = (vr-vl)/2r = angle/t
      t = angle*2r/??????????????
      */
      uint32_t time = 0;
      time = int(angle*2*7*1000/8.6667);

      if(dir) {
        m1->fw();
        m2->bw();
        m3->fw();
        m4->bw();
      } else {
        m1->bw();
        m2->fw();
        m3->bw();
        m4->fw();
      }
      //sleep for calculated time
      delay(time);
      stop();
    }

    void rot(const bool dir) { //rotate on the spot, left : dir = 0; right : dir = 1

      if(dir) {
        m1->fw();
        m2->bw();
        m3->fw();
        m4->bw();
      } else {
        m1->bw();
        m2->fw();
        m3->bw();
        m4->fw();
      }
    }

    void side(const bool dir) { //go either left or right base on ir1 and ir2 readings, left : dir = 0; right : dir = 1

      if (dir) {
        m1->bw();
        m2->fw();
        m3->fw();
        m4->bw();
      } else {
        m1->fw();
        m2->bw();
        m3->bw();
        m4->fw();
      }
    }

    void adjust(const int w1, const int w2) { //w1 front wall distance, w2 right wall distance
      //align the robot parallel to the right wall
      float ird1 = ir1->find_distance();
      float ird2 = ir2->find_distance();

      while(abs(ird1-ird2) > 1.0) {
        if(ird1 > ird2) { //rotate right 
          rot(1);
        } else { //rotate left
          rot(0);
        }

        delay(15);//???????????????????????????//step change
        stop();

        ird1 = ir1->find_distance();
        ird2 = ir2->find_distance();
      }


      //align the robot at a fixed distance from the right wall
      // while(abs(ird1 - w2) > 1.0) {
      //   if(ird1 > w2) { //move right 
      //     side(1);
      //   } else { //move left
      //     side(0);
      //   }

      //   delay(15);//???????????????????????????//step change
      //   stop();

      //   ird1 = ir1->find_distance();
      //   ird2 = ir2->find_distance();
      // }

      if(w1 != NULL) {
        //align the robot at a fixed distance from the front wall
        int uso = usonic->find_distance();
        while(abs(uso - w1) > 1.0) {
          if(uso > w2) { //move fw 
            fw();
          } else { //move bw
            bw();
          }

          delay(15);//???????????????????????????//step change
          stop();

          ird1 = ir1->find_distance();
          ird2 = ir2->find_distance();
        }
      }

    }

  public:
    Arm *marker = new Arm();

    //Ultrasonic sensor
    Ultrasonic *usonic = new Ultrasonic(34, 36);

    //IR sensors
    IR *ir1 = new IR(11, sizeof(LuT_xElements) / sizeof(LuT_xElements[0]), sizeof(LuT_yElements) / sizeof(LuT_yElements[0]), LuT_xElements, LuT_yElements);
    IR *ir2 = new IR(12, sizeof(LuT_xElements) / sizeof(LuT_xElements[0]), sizeof(LuT_yElements) / sizeof(LuT_yElements[0]), LuT_xElements, LuT_yElements);

    //Motors
    //remove standby pin ?
    //control pin 1, control pin 2, PWM pin, stanby pin, rotational direction pin : 0 - left motors, 1 - right motors
    Motor *m1 = new Motor(42, 44, 7, 0); //front left
    Motor *m2 = new Motor(48, 50, 8, 0); //front right
    Motor *m3 = new Motor(26, 24, 5, 1); //rear left
    Motor *m4 = new Motor(32, 30, 6, 1); //rear right

};
#endif