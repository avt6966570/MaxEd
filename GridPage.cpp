// GridOptionsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MaxEd.h"
#include "GridPage.h"
#include "defs.h"

/*
#define LOG2_BASETRANS	0.30102999566398//=log10(2)		// база перевода (для десятичного логарифма в двоичный)
#define LOG2(x)			(log10(x)/LOG2_BASETRANS)		// вычисление двоичного логарифма
*/

// CGridPage dialog

IMPLEMENT_DYNAMIC(CGridPage, CPropertyPage)
CGridPage::CGridPage()
	: CPropertyPage(CGridPage::IDD)
	/* init major */
	, m_bShowMajorGrid(PREFERENCES::Current.sGrid.Major.bShow)
	, m_nMajorGridType(PREFERENCES::Current.sGrid.Major.bDotted
						? 1 : PREFERENCES::Current.sGrid.Major.bStipple ? 2 : 0)
	, m_nMajorLineSize((int)PREFERENCES::Current.sGrid.Major.fWidth - 1)
	, m_nMajorGridSize(::ntz(PREFERENCES::Current.sGrid.Major.nStep) - 1) // without LOG2(x)
	, m_clrMajorLineColor(
		RGB(PREFERENCES::Current.sGrid.Major.cColor.red,
			PREFERENCES::Current.sGrid.Major.cColor.green,
			PREFERENCES::Current.sGrid.Major.cColor.blue)
		)
	/* init minor */
	, m_bShowMinorGrid(PREFERENCES::Current.sGrid.Minor.bShow)
	, m_nMinorGridType(PREFERENCES::Current.sGrid.Minor.bDotted
						? 1 : PREFERENCES::Current.sGrid.Minor.bStipple ? 2 : 0)
	, m_nMinorLineSize((int)PREFERENCES::Current.sGrid.Minor.fWidth - 1)
	, m_nMinorGridSize(::ntz(PREFERENCES::Current.sGrid.Minor.nStep) - 1) // without LOG2(x)
	, m_clrMinorLineColor(
		RGB(PREFERENCES::Current.sGrid.Minor.cColor.red,
			PREFERENCES::Current.sGrid.Minor.cColor.green,
			PREFERENCES::Current.sGrid.Minor.cColor.blue)
		)
	/* init axis */
	, m_bShowAxisGrid(PREFERENCES::Current.sGrid.Axis.bShow)
	, m_nAxisGridType(PREFERENCES::Current.sGrid.Axis.bDotted
						? 1 : PREFERENCES::Current.sGrid.Axis.bStipple ? 2 : 0)
	, m_nAxisLineSize((int)PREFERENCES::Current.sGrid.Axis.fWidth - 1)
	, m_nAxisDotStep(::ntz(PREFERENCES::Current.sGrid.Axis.nDotStep) - 1) // without LOG2(x)
	, m_clrAxisLineColor(
		RGB(PREFERENCES::Current.sGrid.Axis.cColor.red,
			PREFERENCES::Current.sGrid.Axis.cColor.green,
			PREFERENCES::Current.sGrid.Axis.cColor.blue)
		)
{
}

CGridPage::~CGridPage()
{
}

void CGridPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_bShowMajorGrid);
	DDX_CBIndex(pDX, IDC_LINESIZECOMBO, m_nMajorLineSize);
	DDX_CBIndex(pDX, IDC_STEPCOMBO, m_nMajorGridSize);
	DDX_Check(pDX, IDC_CHECK2, m_bShowMinorGrid);
	DDX_CBIndex(pDX, IDC_LINESIZECOMBO2, m_nMinorLineSize);
	DDX_CBIndex(pDX, IDC_STEPCOMBO2, m_nMinorGridSize);
	DDX_Check(pDX, IDC_CHECK3, m_bShowAxisGrid);
	DDX_CBIndex(pDX, IDC_LINESIZECOMBO3, m_nAxisLineSize);
	DDX_CBIndex(pDX, IDC_STEPCOMBO3, m_nAxisDotStep);
}


BEGIN_MESSAGE_MAP(CGridPage, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedShowMajorGrid)
	ON_STN_CLICKED(IDC_MAJORCOLOR, OnStnClickedMajorcolor)
	ON_BN_CLICKED(IDC_CHECK2, OnBnClickedShowMinorGrid)
	ON_STN_CLICKED(IDC_MINORCOLOR, OnStnClickedMinorcolor)
	ON_BN_CLICKED(IDC_CHECK3, OnBnClickedShowAxis)
	ON_STN_CLICKED(IDC_AXISCOLOR, OnStnClickedAxiscolor)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()


// CGridPage message handlers

BOOL CGridPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO: Add extra initialization here

/* INIT MAJOR GRID SECTION */
	// 1. check line type
	CButton *pGridTypeRadio = (CButton *)GetDlgItem(IDC_RADIO1 + m_nMajorGridType);
	ASSERT(pGridTypeRadio);
	pGridTypeRadio->SetCheck(BST_CHECKED);
	// 2. fill line/dot size combo
	CComboBox *pLineSizeCombo = (CComboBox *)GetDlgItem(IDC_LINESIZECOMBO);
	ASSERT(pLineSizeCombo);
	pLineSizeCombo->AddString("1.0");
	pLineSizeCombo->AddString("2.0");
	pLineSizeCombo->AddString("3.0");
	pLineSizeCombo->SetCurSel(m_nMajorLineSize);
	// 3. fill grid step combo
	CComboBox *pGridStepCombo = (CComboBox *)GetDlgItem(IDC_STEPCOMBO);
	ASSERT(pGridStepCombo);
	pGridStepCombo->AddString("2");
	pGridStepCombo->AddString("4");
	pGridStepCombo->AddString("8");
	pGridStepCombo->AddString("16");
	pGridStepCombo->AddString("32");
	pGridStepCombo->AddString("64");
	pGridStepCombo->AddString("128");
	pGridStepCombo->AddString("256");
	pGridStepCombo->AddString("512");
	pGridStepCombo->SetCurSel(m_nMajorGridSize);
	// 4. fill color grid rect
	// this is "owner draw" item:
	// see CGridPage::OnDrawItem()
	// 5. if hide grid -- disable dlg items
	if (!m_bShowMajorGrid)
	{
		pGridTypeRadio->EnableWindow(FALSE);
		pLineSizeCombo->EnableWindow(FALSE);
		pGridStepCombo->EnableWindow(FALSE);
	}


/* INIT MINOR GRID SECTION */
	// 1. check line type
	pGridTypeRadio = (CButton *)GetDlgItem(IDC_RADIO9 + m_nMinorGridType);
	ASSERT(pGridTypeRadio);
	pGridTypeRadio->SetCheck(BST_CHECKED);
	// 2. fill line/dot size combo
	pLineSizeCombo = (CComboBox *)GetDlgItem(IDC_LINESIZECOMBO2);
	ASSERT(pLineSizeCombo);
	pLineSizeCombo->AddString("1.0");
	pLineSizeCombo->AddString("2.0");
	pLineSizeCombo->AddString("3.0");
	pLineSizeCombo->SetCurSel(m_nMinorLineSize);
	// 3. fill grid step combo
	pGridStepCombo = (CComboBox *)GetDlgItem(IDC_STEPCOMBO2);
	ASSERT(pGridStepCombo);
	pGridStepCombo->AddString("2");
	pGridStepCombo->AddString("4");
	pGridStepCombo->AddString("8");
	pGridStepCombo->AddString("16");
	pGridStepCombo->AddString("32");
	pGridStepCombo->AddString("64");
	pGridStepCombo->AddString("128");
	pGridStepCombo->AddString("256");
	pGridStepCombo->AddString("512");
	pGridStepCombo->SetCurSel(m_nMinorGridSize);
	// 4. fill color grid rect
	// this is "owner draw" item:
	// see CGridPage::OnDrawItem()
	// 5. if hide grid -- disable dlg items
	if (!m_bShowMinorGrid)
	{
		pGridTypeRadio->EnableWindow(FALSE);
		pLineSizeCombo->EnableWindow(FALSE);
		pGridStepCombo->EnableWindow(FALSE);
	}


/* INIT AXIS GRID SECTION */
	// 1. check line type
	pGridTypeRadio = (CButton *)GetDlgItem(IDC_RADIO14 + m_nAxisGridType);
	ASSERT(pGridTypeRadio);
	pGridTypeRadio->SetCheck(BST_CHECKED);
	// 2. fill line/dot size combo
	pLineSizeCombo = (CComboBox *)GetDlgItem(IDC_LINESIZECOMBO3);
	ASSERT(pLineSizeCombo);
	pLineSizeCombo->AddString("1.0");
	pLineSizeCombo->AddString("2.0");
	pLineSizeCombo->AddString("3.0");
	pLineSizeCombo->SetCurSel(m_nAxisLineSize);
	// 3. fill grid step combo
	pGridStepCombo = (CComboBox *)GetDlgItem(IDC_STEPCOMBO3);
	ASSERT(pGridStepCombo);
	pGridStepCombo->AddString("2");
	pGridStepCombo->AddString("4");
	pGridStepCombo->AddString("8");
	pGridStepCombo->AddString("16");
	pGridStepCombo->AddString("32");
	pGridStepCombo->AddString("64");
	pGridStepCombo->AddString("128");
	pGridStepCombo->AddString("256");
	pGridStepCombo->AddString("512");
	pGridStepCombo->SetCurSel(m_nAxisDotStep);
	// 4. fill color grid rect
	// this is "owner draw" item:
	// see CGridPage::OnDrawItem()

	// 5. if hide grid -- disable dlg items
	if (!m_bShowAxisGrid)
	{
		pGridTypeRadio->EnableWindow(FALSE);
		pLineSizeCombo->EnableWindow(FALSE);
		pGridStepCombo->EnableWindow(FALSE);
	}

	return TRUE;
}

void CGridPage::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: Add your message handler code here and/or call default
	CBrush swatch;
	switch (nIDCtl)
	{
	case IDC_MAJORCOLOR:
		swatch.CreateSolidBrush(m_clrMajorLineColor);
		::FillRect(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, swatch);
		break;
	case IDC_MINORCOLOR:
		swatch.CreateSolidBrush(m_clrMinorLineColor);
		::FillRect(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, swatch);
		break;
	case IDC_AXISCOLOR:
		swatch.CreateSolidBrush(m_clrAxisLineColor);
		::FillRect(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, swatch);
		break;
	default:
		CPropertyPage::OnDrawItem(nIDCtl, lpDrawItemStruct);
		break;
	}
}

void CGridPage::OnBnClickedShowMajorGrid()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	CButton *pGridTypeRadio = (CButton *)GetDlgItem(IDC_RADIO1);
	ASSERT(pGridTypeRadio);
	CComboBox *pLineSizeCombo = (CComboBox *)GetDlgItem(IDC_LINESIZECOMBO);
	ASSERT(pLineSizeCombo);
	CComboBox *pGridStepCombo = (CComboBox *)GetDlgItem(IDC_STEPCOMBO);
	ASSERT(pGridStepCombo);

	pGridTypeRadio->EnableWindow(m_bShowMajorGrid);
	pLineSizeCombo->EnableWindow(m_bShowMajorGrid);
	pGridStepCombo->EnableWindow(m_bShowMajorGrid);
}

void CGridPage::OnStnClickedMajorcolor()
{
	// TODO: Add your control notification handler code here
	CColorDialog colorDlg(m_clrMajorLineColor);
	if (colorDlg.DoModal() == IDOK && m_clrMajorLineColor != colorDlg.GetColor())
	{
		// set color
		m_clrMajorLineColor = colorDlg.GetColor();
		// fill color picture rect new color
		CRect colorrect;
		CWnd *pClearColorPic = GetDlgItem(IDC_MAJORCOLOR);
		ASSERT(pClearColorPic);
		pClearColorPic->GetWindowRect(&colorrect);
		colorrect.DeflateRect(1,1,1,1);
		ScreenToClient(&colorrect);
		// prepare filling brush
		CBrush swatch;
		swatch.CreateSolidBrush(m_clrMajorLineColor);
		// get draw context
		CClientDC dc(this);
		// fill color picture rect
		dc.FillRect(&colorrect, &swatch);
	}
}

void CGridPage::OnBnClickedShowMinorGrid()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	CButton *pGridTypeRadio = (CButton *)GetDlgItem(IDC_RADIO9);
	ASSERT(pGridTypeRadio);
	CComboBox *pLineSizeCombo = (CComboBox *)GetDlgItem(IDC_LINESIZECOMBO2);
	ASSERT(pLineSizeCombo);
	CComboBox *pGridStepCombo = (CComboBox *)GetDlgItem(IDC_STEPCOMBO2);
	ASSERT(pGridStepCombo);

	pGridTypeRadio->EnableWindow(m_bShowMinorGrid);
	pLineSizeCombo->EnableWindow(m_bShowMinorGrid);
	pGridStepCombo->EnableWindow(m_bShowMinorGrid);
}

void CGridPage::OnStnClickedMinorcolor()
{
	// TODO: Add your control notification handler code here
	CColorDialog colorDlg(m_clrMinorLineColor);
	if (colorDlg.DoModal() == IDOK && m_clrMinorLineColor != colorDlg.GetColor())
	{
		// set color
		m_clrMinorLineColor = colorDlg.GetColor();
		// fill color picture rect new color
		CRect colorrect;
		CWnd *pClearColorPic = GetDlgItem(IDC_MINORCOLOR);
		ASSERT(pClearColorPic);
		pClearColorPic->GetWindowRect(&colorrect);
		colorrect.DeflateRect(1,1,1,1);
		ScreenToClient(&colorrect);
		// prepare filling brush
		CBrush swatch;
		swatch.CreateSolidBrush(m_clrMinorLineColor);
		// get draw context
		CClientDC dc(this);
		// fill color picture rect
		dc.FillRect(&colorrect, &swatch);
	}
}

void CGridPage::OnBnClickedShowAxis()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	CButton *pGridTypeRadio = (CButton *)GetDlgItem(IDC_RADIO14);
	ASSERT(pGridTypeRadio);
	CComboBox *pLineSizeCombo = (CComboBox *)GetDlgItem(IDC_LINESIZECOMBO3);
	ASSERT(pLineSizeCombo);
	CComboBox *pGridStepCombo = (CComboBox *)GetDlgItem(IDC_STEPCOMBO3);
	ASSERT(pGridStepCombo);

	pGridTypeRadio->EnableWindow(m_bShowAxisGrid);
	pLineSizeCombo->EnableWindow(m_bShowAxisGrid);
	pGridStepCombo->EnableWindow(m_bShowAxisGrid);
}

void CGridPage::OnStnClickedAxiscolor()
{
	// TODO: Add your control notification handler code here
	CColorDialog colorDlg(m_clrAxisLineColor);
	if (colorDlg.DoModal() == IDOK && m_clrAxisLineColor != colorDlg.GetColor())
	{
		// set color
		m_clrAxisLineColor = colorDlg.GetColor();
		// fill color picture rect new color
		CRect colorrect;
		CWnd *pClearColorPic = GetDlgItem(IDC_AXISCOLOR);
		ASSERT(pClearColorPic);
		pClearColorPic->GetWindowRect(&colorrect);
		colorrect.DeflateRect(1,1,1,1);
		ScreenToClient(&colorrect);
		// prepare filling brush
		CBrush swatch;
		swatch.CreateSolidBrush(m_clrAxisLineColor);
		// get draw context
		CClientDC dc(this);
		// fill color picture rect
		dc.FillRect(&colorrect, &swatch);
	}
}
