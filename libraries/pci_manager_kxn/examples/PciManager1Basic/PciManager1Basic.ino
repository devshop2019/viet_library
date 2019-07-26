// -- Pin change interrupt
#include <PciManager_Kxn.h>
#include <PciListenerImp_Kxn.h>

#define INPUT_PIN 3
int pintest = 2;
int pintestH = 4;

void onPinChange(byte changeKind);
PciListenerImp_Kxn listener(INPUT_PIN, onPinChange);


void setup() {
  Serial.begin(9600);
  PciManager_Kxn.registerListener(&listener);
  Serial.println("Ready.");
  pinMode(pintest, OUTPUT);
  digitalWrite(pintest,0);
  pinMode(pintestH, OUTPUT);
  digitalWrite(pintestH,1);
}

void onPinChange(byte changeKind) {
  // -- changeKind can be: CHANGEKIND_HIGH_TO_LOW, or CHANGEKIND_LOW_TO_HIGH HIGH
  Serial.print("pci : ");
  Serial.println(changeKind);
}

void loop() {
  Serial.println("NGoc");
  delay(1000);
}
