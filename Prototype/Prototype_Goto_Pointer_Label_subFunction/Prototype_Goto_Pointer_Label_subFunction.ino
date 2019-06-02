/*
 * Note:
 * Đã gọi được sub funtion trong function
 * Không gọi sub function này trong sub function khác được
 * Các subfuntion được gọi phải khai báo phia trên function đang gọi.
*/
#include "Vietduino_NewFunction.h"

#define DBn(x)   Serial.println(x)
#define DBn(x)

#define LedPin    13
//bool binkLed(Vietduino_Task * me);

#define M_DELAY2(delayMillis) \
  do { \
      te->delay(delayMillis); \
        do { \
          __label__ jump_point; \
          if(p!=0);\
          te->setJumPoint(&& jump_point); \
          DBn("Blink3 "  + String(millis()));\
          return false; \
          jump_point: ; \
          DBn("Blink4 "  + String(millis()));\
          te->setJumPoint((void*)0);\
        } while (false);\
    } while (false)

#define M_DELAY3(delayMillis) \
  do { \
      te->delay(delayMillis); \
        do { \
          __label__ jump_point; \
          if(p!=0);\
          te->p_currentPoint =(&& jump_point); \
          DBn("Blink3 "  + String(millis()));\
          return false; \
          jump_point: ; \
          DBn("Blink4 "  + String(millis()));\
          te->setJumPoint((void*)0);\
        } while (false);\
    } while (false)

#define M_DELAY4(delayMillis) \
  do { \
      te->delay(delayMillis); \
        do { \
          __label__ jump_point; \
          te->p_currentPoint =(&& jump_point); \
          DBn("Blink3 "  + String(millis()));\
          return; \
          jump_point: ; \
          DBn("Blink4 "  + String(millis()));\
          te->setJumPoint((void*)0);\
        } while (false);\
    } while (false)

#define M_DELAY5(delayMillis) \
  do { \
      te->delay(delayMillis); \
        do { \
          __label__ jump_point; \
          if(te->p_currentPoint == (void*)0)\
          te->p_currentPoint =(&& jump_point); \
          DBn("Blink3 "  + String(millis()));\
          return; \
          jump_point: ; \
          DBn("Blink4 "  + String(millis()));\
          te->setJumPoint((void*)0);\
        } while (false);\
    } while (false)



#define M_SET_LAST_JUM  \
        do { \
          __label__ jump_point; \
          te->p_lastPoint = (&& jump_point);\
          return ; \
          jump_point: ; \
          DBn("Blink4 "  + String(millis()));\
          te->p_lastPoint = ((void*)0);\
        } while (false);

#define M_SET_LAST_JUM2  \
        do { \
          __label__ jump_point; \
          if(te->p_lastPoint == (void*)0)\
          te->p_lastPoint = (&& jump_point);\
          return ; \
          jump_point: ; \
          DBn("Blink4 "  + String(millis()));\
          te->p_lastPoint = ((void*)0);\
        } while (false);
        
#define CREATE_SUB_FUNCTION(nameFunction) \
                                          void nameFunction(Vietduino_Task * me);\
                                          void nameFunction(Vietduino_Task * me){\
                                          M_LOOP()

#define END_CREATE_SUB_FUNCTION \
                                          }te->setJumPoint(te->p_lastPoint);

//#define CALL_SUBFUNCTION(x)   \
//                                          x(me);\
//                                          M_SET_LAST_JUM2

#define MM_DELAY(x)                       M_DELAY4(x)                                          

#define CALL_SUBFUNCTION(x)   \
                                          x(me);\
                                          M_SET_LAST_JUM2\
                                          MM_DELAY(0);
                                          
void setup() {
  // put your setup code here, to run once:
  pinMode(LedPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Start");

}

void loop() {
  // put your main code here, to run repeatedly:
  VIETDUINO_UPDATE;
}

CREATE_SUB_FUNCTION(binkLed3){

  digitalWrite(LedPin,1);
  Serial.println("Blink 3 ___ " + String(millis()));
  MM_DELAY(1000);
  digitalWrite(LedPin,0);
  Serial.println("Blink 3 +++ "  + String(millis()));
  MM_DELAY(1000);
  
  END_CREATE_SUB_FUNCTION
}

CREATE_SUB_FUNCTION(binkLed2){

  digitalWrite(LedPin,1);
  Serial.println("Blink___2 " + String(millis()));
  
  MM_DELAY(1000);
  CALL_SUBFUNCTION(binkLed3);
  digitalWrite(LedPin,0);
  Serial.println("Blink+++2 "  + String(millis()));
  
  MM_DELAY(1000);
  CALL_SUBFUNCTION(binkLed3);
  MM_DELAY(0);
  
  END_CREATE_SUB_FUNCTION
}

CREATE_SUB_FUNCTION(binkLed){
  digitalWrite(LedPin,1);
  Serial.println("Blink___ " + String(millis()));
  MM_DELAY(1000);
  digitalWrite(LedPin,0);
  Serial.println("Blink+++ "  + String(millis()));
  MM_DELAY(1000);
  END_CREATE_SUB_FUNCTION
}

CREATE_FUNCTION(test1){

  Serial.println("Blink1 "  + String(millis()));

  MM_DELAY(1000);
  CALL_SUBFUNCTION(binkLed2);

  Serial.println("Blink2 "  + String(millis()));

  MM_DELAY(1000);
  CALL_SUBFUNCTION(binkLed2);
  END_CREATE_FUNCTION;
}
