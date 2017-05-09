#include <tchar.h>
#include <stdio.h>

#include "DriverInst.h"
#include "Util.h"
#include "CommonDef.h"





BOOL InstallDriver(const char* lpszDriverName, const char* lpszDriverPath)
{
	char szTempStr[MAX_PATH];
	HKEY hKey;
	DWORD dwData;

	printf("lpszDriverName=%s, lpszDriverPath=%s\n", lpszDriverName, lpszDriverPath);
	if (NULL == lpszDriverName || NULL == lpszDriverPath)
	{
		return FALSE;
	}

	SC_HANDLE hServiceMgr = NULL;// SCM管理器的句柄    
	SC_HANDLE hService = NULL;   // 驱动程序的服务句柄    

								 //打开服务控制管理器    
	hServiceMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hServiceMgr == NULL)
	{
		CloseServiceHandle(hServiceMgr);
		return FALSE;
	}

	//创建驱动所对应的服务    
	hService = CreateServiceA(hServiceMgr,
		lpszDriverName,   
		lpszDriverName, 
		SERVICE_ALL_ACCESS,    
		SERVICE_KERNEL_DRIVER,    
		SERVICE_AUTO_START, 
		SERVICE_ERROR_IGNORE, 
		lpszDriverPath,   
		DriverClassName,   
		NULL,
		NULL,   
		NULL,
		NULL);

	if (hService == NULL)
	{
		printf("CreateService failed:%d", GetLastError());
		if (GetLastError() == ERROR_SERVICE_EXISTS)
		{
			CloseServiceHandle(hService);
			CloseServiceHandle(hServiceMgr);
			return TRUE;
		}
		else
		{
			CloseServiceHandle(hService);
			CloseServiceHandle(hServiceMgr);
			return FALSE;
		}
	}
	CloseServiceHandle(hService);
	CloseServiceHandle(hServiceMgr);
	
	return TRUE;
}

BOOL StartDriver(const char* lpszDriverName)
{
	SC_HANDLE schManager;
	SC_HANDLE schService;
	SERVICE_STATUS svcStatus;

	if (NULL == lpszDriverName)
	{
		return FALSE;
	}

	schManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == schManager)
	{
		CloseServiceHandle(schManager);
		return FALSE;
	}
	schService = OpenServiceA(schManager, lpszDriverName, SERVICE_ALL_ACCESS);
	if (NULL == schService)
	{
		CloseServiceHandle(schService);
		CloseServiceHandle(schManager);
		return FALSE;
	}

	if (!StartService(schService, 0, NULL))
	{
		CloseServiceHandle(schService);
		CloseServiceHandle(schManager);
		if (GetLastError() == ERROR_SERVICE_ALREADY_RUNNING)
		{
			return TRUE;
		}
		return FALSE;
	}

	CloseServiceHandle(schService);
	CloseServiceHandle(schManager);

	return TRUE;
}

BOOL StopDriver(const char* lpszDriverName)
{
	SC_HANDLE schManager;
	SC_HANDLE schService;
	SERVICE_STATUS svcStatus;
	bool bStopped = false;

	schManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == schManager)
	{
		return FALSE;
	}
	schService = OpenServiceA(schManager, lpszDriverName, SERVICE_ALL_ACCESS);
	if (NULL == schService)
	{
		CloseServiceHandle(schManager);
		return FALSE;
	}
	if (!ControlService(schService, SERVICE_CONTROL_STOP, &svcStatus) && (svcStatus.dwCurrentState != SERVICE_STOPPED))
	{
		CloseServiceHandle(schService);
		CloseServiceHandle(schManager);
		return FALSE;
	}

	CloseServiceHandle(schService);
	CloseServiceHandle(schManager);

	return TRUE;
}

BOOL DeleteDriver(const char* lpszDriverName)
{
	SC_HANDLE schManager;
	SC_HANDLE schService;
	SERVICE_STATUS svcStatus;

	schManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == schManager)
	{
		return FALSE;
	}
	schService = OpenServiceA(schManager, lpszDriverName, SERVICE_ALL_ACCESS);
	if (NULL == schService)
	{
		CloseServiceHandle(schManager);
		return FALSE;
	}
	ControlService(schService, SERVICE_CONTROL_STOP, &svcStatus);
	if (!DeleteService(schService))
	{
		CloseServiceHandle(schService);
		CloseServiceHandle(schManager);
		return FALSE;
	}
	CloseServiceHandle(schService);
	CloseServiceHandle(schManager);

	return TRUE;
}