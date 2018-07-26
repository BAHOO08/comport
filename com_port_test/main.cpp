/********************************************************
*														*
*	тестовый проект по разбору последовательного порта	*
*														*
*********************************************************/

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
using namespace std;//терминалка для uart

HANDLE hSerial; // обработчик COM порта

void ReadCOM()
{
      DWORD iSize;
      char sReceivedChar;
      while (true)
      {
            ReadFile(hSerial, &sReceivedChar, 1, &iSize, 0);
			if (iSize > 0)
				cout << sReceivedChar;
      }
}

int main()
{
	LPCTSTR sPortName = L"COM4";  // пока 4 ком порт
	
	//открытие COM порт для чтения и записи как с файлом
	hSerial = ::CreateFile(sPortName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0); 

	//проверка на работоспособность ком порта (русский язык выдаёт кракозябры)
	if (hSerial == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			cout << "serial port does not exist.\n";
		}
		cout << "some other error occurred.\n";
	}


	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (!GetCommState(hSerial, &dcbSerialParams))
	{
		cout << "getting state error\n";
	}
	dcbSerialParams.BaudRate = CBR_256000;// было 9600
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
	if (!SetCommState(hSerial, &dcbSerialParams))
	{
		cout << "error setting serial port state\n";
	}

	char data[] = "Hello from C++";
	DWORD dwSize = sizeof(data);
	DWORD dwBytesWritten;
	LPOVERLAPPED ov;

	BOOL iRet = WriteFile(hSerial, data, dwSize, &dwBytesWritten, NULL);

	cout << dwSize << " Bytes in string. " << dwBytesWritten << " Bytes sended. " << endl;

	while (1)
	{
		ReadCOM();
	}
	return 0;
}


/*
обозначаем работу как с Unicod
int _tmain(int argc, _TCHAR* argv[])
*/