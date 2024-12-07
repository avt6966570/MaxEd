#pragma once

// CEntityInspectorDialog dialog

class CEntityInspectorDialog : public CDialog
{
	DECLARE_DYNAMIC(CEntityInspectorDialog)

public:
	CEntityInspectorDialog(CWnd* pParent = NULL);   // standard constructor
	virtual BOOL Create(CWnd* pParent = NULL);
	virtual ~CEntityInspectorDialog();

// Dialog Data
	enum { IDD = IDD_ENTITYINSPECTORDIALOG };

	void SetInfo();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnCancel();

	//{{AFX_MSG(CEntityInspectorDialog)
	afx_msg void OnSelectEntityList();
	afx_msg void OnLvnEndlabeleditEntityinspectordialogEpairs(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnChangeFlagBoxes(UINT nID);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};