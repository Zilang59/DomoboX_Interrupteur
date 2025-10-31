#include "TM1637_6D.h"

#define CLK 11  // ton branchement
#define DIO 5   // ton branchement

TM1637_6D tm1637_6D(CLK, DIO);

void setup() {
  tm1637_6D.init();
  tm1637_6D.set(BRIGHT_TYPICAL);
}

void loop() {
//   int8_t digits[6] = {HSeg, 5, 4, 3, 2, 1};  // Affiche 123456
    int8_t digits[6] = {6, 5, 4, 3, 2, 1};
    int8_t points[6] = {0, 0, 0, 0, 0, 0};  // Pas de points

    tm1637_6D.display(digits, points);
    delay(1000);
}
