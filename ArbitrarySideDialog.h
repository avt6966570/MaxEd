#pragma once


// CArbitrarySideDialog dialog

class CArbitrarySideDialog : public CDialog
{
	DECLARE_DYNAMIC(CArbitrarySideDialog)

public:
	CArbitrarySideDialog(int* pCurrentNumSide, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	enum { IDD = IDD_ARBITRARYSIDEDIALOG };
	int* m_pSide;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
};
