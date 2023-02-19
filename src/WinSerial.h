/*
참고 https://playground.arduino.cc/Interfacing/CPPWindows/
*/
#ifndef WINSERIAL
#define WINSERIAL

#define ARDUINO_WAIT_TIME 2000 //아두이노 보드 리셋되는 시간

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

class WinSerial {
    private:
    bool connection; //COMPORT 연결 상태
    HANDLE hWinSerial; //WinSerial Handler
    COMSTAT status; //COMPORT에 대한 여러가지 상태 출력
    DWORD errors; //최근 에러를 저장

    public:
    WinSerial(const char* COMPort, const unsigned int& baud, const int& ByteSize, const int& StopBit, const int& Parity, const int& DTR);
    ~WinSerial();

    inline bool connected() {return this->connection;} //connection 값을 반환, class 내부 인라인 함수는 main에서 로드할 때 cpp까지 같이 include하거나 이렇게 헤더에 정의해야함
    int read(char* buffer, unsigned int buf_size); //시리얼 통신으로 읽어온 값을 버퍼에 저장후 읽은 바이트 수를 리턴, 읽을게 없으면 0리턴
    bool send(const char* buffer, unsigned int buf_size); //입력된 값을 시리얼 통신으로 출력, 성공하면 1출력
};

#endif