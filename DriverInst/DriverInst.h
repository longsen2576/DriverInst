#pragma once

#include <windows.h>
#include <winioctl.h>

/**************************************************************************
* 函数名：InstallDriver
* 功  能：安装驱动
* 参  数：const char* lpszDriverName，驱动服务的名称
*         const char* lpszDriverPath，驱动的全路径
*         const char* lpszAltitude，  文件驱动的高度
* 返回值：BOOL ，TRUE为安装成功
**************************************************************************/
BOOL InstallDriver(const char* lpszDriverName, const char* lpszDriverPath);

/**************************************************************************
* 函数名：StartDriver
* 功  能：启动驱动服务
* 参  数：const char* lpszDriverName，驱动服务名称
* 返回值：BOOL ，
**************************************************************************/
BOOL StartDriver(const char* lpszDriverName);

/**************************************************************************
* 函数名：StopDriver
* 功  能：停止驱动服务
* 参  数：const char* lpszDriverName，驱动服务名称
* 返回值：BOOL ，TRUE为成功
**************************************************************************/
BOOL StopDriver(const char* lpszDriverName);

/**************************************************************************
* 函数名：DeleteDriver
* 功  能：卸载驱动服务
* 参  数：const char* lpszDriverName，驱动服务名称
* 返回值：BOOL ，TRUE为成功
**************************************************************************/
BOOL DeleteDriver(const char* lpszDriverName);
