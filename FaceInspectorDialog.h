#pragma once


// CFaceInspectorDialog dialog

class CFaceInspectorDialog : public CDialog
{
	DECLARE_DYNAMIC(CFaceInspectorDialog)

public:
	CFaceInspectorDialog(CWnd* pParent = NULL);   // standard constructor
	virtual BOOL Create(CWnd* pParent = NULL);
	virtual ~CFaceInspectorDialog();

// Dialog Data
	enum { IDD = IDD_FACEINSPECTORDIALOG };

	void SetInfo();
	void SetTexInfo(struct FACE *pActiveFace);
	void SetTexFlags(struct FACE *pActiveFace);
	void SetTexMod(struct FACE *pActiveFace);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFaceInspectorDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CFaceInspectorDialog)
	afx_msg void OnDeltaPosSpin(UINT nID, NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedJustify(UINT nID);
	afx_msg void OnBnClickedAlign(UINT nID);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};