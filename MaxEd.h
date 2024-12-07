// MaxEd.h : main header file for the MaxEd application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CMaxEdApp:
// See MaxEd.cpp for the implementation of this class
//

class CMaxEdApp : public CWinApp
{
public:
	CMaxEdApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMaxEdApp theApp;

extern void LoadPreferencesFromIni(const TCHAR* inifile);
extern void SavePreferencesToIni(const TCHAR* inifile);