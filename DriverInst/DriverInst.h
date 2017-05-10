#pragma once

#include <windows.h>
#include <winioctl.h>

/**************************************************************************
* ��������InstallDriver
* ��  �ܣ���װ����
* ��  ����const char* lpszDriverName���������������
*         const char* lpszDriverPath��������ȫ·��
*         const char* lpszAltitude��  �ļ������ĸ߶�
* ����ֵ��BOOL ��TRUEΪ��װ�ɹ�
**************************************************************************/
BOOL InstallDriver(const char* lpszDriverName, const char* lpszDriverPath);

/**************************************************************************
* ��������StartDriver
* ��  �ܣ�������������
* ��  ����const char* lpszDriverName��������������
* ����ֵ��BOOL ��
**************************************************************************/
BOOL StartDriver(const char* lpszDriverName);

/**************************************************************************
* ��������StopDriver
* ��  �ܣ�ֹͣ��������
* ��  ����const char* lpszDriverName��������������
* ����ֵ��BOOL ��TRUEΪ�ɹ�
**************************************************************************/
BOOL StopDriver(const char* lpszDriverName);

/**************************************************************************
* ��������DeleteDriver
* ��  �ܣ�ж����������
* ��  ����const char* lpszDriverName��������������
* ����ֵ��BOOL ��TRUEΪ�ɹ�
**************************************************************************/
BOOL DeleteDriver(const char* lpszDriverName);
