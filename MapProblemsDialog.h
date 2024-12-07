#pragma once


// CMapProblemsDialog dialog

class CMapProblemsDialog : public CDialog
{
	DECLARE_DYNAMIC(CMapProblemsDialog)

public:
	CMapProblemsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMapProblemsDialog();

// Dialog Data
	enum { IDD = IDD_MAPPROBLEMSDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnLbnSelchangeMaperrorlist();
	afx_msg void OnBnClickedGotoerror();
	afx_msg void OnBnClickedFixerror();
	afx_msg void OnBnClickedFixallerror();
};
