#include "Vietduino_NewFunction.h"

#define setup4  }CREATE_FUNCTION(setup_4)

#define delay(x)    M_DELAY(x)

#define loop1     VIETDUINO_UPDATE;\
                  }CREATE_FUNCTION(loop_1)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  setup4
  Serial.println(millis());
  delay(1000);
  setup_4.disable();
}

void loop() {
  // put your main code here, to run repeatedly:
//  VIETDUINO_UPDATE;
  loop1
  Serial.println("loop________");
  delay(500);
}
