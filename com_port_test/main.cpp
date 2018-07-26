/********************************************************
*														*
*	�������� ������ �� ������� ����������������� �����	*
*														*
*********************************************************/

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
using namespace std;//���������� ��� uart

HANDLE hSerial; // ���������� COM �����

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
	dcbSerialParams.BaudRate = CBR_256000;// ���� 9600
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
	LPCTSTR sPortName = L"COM4";  // ���� 4 ��� ����
	/**********************************************************************************************
	*
	*			LPCTSTR � ��������� �� ����������� ������, ��� UNICODE. 
	*			�� ����� long pointer constant TCHAR string. ��� ���������� ������� LPCSTR.
	*
	*			TCHAR � ���������� ��� � ������ char � wchar_t.
	*
	*			wchar_t � ��� ��� ������ ��������� ANSI/ISO C 
	*			(� ����� �������������� � ������ ������ ����������������) ��� ������������� ������� ��������
	*
	*			L ����� ������� �������� ������ ����������� ��������.
	*----------------------	������ -------------------------------------------------------
	*			
	**			"A"    = 41
	*						"ABC"  = 41 42 43
	*						L"A"   = 00 41
	*						L"ABC" = 00 41 00 42 00 43
	*
	************************************************************************************************************/
	//�������� COM ���� ��� ������ � ������ ��� � ������
	hSerial = ::CreateFile(sPortName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0); 

	//�������� �� ����������������� COM ����� (������� ���� ����� ����������)
	if (hSerial == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			cout << "serial port does not exist.\n";
		}
		cout << "some other error occurred.\n";
	}

	// ��������� ����������
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
���������� ������ ��� � Unicod
int _tmain(int argc, _TCHAR* argv[])
*/