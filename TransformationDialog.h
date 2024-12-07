#pragma once


// CTransformationDialog dialog

class CTransformationDialog : public CDialog
{
	DECLARE_DYNAMIC(CTransformationDialog)

public:
	CTransformationDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTransformationDialog();

// Dialog Data
	enum { IDD = IDD_TRANSFORMATIONDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

public:
	// transformation mode: 0 - rotate, 1 - scale, 2 - translate
	UINT m_uMode;
	// transformation values (X, Y, Z)
	float m_fValue[3];
};
