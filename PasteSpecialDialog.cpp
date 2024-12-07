// PasteSpecialDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MaxEd.h"
#include "PasteSpecialDialog.h"


// CPasteSpecialDialog dialog

IMPLEMENT_DYNAMIC(CPasteSpecialDialog, CDialog)
CPasteSpecialDialog::CPasteSpecialDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPasteSpecialDialog::IDD, pParent)
	, m_fOffsetX(0)
	, m_fOffsetY(0)
	, m_fOffsetZ(0)
	, m_fRotationX(0)
	, m_fRotationY(0)
	, m_fRotationZ(0)
	, m_nNumCopies(1)
	, m_bStartAtCenter(FALSE)
	, m_bGroupPaste(FALSE)
{
}

CPasteSpecialDialog::~CPasteSpecialDialog()
{
}

void CPasteSpecialDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PASTESPECIAL_OFFSETX, m_fOffsetX);
	DDX_Text(pDX, IDC_PASTESPECIAL_OFFSETY, m_fOffsetY);
	DDX_Text(pDX, IDC_PASTESPECIAL_OFFSETZ, m_fOffsetZ);
	DDX_Text(pDX, IDC_PASTESPECIAL_ROTATIONX, m_fRotationX);
	DDX_Text(pDX, IDC_PASTESPECIAL_ROTATIONY, m_fRotationY);
	DDX_Text(pDX, IDC_PASTESPECIAL_ROTATIONZ, m_fRotationZ);
	DDX_Text(pDX, IDC_PASTESPECIAL_NUMCOPIES, m_nNumCopies);
	DDV_MinMaxUInt(pDX, m_nNumCopies, 1, 256);
	DDX_Check(pDX, IDC_PASTESPECIAL_STARTATCENTER, m_bStartAtCenter);
	DDX_Check(pDX, IDC_PASTESPECIAL_GROUPPASTE, m_bGroupPaste);
}


BEGIN_MESSAGE_MAP(CPasteSpecialDialog, CDialog)
END_MESSAGE_MAP()


// CPasteSpecialDialog message handlers
BOOL CPasteSpecialDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	// NOTE: correct "num copies" spin range (1 - 256)
	CSpinButtonCtrl *pNumCopiesSpin =
		(CSpinButtonCtrl *)GetDlgItem(IDC_PASTESPECIAL_NUMCOPIESSPIN);
	ASSERT(pNumCopiesSpin);
	pNumCopiesSpin->SetRange32(1, 256);

	return TRUE;
}
