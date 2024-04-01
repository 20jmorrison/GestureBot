#include <windows.h>
#include <iostream>
#include <fstream>
#include <conio.h> // For _kbhit() and _getch()

using namespace std;

HANDLE hSerial;

void configPort();

int main()
{
    configPort();

    unsigned char byteToSend;
    DWORD dwBytesWritten = 0;

    string fileContents;
    ifstream myFile;
    while (1)
    {
        myFile.open("..\\CV-Gesture-Control\\gestureSharing.txt");
        myFile >> fileContents;
        std::cout << fileContents << endl;
        if (fileContents == "pointer_thumb"){
            byteToSend = 0xAA;
        }
        else if (fileContents == "middle_thumb"){
            byteToSend = 0xAB;
        }
        else if (fileContents == "pinky_thumb"){
            byteToSend = 0xAC;
        }
        else if (fileContents == "ring_thumb"){
            byteToSend = 0xAD;
        }
        else if (fileContents == "open_hand") {
            byteToSend = 0xAE;
        }
        myFile.close();
        WriteFile(hSerial, &byteToSend, 1, &dwBytesWritten, NULL);
    }

    CloseHandle(hSerial);

    return 0;
}

void configPort()
{
    hSerial = CreateFile("COM4",
                         GENERIC_WRITE,
                         0,
                         0,
                         OPEN_EXISTING,
                         FILE_ATTRIBUTE_NORMAL,
                         0);
    if (hSerial == INVALID_HANDLE_VALUE)
    {
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            cout << "Serial port doesn't exist";
        }
    }

    // Set timeouts
    COMMTIMEOUTS timeout = {0};

    timeout.WriteTotalTimeoutConstant = 50;
    timeout.WriteTotalTimeoutMultiplier = 10;

    SetCommTimeouts(hSerial, &timeout);

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams))
    {
        cout << "Error getting state";
    }
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if (!SetCommState(hSerial, &dcbSerialParams))
    {
        cout << "\nerror setting serial port state";
    }
}
