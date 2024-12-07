// MapProblemsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MaxEd.h"
#include "MapProblemsDialog.h"
#include "EditWnd.h"			// "Go to error": EditWnd::CenteredView(BRUSH&)!


// MAP ERRORS (TODO: paste it in "Brush.h"/"EditWnd.h"???)
static enum eMapError {
	ME_NOPLAYERSTART,
	ME_INVALIDBRUSH,
		ME_SIZE
};

static const TCHAR* szaMapErrorName[ME_SIZE] = {
	_T("There is no player start"),
	_T("Invalid brush")
};




// CMapProblemsDialog dialog

IMPLEMENT_DYNAMIC(CMapProblemsDialog, CDialog)

CMapProblemsDialog::CMapProblemsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMapProblemsDialog::IDD, pParent)
{
}

CMapProblemsDialog::~CMapProblemsDialog()
{
}

void CMapProblemsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMapProblemsDialog, CDialog)
	ON_LBN_SELCHANGE(IDC_MAPERRORLIST, OnLbnSelchangeMaperrorlist)
	ON_BN_CLICKED(IDC_GOTOERROR, OnBnClickedGotoerror)
	ON_BN_CLICKED(IDC_FIXERROR, OnBnClickedFixerror)
	ON_BN_CLICKED(IDC_FIXALLERROR, OnBnClickedFixallerror)
END_MESSAGE_MAP()


// CMapProblemsDialog message handlers
BOOL CMapProblemsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CListBox *pErrorList = (CListBox *)GetDlgItem(IDC_MAPERRORLIST);
	ASSERT(pErrorList);

	// 1. find player start
	if (!BRUSH::FindPlayerStart())
	{
		int nPos = pErrorList->AddString(szaMapErrorName[ME_NOPLAYERSTART]);
		pErrorList->SetItemData(nPos, ME_NOPLAYERSTART);
	}

	// 2. find invalid brushes
	BRUSH::ptrbrushlist blist;
	if (BRUSH::FindInvalidBrushes(blist))
		for (BRUSH::ptrbrushlist::const_iterator pB = blist.begin();
			pB != blist.begin();
			++pB)
		{
			int nPos = pErrorList->AddString(szaMapErrorName[ME_INVALIDBRUSH]);
			pErrorList->SetItemData(nPos, ME_INVALIDBRUSH);
			pErrorList->SetItemDataPtr(nPos, *pB);
		}


	return TRUE;
}

void CMapProblemsDialog::OnLbnSelchangeMaperrorlist()
{
	// TODO: Add your control notification handler code here
	CListBox *pErrorList = (CListBox *)GetDlgItem(IDC_MAPERRORLIST);
	ASSERT(pErrorList);
	CButton *pGotoErrorBtn = (CButton *)GetDlgItem(IDC_GOTOERROR);
	ASSERT(pGotoErrorBtn);
	CButton *pFixBtn = (CButton *)GetDlgItem(IDC_FIXERROR);
	ASSERT(pFixBtn);
	CButton *pFixAllBtn = (CButton *)GetDlgItem(IDC_FIXALLERROR);
	ASSERT(pFixAllBtn);

	// get current selection pos
	int nCurSel = pErrorList->GetCurSel();
	ASSERT(nCurSel != LB_ERR);
	// get error data
	switch (pErrorList->GetItemData(nCurSel))
	{
	case ME_NOPLAYERSTART:
		pGotoErrorBtn->EnableWindow(FALSE);
		pFixBtn->EnableWindow(FALSE);
		pFixAllBtn->EnableWindow(FALSE);
		break;
	case ME_INVALIDBRUSH:
		pFixBtn->EnableWindow(FALSE);
		pFixAllBtn->EnableWindow(FALSE);
		break;
	}
}

void CMapProblemsDialog::OnBnClickedGotoerror()
{
	// TODO: Add your control notification handler code here
	BRUSH *pBrush;
	CListBox *pErrorList = (CListBox *)GetDlgItem(IDC_MAPERRORLIST);
	ASSERT(pErrorList);

	// get current selection pos
	int nCurSel = pErrorList->GetCurSel();
	ASSERT(nCurSel != LB_ERR);
	// get error data
	switch (pErrorList->GetItemData(nCurSel))
	{
	case ME_NOPLAYERSTART:
		break;
	case ME_INVALIDBRUSH:
		// get brush pointer
		pBrush = (BRUSH *)pErrorList->GetItemDataPtr(nCurSel);
		// clear all selection
		BRUSH::ClearSelectedBrushList();
		// select brush
		pBrush->flags |= BRUSH::BF_SELECTED;
		// add to selected brush list
		BRUSH::SELECTEDBRUSHLIST.push_back(pBrush);
		// centered ortho views on selection (automatically redraw 2D)
		CEditWnd::CenteredView(*pBrush);
		// and redraw 3d window
		CEditWnd::RedrawWindow(WU_3D);
		break;
	}
}

void CMapProblemsDialog::OnBnClickedFixerror()
{
	// TODO: Add your control notification handler code here
}

void CMapProblemsDialog::OnBnClickedFixallerror()
{
	// TODO: Add your control notification handler code here
}
