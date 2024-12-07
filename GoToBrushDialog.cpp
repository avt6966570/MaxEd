// GoToBrushDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MaxEd.h"
#include "GoToBrushDialog.h"


// CGoToBrushDialog dialog

IMPLEMENT_DYNAMIC(CGoToBrushDialog, CDialog)
CGoToBrushDialog::CGoToBrushDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CGoToBrushDialog::IDD, pParent)
	, m_uBrushN(0)
	, m_nEntityN(0)
	, m_bSearchVisibleOnly(FALSE)
{
}

CGoToBrushDialog::~CGoToBrushDialog()
{
}

void CGoToBrushDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GOTOBRUSH_BRUSHN, m_uBrushN);
	DDX_Text(pDX, IDC_GOTOBRUSH_ENTITYN, m_nEntityN);
	DDX_Check(pDX, IDC_GOTOBRUSH_SEARCHVISIBLEONLY, m_bSearchVisibleOnly);
}


BEGIN_MESSAGE_MAP(CGoToBrushDialog, CDialog)
END_MESSAGE_MAP()


// CGoToBrushDialog message handlers
