// TransformationDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MaxEd.h"
#include "TransformationDialog.h"


// CTransformationDialog dialog

IMPLEMENT_DYNAMIC(CTransformationDialog, CDialog)
CTransformationDialog::CTransformationDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTransformationDialog::IDD, pParent)
	, m_uMode(0)
{
	m_fValue[0] = m_fValue[1] = m_fValue[2] = 0;
}

CTransformationDialog::~CTransformationDialog()
{
}

void CTransformationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TRANSFORMATION_XVALUE, m_fValue[0]);
	DDX_Text(pDX, IDC_TRANSFORMATION_YVALUE, m_fValue[1]);
	DDX_Text(pDX, IDC_TRANSFORMATION_ZVALUE, m_fValue[2]);
}


BEGIN_MESSAGE_MAP(CTransformationDialog, CDialog)
END_MESSAGE_MAP()


// CTransformationDialog message handlers
void CTransformationDialog::OnOK()
{
	// get transformation mode (m_uMode)
	for (UINT nID = IDC_ROTATE; nID <= IDC_MOVE; ++nID)
	{
		CButton *pModeCheck = (CButton *)GetDlgItem(nID);
		ASSERT(pModeCheck);
		if (pModeCheck->GetState() & 0x0003)
		{
			m_uMode = nID - IDC_ROTATE;
			break;
		}
	}

	// close dialog
	CDialog::OnOK();
}
