
// MFC_LD_Code.h : PROJECT_NAME ���ε{�����D�n���Y��
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�� PCH �]�t���ɮ׫e���]�t 'stdafx.h'"
#endif

#include "resource.h"		// �D�n�Ÿ�


// CMFC_LD_CodeApp: 
// �аѾ\��@�����O�� MFC_LD_Code.cpp
//

class CMFC_LD_CodeApp : public CWinApp
{
public:
	CMFC_LD_CodeApp();

// �мg
public:
	virtual BOOL InitInstance();

// �{���X��@

	DECLARE_MESSAGE_MAP()
};

extern CMFC_LD_CodeApp theApp;