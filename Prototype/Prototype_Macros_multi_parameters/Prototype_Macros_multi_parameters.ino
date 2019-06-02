#define myMac(...)  \
  GET_myMac(__VA_ARGS__, myMac2, myMac1)(__VA_ARGS__)

#define GET_myMac(_1, _2, NAMEE, ...) NAMEE

#define myMac1(namee) Serial.println(namee)

//#define myMac2(namee, age)  \
//  Serial.print(namee);\
//  Serial.print(" \t ");\
//  Serial.println(age);

#define myMac2(namee, age)  \
  ((age)>20)?\
  myMac1(namee):\
  myMac1(age)
  
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
myMac("ngoc");

delay(1000);
myMac("Thach", 23);
delay(1000);
myMac("Thach",13);
}

void loop() {
  // put your main code here, to run repeatedly:

}
