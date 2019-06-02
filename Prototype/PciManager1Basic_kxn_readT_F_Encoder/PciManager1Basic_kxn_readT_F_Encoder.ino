// -- Pin change interrupt
#include <PciManager_Kxn.h>
#include <PciListenerImp_Kxn.h>

#define INPUT_PIN 2
#define GET_RPM(x)    (x*60/(PPR_*2))
#define GET_RPM_IN_SEC(x, sec)    (GET_RPM(x)/sec)

unsigned long PPR_ = 20;//5;

void onPinChange(byte changeKind);

unsigned long timer = 0, timer_10s = 0;
unsigned long timer_ngoc = 0;
unsigned long countFre = 0, countFre_10s = 0, countFre_10s_print = 0;

unsigned long time_xt = 0, time_t_count = 0;
unsigned long Time_getSample_ms = 10000;

PciListenerImp_Kxn listener(INPUT_PIN, onPinChange,1);

void setup() {
  Serial.begin(9600);
  PciManager_Kxn.registerListener(&listener);
  Serial.println("Ready.");
//  pinMode(pintest, OUTPUT);
//  digitalWrite(pintest,0);

}

void onPinChange(byte changeKind) {
  // -- changeKind can be: CHANGEKIND_HIGH_TO_LOW, or CHANGEKIND_LOW_TO_HIGH HIGH
    if(changeKind){
    time_t_count = micros();
  }else{
    time_xt = micros() - time_t_count;
  }
  countFre++;
  countFre_10s++;
}

void loop() {
  readF();
//  readT();
//  writef(50);
}

void readF(){
  if(millis() - timer_ngoc >= 1000){
    timer_ngoc = millis();
    Serial.println("NGoc " + String(timer_ngoc));
  }
  
  if(millis() - timer_10s >= Time_getSample_ms){
    timer_10s = millis();
//    countFre_10s_print = countFre_10s*6/10;
    countFre_10s_print = GET_RPM_IN_SEC(countFre_10s, Time_getSample_ms/1000);
    
    countFre_10s = 0;
  }
  
  if(millis() - timer >= 1000){
    timer = millis();
    Serial.print(countFre_10s_print);
    Serial.print("\t");
//    Serial.println(countFre*60/(PPR_*2));
    Serial.println(GET_RPM(countFre));
    countFre = 0;
    countFre_10s_print = 0;
  }
}

void readT(){
  if(millis() - timer_ngoc >= 1000){
    timer_ngoc = millis();
//    Serial.println("NGoc " + String(timer_ngoc));
  }
  
  if(millis() - timer >= 100){
    timer = millis();
    Serial.print("time_xt");
    Serial.print("\t");
    Serial.println(time_xt);
    time_xt = 0;
    countFre = 0;
    
  }
}

void writef(unsigned long ttt){
//  digitalWrite(pintest,digitalRead(pintest)^1);
  delayMicroseconds(ttt);
}
