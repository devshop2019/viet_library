#include "Vietduino_NewFunction.h"

#define DBL(x)      Serial.println(x)

#define ID_KXN            String(1.0)
//#define ID_STORE        String(2.0)
//#define ID_ONLINE       String(3.0)

#if defined(ID_KXN)
  #define QC_ID ID_KXN
#elif defined(ID_STORE)
  #define QC_ID ID_STORE
#elif defined(ID_ONLINE)
  #define QC_ID ID_ONLINE
#endif

int Start_pin = 2;
int Stop_pin = NUM_DIGITAL_PINS - 1;

unsigned char default_pin_value = LOW;
bool b_complete_bit = false;

typedef struct pin_info{
  unsigned char lastValue;
};

pin_info my_pin_array[NUM_DIGITAL_PINS];



void setAllPinPullUp(){
  for(int cf = Start_pin; cf <= Stop_pin; cf++){
    if(cf != 13){
      pinMode(cf, INPUT_PULLUP);
      my_pin_array[cf].lastValue = default_pin_value;
    }
  }
}

void setPin_pullup(unsigned char _Ref_value_){
  for(int cf = Start_pin; cf <= Stop_pin; cf++){
    if(cf == 13){
      continue;
    }
    
    if((cf%2) == _Ref_value_>0?1:0){
      pinMode(cf, INPUT_PULLUP);
    }else{
      pinMode(cf, OUTPUT);
      digitalWrite(cf,0);
    }    
  }
}

void readPin_pullup(unsigned char _Ref_value_){
  for(int cf = Start_pin; cf <= Stop_pin; cf++){
    if(cf == 13){
      continue;
    }
    
    if((cf%2) != ((_Ref_value_)>0)?0:1){
//      if(digitalRead(cf) == 0){
//        Serial.println("May be pin " + String (cf) + " error");
//        b_complete_bit = false;
//      }else;

      if(my_pin_array[cf].lastValue == !default_pin_value){
          continue;
      }else{
        int temValPin = digitalRead(cf);
        if(temValPin == default_pin_value){
          Serial.println("Pin LOW: " + String(cf));
          b_complete_bit = false;
        }else if(temValPin == !default_pin_value){
          my_pin_array[cf].lastValue = !default_pin_value;
        }else;
      }
      my_pin_array[cf].lastValue = default_pin_value;
        
    }else;
  }
}

/*
  @ Kịch bản: Khi các chân đều lên HIGH thì chương trình kết thúc. Nếu còn bất kỳ chân IO nào đang ở mức LOW, chương trình sẽ show ra Serial các IO đó.
*/
CREATE_FUNCTION(TestHigh){
  Serial.println("Test I/O high begin:");
  //Set default value of pin before the test begin!
  default_pin_value = LOW;
  setAllPinPullUp();
  M_DELAY(100);
  while(!b_complete_bit){
    M_DELAY(0);
    
    b_complete_bit = true;
    
    for(static int cf = Start_pin; cf <= Stop_pin; cf++){
      if(cf != 13){
        if(my_pin_array[cf].lastValue == !default_pin_value){
          continue;
        }else{
          int temValPin = digitalRead(cf);
          if(temValPin == default_pin_value){
            Serial.println("Pin LOW: " + String(cf));
            b_complete_bit = false;
          }else if(temValPin == !default_pin_value){
            my_pin_array[cf].lastValue = !default_pin_value;
          }else;
        }
        my_pin_array[cf].lastValue = default_pin_value;
      }
    }
    M_DELAY(1000);
  }

//  while(!b_complete_bit){
//    M_DELAY(0);
//    
//    b_complete_bit = true;
////    
////    for(int cf = Start_pin; cf <= Stop_pin; cf++){
////      if(cf != 13){
//        if(my_pin_array[cf].lastValue == !default_pin_value){
//          continue;
//        }else{
//          int temValPin = digitalRead(cf);
//          if(temValPin == default_pin_value){
//            Serial.println("Pin LOW: " + String(cf));
//            b_complete_bit = false;
//          }else if(temValPin == !default_pin_value){
//            my_pin_array[cf].lastValue = !default_pin_value;
//          }else;
//        }
//        my_pin_array[cf].lastValue = default_pin_value;
////      }
////    }
//  }
//
//  setPin_pullup(1);
//  M_DELAY(10);
//  readPin_pullup(1);
//  b_complete_bit = true;

  while(b_complete_bit){
    digitalWrite(13, digitalRead(13)^1);
    Serial.println(F("HIGH ALL"));
    M_DELAY(1000);
  }
  
//  TestHigh.disable();
  END_CREATE_FUNCTION
}

CREATE_FUNCTION(TestLow){
  Serial.println("Test I/O Low begin:");
  //Set default value of pin before the test begin!
  default_pin_value = HIGH;
  setAllPinPullUp();
  M_DELAY(100);
  
  b_complete_bit = false;
  while(!b_complete_bit){
    M_DELAY(100);
    
    static int cf;
    cf = 0;
    for(cf = Start_pin; cf <= Stop_pin; cf++){
      if(cf != 13){
        if(my_pin_array[cf].lastValue == !default_pin_value){
          continue;
        }else{
          int temValPin = digitalRead(cf);
          b_complete_bit = true;
          if(temValPin == default_pin_value){
//            Serial.println("Pin HIGH " + String(cf));
//            M_DELAY(0);
            b_complete_bit = false;
          }else if(temValPin == !default_pin_value){
            my_pin_array[cf].lastValue = !default_pin_value;
          }else;
        }
      }
    }
  }

  while(b_complete_bit){
    digitalWrite(13, digitalRead(13)^1);
    Serial.println(F("LOW ALL"));
    M_DELAY(1000);
  }
  
//  TestLow.disable();
  END_CREATE_FUNCTION
}

CREATE_FUNCTION(ShowPinState){
  static int cf;
  cf = 0;
  Serial.println(F("==========="));
  for(cf = Start_pin; cf <= Stop_pin; cf++){
    if(cf != 13){
      if(my_pin_array[cf].lastValue == !default_pin_value){
        continue;
      }else{
        int temValPin = digitalRead(cf);
//        if(temValPin == default_pin_value){
        if(my_pin_array[cf].lastValue == default_pin_value){
          Serial.print("Pin ");
          Serial.print(default_pin_value?"HIGH: ":"LOW: ");
          Serial.println(cf);
          M_DELAY(0);
        }
      }
    }
  }
  M_DELAY(1000);
  
  END_CREATE_FUNCTION
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("There are " + String(NUM_DIGITAL_PINS) + " pin need check");
  Serial.println(F("Welcome to Vietduino, test begin!"));
  Serial.println("Version firmWare test: " + QC_ID);
  Serial.println(F("Author: Mr.Ngoc - 0938 022 500"));
  pinMode(13, OUTPUT);
//  TestLow.disable();
  TestHigh.disable();

}

void loop() {
  // put your main code here, to run repeatedly:
  VIETDUINO_UPDATE;
}
