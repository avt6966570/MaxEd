// ViewportOptionsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MaxEd.h"
#include "ViewportPage.h"


// CViewportPage dialog

IMPLEMENT_DYNAMIC(CViewportPage, CPropertyPage)
CViewportPage::CViewportPage()
	: CPropertyPage(CViewportPage::IDD)
	, m_wl(PREFERENCES::Current.eWindowLayout)
{
}

CViewportPage::~CViewportPage()
{
}

void CViewportPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CViewportPage, CPropertyPage)
	ON_COMMAND_RANGE(IDC_RADIO1, IDC_RADIO13, OnCheckViewLayout)
END_MESSAGE_MAP()


// CViewportPage message handlers

BOOL CViewportPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO: Add extra initialization here
	CButton *pViewCheck =
		(CButton *)GetDlgItem(IDC_RADIO1 + m_wl);
	ASSERT(pViewCheck);
	pViewCheck->SetCheck(BST_CHECKED);

	return TRUE;
}

void CViewportPage::OnCheckViewLayout(UINT nID)
{
	// set new layout
	m_wl = WINDOWLAYOUT(nID - IDC_RADIO1);
	// check button
	CButton *pViewCheck = (CButton *)GetDlgItem(nID);
	ASSERT(pViewCheck);
	pViewCheck->SetCheck(BST_CHECKED);
}
