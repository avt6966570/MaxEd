// MapInformationDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MaxEd.h"
#include "MapInformationDialog.h"


// CMapInformationDialog dialog

IMPLEMENT_DYNAMIC(CMapInformationDialog, CDialog)

CMapInformationDialog::CMapInformationDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMapInformationDialog::IDD, pParent)
{
}

CMapInformationDialog::CMapInformationDialog(UINT uSolids, UINT uFaces,
											UINT uPointEntities, UINT uSolidEntities,
											UINT uUniqueTextures, UINT uTextureMemory,
											const CStringArray* parTexPathes,
											CWnd* pParent /*= NULL*/)
	: CDialog(CMapInformationDialog::IDD, pParent)
	, m_uSolids(uSolids)
	, m_uFaces(uFaces)
	, m_uPointEntities(uPointEntities)
	, m_uSolidEntities(uSolidEntities)
	, m_uUniqueTextures(uUniqueTextures)
	, m_uTextureMemory(uTextureMemory)
	, m_parTexPathes(parTexPathes)
{
}

CMapInformationDialog::~CMapInformationDialog()
{
}

void CMapInformationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CMapInformationDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CString str;

	// set solids num
	str.Format("Solids:\t\t  %u", m_uSolids);
	::SetWindowText(GetDlgItem(IDC_NUMSOLIDS)->m_hWnd, str);
	// set faces num
	str.Format("Faces:\t\t  %u", m_uFaces);
	::SetWindowText(GetDlgItem(IDC_NUMFACES)->m_hWnd, str);
	// set point entities num
	str.Format("PointEntities:\t  %u", m_uPointEntities);
	::SetWindowText(GetDlgItem(IDC_NUMPOINTENTITIES)->m_hWnd, str);
	// set solid entities num
	str.Format("SolidEntities:\t  %u", m_uSolidEntities);
	::SetWindowText(GetDlgItem(IDC_NUMSOLIDENTITIES)->m_hWnd, str);
	// set unique textures num
	str.Format("Unique textures:  %u", m_uUniqueTextures);
	::SetWindowText(GetDlgItem(IDC_NUMUNIQUETEXTURES)->m_hWnd, str);
	// set texture memory quantity
	str.Format("Texture memory: %u bytes (%.2fMB)",
		m_uTextureMemory, m_uTextureMemory/1048576.F);
	::SetWindowText(GetDlgItem(IDC_NUMTEXTUREMEMORY)->m_hWnd, str);
	// fill "used texture pathes" listbox
	CListBox *pTextureList = (CListBox *)GetDlgItem(IDC_TEXTUREUSEDLIST);
	ASSERT(pTextureList);
	INT_PTR n = pTextureList->InitStorage(m_parTexPathes->GetSize(), 32);
	ASSERT(n != LB_ERRSPACE);
	for (INT_PTR i = 0; i < m_parTexPathes->GetSize(); ++i)
		pTextureList->AddString((*m_parTexPathes)[i]);

	return TRUE;
}


BEGIN_MESSAGE_MAP(CMapInformationDialog, CDialog)
END_MESSAGE_MAP()


// CMapInformationDialog message handlers
