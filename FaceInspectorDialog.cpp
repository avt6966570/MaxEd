// FaceInspectorDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MaxEd.h"
#include "EditWnd.h"// << #include "FaceInspectorDialog.h"

// CFaceInspectorDialog dialog

IMPLEMENT_DYNAMIC(CFaceInspectorDialog, CDialog)
CFaceInspectorDialog::CFaceInspectorDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFaceInspectorDialog::IDD, pParent)
{
}

BOOL CFaceInspectorDialog::Create(CWnd* pParent /*= NULL*/)
{
	return CDialog::Create(CFaceInspectorDialog::IDD, pParent);
}

CFaceInspectorDialog::~CFaceInspectorDialog()
{
}

void CFaceInspectorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFaceInspectorDialog, CDialog)
	//{{AFX_MSG_MAP(CFaceInspectorDialog)
	ON_NOTIFY_RANGE(UDN_DELTAPOS,
				    IDC_FACEINSPECTOR_TEXTUREINFO_XSHIFTSPIN,
					IDC_FACEINSPECTOR_TEXTUREINFO_ROTATIONSPIN,
					OnDeltaPosSpin)
	ON_COMMAND_RANGE(IDC_FACEINSPECTOR_TEXTUREINFO_JUSTIFY_LEFT,
					IDC_FACEINSPECTOR_TEXTUREINFO_JUSTIFY_FIT,
					OnBnClickedJustify)
	ON_COMMAND_RANGE(IDC_FACEINSPECTOR_TEXTUREINFO_ALIGN_WORLD,
					IDC_FACEINSPECTOR_TEXTUREINFO_ALIGN_FACE,
					OnBnClickedAlign)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CFaceInspectorDialog message handlers
BOOL CFaceInspectorDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
   
	// TODO: Add extra initialization here
	CSpinButtonCtrl *pXShiftSpin, *pYShiftSpin,
					*pXScaleSpin, *pYScaleSpin,
					*pRotationSpin;

	pXShiftSpin = (CSpinButtonCtrl *)GetDlgItem(IDC_FACEINSPECTOR_TEXTUREINFO_XSHIFTSPIN);
	ASSERT(pXShiftSpin);
	pXShiftSpin->SetRange32(UD_MINVAL, UD_MAXVAL);

	pYShiftSpin = (CSpinButtonCtrl *)GetDlgItem(IDC_FACEINSPECTOR_TEXTUREINFO_YSHIFTSPIN);
	ASSERT(pYShiftSpin);
	pYShiftSpin->SetRange32(UD_MINVAL, UD_MAXVAL);


	pXScaleSpin = (CSpinButtonCtrl *)GetDlgItem(IDC_FACEINSPECTOR_TEXTUREINFO_XSCALESPIN);
	ASSERT(pXScaleSpin);
	pXScaleSpin->SetRange32(UD_MINVAL, UD_MAXVAL);
	pXScaleSpin->SetPos(100);

	pYScaleSpin = (CSpinButtonCtrl *)GetDlgItem(IDC_FACEINSPECTOR_TEXTUREINFO_YSCALESPIN);
	ASSERT(pYScaleSpin);
	pYScaleSpin->SetRange32(UD_MINVAL, UD_MAXVAL);
	pYScaleSpin->SetPos(100);


	pRotationSpin = (CSpinButtonCtrl *)GetDlgItem(IDC_FACEINSPECTOR_TEXTUREINFO_ROTATIONSPIN);
	ASSERT(pRotationSpin);
	pRotationSpin->SetRange32(-359, 359);

	return TRUE;
}

void CFaceInspectorDialog::SetInfo()
{
	if (FACE *pActiveFace = !BRUSH::SELECTEDBRUSHLIST.empty() ?
			&BRUSH::SELECTEDBRUSHLIST[0]->flist[0] : FACE::SELECTED.pFace)
	{
		SetTexInfo(pActiveFace);
		SetTexFlags(pActiveFace);
		SetTexMod(pActiveFace);
	}
}

void CFaceInspectorDialog::SetTexInfo(FACE *pActiveFace)
{
	CString str;

	// set texture name
	str.Format("Name:\t %s", pActiveFace->texdef->name);
	::SetWindowText(
		GetDlgItem(IDC_FACEINSPECTOR_TEXTUREINFO_NAME)->m_hWnd, str);

	// set texture path
	if (const TEXTUREDIR* tdr =
		TEXTUREDIR::FindDirInTDCache(0, pActiveFace->texdef, 0))
	{
		str.Format("Path:\t %s\\", tdr->name);
		::SetWindowText(
			GetDlgItem(IDC_FACEINSPECTOR_TEXTUREINFO_PATH)->m_hWnd, str);
	}

	// set texture size
	str.Format("Size:\t %dx%dx%d", pActiveFace->texdef->width,
		pActiveFace->texdef->height, pActiveFace->texdef->bpp);
	::SetWindowText(
		GetDlgItem(IDC_FACEINSPECTOR_TEXTUREINFO_SIZE)->m_hWnd, str
		);

	// set texture ogl ID
	str.Format("GL_ID:\t %d", pActiveFace->texdef->texID);
	::SetWindowText(
		GetDlgItem(IDC_FACEINSPECTOR_TEXTUREINFO_GLID)->m_hWnd, str
		);

	// set texture in-use?
	str = pActiveFace->texdef->bInUse ? "In Use:\t yes" : "In Use:\t no";
	::SetWindowText(
		GetDlgItem(IDC_FACEINSPECTOR_TEXTUREINFO_INUSE)->m_hWnd, str
		);
}

void CFaceInspectorDialog::SetTexFlags(FACE *pActiveFace)
{
	//for (UINT id = IDC_FACEINSPECTOR_TEXTUREINFO_FLAGS_1;
	//	id < IDC_FACEINSPECTOR_TEXTUREINFO_FLAGS_16 + 1;
	//	++id)
	//{
	//	CButton *pCheckBox = (CButton *)GetDlgItem(id);
	//	ASSERT(pCheckBox);
	//	pCheckBox->SetCheck(
	//		pActiveFace->flags & 1 << id - IDC_FACEINSPECTOR_TEXTUREINFO_FLAGS_1
	//		);
	//}
}

void CFaceInspectorDialog::SetTexMod(FACE *pActiveFace)
{
	CSpinButtonCtrl *pXShiftSpin, *pYShiftSpin,
					*pXScaleSpin, *pYScaleSpin,
					*pRotationSpin;
	CString str;


	// set texture s/t-shift information
	pXShiftSpin = (CSpinButtonCtrl *)GetDlgItem(IDC_FACEINSPECTOR_TEXTUREINFO_XSHIFTSPIN);
	ASSERT(pXShiftSpin);
	pXShiftSpin->SetPos((int)pActiveFace->shift.S);

	pYShiftSpin = (CSpinButtonCtrl *)GetDlgItem(IDC_FACEINSPECTOR_TEXTUREINFO_YSHIFTSPIN);
	ASSERT(pYShiftSpin);
	pYShiftSpin->SetPos((int)pActiveFace->shift.T);


	// set texture s/t-scale information
	pXScaleSpin = (CSpinButtonCtrl *)GetDlgItem(IDC_FACEINSPECTOR_TEXTUREINFO_XSCALESPIN);
	ASSERT(pXScaleSpin);
	pXScaleSpin->SetPos((int)(pActiveFace->scale.S * 100.F));
	str.Format("%4.2f", pActiveFace->scale.S);
	pXScaleSpin->GetBuddy()->SetWindowText(str);

	pYScaleSpin = (CSpinButtonCtrl *)GetDlgItem(IDC_FACEINSPECTOR_TEXTUREINFO_YSCALESPIN);
	ASSERT(pYScaleSpin);
	pYScaleSpin->SetPos((int)(pActiveFace->scale.T * 100.F));
	str.Format("%4.2f", pActiveFace->scale.T);
	pYScaleSpin->GetBuddy()->SetWindowText(str);


	// set texture rotation information
	pRotationSpin = (CSpinButtonCtrl *)GetDlgItem(IDC_FACEINSPECTOR_TEXTUREINFO_ROTATIONSPIN);
	ASSERT(pRotationSpin);
	pRotationSpin->SetPos((int)pActiveFace->rotate);


	// set VW-face aligning information
	CButton *pRadioBtn = (CButton *)GetDlgItem(
		pActiveFace->VWHasWorldAxialDir() ? 
		IDC_FACEINSPECTOR_TEXTUREINFO_ALIGN_WORLD :
		IDC_FACEINSPECTOR_TEXTUREINFO_ALIGN_FACE);
	ASSERT(pRadioBtn);
	pRadioBtn->SetCheck(BST_CHECKED);
}

void CFaceInspectorDialog::OnDeltaPosSpin(UINT nID, NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	// TODO: Add your control notification handler code here
	ASSERT(nID >= IDC_FACEINSPECTOR_TEXTUREINFO_XSHIFTSPIN &&
		   nID <= IDC_FACEINSPECTOR_TEXTUREINFO_ROTATIONSPIN);
	ASSERT(nID - IDC_FACEINSPECTOR_TEXTUREINFO_XSHIFTSPIN >= 0 &&
		   nID - IDC_FACEINSPECTOR_TEXTUREINFO_XSHIFTSPIN <= 4);
	float fVal;

	switch(nID)
	{
	case IDC_FACEINSPECTOR_TEXTUREINFO_XSCALESPIN:
	case IDC_FACEINSPECTOR_TEXTUREINFO_YSCALESPIN:
		{
		// calc new spin pos
		int pos = pNMUpDown->iPos + pNMUpDown->iDelta*10;
		// set spin pos (NOTE: dec iDelta)
		CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*)GetDlgItem(nID);
		ASSERT(pSpin);
		pSpin->SetPos(pos - pNMUpDown->iDelta);
		// calc val
		fVal = pos / 100.F;
		// set new window text
		CString str;
		str.Format("%4.2f", fVal);
		pSpin->GetBuddy()->SetWindowText(str);
		break;
		}
	case IDC_FACEINSPECTOR_TEXTUREINFO_ROTATIONSPIN:
		if (pNMUpDown->iPos == -359 && pNMUpDown->iDelta == -1 ||
			pNMUpDown->iPos == 359 && pNMUpDown->iDelta == 1)
		{
			*pResult = TRUE;
			return;
			break;
		}
	default:
		// calc val
		fVal = pNMUpDown->iPos + pNMUpDown->iDelta;
		break;
	}

	if (FACE::SELECTED.pFace)
	{
		FACE::SELECTED.pFace->shift[nID - IDC_FACEINSPECTOR_TEXTUREINFO_XSHIFTSPIN] = fVal;
		FACE::SELECTED.pFace->GenerateTextureCoords(
			FACE::SELECTED.pFaceOwner->vlist, ::ViewForPlane(FACE::SELECTED.pFace->N)
			);

		CEditWnd::RedrawWindow(WU_ALLEDIT);
	}
	else if (!BRUSH::SELECTEDBRUSHLIST.empty())
	{
		for (BRUSH::ptrbrushlist::iterator pb = BRUSH::SELECTEDBRUSHLIST.begin();
			pb != BRUSH::SELECTEDBRUSHLIST.end();
			++pb)
			for (BRUSH::facelist::iterator f = (*pb)->flist.begin();
				f != (*pb)->flist.end();
				++f)
			{
				f->shift[nID - IDC_FACEINSPECTOR_TEXTUREINFO_XSHIFTSPIN] = fVal;
				f->GenerateTextureCoords((*pb)->vlist, ::ViewForPlane(f->N));
			}

		CEditWnd::RedrawWindow(WU_ALLEDIT);
	}

	*pResult = FALSE;
}

void CFaceInspectorDialog::OnCancel()
{
	ShowWindow(SW_HIDE);
	ASSERT(CEditWnd::pActiveWindow);
	CEditWnd::pActiveWindow->SetFocus();	// set focus on active editwnd
}

void CFaceInspectorDialog::OnOK()
{
//	CDialog::OnOK();
	// TODO: Add your control notification handler code here
	CString buf;
	CEdit* pTMEdit;
	CSpinButtonCtrl* pTMSpin;
	VECTOR2 txshift, txscale;
	float txrotate;


	// *** GET TEXMOD INFO FROM DLG ITEMS ***
	// get s-shift
	pTMEdit = (CEdit*)GetDlgItem(IDC_FACEINSPECTOR_TEXTUREINFO_XSHIFTEDIT);
	ASSERT(pTMEdit);
	pTMEdit->GetWindowText(buf);
	txshift.S = ::atof(buf);

	// get t-shift
	pTMEdit = (CEdit*)GetDlgItem(IDC_FACEINSPECTOR_TEXTUREINFO_YSHIFTEDIT);
	ASSERT(pTMEdit);
	pTMEdit->GetWindowText(buf);
	txshift.T = ::atof(buf);

	// get s-scale
	pTMEdit = (CEdit*)GetDlgItem(IDC_FACEINSPECTOR_TEXTUREINFO_XSCALEEDIT);
	ASSERT(pTMEdit);
	pTMEdit->GetWindowText(buf);
	txscale.S = ::atof(buf);
	pTMSpin = (CSpinButtonCtrl *)GetDlgItem(IDC_FACEINSPECTOR_TEXTUREINFO_XSCALESPIN);
	ASSERT(pTMSpin);
	pTMSpin->SetPos(txscale.S * 100.F);	// update scale spin pos (becouse non correspond with val)

	// get t-scale
	pTMEdit = (CEdit*)GetDlgItem(IDC_FACEINSPECTOR_TEXTUREINFO_YSCALEEDIT);
	ASSERT(pTMEdit);
	pTMEdit->GetWindowText(buf);
	txscale.T = ::atof(buf);
	pTMSpin = (CSpinButtonCtrl *)GetDlgItem(IDC_FACEINSPECTOR_TEXTUREINFO_YSCALESPIN);
	ASSERT(pTMSpin);
	pTMSpin->SetPos(txscale.T * 100.F);	// update scale spin pos (becouse non correspond with val)

	// get rotation
	pTMEdit = (CEdit*)GetDlgItem(IDC_FACEINSPECTOR_TEXTUREINFO_ROTATIONEDIT);
	ASSERT(pTMEdit);
	pTMEdit->GetWindowText(buf);
	txrotate = ::atof(buf);


	// *** UPDATE TEXTURE INFO ON SELECTED ***
	if (FACE::SELECTED.pFace)
	{
		// update shift, scale, rotation
		FACE::SELECTED.pFace->shift = txshift;
		FACE::SELECTED.pFace->scale = txscale;
		FACE::SELECTED.pFace->rotate = txrotate;
		// recalc new texture coords
		FACE::SELECTED.pFace->GenerateTextureCoords(
			FACE::SELECTED.pFaceOwner->vlist, ::ViewForPlane(FACE::SELECTED.pFace->N)
			);
		// redraw all edit wnd
		CEditWnd::RedrawWindow(WU_ALLEDIT);
	}
	else if (!BRUSH::SELECTEDBRUSHLIST.empty())
	{
		// for all selected brushes
		for (BRUSH::ptrbrushlist::iterator pb = BRUSH::SELECTEDBRUSHLIST.begin();
			pb != BRUSH::SELECTEDBRUSHLIST.end();
			++pb)
			// for all faces on brush
			for (BRUSH::facelist::iterator f = (*pb)->flist.begin();
				f != (*pb)->flist.end();
				++f)
			{
				// update shift, scale, rotation
				f->shift = txshift;
				f->scale = txscale;
				f->rotate = txrotate;
				// recalc new texture coords
				f->GenerateTextureCoords((*pb)->vlist, ::ViewForPlane(f->N));
			}

		// redraw all edit wnd
		CEditWnd::RedrawWindow(WU_ALLEDIT);
	}
}

void CFaceInspectorDialog::OnBnClickedJustify(UINT nID)
{
	// TODO: Add your control notification handler code here
	ASSERT(nID >= IDC_FACEINSPECTOR_TEXTUREINFO_JUSTIFY_LEFT &&
		nID <= IDC_FACEINSPECTOR_TEXTUREINFO_JUSTIFY_FIT);
	ASSERT(nID - IDC_FACEINSPECTOR_TEXTUREINFO_JUSTIFY_LEFT >= 0 &&
		nID - IDC_FACEINSPECTOR_TEXTUREINFO_JUSTIFY_LEFT <= 5);


	// *** JUSTIFY TEXTURE INFO ON SELECTED ***
	if (FACE::SELECTED.pFace)
	{
		// justify texture
		FACE::SELECTED.pFace->JustifyTexture(
			FACE::SELECTED.pFaceOwner->vlist,
			nID - IDC_FACEINSPECTOR_TEXTUREINFO_JUSTIFY_LEFT
			);

		// update dlg info
		SetTexMod(FACE::SELECTED.pFace);

		// redraw all edit wnd
		CEditWnd::RedrawWindow(WU_ALLEDIT);
	}
	else if (!BRUSH::SELECTEDBRUSHLIST.empty())
	{
		// for all selected brushes
		for (BRUSH::ptrbrushlist::iterator pb = BRUSH::SELECTEDBRUSHLIST.begin();
			pb != BRUSH::SELECTEDBRUSHLIST.end();
			++pb)
			// for all faces on brush
			for (BRUSH::facelist::iterator f = (*pb)->flist.begin();
				f != (*pb)->flist.end();
				++f)
			{
				// justify texture
				f->JustifyTexture(
					(*pb)->vlist, nID - IDC_FACEINSPECTOR_TEXTUREINFO_JUSTIFY_LEFT);
			}

		// update dlg info
		SetTexMod(&BRUSH::SELECTEDBRUSHLIST[0]->flist[0]);

		// redraw all edit wnd
		CEditWnd::RedrawWindow(WU_ALLEDIT);
	}
}

void CFaceInspectorDialog::OnBnClickedAlign(UINT nID)
{
	// TODO: Add your control notification handler code here
	ASSERT(nID >= IDC_FACEINSPECTOR_TEXTUREINFO_ALIGN_WORLD &&
		nID <= IDC_FACEINSPECTOR_TEXTUREINFO_ALIGN_FACE);
	ASSERT(nID - IDC_FACEINSPECTOR_TEXTUREINFO_ALIGN_WORLD >= 0 &&
		nID - IDC_FACEINSPECTOR_TEXTUREINFO_ALIGN_WORLD <= 1);


	// *** JUSTIFY TEXTURE INFO ON SELECTED ***
	if (FACE::SELECTED.pFace)
	{
		// align texture
		FACE::SELECTED.pFace->AlignTexture(
			FACE::SELECTED.pFaceOwner->vlist,
			nID - IDC_FACEINSPECTOR_TEXTUREINFO_ALIGN_WORLD
			);

		// redraw all edit wnd
		CEditWnd::RedrawWindow(WU_ALLEDIT);
	}
	else if (!BRUSH::SELECTEDBRUSHLIST.empty())
	{
		// for all selected brushes
		for (BRUSH::ptrbrushlist::iterator pb = BRUSH::SELECTEDBRUSHLIST.begin();
			pb != BRUSH::SELECTEDBRUSHLIST.end();
			++pb)
			// for all faces on brush
			for (BRUSH::facelist::iterator f = (*pb)->flist.begin();
				f != (*pb)->flist.end();
				++f)
			{
				// align texture
				f->AlignTexture(
					(*pb)->vlist, nID - IDC_FACEINSPECTOR_TEXTUREINFO_ALIGN_WORLD);
			}

		// redraw all edit wnd
		CEditWnd::RedrawWindow(WU_ALLEDIT);
	}
}