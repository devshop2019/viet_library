#include "Vietduino_NewFunction.h"

#define setup4  }CREATE_FUNCTION(setup_4)

#define delay(x)    M_DELAY(x)

#define loop1     VIETDUINO_UPDATE;\
                  }CREATE_FUNCTION(loop_1)

#define loop2(xyz)     VIETDUINO_UPDATE;\
                  }CREATE_FUNCTION(loop_1_##xyz)
                  
#define loop3          loop2(__LINE__)           

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
  loop1
  Serial.println("loop________");
  delay(500);
}

void doc_tn(){
  loop3
  Serial.println("doc_tn  1 " + String(millis()));
  delay(1000);
  Serial.println("doc_tn  2 " + String(millis()));
  delay(1000);
  Serial.println("doc_tn  3 " + String(millis()));
  delay(1000);
  
}
