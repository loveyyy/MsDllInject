
// DllInject.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
#endif

#include "resource.h"		// 主符号


// CDllInjectApp:
// 有关此类的实现，请参阅 DllInject.cpp
//

class CDllInjectApp : public CWinApp
{
public:
	CDllInjectApp();

// 重写
public:
	virtual BOOL InitInstance();
	DWORD ProcessID;
// 实现
	DECLARE_MESSAGE_MAP()
	
};

extern CDllInjectApp theApp;
