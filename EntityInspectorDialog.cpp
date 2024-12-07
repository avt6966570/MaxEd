// EntityInspectorDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MaxEd.h"
#include "EditWnd.h"// << #include "EntityInspectorDialog.h"

// CEntityInspectorDialog dialog

IMPLEMENT_DYNAMIC(CEntityInspectorDialog, CDialog)

CEntityInspectorDialog::CEntityInspectorDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEntityInspectorDialog::IDD, pParent)
{
}

BOOL CEntityInspectorDialog::Create(CWnd* pParent /*= NULL*/)
{
	return CDialog::Create(CEntityInspectorDialog::IDD, pParent);
}

BOOL CEntityInspectorDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
   
	// TODO: Add extra initialization here

	// 1. INIT WORLD ENTITY DEFINITIONS
	CFile deffile;
	CFileException fileException;
	if (!deffile.Open(EDITORENV_ENTITYDEF_PATH,CFile::modeRead, &fileException))
	{
		::AfxThrowFileException(fileException.m_cause, -1, EDITORENV_ENTITYDEF_PATH);
		//::AfxMessageBox("Couldn't open entity definitions file \"entities.def\"!");
		return FALSE;
	}
	else if (deffile.GetLength() == INVALID_FILE_SIZE)
	{
		::AfxMessageBox("Couldn't open entity definitions file \"entities.def\": INVALID_FILE_SIZE");		
		return FALSE;
	}
	else if (deffile.GetLength() > 1<<18)
	{
		::AfxMessageBox("Couldn't open entity definitions file \"entities.def\": file too large (need < 262162 bytes)");
		return FALSE;
	}
	else
	{
		CString defstring;
		if (!deffile.Read(defstring.GetBuffer(deffile.GetLength()), deffile.GetLength()))
		{
			::AfxMessageBox("Couldn't read entity definitions file \"entities.def\"!");
			return FALSE;
		}
		defstring.ReleaseBuffer(deffile.GetLength());
		if (!ENTITYINFO::ParseDefFile(defstring))
		{
			::AfxMessageBox("Couldn't parse entity definitions file \"entities.def\"!");
			return FALSE;
		}
	}

	// 2. FILL CLASS LIST
	CListBox* pEntityList = (CListBox*)GetDlgItem(IDC_ENTITYINSPECTORDIALOG_ENTITYLIST);
	ASSERT(pEntityList);
	pEntityList->ResetContent();
	pEntityList->AddString("");
	for(INT_PTR i = 0; i < ENTITYINFO::GLOBALENTITYINFOLIST.GetSize(); ++i)
		pEntityList->AddString(ENTITYINFO::GLOBALENTITYINFOLIST[i].name);

	// 3. PREPARE EPAIRS CLISTCTRL
	CListCtrl* pEpairsList = (CListCtrl*)GetDlgItem(IDC_ENTITYINSPECTORDIALOG_EPAIRS);
	ASSERT(pEpairsList);
	pEpairsList->InsertColumn(0, "Key");
	pEpairsList->InsertColumn(1, "Value");
	pEpairsList->SetColumnWidth(0, 140);
	pEpairsList->SetColumnWidth(1, 140);
	pEpairsList->SetExtendedStyle(
//		LVS_EX_BORDERSELECT |	// when an item is selected the border color of the item changes rather than the item being highlighted
//		LVS_EX_HEADERDRAGDROP |
		LVS_EX_FULLROWSELECT |	// when an item is selected, the item and all its subitems are highlighted
		LVS_EX_GRIDLINES		// displays gridlines around items and subitems
		);

	return TRUE;
}

void CEntityInspectorDialog::SetInfo()
{
	ENTITYINFO *pCurEntityInfo;
	CString key, value;

	// clear epairs
	CListCtrl* pEpairsList = (CListCtrl*)GetDlgItem(IDC_ENTITYINSPECTORDIALOG_EPAIRS);
	ASSERT(pEpairsList);
	pEpairsList->DeleteAllItems();

	// clear description
	CEdit* pDescriptionEdit = (CEdit*)GetDlgItem(IDC_ENTITYINSPECTORDIALOG_DESCRIPTION);
	ASSERT(pDescriptionEdit);
	pDescriptionEdit->SetWindowText("");

	// clear flags
	for(int f = IDC_ENTITYINSPECTORDIALOG_FLAG0; f <= IDC_ENTITYINSPECTORDIALOG_FLAG14; ++f)
	{
		CButton* pFlagBox = (CButton*)GetDlgItem(f);
		ASSERT(pFlagBox);
		pFlagBox->SetCheck(BST_UNCHECKED);
		pFlagBox->SetWindowText("");
	}

		// select entity class in entity list
	CListBox* pEntityList = (CListBox*)GetDlgItem(IDC_ENTITYINSPECTORDIALOG_ENTITYLIST);
	ASSERT(pEntityList);

	if(!BRUSH::SELECTEDBRUSHLIST.empty() &&
		BRUSH::SELECTEDBRUSHLIST[0]->owner &&
		pEntityList->SelectString(-1, BRUSH::SELECTEDBRUSHLIST[0]->owner->name) != LB_ERR)
	{
		// fill current entity epairs
		int iListRow = 0;
		for(POSITION pos = BRUSH::SELECTEDBRUSHLIST[0]->owner->epairs.GetStartPosition();
			pos != 0;
			)
		{
			BRUSH::SELECTEDBRUSHLIST[0]->owner->epairs.GetNextAssoc(pos, key, value);
			pEpairsList->InsertItem(iListRow, key);
			pEpairsList->SetItemText(iListRow, 1, value);
			++iListRow;
		}
		// + "add new (filler <...>)" for epairs list
		pEpairsList->InsertItem(iListRow, "<...>");

		if(pCurEntityInfo = ENTITYINFO::FindEntityByName(BRUSH::SELECTEDBRUSHLIST[0]->owner->name))
		{
		// print "Description" for entity
			pDescriptionEdit->SetWindowText(pCurEntityInfo->desc);

		// fill entity flags
			for(int f = IDC_ENTITYINSPECTORDIALOG_FLAG0, bitoff = 0;
				f <= IDC_ENTITYINSPECTORDIALOG_FLAG14 &&
					pCurEntityInfo->flagnames.GetSize() > bitoff;
				++f, ++bitoff)
				{
					CButton* pFlagBox = (CButton*)GetDlgItem(f);
					ASSERT(pFlagBox);
					pFlagBox->SetCheck(BRUSH::SELECTEDBRUSHLIST[0]->owner->flags & 1<<bitoff ? BST_CHECKED : BST_UNCHECKED);
					pFlagBox->SetWindowText(pCurEntityInfo->flagnames[bitoff]);
				}
		}
	}
	else
		pEntityList->SetCurSel(0);
}

CEntityInspectorDialog::~CEntityInspectorDialog()
{
}

void CEntityInspectorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEntityInspectorDialog, CDialog)
	//{{AFX_MSG_MAP(CEditorWnd)
	ON_LBN_SELCHANGE(IDC_ENTITYINSPECTORDIALOG_ENTITYLIST, OnSelectEntityList)
	ON_NOTIFY(LVN_ENDLABELEDIT,
			  IDC_ENTITYINSPECTORDIALOG_EPAIRS,
			  OnLvnEndlabeleditEntityinspectordialogEpairs)
	ON_COMMAND_RANGE(IDC_ENTITYINSPECTORDIALOG_FLAG0,
					 IDC_ENTITYINSPECTORDIALOG_FLAG14,
					 OnChangeFlagBoxes)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// CEntityInspectorDialog message handlers

void CEntityInspectorDialog::OnSelectEntityList()
{
	if(!BRUSH::SELECTEDBRUSHLIST.empty())
	{
		CListBox* pEntityList = (CListBox*)GetDlgItem(IDC_ENTITYINSPECTORDIALOG_ENTITYLIST);
		ASSERT(pEntityList);

		int nCurSel = pEntityList->GetCurSel();
		switch(nCurSel)
		{
		case 0:			// unmake entity
			for(BRUSH::ptrbrushlist::iterator pb = BRUSH::SELECTEDBRUSHLIST.begin();
					pb != BRUSH::SELECTEDBRUSHLIST.end();
					++pb)
						(*pb)->owner = 0
						;
			break;
		case LB_ERR:
			// no item is currently selected or
			// if the list box is a multiple-selection list box.
			break;
		default:		// make entity
			{{{
			CString ename;
			pEntityList->GetText(nCurSel, ename);
			if(ENTITYINFO *pEntityInfo = ENTITYINFO::FindEntityByName(ename))
			{
				// for all selected brushes
				*ENTITY::GLOBALENTITYLISTPTR = *pEntityInfo;
				for(BRUSH::ptrbrushlist::iterator pb = BRUSH::SELECTEDBRUSHLIST.begin();
						pb != BRUSH::SELECTEDBRUSHLIST.end();
						++pb)
							(*pb)->owner = ENTITY::GLOBALENTITYLISTPTR;
				++ENTITY::GLOBALENTITYLISTPTR;
			}
			}}}
			break;
		}

		SetInfo();	// update dlg items
	}
}

void CEntityInspectorDialog::OnLvnEndlabeleditEntityinspectordialogEpairs(NMHDR *pNMHDR,
																		  LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

	// TODO: Add your control notification handler code here
	*pResult = 0;

	// +++ PARSE NEW EPAIR +++
	if(pDispInfo->item.mask & LVIF_TEXT)
	{
		CListCtrl* pEpairsList = (CListCtrl*)GetDlgItem(IDC_ENTITYINSPECTORDIALOG_EPAIRS);
		ASSERT(pEpairsList);
		CString epair(pDispInfo->item.pszText, pDispInfo->item.cchTextMax), key, value;
		const bool bAddNew = pEpairsList->GetItemCount() - 1 == pDispInfo->item.iItem;	// click on last (filler) item?
		
		if(epair == "")
		{	// delete epair ?
			if(!bAddNew)
			{
				key = pEpairsList->GetItemText(pDispInfo->item.iItem, 0);
				for(BRUSH::ptrbrushlist::iterator pb = BRUSH::SELECTEDBRUSHLIST.begin();
					pb != BRUSH::SELECTEDBRUSHLIST.end();
					++pb)
						(*pb)->owner->RemoveKey(key);
				pEpairsList->DeleteItem(pDispInfo->item.iItem);
			}
		}
		else
		{	// update epair
			int iStart = 0;
			if((key = epair.Tokenize(" ", iStart)) != "")
			{
				while(epair[iStart] == ' ')	// skip whitespace
					++iStart;				//
				value = epair.Mid(iStart);
				for(BRUSH::ptrbrushlist::iterator pb = BRUSH::SELECTEDBRUSHLIST.begin();
					pb != BRUSH::SELECTEDBRUSHLIST.end();
					++pb)
						(*pb)->owner->UpdateKey(key, value);
				pEpairsList->SetItemText(pDispInfo->item.iItem, 0, key);
				pEpairsList->SetItemText(pDispInfo->item.iItem, 1, value);
			}

			if(bAddNew)		// inc filler
				pEpairsList->InsertItem(pEpairsList->GetItemCount(), "<...>");
		}
	}
}

void CEntityInspectorDialog::OnChangeFlagBoxes(UINT nID)
{
	ASSERT(nID >= IDC_ENTITYINSPECTORDIALOG_FLAG0 &&
		   nID <= IDC_ENTITYINSPECTORDIALOG_FLAG14);

	if(!BRUSH::SELECTEDBRUSHLIST.empty())
	{
		CButton* pFlagBox = (CButton*)GetDlgItem(nID);
		ASSERT(pFlagBox);

		switch(pFlagBox->GetCheck())
		{
		case BST_UNCHECKED:
			for(BRUSH::ptrbrushlist::iterator pb = BRUSH::SELECTEDBRUSHLIST.begin();
					pb != BRUSH::SELECTEDBRUSHLIST.end();
					++pb)
			{
				if((*pb)->owner)
					(*pb)->owner->flags &= ~(1 << (nID - IDC_ENTITYINSPECTORDIALOG_FLAG0));
			}
			break;
		case BST_CHECKED:
			for(BRUSH::ptrbrushlist::iterator pb = BRUSH::SELECTEDBRUSHLIST.begin();
					pb != BRUSH::SELECTEDBRUSHLIST.end();
					++pb)
			{
				if((*pb)->owner)
					(*pb)->owner->flags |= 1 << (nID - IDC_ENTITYINSPECTORDIALOG_FLAG0);
			}
			break;
		}
	}
}

void CEntityInspectorDialog::OnCancel()
{
	ShowWindow(SW_HIDE);
	ASSERT(CEditWnd::pActiveWindow);
	CEditWnd::pActiveWindow->SetFocus();	// set focus on active editwnd
}