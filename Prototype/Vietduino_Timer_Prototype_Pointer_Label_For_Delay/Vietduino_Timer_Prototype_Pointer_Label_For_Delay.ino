#include "Vietduino_Manager.h"
#include "Vietduino_Timer.h"

Vietduino_Timer myjob1;
 Vietduino_Timer myjob2;
Vietduino_Timer myjob3;

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  
  myjob1.begin(callBack_job1);
  myjob2.begin(&callBack_job2);
//  myjob3.begin(callBack_job3);
}

void loop() {
  VIETDUINO_UPDATE
}

void* mJumpPoint = 0;
////////////////////////////////////////////
#define M_DELAY(delayMillis) \
  do { \
      te->delay(delayMillis); \
        do { \
          __label__ jumpLabel; \
          setJump(&& jumpLabel); \
          return; \
          jumpLabel: ; \
          setJump((void*)0);\
        } while (false);\
    } while (false)

//------------------------------------------    

////////////////////////////////////////////
#define M_LOOP()  \
  Vietduino_Timer *te  =  (Vietduino_Timer*)me;\
  void* p = getJump();\
    if (p != 0) {\
      goto *p;\
    }
//------------------------------------------    

void setJump(void* jumpPoint) {
  mJumpPoint = jumpPoint; 
}

void * getJump(){
  return mJumpPoint;
}

void callBack_job1(boolean VIETDUINO_TASK){
//  Vietduino_Timer *te  =  (Vietduino_Timer*)me;
//  void* p = getJump();
//    if (p != 0) {
//      goto *p;
//    }
    M_LOOP(){
    static int cf = 0;
//    for(cf = 0; cf <= 2 ; cf++){
//      Serial.println("Run Vietduino_Timer " + String(cf) + " " + String (millis()));
//      M_DELAY(100);
//    }

    while(cf<2){
      Serial.println("Run Vietduino_Timer " + String(cf) + " " + String (millis()));
      M_DELAY(0);
      cf++;
    }
    cf = 0;
    M_DELAY(1000);
    
//    Serial.println("Run Vietduino_Timer 1 " + String (millis()));
//    M_DELAY(1000);
//    Serial.println("Run Vietduino_Timer 2 " + String (millis()));
//    M_DELAY(1000);
    }
}

void callBack_job2(Vietduino_Task * me){
  START_IF()
      digitalWrite(13,1);
  DL_IF(100,2)
      digitalWrite(13,0);
  END_IF(100)
}

void callBack_job3(boolean * me){
  static unsigned char st_count = 0;
  START_IF()
      digitalWrite(13,1);
  DL_IF(100,2)
      digitalWrite(13,0);
      st_count++;
      WHILE_LOOP(st_count < 3, 1)
  DL_IF(100,3)
      digitalWrite(13,1);
  DL_IF(1000,4)
      digitalWrite(13,0);
      WHILE_LOOP(st_count >= 3, 3)
  END_IF(1000)
  
}
