#include "Vietduino_Serial.h"
#include "Vietduino_Manager_Priority.h"
#include "arduino.h"

size_t Vietduino_Serial::write(uint8_t ch) {

    if(!myStr_write_p){
        myStr_write_p = new String();
    }

    *myStr_write_p += (char)ch;

    if((ch == '\n') || (myStr_write_p->length() >= 40)){
        String temStr = *myStr_write_p;
        delete myStr_write_p;
        myStr_write_p = 0;

        Struct_Serial_Queue tem_serial_queue;
        tem_serial_queue.printer = this->_duplicate;
        tem_serial_queue.string = temStr;

        queue_Serial_write.push(tem_serial_queue);
    }

    return sizeof(ch);
}

Vietduino_Serial::Vietduino_Serial():
    Print()
{
        if(H_Serial_p){
            delete H_Serial_p;
            H_Serial_p = 0;
        }
        H_Serial_p = &Serial;
        _duplicate = (Stream*)&Serial;
}

void Vietduino_Serial::begin(unsigned long _baud_){
    if(H_Serial_p){
        H_Serial_p->begin(_baud_);
    }

    #if defined( SoftwareSerial_h )
        if(S_Serial_p){
            S_Serial_p->begin(_baud_);
        }
    #endif
    Vietduino_Manager_2.add(&Serial_Task);
}

int Vietduino_Serial::available(){
    if(H_Serial_p){
        return H_Serial_p->available();
    }

    #if defined( SoftwareSerial_h )
        if(S_Serial_p){
            return S_Serial_p->available();
        }
    #endif

}

String Vietduino_Serial::readStringUntil(char _mychar_){
    while(this->available()){
        if(myStr_p == 0){
            myStr_p = new String();
        }

        char _temchar = this->Read();

        if((_temchar != '\r') && (_temchar != '\n')) *myStr_p += _temchar;

        if(_temchar == _mychar_){
            String _temString_ = *myStr_p;
            delete myStr_p;
            myStr_p = 0;

            return _temString_;
        }else{
            return "";
        }
    }
}

int Vietduino_Serial::Read(){
    if(H_Serial_p){
        return H_Serial_p->read();
    }

    #if defined( SoftwareSerial_h )
        if(S_Serial_p){
            char kk = S_Serial_p->read();
            return kk;
        }
    #endif

}

Vietduino_Serial::Vietduino_Serial(HardwareSerial &duplicate):
    Print()
{
    if(H_Serial_p){
        delete H_Serial_p;
        H_Serial_p = 0;
    }
    H_Serial_p = &duplicate;
    _duplicate = (Stream*)&duplicate;
}

#if defined( SoftwareSerial_h )

Vietduino_Serial::Vietduino_Serial(SoftwareSerial &duplicate):
    Print()
{
    if(S_Serial_p){
        delete S_Serial_p;
        S_Serial_p = 0;
    }

    S_Serial_p = &duplicate;
    _duplicate = (Stream*)&duplicate;
}

Vietduino_Serial::Vietduino_Serial(uint8_t _RxPin_, uint8_t _TxPin_):
    Print()
{
    if(S_Serial_p){
        delete S_Serial_p;
        S_Serial_p = 0;
    }
    S_Serial_p = new SoftwareSerial(_RxPin_, _TxPin_);
    _duplicate = (Stream*)S_Serial_p;
}
#endif

Vietduino_Queue <Struct_Serial_Queue> queue_Serial_write;// move to private when test done
Struct_Serial_Queue serial_queue;

Vietduino_Task  Serial_Task(0, Serial_funct);

void Serial_funct(Vietduino_Task* me){
    if(!queue_Serial_write.isEmpty()){
        Struct_Serial_Queue tem_serial_queue = queue_Serial_write.pop();
        tem_serial_queue.printer->print(tem_serial_queue.string);
    }
}

void readBuffer(Vietduino_Serial & Serialm, uint8_t * mBuffer,uint8_t _lenBuffer_, uint8_t * _headerBuffer_){
    static uint8_t isStart = false;
    static uint8_t prevc = 0;
    static uint8_t index = 0, dataLen = 0;


    if (Serialm.available()) {
        unsigned char c = Serialm.Read() & 0xff;
//        debug_Serialm("re: " + String (c,HEX));
        if (c == 0x55 && isStart == false) {
            if (prevc == 0xff) {
                index = 1;
                isStart = true;
            }
        } else {
            prevc = c;
            if (isStart) {
                if (index == 2) {
                    dataLen = c;
                    //          debug_Serialm("--------");
                    //          debug_Serialm(dataLen);
                    //          debug_Serialm("=datalen=");
                } else if (index > 2) {
                    dataLen--;
                    mBuffer[index - 2] = c;
                }
                //        writeBuffer(index, c);
//                mBuffer[index] = c;
            }
        }
        index++;
        if (index > 51) {
            index = 0;
            isStart = false;
        }
        if (isStart && dataLen == 0 && index > 3) {
            //      debug_Serialm("--------");
            //      debug_Serialm(isStart);
            //      debug_Serialm(dataLen);
            //      debug_Serialm(index);
            //      debug_Serialm("=show data=");
            isStart = false;
            ShowData(Serialm, mBuffer, _lenBuffer_);
            //      memset(mBuffer, 0, _lenBuffer_);
            index = 0;
        }
    }
}

void ShowData(Vietduino_Serial & Serialm, uint8_t * _buffer_, uint8_t _len_buffer_){
  uint8_t tem_lenBuffer = _len_buffer_ - 1;
  uint8_t temIndex = 0;

  Serialm.println();
  while(tem_lenBuffer){
    tem_lenBuffer--;
    Serialm.print(_buffer_[temIndex]);
    Serialm.print("\t");
    temIndex++;
  }

  Serialm.println();
}

String splitString(String v_G_motherString, String v_G_Command, String v_G_Start_symbol, String v_G_Stop_symbol, unsigned char v_G_Offset){
    unsigned char tem_Offset = v_G_Offset - 1;
    unsigned char lenOfCommand=v_G_Command.length();
    int posOfCommand=v_G_motherString.indexOf(v_G_Command);
    int PosOfStartSymbol=v_G_motherString.indexOf(v_G_Start_symbol,posOfCommand+lenOfCommand);
    if(posOfCommand < 0) return "";

    if(v_G_Offset == 0){
        String tem__ = v_G_motherString.substring(posOfCommand, PosOfStartSymbol);
        if(tem__ == v_G_Command){
            return tem__;
        }
        return  "";
    }
  
    while(tem_Offset > 0){
        tem_Offset--;
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

String splitString2(String v_G_motherString, String v_G_Command, String v_G_Start_symbol, String v_G_Stop_symbol, unsigned char v_G_Offset){
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