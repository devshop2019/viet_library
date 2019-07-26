// -- Pin change interrupt
#include <PciManager_Kxn.h>
#include <PciListenerImp_Kxn.h>

#define INPUT_PIN 8
#define INPUT_PIN2 3
int pintest = 2;
int pintestH = 4;

void onPinChange(byte changeKind);
void onPinChange2(byte changeKind);
unsigned long timer = 0;
unsigned long timer_ngoc = 0;
unsigned long countFre = 0;
unsigned long countFre2 = 0;

unsigned long time_xt = 0, time_t_count = 0;
unsigned long time_xt2 = 0, time_t_count2 = 0;

PciListenerImp_Kxn listener(INPUT_PIN, onPinChange,1);
PciListenerImp_Kxn listener2(INPUT_PIN2, onPinChange2,1);


void setup() {
  Serial.begin(9600);
  PciManager_Kxn.registerListener(&listener);
  PciManager_Kxn.registerListener(&listener2);
  Serial.println("Ready.");
  pinMode(pintest, OUTPUT);
  digitalWrite(pintest,0);
  pinMode(pintestH, OUTPUT);
  digitalWrite(pintestH,1);
}

void onPinChange(byte changeKind) {
  // -- changeKind can be: CHANGEKIND_HIGH_TO_LOW, or CHANGEKIND_LOW_TO_HIGH HIGH
//  Serial.print("pci : ");
//  Serial.println(changeKind);
    if(changeKind){
    time_t_count = micros();
  }else{
    time_xt = micros() - time_t_count;
  }
  countFre++;
}

void onPinChange2(byte changeKind) {
  // -- changeKind can be: CHANGEKIND_HIGH_TO_LOW, or CHANGEKIND_LOW_TO_HIGH HIGH
//  Serial.print("pci 2 : ");
//  Serial.println(changeKind);
    if(changeKind){
    time_t_count2 = micros();
  }else{
    time_xt2 = micros() - time_t_count2;
  }
  countFre2++;
}

void loop() {
//  readF();
  readT();
//  writef(50);
}

void readF(){
  if(millis() - timer_ngoc >= 1000){
    timer_ngoc = millis();
    Serial.println("NGoc " + String(timer_ngoc));
  }
  
  if(millis() - timer >= 1000){
    timer = millis();
    Serial.println(countFre);
    countFre = 0;
//    digitalWrite(pintest,digitalRead(pintest)^1);
  }
}

void readT(){
  if(millis() - timer_ngoc >= 1000){
    timer_ngoc = millis();
//    Serial.println("NGoc " + String(timer_ngoc));
  }
  
  if(millis() - timer >= 100){
    timer = millis();
    Serial.print(time_xt);
    Serial.print("\t");
    Serial.println(time_xt2);
    time_xt = 0;
    countFre = 0;
    
    time_xt2 = 0;
    countFre2 = 0;
//    digitalWrite(pintest,digitalRead(pintest)^1);
  }
}

void writef(unsigned long ttt){
  digitalWrite(pintest,digitalRead(pintest)^1);
  delayMicroseconds(ttt);
}

