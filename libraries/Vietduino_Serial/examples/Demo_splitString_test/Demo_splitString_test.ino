#include "Vietduino_Serial.h"

#define mSer    Serial
void setup() {
  // put your setup code here, to run once:
  mSer.begin(9600);
  mSer.println("Start");

  String hh = "ads,ngoc,123,456,789,";
  String gg = "";
//  hh = "";
  gg = splitString(hh, "ngo", ",", ",", 0);
  mSer.println(gg);
  mSer.println(F("___________________"));
  gg = splitString(hh, "ngoc", ",", ",", 1);
  mSer.println(gg);
  mSer.println(F("___________________"));
  gg = splitString(hh, "ngoc", ",", ",", 2);
  mSer.println(gg);
  mSer.println(F("___________________"));
  gg = splitString("", "ngoc", ",", ",", 2);
  mSer.println(gg);
  mSer.println(F("___________________"));
  gg = splitString(hh, "d", ",", ",", 0);
  mSer.println(gg);
  mSer.println(F("___________________"));
  gg = splitString(hh, "ngoc", ",", ",", 4);
  mSer.println(gg);
mSer.println(F("______=========______"));

  gg = splitString(hh, "", ",", ",", 0);
  mSer.println(gg);
  mSer.println(F("___________________"));

  gg = splitString("", "", ",", ",", 0);
  mSer.println(gg);
  mSer.println(F("___________________"));

  gg = splitString("", "", ",", ",", 1);
  mSer.println(gg);
  mSer.println(F("___________________"));

  gg = splitString("", "", ",", ",", 4);
  mSer.println(gg);
  mSer.println(F("___________________"));

  mSer.println("End");
}

void loop() {
  // put your main code here, to run repeatedly:

}
