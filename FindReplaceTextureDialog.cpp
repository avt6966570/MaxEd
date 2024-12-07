// FindReplaceTextureDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MaxEd.h"
#include "FindReplaceTextureDialog.h"


// CFindReplaceTextureDialog dialog

IMPLEMENT_DYNAMIC(CFindReplaceTextureDialog, CDialog)
CFindReplaceTextureDialog::CFindReplaceTextureDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFindReplaceTextureDialog::IDD, pParent)
	, m_strFindPath(_T(""))
	, m_strReplacePath(_T(""))
	, m_bSelectedOnly(FALSE)
{
}

CFindReplaceTextureDialog::~CFindReplaceTextureDialog()
{
}

void CFindReplaceTextureDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FINDEDIT, m_strFindPath);
	DDX_Text(pDX, IDC_REPLACEEDIT, m_strReplacePath);
	DDX_Check(pDX, IDC_REPLACEWITHINSELECTED, m_bSelectedOnly);
}


BEGIN_MESSAGE_MAP(CFindReplaceTextureDialog, CDialog)
	ON_BN_CLICKED(IDAPPLY, OnBnClickedApply)
END_MESSAGE_MAP()


// CFindReplaceTextureDialog message handlers

void CFindReplaceTextureDialog::OnBnClickedApply()
{
	// TODO: Add your control notification handler code here
}
