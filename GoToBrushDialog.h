#pragma once


// CGoToBrushDialog dialog

class CGoToBrushDialog : public CDialog
{
	DECLARE_DYNAMIC(CGoToBrushDialog)

public:
	CGoToBrushDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGoToBrushDialog();

// Dialog Data
	enum { IDD = IDD_GOTOBRUSHDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// finding brush number
	UINT m_uBrushN;
	// finding entity number
	UINT m_nEntityN;
	BOOL m_bSearchVisibleOnly;
};
