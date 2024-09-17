#ifndef MOTOR_HPP
#define MOTOR_HPP

#define EN_DEBUG 1
#define PWM_VALUE 40

class Motor {
  public:
    Motor(int c1Pin, int c2Pin, int pwmPin, bool rdPin):pinCI1(c1Pin), pinCI2(c2Pin), pinPWM(pwmPin), rotDirect(rdPin){
      // Assign the digital I/O pin directions
      pinMode(pinCI1, OUTPUT);
      pinMode(pinCI2, OUTPUT);
      pinMode(pinPWM, OUTPUT);
      //pinMode(pinStandBy, OUTPUT);

      // Drive the standby pin high to enable the output
      // standBy = true;
      // digitalWrite(pinStandBy, standBy);

      // Set Initial values for CI1 and CI2 control function pins
      CI1 = true;
      CI2 = false;

    }
    ~Motor(){}

    void fw() {
      // Go forwards
      if (rotDirect == 1) {
        CI1 = true;
        CI2 = false;
      } else {
        CI1 = false;
        CI2 = true;
      }

      digitalWrite(pinCI1, CI1);
      digitalWrite(pinCI2, CI2);

      // Write the pwnValue to the PWM pin
      analogWrite(pinPWM, int(PWM_VALUE));

#if EN_DEBUG
      // Display the board variable status to the Serial Monitor
      Serial.print("Motor forward : ");
      Serial.print("PWM = ");
      Serial.print(PWM_VALUE);
      // Serial.print(", Standby = ");
      // Serial.print(standBy);
      Serial.print(", CI1 = ");
      Serial.print(CI1);
      Serial.print(", CI2 = ");
      Serial.println(CI2);
#endif

    }

    void bw() {
      // Reverse
      if (rotDirect == 1) {
        CI1 = false;
        CI2 = true;
      } else {
        CI1 = true;
        CI2 = false;
      }

      digitalWrite(pinCI1, CI1);
      digitalWrite(pinCI2, CI2);

      // Write the pwnValue to the PWM pin
      analogWrite(pinPWM, int(PWM_VALUE));

#if EN_DEBUG
      // Display the board variable status to the Serial Monitor
      Serial.print("Motor reverse : ");
      Serial.print("PWM = ");
      Serial.print(PWM_VALUE);
      // Serial.print(", Standby = ");
      // Serial.print(standBy);
      Serial.print(", CI1 = ");
      Serial.print(CI1);
      Serial.print(", CI2 = ");
      Serial.println(CI2);
#endif

    }

    void stop() {
      // Stop
      // Write the pwnValue to the PWM pin
      analogWrite(pinPWM, 0);

#if EN_DEBUG
      // Display the board variable status to the Serial Monitor
      Serial.print("Motor stop : ");
      Serial.print("PWM = ");
      Serial.println(0);
#endif

    }

  private:
    const int pinCI1;       // Pin allocation for CI1
    const int pinCI2;       // Pin allocation for CI2
    const int pinPWM;       // Pin allocation for the PWM pin
    //const int pinStandBy;   // Pin allocation for the standby pin

    bool CI1;               // CI1 pin value
    bool CI2;               // CI2 pin value
    //bool standBy;           // standBy pin Value

    bool rotDirect;   // Rotation direction variable 0 = right motor, 1 = left motor

};
#endif