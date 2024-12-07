#pragma once


// CGridPage dialog

class CGridPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CGridPage)

public:
	CGridPage();
	virtual ~CGridPage();

// Dialog Data
	enum { IDD = IDD_GRIDOPTIONSDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedShowMajorGrid();
	afx_msg void OnStnClickedMajorcolor();
	afx_msg void OnBnClickedShowMinorGrid();
	afx_msg void OnStnClickedMinorcolor();
	afx_msg void OnBnClickedShowAxis();
	afx_msg void OnStnClickedAxiscolor();

public:

	/* major grid */
	// is show?
	BOOL m_bShowMajorGrid;
	// type: 0 - line, 1 - dotted, 2 - stipples
	int m_nMajorGridType;
	// line/dot size
	int m_nMajorLineSize;
	// grid size (0 - 2pt ... 8 - 512pt)
	int m_nMajorGridSize;
	COLORREF m_clrMajorLineColor;

	/* minor grid */
	// is show?
	BOOL m_bShowMinorGrid;
	// type: 0 - line, 1 - dotted, 2 - stipples
	int m_nMinorGridType;
	// line/dot size
	int m_nMinorLineSize;
	// grid size (0 - 2pt ... 8 - 512pt)
	int m_nMinorGridSize;
	COLORREF m_clrMinorLineColor;

	/* axis */
	// is show?
	BOOL m_bShowAxisGrid;
	// type: 0 - line, 1 - dotted, 2 - stipples
	int m_nAxisGridType;
	// line/dot size
	int m_nAxisLineSize;
	// grid size (0 - 2pt ... 8 - 512pt)
	int m_nAxisDotStep;
	COLORREF m_clrAxisLineColor;
};
