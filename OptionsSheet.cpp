// OptionsSheet.cpp : implementation file
//

#include "stdafx.h"
#include "MaxEd.h"
#include "OptionsSheet.h"


// COptionsSheet

IMPLEMENT_DYNAMIC(COptionsSheet, CPropertySheet)
COptionsSheet::COptionsSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_GeneralPage);
	AddPage(&m_ViewportPage);
	AddPage(&m_GridPage);
	AddPage(&m_PathesPage);
}

COptionsSheet::COptionsSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_GeneralPage);
	AddPage(&m_ViewportPage);
	AddPage(&m_GridPage);
	AddPage(&m_PathesPage);
}

COptionsSheet::~COptionsSheet()
{
}


BEGIN_MESSAGE_MAP(COptionsSheet, CPropertySheet)
END_MESSAGE_MAP()


// COptionsSheet message handlers
