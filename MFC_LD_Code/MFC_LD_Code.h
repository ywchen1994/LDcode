
// MFC_LD_Code.h : PROJECT_NAME 應用程式的主要標頭檔
//

#pragma once

#ifndef __AFXWIN_H__
	#error "對 PCH 包含此檔案前先包含 'stdafx.h'"
#endif

#include "resource.h"		// 主要符號


// CMFC_LD_CodeApp: 
// 請參閱實作此類別的 MFC_LD_Code.cpp
//

class CMFC_LD_CodeApp : public CWinApp
{
public:
	CMFC_LD_CodeApp();

// 覆寫
public:
	virtual BOOL InitInstance();

// 程式碼實作

	DECLARE_MESSAGE_MAP()
};

extern CMFC_LD_CodeApp theApp;