/********************************************************
*														*
*	тестовый проект по разбору последовательного порта	*
*														*
*********************************************************/

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
using namespace std;//терминалка дл€ uart

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

void SettingCOM() {
	
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
}

int main()
{
	LPCTSTR sPortName = L"COM4";  // пока 4 ком порт
	/**********************************************************************************************
	*
	*			LPCTSTR Ц указатель на константную строку, без UNICODE. 
	*			ќт фразы long pointer constant TCHAR string. Ёто надстройка функции LPCSTR.
	*
	*			TCHAR Ц символьный тип Ч аналог char и wchar_t.
	*
	*			wchar_t Ч это тип данных стандарта ANSI/ISO C 
	*			(а также использующийс€ в других €зыках программировани€) дл€ представлени€ широких символов
	*
	*			L перед строкой означает строку двухбайтных символов.
	*----------------------	ѕример -------------------------------------------------------
	*			
	**			"A"    = 41
	*						"ABC"  = 41 42 43
	*						L"A"   = 00 41
	*						L"ABC" = 00 41 00 42 00 43
	*
	************************************************************************************************************/
	//открытие COM порт дл€ чтени€ и записи как с файлом
	hSerial = ::CreateFile(sPortName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0); 

	//проверка на работоспособность COM порта (русский €зык выдаЄт кракоз€бры)
	if (hSerial == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			cout << "serial port does not exist.\n";
		}
		cout << "some other error occurred.\n";
	}

	// Ќастройка соединени€
	SettingCOM();

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