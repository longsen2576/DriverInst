
#include <stdio.h>

#include "DriverInst.h"
#include "CommonDef.h"

int main(int argc, char * argv[])
{
	BOOL bRet = InstallDriver(ServiceName, "D:\\VirtDisk.sys", "370030");
	if (bRet)
	{
		StartDriver(ServiceName);
		printf("driver install success!");
	}
	else
	{
		printf("driver install failed!");
	}
	return 0;
}