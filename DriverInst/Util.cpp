#include <iostream>
#include <string>
#include <Windows.h>

#include "Util.h"

using namespace std;

LPSTR ConvertErrorCodeToString(DWORD ErrorCode)
{
	HLOCAL LocalAddress = NULL;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, ErrorCode, 0, (PTSTR)&LocalAddress, 0, NULL);
	return (LPSTR)LocalAddress;
}

int PrintUsage()
{
	int ret_value = 0;
	cout << "input something for continue..." << endl;
	string str;
	cin >> str;
	return ret_value;
}