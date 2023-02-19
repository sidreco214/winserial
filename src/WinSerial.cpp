#include "WinSerial.h"

WinSerial::WinSerial(const char* COMPort, const unsigned int& baud, const int& ByteSize, const int& StopBit, const int& Parity, const int& DTR)
: connection(false)
{
    hWinSerial = CreateFile(COMPort,
                    GENERIC_READ | GENERIC_WRITE,
                    0,
                    NULL,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL
                );
    //시리얼 연결 확인
    if(hWinSerial == INVALID_HANDLE_VALUE) {
        if(GetLastError() == ERROR_FILE_NOT_FOUND)  {
            printf("ERROR: Handle was not attached. Reason: %s not available.\n", COMPort);
        }
        else printf("ERROR!!");
    }
    else {
        //DCB 구조체 설정, 기본적으로 0이어야하는 게 있기 때문에 구조체 초기화
        DCB dcbSerialParams = {0}; 

        //현 상태 확인
        if(!GetCommState(hWinSerial, &dcbSerialParams)) {
            printf("failed to get current serial parameters!");
        }
        else {
            dcbSerialParams.BaudRate = baud;
            dcbSerialParams.ByteSize = ByteSize;
            dcbSerialParams.StopBits = StopBit;
            dcbSerialParams.Parity   = Parity;
            dcbSerialParams.fDtrControl = DTR;

            if(SetCommState(hWinSerial, &dcbSerialParams)) {
                //설정 저장한 구조체가 제대로 전달됨
                connection = true;
                PurgeComm(hWinSerial, PURGE_RXCLEAR | PURGE_TXCLEAR); //버퍼 비우기
                Sleep(ARDUINO_WAIT_TIME); //아두이노 보드가 리셋될때 까지 기다리기
                
            }
            else {
                //구조체 전달 실패
                printf("ALERT: Could not set Serial Port parameters");
            }
        }
    }
}

WinSerial::~WinSerial() {
    if(connection) {
        connection = false;
        CloseHandle(hWinSerial);
    }
}

int WinSerial::read(char* buffer, unsigned int buf_size) {
    DWORD bytesRead;
    unsigned int num; //읽을 데이터 수, 오버플로우 방지

    ClearCommError(hWinSerial, &(errors), &(status));  //COM포트 상태 읽기
    //읽을 게 있으면
    if(status.cbInQue > 0) {
        if(status.cbInQue > buf_size-1) num = buf_size-1; //배열 인덱스는 배열크기-1 까지
        else                            num = status.cbInQue;

        if(ReadFile(hWinSerial, buffer, num, &bytesRead, NULL)) return bytesRead;
    }
    //읽어올게 없는 경우, '0'을 int로 바꾸면 0이 아니니 이래도 괜찮음
    return 0;
}

bool WinSerial::send(const char* buffer, unsigned int buf_size) { 
    DWORD bytesSend;

    if(WriteFile(hWinSerial, (void*)buffer, buf_size-1, &bytesSend, 0)) return true;
    else {
        ClearCommError(hWinSerial, &(errors), &(status));
        return false;
    }
}
