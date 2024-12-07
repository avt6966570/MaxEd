// ArbitrarySideDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MaxEd.h"
#include "ArbitrarySideDialog.h"


// CArbitrarySideDialog dialog

IMPLEMENT_DYNAMIC(CArbitrarySideDialog, CDialog)
CArbitrarySideDialog::CArbitrarySideDialog(int* pCurrentNumSide, CWnd* pParent /*=NULL*/)
	: CDialog(CArbitrarySideDialog::IDD, pParent), m_pSide(pCurrentNumSide)
{

}

BOOL CArbitrarySideDialog::OnInitDialog()
{
	CSpinButtonCtrl* pSpinButtonCtrl = (CSpinButtonCtrl*)GetDlgItem(IDC_ARBITRARYSIDESPIN);
	ASSERT(pSpinButtonCtrl);
	pSpinButtonCtrl->SetRange32(3, 64);
	pSpinButtonCtrl->SetPos(*m_pSide);
	return TRUE;
}

void CArbitrarySideDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CArbitrarySideDialog::OnOK()
{
	// getting spin button ctrl
	CSpinButtonCtrl* pSpinButtonCtrl = (CSpinButtonCtrl*)GetDlgItem(IDC_ARBITRARYSIDESPIN);
	ASSERT(pSpinButtonCtrl);
	int nside = pSpinButtonCtrl->GetPos();
	if(nside >= 3 && nside <= 64)
		EndDialog(*m_pSide = nside);		// return correct num side
	else
		MessageBox("Face count must be in range 3 to 64", "MaxEd", MB_ICONWARNING | MB_OK);
}

BEGIN_MESSAGE_MAP(CArbitrarySideDialog, CDialog)
END_MESSAGE_MAP()


// CArbitrarySideDialog message handlers
