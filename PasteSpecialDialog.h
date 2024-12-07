#pragma once


// CPasteSpecialDialog dialog

class CPasteSpecialDialog : public CDialog
{
	DECLARE_DYNAMIC(CPasteSpecialDialog)

public:
	CPasteSpecialDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPasteSpecialDialog();

// Dialog Data
	enum { IDD = IDD_PASTESPECIALDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	float m_fOffsetX;
	float m_fOffsetY;
	float m_fOffsetZ;
	float m_fRotationX;
	float m_fRotationY;
	float m_fRotationZ;
	UINT m_nNumCopies;
	BOOL m_bStartAtCenter;
	BOOL m_bGroupPaste;
};
