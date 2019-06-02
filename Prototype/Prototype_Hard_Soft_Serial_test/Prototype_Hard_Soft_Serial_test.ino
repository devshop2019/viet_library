//trong 444 - 9

#include "SoftwareSerial.h"
//464 - 11

//SoftwareSerial mySer(2,3);
////1926 - 129

// Define where debug output will be printed.
#define DEBUG_PRINTER Serial

// Setup debug printing macros.
#ifdef DHT_DEBUG
  #define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
  #define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
  #define DEBUG_PRINT(...) {}
  #define DEBUG_PRINTLN(...) {}
#endif

#define writeToSerial(...) { port->write(__VA_ARGS__); }

enum{
  HARD_SERIAL,
  SOFT_SERIAL
};
class test{
  public:
  uint8_t typeSerial;
  Stream * port;
  
  inline test(){
    port = &Serial;
    typeSerial = HARD_SERIAL;
  }
  
  inline test(HardwareSerial * serial){
    port = serial;
    typeSerial = HARD_SERIAL;
  }

  inline test(SoftwareSerial * serial){
    port = serial;
    typeSerial = SOFT_SERIAL;
  }

  inline test(int rxPin, int txPin){
    SoftwareSerial * yy = new SoftwareSerial(rxPin, rxPin);
    port = yy;
    typeSerial = SOFT_SERIAL;
  }

  inline void begin(unsigned long _tembaud){
    if(typeSerial == HARD_SERIAL){
      HardwareSerial * tt = port;
      tt->begin(_tembaud);
    }else{
      SoftwareSerial * tt = port;
      tt->begin(_tembaud);
    }
  }

  void readData(){
    if(port->available()){
      port->println(port->read(), HEX);
      char uu[4] = "1234";
      writeToSerial(uu,4);
    }
  }
};

class test2{
  public:
  HardwareSerial * hs;
  SoftwareSerial * ss;
  uint8_t typeSerial;
  
  inline test2(){
    hs=&Serial;
    typeSerial = HARD_SERIAL;
  }
  
  inline test2(HardwareSerial * gg){
    hs=gg;
    typeSerial = HARD_SERIAL;
  }

  inline test2(int rxPin, int txPin){
    ss= new SoftwareSerial(rxPin, rxPin);
    typeSerial = SOFT_SERIAL;
  }

  inline void begin(unsigned long _tembaud){
    if(typeSerial == HARD_SERIAL){
      hs->begin(_tembaud);
    }else{
      ss->begin(_tembaud);
    }
  }
};

//test mySer(&Serial);
//1356  - 186
//test mySer(&kk);
//test mySer(2,3);
//2802 - 108

//test2 mySer(2,3);
//test2 mySer3(2,3);
//2810 - 110

test mySer;

void setup() {
  mySer.begin(9600);
//  mySer
//  mySer.readData();
  
  // put your setup code here, to run once:
//  Serial.begin(9600);
//  
//  Serial.println("Serial 9600");
//  
//  Stream *st_pp = &Serial;
//  st_pp->println("st_pp 9600");
//
//  HardwareSerial * hst_pp;
//
//  hst_pp = st_pp;
//
//  delay(1000);
//
//  while(!Serial.available()) delay(100);
//  
//  hst_pp->begin(115200);
//  hst_pp->println("hst_pp 115200");
//  
//  
//  pinMode(13, OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
//  digitalWrite(13, digitalRead(13)^1);
//  delay(100);
  mySer.readData();
}
