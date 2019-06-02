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
      pinMode(cf, INPUT_PULLUP);
      my_pin_array[cf].lastValue = HIGH;
  }
}

CREATE_FUNCTION(ShowPinState){
  static int cf;
  cf = 0;
  Serial.println(F("==========="));
  for(cf = Start_pin; cf <= Stop_pin; cf++){
      if(my_pin_array[cf].lastValue == !default_pin_value){
        continue;
      }else{
        int temValPin = digitalRead(cf);
        if(my_pin_array[cf].lastValue == default_pin_value){
          Serial.print("Pin ");
          Serial.print(default_pin_value?"HIGH: ":"LOW: ");
          if(Stop_pin > 50 && cf >= 54){
            Serial.println("A" + String(cf-54));
          }else if(Stop_pin < 50 && cf >= 14){
            Serial.println("A" + String(cf-14));
          }else Serial.println(cf);
          M_DELAY(0);
        }
      }
  }
  M_DELAY(1000);
  ShowPinState.disable();
  
  END_CREATE_FUNCTION
}

CREATE_FUNCTION(blink_led_ok){
  pinMode(13, OUTPUT);

  while(1){
    digitalWrite(13, digitalRead(13)^1);
    M_DELAY(100);
  }
  END_CREATE_FUNCTION
}

/*
  @ Kịch bản: Khi các chân đều LOW thì chương trình kết thúc. Nếu còn bất kỳ chân IO nào đang ở mức HIGH, chương trình sẽ show ra Serial các IO đó.
*/
CREATE_FUNCTION(TestLow){
  Serial.println("Test I/O Low begin:");
  //Set default value of pin before the test begin!
  default_pin_value = HIGH;
  
  blink_led_ok.disable();
  setAllPinPullUp();
  M_DELAY(100);
  b_complete_bit = false;
  while(!b_complete_bit){
    M_DELAY(100);
    
    static int cf;
    cf = 0;
    static bool flag_check_again = false;
    flag_check_again = false;
    
    for(cf = Start_pin; cf <= Stop_pin; cf++){
//      if(cf != 13){
        if(my_pin_array[cf].lastValue == !default_pin_value){
          continue;
        }else{
          int temValPin = digitalRead(cf);
          b_complete_bit = true;
          if(temValPin == default_pin_value){
            flag_check_again = true;
          }else if(temValPin == !default_pin_value){
            my_pin_array[cf].lastValue = !default_pin_value;
          }else;
        }
//      }
    }
    if(flag_check_again){
      b_complete_bit = false;
    }
    
    if(!ShowPinState.available())ShowPinState.enable();
  }

  if(b_complete_bit)
//  pinMode(13,OUTPUT);
//  while(b_complete_bit){
//    digitalWrite(13, digitalRead(13)^1);
    Serial.println(F("Vietduino Test wire done, all pins are OK!"));
//    M_DELAY(1000);
//  }
  
  TestLow.disable();
  blink_led_ok.enable();
  END_CREATE_FUNCTION
}

CREATE_FUNCTION(CheckSeria_command){
  if(Serial.available()){
    char kk = Serial.read();
    if(b_complete_bit){
      if(!TestLow.available()){
        TestLow.enable();
      }
    }
  }
  
  END_CREATE_FUNCTION
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("There are " + String(NUM_DIGITAL_PINS) + " pin need check");
  Serial.println(F("Welcome to Vietduino, test begin!"));
  Serial.println("Version firmWare test: " + QC_ID);
  Serial.println(F("Author: Mr.Ngoc - 0938 022 500"));
  TestLow.disable();
  ShowPinState.disable();
  blink_led_ok.disable();
  testChap_setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  VIETDUINO_UPDATE;
}

void testChap_setup(){
  for(int cf = Start_pin; cf <= Stop_pin; cf++){
    if((cf%2) == 0){
      pinMode(cf, INPUT_PULLUP);
    }else{
      pinMode(cf, OUTPUT);
      digitalWrite(cf,0);
    }    
  }

  delay(10);
  for(int cf = Start_pin; cf <= Stop_pin; cf++){
    if((cf%2) == 0){
      if(digitalRead(cf) == 0){
//        Serial.println("May be pin " + String (cf) + " error");
        Serial.print("May be pin ");
        if(Stop_pin > 50 && cf >= 54){
          Serial.print("A" + String(cf-54));
        }else if(Stop_pin < 50 && cf >= 14){
          Serial.print("A" + String(cf-14));
        }else Serial.print(cf);

        Serial.println(" error");
        b_complete_bit = true;
      }else;
    }else;
  }

  delay(10);

  for(int cf = Start_pin; cf <= Stop_pin; cf++){
    if((cf%2) != 0){
      pinMode(cf, INPUT_PULLUP);
    }else{
      pinMode(cf, OUTPUT);
      digitalWrite(cf,0);
    }    
  }

  delay(10);
  for(int cf = Start_pin; cf <= Stop_pin; cf++){
    if((cf%2) != 0){
      if(digitalRead(cf) == 0){
//        Serial.println("May be pin " + String (cf) + " error");

        Serial.print("May be pin ");
        if(Stop_pin > 50 && cf >= 54){
          Serial.print("A" + String(cf-54));
        }else if(Stop_pin < 50 && cf >= 14){
          Serial.print("A" + String(cf-14));
        }else Serial.print(cf);

        Serial.println(" error");
        
        b_complete_bit = true;
      }else;
    }else;
  }
  
  for(int cf = Start_pin; cf <= Stop_pin; cf++){
    pinMode(cf, INPUT);
    my_pin_array[cf].lastValue = HIGH;
  }

  if(!b_complete_bit){
    Serial.println("Congratulation, Vietduino is working very good!");
    blink_led_ok.enable();
  }else{
    Serial.println("Some pin has proplem please check hardware again!");
  }
}
