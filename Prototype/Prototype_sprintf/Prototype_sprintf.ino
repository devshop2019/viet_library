//#include <stdarg.h>
#include <stdio.h>

const char TEMP_HUMI_COMMAND[] = "TEMP_HUMI %d.%d %d.%d\n";
#define MAX_LENGTH        100
#define EXP               1000  // độ chính xác đến 3 chữ số thập phân (1
// - end define
 
// Trả về phần nguyên và phần thực của một số thực, các biến prefix và suffix là "tham biến" (có dấu & ở trước)
// nên giá trị của 2 biến đó sẽ thay đổi khi chạy xong hàm
void parse_float(float val, int &prefix, int &suffix) {
  prefix = int(val);
  suffix = (val - prefix) * EXP;
}
 
//Hàm in giá trị temp và humi ra Serial
//Chúng ta không thể in một biến kiểu float ra ngoài một cách trực tiếp được. Vì họ AVR's không hỗ trợ việc in một biến kiểu float hoặc tương tự.
// Giải pháp: Ta sẽ cắt số thực thành 2 phần: phần nguyên, và phần thực, sau đó in phần nguyên và phần thực lần lượt ra
void print_temp_humi(float temp, float humi) {
  int temp_pre, temp_suf, humi_pre, humi_suf;
  
  parse_float(temp, temp_pre, temp_suf);  // 2 biến prefix, suffix là ánh xạ đến vùng nhớ của biến temp_pre, temp_suf
  parse_float(humi, humi_pre, humi_suf);
 
  char buffer[MAX_LENGTH];
  sprintf(buffer, TEMP_HUMI_COMMAND, temp_pre, temp_suf, humi_pre, humi_suf);
  Serial.println(buffer); 
}
 
void setup () {
    Serial.begin(9600);
    String kk = "faff ";
    float hh = 0.25f + 0.3f;
    kk = kk + String(hh);
    kk = hh;
    Serial.println(hh);
    Serial.println(kk);
//    Serial.println("af" + String(hh) );
    print_temp_humi(23.231, 87.21);
    test2();
}
void loop () {/*do nothing*/}

void p(char *fmt, ... ){
        char buf[128]; // resulting string limited to 128 chars
        va_list args;
        va_start (args, fmt );
        vsnprintf(buf, 128, fmt, args);
        va_end (args);
        Serial.print(buf);
}

void p(const __FlashStringHelper *fmt, ... ){
  char buf[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt);
#ifdef __AVR__
  vsnprintf_P(buf, sizeof(buf), (const char *)fmt, args); // progmem for AVR
#else
  vsnprintf(buf, sizeof(buf), (const char *)fmt, args); // for the rest of the world
#endif
  va_end(args);
  Serial.print(buf);
}

void test2(){
//  https://playground.arduino.cc/main/printf
  p("%s", "Hello world");
  p("%s\n", "Hello world"); // with line break
  unsigned long a=0xAFFFFFFF;
//  a=1;
  unsigned int b = 0xFFFF;
  p("Decimal a: %l\nDecimal unsigned a: %l\n", a, a); 
  p("Hex a: %x\n", a); 

  p("Decimal b: %i\nDecimal unsigned b: %u\n", b, b); 
  p("Hex b: %x\n", b); 
  
  p(F("Best to store long strings to flash to save %s\n"),"memory");

  
//http://arduino.vn/bai-viet/408-mot-chuoi-voi-noi-dung-duoc-quy-dinh-san-trong-arduino-formatted-string
  char buffer[MAX_LENGTH];
//  memset(buffer,0,1);
  sprintf(buffer, "D %lu\n", a);
  Serial.println(buffer); 
  Serial.println(a, HEX);
  Serial.println(a);
}
