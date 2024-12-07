#pragma once
#include "defs.h"


// CViewportPage dialog

class CViewportPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CViewportPage)

public:
	CViewportPage();
	virtual ~CViewportPage();

// Dialog Data
	enum { IDD = IDD_VIEWPORTOPTIONSDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnCheckViewLayout(UINT nID);

public:
	WINDOWLAYOUT	m_wl;
};
