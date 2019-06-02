#ifndef _VIETDUINO_H_SERIAL_H_
#define _VIETDUINO_H_SERIAL_H_

#include "Vietduino_Task.h"
#include "Vietduino_Queue.h"

#if defined(__AVR__) || defined(ARDUINO_ARCH_ESP8266)
    #include "SoftwareSerial.h"
#endif

typedef struct{
    Stream * printer;
    String string;
}Struct_Serial_Queue;

class Vietduino_Serial : public Print{
public:
    Vietduino_Serial();
    virtual ~Vietduino_Serial(){};
    void begin(unsigned long _baud_);

    int available();

    virtual String readStringUntil(char _mychar_);

    void readBuffer(uint8_t * _buffer_, uint8_t _lenBuffer_, uint8_t * _headerBuffer_);

    int Read();

    Vietduino_Serial(HardwareSerial & duplicate);

    #if defined( SoftwareSerial_h )
        Vietduino_Serial(SoftwareSerial & duplicate);
        Vietduino_Serial(uint8_t _RxPin_, uint8_t _TxPin_);
    #endif

  size_t write(uint8_t ch) override;

protected:
  Stream* _duplicate;
  HardwareSerial * H_Serial_p = 0;
  #if defined( SoftwareSerial_h )
    SoftwareSerial * S_Serial_p = 0;
  #endif
  String * myStr_p = 0;
  String * myStr_write_p = 0;
};

extern Vietduino_Queue <Struct_Serial_Queue> queue_Serial_write;// move to private when test done
extern Struct_Serial_Queue serial_queue;
extern Vietduino_Task  Serial_Task;

extern void Serial_funct(Vietduino_Task* me);
extern void readBuffer(Vietduino_Serial & Serialm, uint8_t * mBuffer,uint8_t _lenBuffer_, uint8_t * _headerBuffer_);
extern void ShowData(Vietduino_Serial & Serialm, uint8_t * _buffer_, uint8_t _len_buffer_);
extern String splitString(String v_G_motherString, String v_G_Command, String v_G_Start_symbol, String v_G_Stop_symbol, unsigned char v_G_Offset);
extern String splitString2(String v_G_motherString, String v_G_Command, String v_G_Start_symbol, String v_G_Stop_symbol, unsigned char v_G_Offset);

#endif
