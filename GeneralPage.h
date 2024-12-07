#pragma once
#include "afxcmn.h"


// CGeneralPage dialog

class CGeneralPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CGeneralPage)

public:
	CGeneralPage();
	virtual ~CGeneralPage();

// Dialog Data
	enum { IDD = IDD_GENERALOPTIONSDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedLoadschemefromfilebutton();
	afx_msg void OnStnClickedGlobalclearcolor();
	afx_msg void OnBnClickedEnableUndo();

public:
	// is undo/redo enable?
	BOOL m_bEnableUndo;
	// num undo stack positions
	INT m_nUndoStackDepth;
	CSpinButtonCtrl m_cUndoLevelSpin;
	// color scheme num (0 - ala 3DMax, 1 - Black-and-Green, 2 - White-and-Black, 3 - Custom)
	int m_nColorScheme;
	// using global clear color in draw operations (clear frame)
	BOOL bUseGlobalClearColor;
	COLORREF clrClearColor;
	BOOL bForceCenteredWhenScaleMode;
};
