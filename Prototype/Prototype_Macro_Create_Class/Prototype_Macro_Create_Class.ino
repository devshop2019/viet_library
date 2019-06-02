#include "Vietduino_Manager_Priority.h"
#include "Vietduino_NewFunction.h"

void runLed1(Vietduino_Task * me);
Vietduino_Task blink_led1(0,runLed1);

class blink_class : public Vietduino_Task{
  public:
  int pink;
  unsigned long time_delay;
  Vietduino_Task * p_run;
  
  blink_class(int _pin_, unsigned long time_delay_ = 1000);
//  {
////    Serial.begin(9600);
//    
//    this->pink = _pin_;
//    time_delay = time_delay_;
//    
//    
//    pinMode(this->pink, OUTPUT);
//    p_run = new Vietduino_Task(time_delay, &blink_class::run_1);
//    Vietduino_Manager_3.add(p_run);
////    Serial.println("begin led ");
////    Serial.println(pin);
//  }
  virtual ~blink_class(){
    Vietduino_Manager_3.remove(p_run);
    delete p_run;
  }
  static void run_1(Vietduino_Task * me);
  static void temrun(Vietduino_Task * me){
    
  }
  
  
};

blink_class::blink_class(int _pin_h, unsigned long time_delay_ = 1000):Vietduino_Task(time_delay_, &(blink_class::run_1)){
//    Serial.begin(9600);
    
    this->pink = _pin_h;
    time_delay = time_delay_;
    
    
    pinMode(this->pink, OUTPUT);
    p_run = new Vietduino_Task(time_delay, &(blink_class::run_1));
    Vietduino_Manager_3.add(this);
//    Serial.println("begin led ");
//    Serial.println(pin);
  }
  
void blink_class::run_1(Vietduino_Task * me){
    blink_class *tt = (blink_class*) me;
    int state = digitalRead(tt->pink);
    Serial.println("Run led pin---" + String(tt->pink));
    Serial.println("Run led 1---" + String(state));
    digitalWrite(tt->pink, !state);
    Serial.println("Run led 2---" + String(state));
  }



class blink_3led{
  public:
  blink_class * p_blink_class_1;
  blink_class * p_blink_class_2;
  
  blink_3led(int pin1, int pin2){
    p_blink_class_1 = new blink_class(pin1, 1000);
    p_blink_class_2 = new blink_class(pin2, 2000);
  }
  virtual ~blink_3led(){};
  
};

//blink_class kk(13, 2000);
blink_3led kk(12,13);

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  VIETDUINO_UPDATE;
}

void runLed1(Vietduino_Task * me){
  
}

CREATE_FUNCTION(Serial_F){
  Serial.begin(9600);
  Serial.println("Start");

  while(1){
    Serial.println("I live");
    M_DELAY(1000);
  }
  
  END_CREATE_FUNCTION
}
