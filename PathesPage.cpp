// PathesPage.cpp : implementation file
//

#include "stdafx.h"
#include "MaxEd.h"
#include "PathesPage.h"


// CPathesPage dialog

IMPLEMENT_DYNAMIC(CPathesPage, CPropertyPage)
CPathesPage::CPathesPage()
	: CPropertyPage(CPathesPage::IDD)
	, m_strRootPath(EDITORENV_ROOT_PATH)
	, m_strTexturesPath(EDITORENV_TEXTURES_PATH)
	, m_strModelsPath(EDITORENV_MODELS_PATH)
	, m_strSoundPath(EDITORENV_SOUND_PATH)
	, m_strMusicPath(EDITORENV_MUSIC_PATH)
	, m_strSpritesPath(EDITORENV_SPRITES_PATH)
	, m_strEntityPath(EDITORENV_ENTITY_PATH)
	, m_strPreferencesPath(EDITORENV_PREFERENCES_PATH)
	, m_strShaderPath(EDITORENV_SHADER_PATH)
	, m_strUtilsPath(EDITORENV_UTILS_PATH)
	, m_strTEMPPath(EDITORENV_TEMP_PATH)
{
}

CPathesPage::~CPathesPage()
{
}

void CPathesPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ROOTPATH, m_strRootPath);
	DDX_Text(pDX, IDC_TEXTURESPATH, m_strTexturesPath);
	DDX_Text(pDX, IDC_MODELSPATH, m_strModelsPath);
	DDX_Text(pDX, IDC_SOUNDPATH, m_strSoundPath);
	DDX_Text(pDX, IDC_MUSICPATH, m_strMusicPath);
	DDX_Text(pDX, IDC_SPRITESPATH, m_strSpritesPath);
	DDX_Text(pDX, IDC_ENTITYPATH, m_strEntityPath);
	DDX_Text(pDX, IDC_PREFERENCESPATH, m_strPreferencesPath);
	DDX_Text(pDX, IDC_SHADERPATH, m_strShaderPath);
	DDX_Text(pDX, IDC_UTILSPATH, m_strUtilsPath);
	DDX_Text(pDX, IDC_TEMPPATH, m_strTEMPPath);
}


BEGIN_MESSAGE_MAP(CPathesPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BROWSEGAMEEXECUTABLEBUTTON, OnBnClickedBrowsegameexecutablebutton)
	ON_BN_CLICKED(IDC_BROWSEGAMEDIRBUTTON, OnBnClickedBrowsegamedirbutton)
	ON_BN_CLICKED(IDC_BROWSEMODDIRBUTTON, OnBnClickedBrowsemoddirbutton)
	ON_BN_CLICKED(IDC_BROWSEMAPDIRBUTTON, OnBnClickedBrowsemapdirbutton)
END_MESSAGE_MAP()


// CPathesPage message handlers

void CPathesPage::OnBnClickedBrowsegameexecutablebutton()
{
	// TODO: Add your control notification handler code here
	CFileDialog fileDlg(TRUE, "exe", "*.exe", 0, "Exe Files (*.exe)|*.exe||");
	if (fileDlg.DoModal() == IDOK)
	{
		const CString pathName = fileDlg.GetPathName();
		// TODO: ...
	}
}

void CPathesPage::OnBnClickedBrowsegamedirbutton()
{
	// TODO: Add your control notification handler code here
	CFileDialog fileDlg(TRUE);
	if (fileDlg.DoModal() == IDOK)
	{
		const CString pathName = fileDlg.GetPathName();
		// TODO: ...
	}
}

void CPathesPage::OnBnClickedBrowsemoddirbutton()
{
	// TODO: Add your control notification handler code here
	CFileDialog fileDlg(TRUE);
	if (fileDlg.DoModal() == IDOK)
	{
		const CString pathName = fileDlg.GetPathName();
		// TODO: ...
	}
}

void CPathesPage::OnBnClickedBrowsemapdirbutton()
{
	// TODO: Add your control notification handler code here
	CFileDialog fileDlg(TRUE);
	if (fileDlg.DoModal() == IDOK)
	{
		const CString pathName = fileDlg.GetPathName();
		// TODO: ...
	}
}
