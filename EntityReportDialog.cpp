// EntityReportDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MaxEd.h"
#include "EditWnd.h"
#include "EntityReportDialog.h"
#include ".\entityreportdialog.h"


// CEntityReportDialog dialog

IMPLEMENT_DYNAMIC(CEntityReportDialog, CDialog)
CEntityReportDialog::CEntityReportDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEntityReportDialog::IDD, pParent)
	, m_nBrushFilter(0)
	, m_bIncludeHidden(FALSE)
	, m_bByKeyValue(FALSE)
	, m_bExactMatch(FALSE)
	, m_strKey(_T(""))
	, m_strValue(_T(""))
	, m_bByClass(FALSE)
	, m_strClassName(_T(""))
{
}

CEntityReportDialog::~CEntityReportDialog()
{
}

BOOL CEntityReportDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	/* clear selection */
	BRUSH::ClearSelectedBrushList();
	CEditWnd::RedrawWindow(WU_ALLEDIT);

	/* fill entity-class list*/
	for (INT_PTR i = 0; i < ENTITYINFO::GLOBALENTITYINFOLIST.GetSize(); ++i)
		m_cClassList.AddString(ENTITYINFO::GLOBALENTITYINFOLIST[i].name);

	/* init entity list */
	UpdateEntityList();

	return TRUE;
}

void CEntityReportDialog::AddEntityToList(const CString& name, ENTITY* pOwner)
{
	// add entity name to list-box
	int nPos = m_cEntityList.AddString(name);
	ASSERT(nPos != LB_ERR);
	ASSERT(nPos != LB_ERRSPACE);
	// add ENTITY*-pointer to list-box pos
	m_cEntityList.SetItemDataPtr(nPos, pOwner);
}

void CEntityReportDialog::UpdateEntityList()
{
	CString val;

	// update dialog data variables (via DDE/DDX)
	UpdateData();
	// clear all strings
	m_cEntityList.ResetContent();


	for (ENTITY* pE = ENTITY::GLOBALENTITYLIST; pE != ENTITY::GLOBALENTITYLISTPTR; ++pE)
	{
		// main filter
		if (m_nBrushFilter)
			switch (m_nBrushFilter)
			{
			case 1:	// -- solid?
				if (pE->color != ENTITYINFO::BRUSHCOLOR)
					continue;
				break;
			case 2:	// -- point?
				if (pE->color == ENTITYINFO::BRUSHCOLOR)
					continue;
				break;
			}
		// else -- everything (skip!)

		if (m_bByKeyValue)
		{
			if (pE->Lookup(m_strKey, val))
			{
				if (m_bExactMatch) {
					if (val == m_strValue)
						AddEntityToList(pE->name, pE);
				}
				else
					if (val.Find(m_strValue) != -1)
						AddEntityToList(pE->name, pE);
			}
		}
		else if (m_bByClass && !m_strClassName.IsEmpty())
		{
			if (pE->name == m_strClassName)
				AddEntityToList(pE->name, pE);
		}
		else
			AddEntityToList(pE->name, pE);
	}
}

void CEntityReportDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ENTITYLIST, m_cEntityList);
	DDX_Radio(pDX, IDC_EVERYTHINGCHECK, m_nBrushFilter);
	DDX_Check(pDX, IDC_INCLUDEHIDDENCHECK, m_bIncludeHidden);
	DDX_Check(pDX, IDC_BYKEYVALUECHECK, m_bByKeyValue);
	DDX_Check(pDX, IDC_EXACTMATCH, m_bExactMatch);
	DDX_Text(pDX, IDC_KEYEDIT, m_strKey);
	DDX_Text(pDX, IDC_VALUEEDIT, m_strValue);
	DDX_Check(pDX, IDC_BYCLASSCHECK, m_bByClass);
	DDX_Control(pDX, IDC_BYCLASSCOMBO, m_cClassList);
	DDX_CBString(pDX, IDC_BYCLASSCOMBO, m_strClassName);
}


BEGIN_MESSAGE_MAP(CEntityReportDialog, CDialog)
	ON_BN_CLICKED(IDC_MARKBUTTON, OnBnClickedMarkbutton)
	ON_BN_CLICKED(IDC_GOTOBUTTON, OnBnClickedGotobutton)
	ON_BN_CLICKED(IDC_DELETEBUTTON, OnBnClickedDeletebutton)
	ON_BN_CLICKED(IDC_PROPERTIESBUTTON, OnBnClickedPropertiesbutton)
	ON_BN_CLICKED(IDC_BYKEYVALUECHECK, OnBnClickedBykeyvaluecheck)
	ON_BN_CLICKED(IDC_BYCLASSCHECK, OnBnClickedByclasscheck)
	ON_CBN_SELCHANGE(IDC_BYCLASSCOMBO, OnCbnSelchangeByclasscombo)
END_MESSAGE_MAP()


// CEntityReportDialog message handlers

void CEntityReportDialog::OnBnClickedMarkbutton()
{
	// TODO: Add your control notification handler code here
	if (ENTITY* pEntity = (ENTITY*)GetEntityFromList())
	{
		// clear selection
		BRUSH::ClearSelectedBrushList();
		// select entity`s brushes
		if (BRUSH::SelectByOwner(pEntity))
		{
			// reinit selection
			BRUSH::ReInitSelectedBrushList();
			// and redraw all editable window
			CEditWnd::RedrawWindow(WU_ALLEDIT);
		}
	}
}

void CEntityReportDialog::OnBnClickedGotobutton()
{	// same as CEntityReportDialog::OnBnClickedMarkbutton() + CEditWnd::CenteredView()

	// TODO: Add your control notification handler code here
	if (ENTITY* pEntity = (ENTITY*)GetEntityFromList())
	{
		// clear selection
		BRUSH::ClearSelectedBrushList();
		// select entity`s brushes
		if (BRUSH::SelectByOwner(pEntity))
		{
			// reinit selection
			BRUSH::ReInitSelectedBrushList();
			// centered ortho views on selection (automatically redraw 2D)
			CEditWnd::CenteredView(*BRUSH::SELECTEDBRUSHLIST[0]);
			// and redraw 3d window
			CEditWnd::RedrawWindow(WU_3D);
		}
	}
}

void CEntityReportDialog::OnBnClickedDeletebutton()
{
	// TODO: Add your control notification handler code here
	if (ENTITY* pEntity = (ENTITY*)GetEntityFromList())
	{
		// clear selection
		BRUSH::ClearSelectedBrushList();
		// select entity`s brushes
		if (BRUSH::SelectByOwner(pEntity))
		{
			// reinit selection
			BRUSH::ReInitSelectedBrushList();
			// delete selection
			BRUSH::DeleteSelectedBrushList();
			// and redraw all editable window
			CEditWnd::RedrawWindow(WU_ALLEDIT);
		}
	}
}

void CEntityReportDialog::OnBnClickedPropertiesbutton()
{
	// TODO: Add your control notification handler code here
	if (ENTITY* pEntity = (ENTITY*)GetEntityFromList())
	{
		// clear selection
		BRUSH::ClearSelectedBrushList();
		// select entity`s brushes
		if (BRUSH::SelectByOwner(pEntity))
		{
			// reinit selection
			BRUSH::ReInitSelectedBrushList();
			// and redraw all editable window
			CEditWnd::RedrawWindow(WU_ALLEDIT);
			// update dlg info in "Entity Inspector" dialog
			CEditWnd::ENTITYINSPECTORDIALOG.SetInfo();
			// enable "Entity Inspector" dialog (becouse "Entity Report" is modal dlg!)
			CEditWnd::ENTITYINSPECTORDIALOG.EnableWindow(TRUE);
			// show "Entity Inspector" dialog
			CEditWnd::ENTITYINSPECTORDIALOG.ShowWindow(SW_SHOW);
		}
	}
}

void CEntityReportDialog::OnBnClickedBykeyvaluecheck()
{
	// TODO: Add your control notification handler code here
	CEdit *pKeyEdit = (CEdit *)GetDlgItem(IDC_KEYEDIT);
	ASSERT(pKeyEdit);
	CEdit *pValueEdit = (CEdit *)GetDlgItem(IDC_VALUEEDIT);
	ASSERT(pValueEdit);
	CButton *pExactCheckBox = (CButton *)GetDlgItem(IDC_EXACTMATCH);
	ASSERT(pExactCheckBox);

	// update dialog data variables (via DDE/DDX)
	UpdateData();
	// update filter and prepare new entity list
	UpdateEntityList();
	// enable/disable key edit for input
	pKeyEdit->EnableWindow(m_bByKeyValue);
	// enable/disable value edit for input
	pValueEdit->EnableWindow(m_bByKeyValue);
	// enable/disable "exact" check box
	pExactCheckBox->EnableWindow(m_bByKeyValue);
}

void CEntityReportDialog::OnBnClickedByclasscheck()
{
	// TODO: Add your control notification handler code here

	// update dialog data variables (via DDE/DDX)
	UpdateData();
	// update filter and prepare new entity list
	UpdateEntityList();
	// enable/disable window
	m_cClassList.EnableWindow(m_bByClass);
}

void CEntityReportDialog::OnCancel()
{
	// TODO: Add your control notification handler code here
	if (CWnd::GetFocus() == GetDlgItem(IDC_KEYEDIT) ||
		CWnd::GetFocus() == GetDlgItem(IDC_VALUEEDIT))
	{
		// update dialog data variables (via DDE/DDX)
		UpdateData();
		// update filter and prepare new entity list
		UpdateEntityList();
	}
	else
		CDialog::OnCancel();
}

void CEntityReportDialog::OnCbnSelchangeByclasscombo()
{
	// TODO: Add your control notification handler code here
	// update dialog data variables (via DDE/DDX)
	UpdateData();
	// update filter and prepare new entity list
	UpdateEntityList();
}
