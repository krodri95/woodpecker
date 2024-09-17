#ifndef IR_HPP
#define IR_HPP

#define EN_DEBUG 1

class IR {
  public:
    IR(int aPin, uint8_t sx, uint8_t sy, float arrX[], float arrY[]):irAnalogPin(aPin), sz_x(sx), sz_y(sy), LuT_x(arrX), LuT_y(arrY){
      CheckLuTArrays();
    }
    ~IR(){}

    float find_distance() {
      // simple moving average
      float irReading = 0;
      float sum = 0;
      int P=5;
      int N=10;

      // simple moving average
      for (int p = 0; p < P; p++) {
        float irReading = 0;
        for (int n = 0; n < N; n++) {
          irReading += (float)analogRead(irAnalogPin);
          delay(5);
        }
        sum += irReading / (float)N;
      }

      // Perform the lookup table operation
      float interpVal = LookUpTable1D(sum/P);

#if EN_DEBUG
      //Display results
      Serial.print("Distance (cm) = ");
      Serial.println(interpVal);
#endif
      return interpVal;
    }

  private:
    const int irAnalogPin;
    uint8_t sz_x;
    uint8_t sz_y;
    float* LuT_x;
    float* LuT_y;

    // Check the LuT Arrays for size and increasing xElement Values
    CheckLuTArrays() {
      // Check for array size missmatch between the xElements and yElements arrays
      if (sz_x != sz_y) {
#if EN_DEBUG
        Serial.println();
        Serial.println("Error: Number of elements in xElements array is not equal to the number of");
        Serial.println("elements in the yElements array.");
        Serial.println();
        Serial.println("This program will not execute further, and sit in an infinite loop");
#endif
        while (1); // Hold the execution at this point
      }

      //Check that the values in xElements are increasing as the index values increase
      for (uint8_t n = 0; n <= sz_x - 2; n++) {
        // Scan through the xElement indexes
        if (LuT_x[n] >= LuT_x[n + 1]) {
#if EN_DEBUG
          Serial.println();
          Serial.println("Error: the values in xElements are not all increasing with increasing");
          Serial.println("index values");
          Serial.println();
          Serial.println("This program will not execute further, and sit in an infinite loop");
#endif
          while (1); // Hold the execution at this point
        }
      }
    }

    float LookUpTable1D (float inVal) {
      float fraction = 0.0;
      for (uint8_t n = 0; n <= sz_x - 2; n++) {
        if ((inVal >= LuT_x[n]) && (inVal <= LuT_x[n + 1])) {
          fraction = (inVal - LuT_x[n]) / (LuT_x[n + 1] - LuT_x[n]);
          // Return the output value and exit the function
          return (fraction * (LuT_y[n + 1] - LuT_y[n])) + LuT_y[n];
        }
      }
      // Return an overflow value of the input value is not within the input range of the xElements array
      return 0xFFFFFFFF;
    }

};
#endif