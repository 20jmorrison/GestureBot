#include <windows.h>
#include <iostream>
#include <conio.h> // For _kbhit() and _getch()

using namespace std;

HANDLE hSerial;

void configPort();

int main()
{
    configPort();

    unsigned char byteToSend;
    char userInput;
    cout << "Use w a s d as your controls. Press 'q' to quit.\n\n";

    do
    {
        // Check if a key has been pressed
        if (_kbhit())
        {
            userInput = _getch();

            switch (userInput)
            {
            case 'w':
            {
                byteToSend = 0xAA;
                break;
            }
            case 'a':
            {
                byteToSend = 0xAB;
                break;
            }
            case 's':
            {
                byteToSend = 0xAC;
                break;
            }
            case 'd':
            {
                byteToSend = 0xAD;
                break;
            }
            default:
            {
                break;
            }
            }

            DWORD dwBytesWritten = 0;
            if (userInput != 'q' && !WriteFile(hSerial, &byteToSend, 1, &dwBytesWritten, NULL))
            {
                cout << "Error writing";
            }
        }

        // Delay to prevent busy-waiting
        Sleep(100);

    } while (userInput != 'q');

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
