/*
참고 https://playground.arduino.cc/Interfacing/CPPWindows/
*/

#include <stdio.h>
#include <string>
using std::string;
#include "src/WinSerial.h"

#define BUF_LENGTH 256

const string Arg[] = {"-p",                              //COMPORT
                      "-b",                              //Baud Rate
                      "-B",                              //Byte Size
                      "-S0", "-S1", "-S2",               //Stop Bit
                      "-P0", "-P1", "-P2", "-P3", "-P4", //Parity
                      "-D0", "-D1", "-D2",               //DTR
                      };

enum _arg {
    p = 0,
    b,
    B,
    S0, S1, S2,
    P0, P1, P2, P3, P4,
    D0, D1, D2
};

int main(int arc, char* argv[]) {
    char* comport = argv[0];
    //printf("arc: %d\n",arc);
    //printf("%s\n",comport);
    unsigned int baud = 9600, ByteSize = 8, StopBit = ONESTOPBIT,
                 Parity = NOPARITY, DTR = DTR_CONTROL_ENABLE;

    //argv 의 첫 문자열은 winserial.exe, arc는 배열 원소 갯수이니
    //그래서 1번 인덱스 부터 arc-1 인덱스까지 검색
    for(int i=1; i<arc-1; i++) {
        int arg = -1;
         for(int j=0; j<sizeof(_arg)/sizeof(p); j++) {if(Arg[j] == argv[i]) arg = j; break;}
        
        switch(arg) {
            default:
            break;

            case p:
            comport = argv[++i];
            break;

            case b:
            baud = atoi(argv[++i]);
            break;

            case B:
            ByteSize = atoi(argv[++i]);
            break;


            case S0:
            StopBit = ONESTOPBIT;
            break;

            case S1:
            StopBit = ONE5STOPBITS;
            break;

            case S2:
            StopBit = TWOSTOPBITS;
            break;


            case P0:
            Parity = NOPARITY;
            break;

            case P1:
            Parity = ODDPARITY;
            break;

            case P2:
            Parity = EVENPARITY;
            break;

            case P3:
            Parity = MARKPARITY;
            break;

            case P4:
            Parity = SPACEPARITY;
            break;


            case D0:
            DTR = DTR_CONTROL_DISABLE;
            break;

            case D1:
            DTR = DTR_CONTROL_ENABLE;
            break;

            case D2:
            DTR = DTR_CONTROL_HANDSHAKE;
            break;
        }
    }

    //comport 입력이 없는 경우
    if(string(comport) == argv[0]) {printf("ERROR: COMPORT is not connncted"); return 0;}
    //printf("TEST Baud Rate: %d, Byte Size: %d\n%s is connected\n", baud, ByteSize, comport);
    
    WinSerial Serial(comport, baud, ByteSize, StopBit, Parity, DTR);

    if(Serial.connected()) printf("Baud Rate: %d, Byte Size: %d\n%s is connected\n", baud, ByteSize, comport);
    
    char buffer[BUF_LENGTH] = "";
    int readResult = 0;

    while(Serial.connected()) {
        readResult = Serial.read(buffer, BUF_LENGTH);
        buffer[readResult] = 0; //저장한 데이터 바로 뒤의 바이트를 초기화
        printf("%s", buffer);
    }
    return 0;
}