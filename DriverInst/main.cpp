
#include <iostream>
#include <string>

#include "DriverInst.h"
#include "CommonDef.h"
#include "Util.h"

using namespace std;

int main(int argc, char * argv[])
{
	PrintUsage();
	BOOL bRet = InstallDriver(ServiceName, "D:\\VirtualDisk.sys", "370030");
	if (bRet)
	{
		//StartDriver(ServiceName);
		cout << "driver install success!" << endl;
	}
	else
	{
		cout << "driver install failed!" << endl;
	}
	return 0;
}