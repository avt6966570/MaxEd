#pragma once


// CPathesPage dialog

class CPathesPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CPathesPage)

public:
	CPathesPage();
	virtual ~CPathesPage();

// Dialog Data
	enum { IDD = IDD_PATHESOPTIONSDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedBrowsegameexecutablebutton();
	afx_msg void OnBnClickedBrowsegamedirbutton();
	afx_msg void OnBnClickedBrowsemoddirbutton();
	afx_msg void OnBnClickedBrowsemapdirbutton();

public:
	CString m_strRootPath;
	CString m_strTexturesPath;
	CString m_strModelsPath;
	CString m_strSoundPath;
	CString m_strMusicPath;
	CString m_strSpritesPath;
	CString m_strEntityPath;
	CString m_strPreferencesPath;
	CString m_strShaderPath;
	CString m_strUtilsPath;
	CString m_strTEMPPath;
};
