// GeneralOptionsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MaxEd.h"
#include "GeneralPage.h"
#include "defs.h"


// CGeneralPage dialog

IMPLEMENT_DYNAMIC(CGeneralPage, CPropertyPage)
CGeneralPage::CGeneralPage()
	: CPropertyPage(CGeneralPage::IDD)
	, m_bEnableUndo(PREFERENCES::Current.bEnableUndo)
	, m_nUndoStackDepth(PREFERENCES::Current.nUndoStackDepth)
	, m_nColorScheme(3)
	, bUseGlobalClearColor(PREFERENCES::Current.bUseGlobalClearColor)
	, clrClearColor(
		RGB(
		PREFERENCES::Current.clrGlobalClearColor.red*255,
		PREFERENCES::Current.clrGlobalClearColor.green*255,
		PREFERENCES::Current.clrGlobalClearColor.blue*255
		))
	, bForceCenteredWhenScaleMode(PREFERENCES::Current.bForceCenteredWhenScaleMode)
{
}

CGeneralPage::~CGeneralPage()
{
}

void CGeneralPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_bEnableUndo);
	DDX_Control(pDX, IDC_UNDOLEVELSSPIN, m_cUndoLevelSpin);
	DDX_CBIndex(pDX, IDC_SCHEMECOMBO, m_nColorScheme);
	DDX_Check(pDX, IDC_CHECK2, bUseGlobalClearColor);
	DDX_Check(pDX, IDC_CHECK3, bForceCenteredWhenScaleMode);
	DDX_Text(pDX, IDC_UNDOLEVELSEDIT, m_nUndoStackDepth);
	DDV_MinMaxUInt(pDX, m_nUndoStackDepth, 5, 50);
}


BEGIN_MESSAGE_MAP(CGeneralPage, CPropertyPage)
	ON_BN_CLICKED(IDC_LOADSCHEMEFROMFILEBUTTON, OnBnClickedLoadschemefromfilebutton)
	ON_STN_CLICKED(IDC_GLOBALCLEARCOLOR, OnStnClickedGlobalclearcolor)
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedEnableUndo)
	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()


// CGeneralPage message handlers

BOOL CGeneralPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO: Add extra initialization here

	/* INIT UNDO SECTION */
	m_cUndoLevelSpin.SetRange(5, 50);
	if (m_bEnableUndo)
	{	// set undo position to spin
		m_cUndoLevelSpin.SetPos(m_nUndoStackDepth);
	}
	else
	{	// disable undo edit window
		CEdit *pUndoStackDepth = (CEdit *)GetDlgItem(IDC_UNDOLEVELSEDIT);
		ASSERT(pUndoStackDepth);
		pUndoStackDepth->EnableWindow(FALSE);
		// disable spin
		m_cUndoLevelSpin.EnableWindow(FALSE);
	}

	/* INIT COLOR SCHEME SECTION */
	CComboBox *pSchemeCombo = (CComboBox *)GetDlgItem(IDC_SCHEMECOMBO);
	ASSERT(pSchemeCombo);
	pSchemeCombo->AddString("ala 3DMax");
	pSchemeCombo->AddString("Black-and-Green");
	pSchemeCombo->AddString("White-and-Black");
	pSchemeCombo->AddString("Custom");
	pSchemeCombo->SetCurSel(m_nColorScheme);

	/* INIT GLOBAL CLEAR COLOR SECTION */
	// this is "owner draw" element:
	// see CGeneralPage::OnDrawItem()

	return TRUE;
}

void CGeneralPage::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: Add your message handler code here and/or call default

	if (nIDCtl == IDC_GLOBALCLEARCOLOR)
	{
		CBrush swatch;
		swatch.CreateSolidBrush(clrClearColor);
		::FillRect(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, swatch);
	}
	else
		CPropertyPage::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CGeneralPage::OnBnClickedLoadschemefromfilebutton()
{
	// TODO: Add your control notification handler code here
	CFileDialog fileDlg(TRUE, "csh", "*.csh", 0, "Scheme Files (*.csh)|*.csh||");
	if (fileDlg.DoModal() == IDOK)
	{
		const CString pathName = fileDlg.GetPathName();
		// TODO: ...
	}
}

void CGeneralPage::OnStnClickedGlobalclearcolor()
{
	// TODO: Add your control notification handler code here
	CColorDialog colorDlg(clrClearColor);
	if (colorDlg.DoModal() == IDOK && clrClearColor != colorDlg.GetColor())
	{
		// set color
		clrClearColor = colorDlg.GetColor();
		// fill color picture rect new color
		CRect colorrect;
		CWnd *pClearColorPic = GetDlgItem(IDC_GLOBALCLEARCOLOR);
		ASSERT(pClearColorPic);
		pClearColorPic->GetWindowRect(&colorrect);
		colorrect.DeflateRect(1,1,1,1);
		ScreenToClient(&colorrect);
		// prepare filling brush
		CBrush swatch;
		swatch.CreateSolidBrush(clrClearColor);
		// get draw context
		CClientDC dc(this);
		// fill color picture rect
		dc.FillRect(&colorrect, &swatch);
	}
}

void CGeneralPage::OnBnClickedEnableUndo()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	// enable/disable undo edit window
	CEdit *pUndoStackDepth = (CEdit *)GetDlgItem(IDC_UNDOLEVELSEDIT);
	ASSERT(pUndoStackDepth);
	pUndoStackDepth->EnableWindow(m_bEnableUndo);
	// enable/disable spin
	m_cUndoLevelSpin.EnableWindow(m_bEnableUndo);
}
