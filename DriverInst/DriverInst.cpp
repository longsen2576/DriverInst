#include <tchar.h>
#include <stdio.h>

#include "DriverInst.h"
#include "Util.h"
#include "CommonDef.h"





BOOL InstallDriver(const char* lpszDriverName, const char* lpszDriverPath, const char* lpszAltitude)
{
	char szTempStr[MAX_PATH];
	HKEY hKey;
	DWORD dwData;

	printf("lpszDriverName=%s, lpszDriverPath=%s\n", lpszDriverName, lpszDriverPath);
	if (NULL == lpszDriverName || NULL == lpszDriverPath)
	{
		return FALSE;
	}

	SC_HANDLE hServiceMgr = NULL;// SCM�������ľ��    
	SC_HANDLE hService = NULL;   // ��������ķ�����    

								 //�򿪷�����ƹ�����    
	hServiceMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hServiceMgr == NULL)
	{
		CloseServiceHandle(hServiceMgr);
		return FALSE;
	}

	//������������Ӧ�ķ���    
	hService = CreateServiceA(hServiceMgr,
		lpszDriverName, // �����������ע����е�����    
		lpszDriverName, // ע������������DisplayName ֵ    
		SERVICE_ALL_ACCESS, // ������������ķ���Ȩ��    
		SERVICE_KERNEL_DRIVER, // ��ʾ���صķ������ļ�ϵͳ��������    
		SERVICE_AUTO_START,//SERVICE_DEMAND_START,ע������������Start ֵ  
		SERVICE_ERROR_IGNORE, // ע������������ErrorControl ֵ    
		lpszDriverPath, // ע������������ImagePath  
		DriverClassName,// ע������������Group ֵ    
		NULL,
		NULL, // ���������ķ���ע������������DependOnServiceֵ    
		NULL,
		NULL);

	if (hService == NULL)
	{
		printf("CreateService failed:%d(%s)", GetLastError(), ConvertErrorCodeToString(GetLastError()));
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

	//-------------------------------------------------------------------------------------------------------    
	// SYSTEM\\CurrentControlSet\\Services\\DriverName\\Instances�ӽ��µļ�ֵ��    
	//-------------------------------------------------------------------------------------------------------    
	strcpy(szTempStr, "SYSTEM\\CurrentControlSet\\Services\\");
	strcat(szTempStr, lpszDriverName);
	strcat(szTempStr, "\\Instances");
	if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, szTempStr, 0, "", TRUE, KEY_ALL_ACCESS, NULL, &hKey, (LPDWORD)&dwData) != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// ע������������DefaultInstance ֵ    
	strcpy(szTempStr, lpszDriverName);
	strcat(szTempStr, " Instance");
	if (RegSetValueExA(hKey, "DefaultInstance", 0, REG_SZ, (CONST BYTE*)szTempStr, (DWORD)strlen(szTempStr)) != ERROR_SUCCESS)
	{
		return FALSE;
	}
	RegFlushKey(hKey);
	RegCloseKey(hKey);

	//-------------------------------------------------------------------------------------------------------    
	// SYSTEM\\CurrentControlSet\\Services\\DriverName\\Instances\\DriverName Instance�ӽ��µļ�ֵ��    
	//-------------------------------------------------------------------------------------------------------    
	strcpy(szTempStr, "SYSTEM\\CurrentControlSet\\Services\\");
	strcat(szTempStr, lpszDriverName);
	strcat(szTempStr, "\\Instances\\");
	strcat(szTempStr, lpszDriverName);
	strcat(szTempStr, " Instance");
	if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, szTempStr, 0, "", TRUE, KEY_ALL_ACCESS, NULL, &hKey, (LPDWORD)&dwData) != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// ע������������Altitude ֵ    
	strcpy(szTempStr, lpszAltitude);
	if (RegSetValueExA(hKey, "Altitude", 0, REG_SZ, (CONST BYTE*)szTempStr, (DWORD)strlen(szTempStr)) != ERROR_SUCCESS)
	{
		return FALSE;
	}
	// ע������������Flags ֵ    
	dwData = 0x0;
	if (RegSetValueExA(hKey, "Flags", 0, REG_DWORD, (CONST BYTE*)&dwData, sizeof(DWORD)) != ERROR_SUCCESS)
	{
		return FALSE;
	}
	RegFlushKey(hKey);
	RegCloseKey(hKey);

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