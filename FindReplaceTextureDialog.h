#pragma once


// CFindReplaceTextureDialog dialog

class CFindReplaceTextureDialog : public CDialog
{
	DECLARE_DYNAMIC(CFindReplaceTextureDialog)

public:
	CFindReplaceTextureDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFindReplaceTextureDialog();

// Dialog Data
	enum { IDD = IDD_FINDREPLACEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedApply();

public:
	// finding texture name(w, w/o extension)
	CString m_strFindPath;
	// replace texture name(w, w/o extension)
	CString m_strReplacePath;
	// search in selected brushes only
	BOOL m_bSelectedOnly;
};
