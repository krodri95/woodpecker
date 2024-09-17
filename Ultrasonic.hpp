#ifndef ULTRASONIC_HPP
#define ULTRASONIC_HPP

#define EN_DEBUG 1

class Ultrasonic {
  public:
    Ultrasonic(int tPin, int ePin): trigPin(tPin), echoPin(ePin) {
      pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
      pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    }
    ~Ultrasonic(){}

    int find_distance() {

      long duration;
      int distance;
      // Clears the trigPin
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(5);
      digitalWrite(trigPin, LOW);
      // Reads the echoPin, returns the sound wave travel time in microseconds
      duration = pulseIn(echoPin, HIGH);
      distance = duration * 0.034 / 2;

#if EN_DEBUG
      //Display results
      Serial.print("Ultrasonic: Distance (cm) = ");
      Serial.println(distance);
#endif
      return distance;
    }

  private:
    const int trigPin;
    const int echoPin;

};
#endif