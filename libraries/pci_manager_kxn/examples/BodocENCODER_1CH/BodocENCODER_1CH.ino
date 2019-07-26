/*
 *Code đang dùng cho Pro mini 3V3 - 8Mhz
 *
 *Nguyên lý:
 *    _ Đọc tần số của rada
 *    - Đo pin (1 cell 18650) chân RAW
 *    _ Gửi RF UART 0
 *    
 *
 *Nếu dùng board khác cần chỉnh lại các thông số cho phù hợp:
 *#define R_GND  9.70 //10k
  #define R_RAW  9.88 //10k
  #define V_REF   3.31//5.0   //V
  #define V_RAW_WARN  3700   // mV
 *
*/

// -- Pin change interrupt
#include <PciManager.h>
#include <PciListenerImp.h>
#include <EEPROM.h>

#define RADA_ID   1
#define rada_PIN  8
#define analogVRaw_pin  A0
#define mSerial   Serial
#define warning_fre_address 0
#define MAXFREQ   3000

#define DATA  "DATA"
#define WARNING  "WARNING"

void onPinChange_rada(byte changeKind);
unsigned long timer = 0;
unsigned long timer_ngoc = 0, timer_VoltRaw = 0;
unsigned long timer_VoltRaw_max = 100;
unsigned long countFre = 0, last_countFre = 0;

unsigned long time_t = 0, time_t_count = 0;
unsigned long warning_fre = 1;
String mSerial_temStr = "";

#define R_GND  9.70 //10k
#define R_RAW  9.88 //10k
#define R_CALCULATE   (R_GND/(R_GND+R_RAW)) 
#define V_REF   3.31//5.0   //V
#define V_RAW_WARN  3700   // mV

const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
unsigned char readIndex = 0;              // the index of the current reading
unsigned char tem_readIdex = 0;
int total = 0;                  // the running total
float average = 0;                // the average
int last_Volt = 0;

int inputPin = A3;

PciListenerImp listener_rada(rada_PIN, onPinChange_rada,1);


void setup() {
  mSerial.begin(9600);
//  pinMode(analogVRaw_pin, INPUT);
  PciManager.registerListener(&listener_rada);
  EEPROM.get(warning_fre_address, warning_fre);
  mSerial.println("Ready." + String(warning_fre));

  if((warning_fre < 1) || (warning_fre > MAXFREQ)){
    warning_fre = 1;
    EEPROM.put(warning_fre_address, warning_fre);
  }
  // analog reset buffer
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  
//  int v2 = rand() % 100 + 1;
}

void onPinChange_rada(byte changeKind) {
  // -- changeKind can be: CHANGEKIND_HIGH_TO_LOW, or CHANGEKIND_LOW_TO_HIGH HIGH
//  Serial.print("pci : ");
//  Serial.println(changeKind);
  if(changeKind){
    time_t_count = micros();
  }else{
    time_t = micros() - time_t_count;
  }
  countFre++;
}

void loop() {
  readSerial();
  readF();
  checkVoltRaw();
//  readT();
}

void readF(){
  if(millis() - timer >= 1000){
    timer = millis();
//    Serial.println(countFre);
    if(countFre >= warning_fre){
      mSerial.print(DATA);
      mSerial.print(WARNING);
      mSerial.println("," +String(RADA_ID) + ".,"+ String(countFre) + ".");
    }
    last_countFre = countFre;
    countFre = 0;
  }
}

void readSerial(){
  if(mSerial.available()){
    char temchar = mSerial.read();
    mSerial_temStr += temchar;

    if(temchar == '\n'){
      if(mSerial_temStr.indexOf(DATA + String(RADA_ID)) > -1){        //    DATA1
       mSerial.println(DATA + String(RADA_ID) + "," + String(last_countFre) + ".");
      }else if(mSerial_temStr.indexOf("SETRDA" + String(RADA_ID)) > -1){                // SETRDA1,100.
        warning_fre = (unsigned long)splitString(mSerial_temStr, "SETRDA", "," , ".", 0).toInt();
        EEPROM.put(warning_fre_address, warning_fre);
        mSerial.println("OKSETRDA" + String(RADA_ID) + "," + String(warning_fre) + ".");
      }else if(mSerial_temStr.indexOf("READVOLT" + String(RADA_ID)) > -1){
        mSerial.println("OKREADVOLTRDA" + String(RADA_ID) + "," + String(last_Volt) + ".");
      }else if(mSerial_temStr.indexOf("READWARNFRDA" + String(RADA_ID)) > -1){
        mSerial.println("OKREADRADAWARNF" + String(RADA_ID) + "," + String(warning_fre) + ".");
      }else;
      

      mSerial_temStr = "";
    }
    
  }
}

void checkVoltRaw(){
  if(millis() - timer_VoltRaw >= timer_VoltRaw_max){
    float val = 0;
    timer_VoltRaw = millis();
    total = total - readings[readIndex];
    // read from the sensor:
    readings[readIndex] = analogRead(inputPin);
    // add the reading to the total:
    total = total + readings[readIndex];
    // advance to the next position in the array:
    readIndex = readIndex + 1;
    tem_readIdex++;
  
    // if we're at the end of the array...
    if (readIndex >= numReadings) {
      // ...wrap around to the beginning:
      readIndex = 0;
    }
    if (tem_readIdex >= numReadings) {
      tem_readIdex = numReadings;
    }
  
    // calculate the average:
    average = total / tem_readIdex;
    long average2 = average * 1000;
    average2 = average2 * ((R_GND + R_RAW) / R_RAW) * V_REF / 1023;
    last_Volt = average2;
  
    if(average2 <= V_RAW_WARN){
      mSerial.println("RDALOWVOLT," + String(RADA_ID) + ".," + String(last_Volt) + ".");
      timer_VoltRaw_max = 2000;
//      mSerial.println("RDALOWVOLT," + String(RADA_ID) + ".," + String(aa * V_REF/1023) + ".");
    }
  }
}

String splitString(String v_G_motherString, String v_G_Command, String v_G_Start_symbol, String v_G_Stop_symbol, unsigned char v_G_Offset){
 unsigned char lenOfCommand=v_G_Command.length();
    unsigned char posOfCommand=v_G_motherString.indexOf(v_G_Command);
    int PosOfStartSymbol=v_G_motherString.indexOf(v_G_Start_symbol,posOfCommand+lenOfCommand);
  
    while(v_G_Offset>0){
        v_G_Offset--;
        PosOfStartSymbol=v_G_motherString.indexOf(v_G_Start_symbol,PosOfStartSymbol+1);
    }
  
  if(v_G_Stop_symbol != ""){
    int PosOfStopSymbol=v_G_motherString.indexOf(v_G_Stop_symbol,PosOfStartSymbol+1);
    if(PosOfStopSymbol == -1){
      return "";
    }else;    
    return v_G_motherString.substring(PosOfStartSymbol+1,PosOfStopSymbol);
  }else{
    return v_G_motherString.substring(PosOfStartSymbol+1);
  }
}

void readT(){
  if(millis() - timer_ngoc >= 1000){
    timer_ngoc = millis();
//    Serial.println("NGoc " + String(timer_ngoc));
  }
  
  if(millis() - timer >= 100){
    timer = millis();
    mSerial.println(time_t);
    time_t = 0;
    countFre = 0;
//    digitalWrite(pintest,digitalRead(pintest)^1);
  }
}


// Tham khảo smooth https://www.arduino.cc/en/Tutorial/Smoothing

//const int numReadings = 10;
//
//int readings[numReadings];      // the readings from the analog input
//unsigned char readIndex = 0;              // the index of the current reading
//unsigned char tem_readIdex = 0;
//int total = 0;                  // the running total
//float average = 0;                // the average
//
//int inputPin = A0;
//
//void setup() {
//  // initialize serial communication with computer:
//  Serial.begin(9600);
//  // initialize all the readings to 0:
//  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
//    readings[thisReading] = 0;
//  }
//}
//
//void loop() {
//  // subtract the last reading:
//  total = total - readings[readIndex];
//  // read from the sensor:
//  readings[readIndex] = analogRead(inputPin);
//  // add the reading to the total:
//  total = total + readings[readIndex];
//  // advance to the next position in the array:
//  readIndex = readIndex + 1;
//  tem_readIdex++;
//
//  // if we're at the end of the array...
//  if (readIndex >= numReadings) {
//    // ...wrap around to the beginning:
//    readIndex = 0;
//  }
//
//  if (tem_readIdex >= numReadings) {
//    tem_readIdex = numReadings;
//  }
//
//  // calculate the average:
//  average = total / tem_readIdex;
//  float average2 = average * 1000;
//  average2 = average2 * ((9.7+9.88)/9.88)*5/1023;
//
//  
//  // send it to the computer as ASCII digits
//  Serial.println((int)average2);
//  delay(100);        // delay in between reads for stability
//}
