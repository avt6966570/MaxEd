// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MaxEd.h"

#include "MainFrm.h"
#include "ArbitrarySideDialog.h"
#include "GoToBrushDialog.h"
#include "MapInformationDialog.h"
#include "EntityReportDialog.h"
#include "MapProblemsDialog.h"
#include "TransformationDialog.h"
#include "FindReplaceTextureDialog.h"
#include "OptionsSheet.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND_RANGE(ID_BRUSH_BLOCK, ID_BRUSH_DSPIKE, OnChangeShapeType)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND_RANGE(ID_TEXTURESMENU_DIRNAME_STARTCMDID,
					 ID_TEXTURESMENU_DIRNAME_STARTCMDID + 100,
					 OnLoadTexturesFromDir)
	ON_COMMAND(ID_TEXTURES_SHOWALL, OnTexturesShowAll)
	ON_COMMAND(ID_TEXTURES_SHOWINUSE, OnTexturesShowInUse)
	ON_COMMAND(ID_TEXTURES_FACEINSPECTOR, OnTexturesFaceinspector)
	ON_COMMAND(ID_TEXTURES_FIND, OnTexturesFindReplace)
	ON_COMMAND(ID_TEXTURES_TEXTURELOCK, OnTexturesTexturelock)
	ON_COMMAND(ID_EDIT_DISABLEUNDOREDO, OnEditDisableundoredo)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_PASTESPECIAL, OnEditPastespecial)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_COMMAND(ID_EDIT_CLEARSELECTION, OnEditClearSelection)
	ON_COMMAND(ID_EDIT_SELECTALL, OnEditSelectAll)
	ON_COMMAND(ID_EDIT_ENTITYINSPECTOR, OnEditEntityinspector)
	ON_COMMAND_RANGE(ID_ORTHO_FRONT, ID_TYPE_TEXTURE, OnViewChangeViewMode2)
	ON_COMMAND_RANGE(ID_ZOOM_100, ID_ZOOM_CUBICCLIPZOOMOUT, OnViewZoom)
	ON_COMMAND_RANGE(ID_ENABLE_ALL, ID_ENABLE_ALLPERSPECTIVE, OnViewEnableView)
	ON_COMMAND_RANGE(ID_DISABLE_ALLORTHO, ID_DISABLE_CURRENT, OnViewDisableView)
	ON_COMMAND_RANGE(ID_GRID_ALL, ID_SHOW_WORLDAXISICON, OnViewShow)
	ON_COMMAND(ID_VIEW_NEXTVIEW, OnViewNextview)
	ON_COMMAND(ID_HIDE_HIDESELECTED, OnViewHideSelected)
	ON_COMMAND(ID_HIDE_HIDEUNSELECTED, OnViewHideUnselected)
	ON_COMMAND(ID_HIDE_SHOWHIDDEN, OnViewShowHidden)
	ON_COMMAND_RANGE(ID_BRUSHAS_BOX, ID_BRUSHAS_TEXTURED, OnViewBrushAs)
	ON_COMMAND(ID_VIEW_CUBICCLIPPING, OnViewCubicClipping)
	ON_COMMAND(ID_VIEW_OPENGLLIGHTING, OnViewOpenGLLighting)
	ON_COMMAND_RANGE(ID_GRIDSETTINGS_SMALLER, ID_GRIDSETTINGS_BIGGER, OnMapChangeGridSettings)
	ON_COMMAND(ID_MAP_SNAPTOGRID, OnMapSnaptogrid)
	ON_COMMAND(ID_MAP_SHOWGRID, OnMapShowgrid)
	ON_COMMAND(ID_MAP_GOTOBRUSHNUMBER, OnMapGotobrushnumber)
	ON_COMMAND(ID_MAP_SHOWINFORMATION, OnMapShowinformation)
	ON_COMMAND(ID_MAP_ENTITYREPORT, OnMapEntityreport)
	ON_COMMAND(ID_MAP_CHECKFORPROBLEMS, OnMapCheckforproblems)
	ON_COMMAND(ID_MAP_MAPPROPERTIES, OnMapMapproperties)
	ON_COMMAND(ID_TOOLS_CARVE, OnToolsCarve)
	ON_COMMAND(ID_TOOLS_HOLLOW, OnToolsHollow)
	ON_COMMAND(ID_TOOLS_UNION, OnToolsUnion)
	ON_COMMAND(ID_MORPHING_VERTICES, OnMorphingVertices)
	ON_COMMAND(ID_MORPHING_EDGES, OnMorphingEdges)
	ON_COMMAND(ID_TOOLS_CLONE, OnToolsClone)
	ON_COMMAND(ID_TOOLS_CLIP, OnToolsClip)
	ON_COMMAND(ID_TOOLS_GROUP, OnToolsGroup)
	ON_COMMAND(ID_TOOLS_UNGROUP, OnToolsUngroup)
	ON_COMMAND(ID_TOOLS_TIETOENTITY, OnToolsTietoentity)
	ON_COMMAND(ID_TOOLS_MOVETOWORLD, OnToolsMovetoworld)
	ON_COMMAND(ID_TOOLS_TRANSFORM, OnToolsTransform)
	ON_COMMAND_RANGE(ID_ALIGN_BYFRONT, ID_ALIGN_BYTOP, OnToolsAlign)
	ON_COMMAND_RANGE(ID_FLIP_BYX, ID_FLIP_BYZ, OnToolsFlip)
	ON_COMMAND_RANGE(ID_TEXTUREWINDOWSCALE_100, ID_TEXTUREWINDOWSCALE_25,
					 OnTexturesChangeWindowScale)
	ON_COMMAND(ID_TOOLS_OPTIONS, OnToolsOptions)

	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DISABLEUNDOREDO, OnUpdateEditDisableundoredo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTESPECIAL, OnUpdateEditPastespecial)
	ON_UPDATE_COMMAND_UI(ID_GRID_ALL, OnUpdateGridAll)
	ON_UPDATE_COMMAND_UI(ID_GRID_MAJOR, OnUpdateGridMajor)
	ON_UPDATE_COMMAND_UI(ID_GRID_MINOR, OnUpdateGridMinor)
	ON_UPDATE_COMMAND_UI(ID_GRID_AXIS, OnUpdateGridAxis)
	ON_UPDATE_COMMAND_UI(ID_SHOW_WINDOWLABEL, OnUpdateShowWindowlabel)
	ON_UPDATE_COMMAND_UI(ID_SHOW_ACTIVERECT, OnUpdateShowActiverect)
	ON_UPDATE_COMMAND_UI(ID_SHOW_WORLDAXIS, OnUpdateShowWorldaxis)
	ON_UPDATE_COMMAND_UI(ID_SHOW_WORLDAXISICON, OnUpdateShowWorldaxisicon)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CUBICCLIPPING, OnUpdateViewCubicclipping)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OPENGLLIGHTING, OnUpdateViewOpengllighting)
	ON_UPDATE_COMMAND_UI(ID_MAP_SNAPTOGRID, OnUpdateMapSnaptogrid)
	ON_UPDATE_COMMAND_UI(ID_MAP_SHOWGRID, OnUpdateMapShowgrid)
	ON_UPDATE_COMMAND_UI(ID_TEXTURES_TEXTURELOCK, OnUpdateTexturesTexturelock)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TEXTUREWINDOWSCALE_100,
								ID_TEXTUREWINDOWSCALE_25,
								OnUpdateTexturesTextureWindowScale)
	ON_UPDATE_COMMAND_UI_RANGE(ID_BRUSH_BLOCK,
								ID_BRUSH_DSPIKE,
								OnUpdateChangeShapeType)
	ON_UPDATE_COMMAND_UI(ID_MORPHING_VERTICES, OnUpdateMorphingVertices)
	ON_UPDATE_COMMAND_UI(ID_MORPHING_EDGES, OnUpdateMorphingEdges)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,							// status line indicator
		ID_INDICATOR_SELECTION_INFORMATION,	// show selection info
		ID_INDICATOR_EDITVIEW_WORLDCOORD,	// show mouse world coord
		ID_INDICATOR_SELECTION_SIZE,		// show selection size
		ID_INDICATOR_ZOOM_SIZE,				// show zoom factor
		ID_INDICATOR_GRID_INFORMATION,		// show grid info
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


const CMainFrame::SplitterInfo CMainFrame::SplitterLayoutInfo[WL_TOTAL][SPLITTERS_TOTAL] = {
	// *** PROPORTIONAL ***
	// WL_H2L2
	{ { SPLITTER0, FRAMEWND, 2, 2, -1, -1 }, { NOT_USE }, { NOT_USE } },

	// *** NON-PROPORTIONAL ***
	// WL_H1L2 - WL_H2L1
	{ { SPLITTER0, FRAMEWND, 2, 1, -1, -1 },
		{ SPLITTER1, SPLITTER0, 1, 2, 1, 0 },
			{ NOT_USE } },
	{ { SPLITTER0, FRAMEWND, 2, 1, -1, -1 },
		{ SPLITTER1, SPLITTER0, 1, 2, 0, 0 },
			{ NOT_USE } },

	// WL_H1L3 - WL_H3L1
	{ { SPLITTER0, FRAMEWND, 2, 1, -1, -1 },
		{ SPLITTER1, SPLITTER0, 1, 3, 1, 0 },
			{ NOT_USE } },
	{ { SPLITTER0, FRAMEWND, 2, 1, -1, -1 },
		{ SPLITTER1, SPLITTER0, 1, 3, 0, 0 },
			{ NOT_USE } },

	// WL_H2L3 - WL_H3L2
	{ { SPLITTER0, FRAMEWND, 2, 1, -1, -1 },
		{ SPLITTER1, SPLITTER0, 1, 2, 0, 0 },
			{ SPLITTER2, SPLITTER0, 1, 3, 1, 0 } },
	{ { SPLITTER0, FRAMEWND, 2, 1, -1, -1 },
		{ SPLITTER1, SPLITTER0, 1, 2, 1, 0 },
			{ SPLITTER2, SPLITTER0, 1, 3, 0, 0 } },

	// WL_L1R2, WL_L2R1,
	{ { SPLITTER0, FRAMEWND, 1, 2, -1, -1 },
		{ SPLITTER1, SPLITTER0, 2, 1, 0, 1 },
			{ NOT_USE } },
	{ { SPLITTER0, FRAMEWND, 1, 2, -1, -1 },
		{ SPLITTER1, SPLITTER0, 2, 1, 0, 0 },
			{ NOT_USE } },

	// WL_L1R3, WL_L3R1,
	{ { SPLITTER0, FRAMEWND, 1, 2, -1, -1 },
		{ SPLITTER1, SPLITTER0, 3, 1, 0, 1 },
			{ NOT_USE } },
	{ { SPLITTER0, FRAMEWND, 1, 2, -1, -1 },
		{ SPLITTER1, SPLITTER0, 3, 1, 0, 0 },
			{ NOT_USE } },

	// WL_L2R3, WL_L3R2
	{ { SPLITTER0, FRAMEWND, 1, 2, -1, -1 },
		{ SPLITTER1, SPLITTER0, 2, 1, 0, 0 },
			{ SPLITTER2, SPLITTER0, 3, 1, 0, 1 } },
	{ { SPLITTER0, FRAMEWND, 1, 2, -1, -1 },
		{ SPLITTER1, SPLITTER0, 2, 1, 0, 1 },
			{ SPLITTER2, SPLITTER0, 3, 1, 0, 0 } }
};



// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here

	// Set m_bAutoMenuEnable to FALSE so no ON_UPDATE_COMMAND_UI or
	// ON_COMMAND handlers are needed, and
	// CMenu::EnableMenuItem() will work as expected.
//	m_bAutoMenuEnable = FALSE;
}

CMainFrame::~CMainFrame()
{
	// delete pEditorWnd pointers
	for(unsigned n = 0; n < WL_MAXWINDOW; ++n)
		delete CEditWnd::pEditWindow[n];
}

int CMainFrame::BuildRowColETable(int splitname, int extable[SPLITTERS_TOTAL-1][2])
{
	int deep = 0;
	for(int i = 0; i < SPLITTERS_TOTAL; ++i)
		if (SplitterLayoutInfo[PREFERENCES::Current.eWindowLayout][i].Name != NOT_USE &&
			SplitterLayoutInfo[PREFERENCES::Current.eWindowLayout][i].Parent == splitname)
		{
			extable[deep][0] =
				SplitterLayoutInfo[PREFERENCES::Current.eWindowLayout][i].ParentsRow;
			extable[deep][1] =
				SplitterLayoutInfo[PREFERENCES::Current.eWindowLayout][i].ParentsCol;
			++deep;
		}

	return deep;
}

BOOL CMainFrame::CheckExcludedRowCol(int extable[SPLITTERS_TOTAL-1][2],
									 int deep, int row, int col)
{
	for(int i = 0; i < deep; ++i)
		if(row == extable[i][0] && col == extable[i][1])
			return FALSE;
	return TRUE;
}

static bool g_bNeedAlignWindowLayout = false;
BOOL CMainFrame::InitWindowLayout(WINDOWLAYOUT newLayout, CCreateContext* pContext)
{
	/* DELETE OLD SPLITTERS & RESOURCES */

	// delete pEditorWnd pointers
	for(unsigned n = 0; n < WL_MAXWINDOW; ++n)
		if(CEditWnd::pEditWindow[n])
		{
			CEditWnd::pEditWindow[n]->DestroyWindow();
			delete CEditWnd::pEditWindow[n],
				CEditWnd::pEditWindow[n] = 0;
		}

	// reinit splitters from old configuration
	static bool bFirstStart = true;
	if(!bFirstStart)
		for(unsigned n = 0; n < SPLITTERS_TOTAL; ++n)
			if(SplitterLayoutInfo[PREFERENCES::Current.eWindowLayout][n].Name != NOT_USE)
				m_wndSplitter[n].ReInit();
	bFirstStart = false;

	/* CREATE NEW SPLITTER CONFIGURATION */

	// apply new splitter configuration
	PREFERENCES::Current.eWindowLayout = newLayout;
	PREFERENCES::Current.nNumEditWindow = WINDOWNUM[newLayout];

	// create new splitter layout
	for(unsigned n = 0, wnum = 0; n < SPLITTERS_TOTAL; ++n)
	{
		const SplitterInfo& SLI = SplitterLayoutInfo[newLayout][n];
		if (SLI.Name != NOT_USE)
		{
			// create splitter
			if (SLI.Name == SPLITTER0 ?
				!m_wndSplitter[SLI.Name].CreateStatic(
					this, SLI.Row, SLI.Col) :
				!m_wndSplitter[SLI.Name].CreateStatic(
					&m_wndSplitter[SLI.Parent], SLI.Row, SLI.Col,
					WS_CHILD | WS_VISIBLE | WS_BORDER,
					m_wndSplitter[SLI.Parent].IdFromRowCol(
						SLI.ParentsRow, SLI.ParentsCol)))
			{
				TRACE1("Cannot create SPLITTER%d\n", SLI.Name);
				return FALSE;
			}

			// build rows-cols excluded table
			int extable[SPLITTERS_TOTAL - 1][2],
				deep = BuildRowColETable(SLI.Name, extable);

			// create splitter panes
			for(int row = 0; row < SLI.Row; ++row)
				for(int col = 0; col < SLI.Col; ++col)
					if(CheckExcludedRowCol(extable, deep, row, col))
					{
						if(!m_wndSplitter[SLI.Name].CreateView(row, col,
							RUNTIME_CLASS(CEditWnd), CSize(10, 10), pContext))
						{
							TRACE3("Cannot create SPLITTER%d, PANE%d-%d\n", SLI.Name, row, col);
							return FALSE;
						}

						// init pEditorWnd pointers
						ASSERT(wnum < WL_MAXWINDOW);
						CEditWnd::pEditWindow[wnum] =
							(CEditWnd*)m_wndSplitter[SLI.Name].GetPane(row, col);
						ASSERT(CEditWnd::pEditWindow[wnum]);
						CEditWnd::pEditWindow[wnum]->
							SetWindowInfo(WINDOWINFO::STD[WINDOWINFO::STDWISET[newLayout][wnum]]);

						// prepare OpenGL font (make fontlist or share with pEditWindow[0])
						if(wnum) {
							BOOL result = CEditWnd::pEditWindow[wnum]->
								ShareFontLists(CEditWnd::pEditWindow[0]->GetRenderingContext());
							ASSERT(result);
						} else {
							// 1.make window label font (SYSTEM_FONT)
							BOOL result = CEditWnd::pEditWindow[0]->
								PrepareOpenGLFont(0, 127, VIEWLABEL::VL_BASELISTINDEX, SYSTEM_FONT);
							ASSERT(result);
							// 2.make texture label font
							CFont font;
							VERIFY(font.CreateFont(
								10,						// nHeight
								6,                      // nWidth
								0,                      // nEscapement
								0,                      // nOrientation
								FW_DONTCARE,            // nWeight
								FALSE,                  // bItalic
								FALSE,                  // bUnderline
								0,                      // cStrikeOut
								ANSI_CHARSET,           // nCharSet
								OUT_DEFAULT_PRECIS,     // nOutPrecision
								CLIP_DEFAULT_PRECIS,    // nClipPrecision
								DEFAULT_QUALITY,        // nQuality
								DEFAULT_PITCH,			// nPitchAndFamily
								0));					// lpszFacename

							result = CEditWnd::pEditWindow[0]->
								PrepareOpenGLFont(0, 127, VIEWLABEL::VL_TEXNAMELISTINDEX, 0, font);
							ASSERT(result);
							font.DeleteObject();
						}
						++wnum;
					}
		} // SLI.Name != NOT_USE
	}

	// set active window
	CEditWnd::pActiveWindow = CEditWnd::pEditWindow[0];

	// align new splitters
	RECT framerect;
	GetClientRect(&framerect);
	AlignWindowLayout(framerect.right, framerect.bottom);

	// recalc main frane
	CMainFrame::RecalcLayout(FALSE);

	// further aligh need
	g_bNeedAlignWindowLayout = true;

	return TRUE;
}

void CMainFrame::AlignWindowLayout(int cx, int cy)
{
	for(unsigned nums = 0; nums < SPLITTERS_TOTAL; ++nums)
	{
		const SplitterInfo& SLI =
			SplitterLayoutInfo[PREFERENCES::Current.eWindowLayout][nums];
		if(SLI.Name != NOT_USE)
		{
			for(int row = 0; row < SLI.Row; ++row)
				m_wndSplitter[SLI.Name].SetRowInfo(row, cy/SLI.Row, 10);
			for(int col = 0; col < SLI.Col; ++col)
				m_wndSplitter[SLI.Name].SetColumnInfo(col, cx/SLI.Col, 10);
			m_wndSplitter[SLI.Name].RecalcLayout();
		}
	}
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	if(g_bNeedAlignWindowLayout)
		AlignWindowLayout(cx, cy);
	g_bNeedAlignWindowLayout = false;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	cs.style =	WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE | 
				WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX |
				WS_MAXIMIZE | WS_SYSMENU;

	return CFrameWnd::PreCreateWindow(cs);
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

static void AlphabetizedAdd(CStringArray& sa, const CString& str)
{	// alphabetized push in CStringArray
	INT_PTR i;
	for (i = 0; i < sa.GetSize(); ++i)
		if(sa[i] > str)
			break;
	sa.InsertAt(i, str);
}

void CMainFrame::FillTextureDirInMenu() const
{
	CMenu* pMenu;

		// 1. get "Textures" main submenu
	if ((pMenu = GetMenu()) && (pMenu = pMenu->GetSubMenu(4)))
	{
		CStringArray textureDirNames;
		textureDirNames.SetSize(0, 100);

		// 2. scan directory names
		CFileFind finder;
		BOOL bWorking = finder.FindFile(_T("textures//*.*"));
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			if (finder.IsDirectory() && !finder.IsDots())
				AlphabetizedAdd(textureDirNames, finder.GetFileName());	// push & sort alphabetized
		}
		finder.Close();

		// 3. insert alphabetized-sorted texture directory names
		for (INT_PTR i = 0, cmdid = ID_TEXTURESMENU_DIRNAME_STARTCMDID;
			i < textureDirNames.GetSize();
			++i, ++cmdid)
				pMenu->AppendMenu(MF_STRING, cmdid, textureDirNames[i]);
	}
}

void CMainFrame::OnLoadTexturesFromDir(UINT nID)
{
	CMenu* pMenu;
	CString dirname;

	if ((pMenu = GetMenu()) && (pMenu = pMenu->GetSubMenu(4)))
	{
		for (int pos = 3, num = pMenu->GetMenuItemCount(); pos < num; ++pos)
		{
			if ( nID == pMenu->GetMenuItemID(pos) &&
				 pMenu->GetMenuString(nID, dirname, MF_BYCOMMAND) )
			{	// fill texture cash
				if (TEXTUREDIR* TD = TEXTUREDIR::FindDirInTDCache(dirname))
				{	// if texture dirname already in tex cache
					if (TEXTUREDIR::CURRENTCACHEDIR == TD)
					{
						if (TD->Update(EDITORENV_TEXTURES_PATH, dirname))
						{
							CEditWnd::ResetTextureScrollPos();
							CEditWnd::RedrawWindow(WU_ALL);
						}
					}
					else
					{
						TEXTUREDIR::CURRENTCACHEDIR = TD;
						TD->Update(EDITORENV_TEXTURES_PATH, dirname);
						CEditWnd::ResetTextureScrollPos();
						CEditWnd::RedrawWindow(WU_ALL);
					}
				}
				else
				{	// else load new texture cache line
					TEXTUREDIR::CACHE.push_back(TEXTUREDIR());
					if (TEXTUREDIR::CACHE.back().Init(EDITORENV_TEXTURES_PATH, dirname))
					{
						TEXTUREDIR::CURRENTCACHEDIR = &TEXTUREDIR::CACHE.back();
						CEditWnd::ResetTextureScrollPos();
						CEditWnd::RedrawWindow(WU_TEXTURE);
					}
					else
						TEXTUREDIR::CACHE.pop_back();
				}

				::SetCurrentDirectory(EDITORENV_ROOT_PATH);
				break;
			}
		}
	}
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpCreateStruct, CCreateContext* pContext)
{
	// FILL PATH DIR IN "TEXTURES" MAIN MENU
	FillTextureDirInMenu();

	// CREATE "ENTITY INSPECTOR" DIALOG
	CEditWnd::ENTITYINSPECTORDIALOG.Create(this);

	// CREATE "ENTITY INSPECTOR" DIALOG
	CEditWnd::FACEINSPECTORDIALOG.Create(this);

	// INIT WINDOW LAYOUT
	return InitWindowLayout(PREFERENCES::Current.eWindowLayout, pContext);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// create main frame
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// create toolbar
	if (!m_wndToolBar.Create/*Ex*/(this, /*TBSTYLE_FLAT,*/ WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	// create status bar
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// init status bar panes
	m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_SELECTION_INFORMATION, SBPS_NORMAL, 125);
	m_wndStatusBar.SetPaneInfo(2, ID_INDICATOR_EDITVIEW_WORLDCOORD, SBPS_NORMAL, 75);
	m_wndStatusBar.SetPaneInfo(3, ID_INDICATOR_SELECTION_SIZE, SBPS_NORMAL, 125);
	m_wndStatusBar.SetPaneInfo(4, ID_INDICATOR_ZOOM_SIZE, SBPS_NORMAL, 50);
	m_wndStatusBar.SetPaneInfo(5, ID_INDICATOR_GRID_INFORMATION, SBPS_NORMAL, 100);

	// + set grid info
	CString str;
	str.Format("Snap: %s, Grid: %d",
		(PREFERENCES::Current.bSnapToGrid ? "On" : "Off"),
		PREFERENCES::Current.sGrid.Major.nStep);
	m_wndStatusBar.SetPaneText(5, str);

	// +++ INIT DEFAULT TEXTURE: +++
	TEXTUREDEF::InitDefaultTexture();

	return 0;
}

void CMainFrame::OnChangeShapeType(UINT nID)
{
	ASSERT(nID >= ID_BRUSH_BLOCK && nID <= ID_BRUSH_DSPIKE);
	ASSERT(nID - ID_BRUSH_BLOCK >= 0 && nID - ID_BRUSH_BLOCK < 9);

	// change current shape
	CEditWnd::CURRENTSHAPE = BRUSH::eShapeType(nID - ID_BRUSH_BLOCK);

	//// check menu
	//CMenu* pMenu = ::AfxGetMainWnd()->GetMenu();	// get main menu
	//ASSERT(pMenu);
	//pMenu = pMenu->GetSubMenu(5);					// get "brush" menu
	//ASSERT(pMenu);
	//pMenu->CheckMenuRadioItem(0, 8, CEditWnd::CURRENTSHAPE, MF_BYPOSITION);

	if(CEditWnd::CURRENTSHAPE >= BRUSH::eShapeType::TOTAL_NONCHANGEABLE)
	{	// show dialog set side
		CArbitrarySideDialog dlgArbitrarySide(&CEditWnd::CURRENTSHAPENSIDE);
		dlgArbitrarySide.DoModal();
	}
}

void CMainFrame::OnEditUndo()
{
	// TODO: Add your command handler code here
	if (const UNDOINFO* pUndoInfo = CEditWnd::GLOBALUNDOSTACK.undoinfo())
	{
		switch(pUndoInfo->optype)
		{
		case UNDOINFO::OT_NEWOBJECT:
		case UNDOINFO::OT_NEWENTITY:
		case UNDOINFO::OT_PASTE:
		case UNDOINFO::OT_PASTESPECIAL:
		case UNDOINFO::OT_CLONE:
			// delete brushes by ID
			for(UNDOINFO::brushlist::const_iterator b = pUndoInfo->eframe.begin();
				b != pUndoInfo->eframe.end();
				++b)
					BRUSH::DeleteBrushByID(b->ID);
			break;
		case UNDOINFO::OT_DELETE:
			// insert brushes
			BRUSH::WORLDBRUSHLIST.insert(
				BRUSH::WORLDBRUSHLIST.end(),
				pUndoInfo->sframe.begin(), pUndoInfo->sframe.end()
				);
			break;
		case UNDOINFO::OT_CLIPPING:
		case UNDOINFO::OT_UNION:
		case UNDOINFO::OT_CARVE:
		case UNDOINFO::OT_HOLLOW:
			// 1. delete new brushes
			for (UNDOINFO::brushlist::const_iterator b = pUndoInfo->eframe.begin();
				b != pUndoInfo->eframe.end();
				++b)
					BRUSH::DeleteBrushByID(b->ID);
			// 2. insert old brushes
			BRUSH::WORLDBRUSHLIST.insert(
				BRUSH::WORLDBRUSHLIST.end(),
				pUndoInfo->sframe.begin(), pUndoInfo->sframe.end()
				);
			break;
		default:
			// replace brushes
			for (UNDOINFO::brushlist::const_iterator b = pUndoInfo->sframe.begin();
				b != pUndoInfo->sframe.end();
				++b)
			{
				for (BRUSH::brushlist::iterator wb = BRUSH::WORLDBRUSHLIST.begin();
					wb != BRUSH::WORLDBRUSHLIST.end();
					++wb)
						if (wb->ID == b->ID)
						{
							*wb = *b;
							break;
						}
			}
			break;
		}

		BRUSH::ReInitSelectedBrushList();
		CEditWnd::GLOBALUNDOSTACK.undo();
		::UpdateUndoRedoMenu(false);	
	}
	else
		::UpdateUndoRedoMenu(true);			// true: clear sel bruh list & gray undo menu

	CEditWnd::RedrawWindow(WU_ALLEDIT);		// update all edit-views
}

void CMainFrame::OnEditRedo()
{
	// TODO: Add your command handler code here
	if(const UNDOINFO *pRedoInfo = CEditWnd::GLOBALUNDOSTACK.redo())
	{
		switch(pRedoInfo->optype)
		{
		case UNDOINFO::OT_NEWOBJECT:
		case UNDOINFO::OT_NEWENTITY:
		case UNDOINFO::OT_PASTE:
		case UNDOINFO::OT_PASTESPECIAL:
		case UNDOINFO::OT_CLONE:
			BRUSH::WORLDBRUSHLIST.insert(
				BRUSH::WORLDBRUSHLIST.end(), pRedoInfo->eframe.begin(), pRedoInfo->eframe.end()
				);
			break;
		case UNDOINFO::OT_DELETE:
			for(UNDOINFO::brushlist::const_iterator b = pRedoInfo->sframe.begin();
				b != pRedoInfo->sframe.end();
				++b)
					BRUSH::DeleteBrushByID(b->ID);
			break;
		case UNDOINFO::OT_CLIPPING:
		case UNDOINFO::OT_UNION:
		case UNDOINFO::OT_CARVE:
		case UNDOINFO::OT_HOLLOW:
			// 1. delete old brushes
			for(UNDOINFO::brushlist::const_iterator b = pRedoInfo->sframe.begin();
				b != pRedoInfo->sframe.end();
				++b)
					BRUSH::DeleteBrushByID(b->ID);
			// 2. insert new brushes
			BRUSH::WORLDBRUSHLIST.insert(
				BRUSH::WORLDBRUSHLIST.end(),
				pRedoInfo->eframe.begin(), pRedoInfo->eframe.end()
				);
			break;
		default:
			// replace brushes
			for(UNDOINFO::brushlist::const_iterator b = pRedoInfo->eframe.begin();
				b != pRedoInfo->eframe.end();
				++b)
			{
				for(BRUSH::brushlist::iterator wb = BRUSH::WORLDBRUSHLIST.begin();
					wb != BRUSH::WORLDBRUSHLIST.end();
					++wb)
						if(wb->ID == b->ID)
						{
							*wb = *b;
							break;
						}
			}
			break;
		}

		BRUSH::ReInitSelectedBrushList();
		::UpdateUndoRedoMenu(false);	
	}

	CEditWnd::RedrawWindow(WU_ALLEDIT);		// update all edit-views
}

void CMainFrame::OnTexturesShowAll()
{
	CMenu* pMenu;

	// reset current cache line pointer
	TEXTUREDIR::CURRENTCACHEDIR = 0;
	// reset "ShowInUseTextures" property
	CEditWnd::bTexturesShowInUseOnly = false;
	// update main menu flags
	if ((pMenu = GetMenu()) && (pMenu = pMenu->GetSubMenu(4)))
		pMenu->CheckMenuItem(ID_TEXTURES_SHOWINUSE, MF_BYCOMMAND|MF_UNCHECKED);
	// redraw texture view
	CEditWnd::RedrawWindow(WU_TEXTURE);
}

void CMainFrame::OnTexturesShowInUse()
{
	CMenu* pMenu;

	// negate "ShowInUseTextures" property
	if(CEditWnd::bTexturesShowInUseOnly = !CEditWnd::bTexturesShowInUseOnly)
		// full rescan "inuse" textures
		BRUSH::SetTextureInUse()
		;
	// update main menu flags
	if ((pMenu = GetMenu()) && (pMenu = pMenu->GetSubMenu(4)))
		pMenu->CheckMenuItem(ID_TEXTURES_SHOWINUSE,
			MF_BYCOMMAND | ( CEditWnd::bTexturesShowInUseOnly ? MF_CHECKED : MF_UNCHECKED)
			);
	// redraw texture view
	CEditWnd::RedrawWindow(WU_TEXTURE);
}

void CMainFrame::OnEditDisableundoredo()
{
//	CMenu *pMenu = GetMenu();
//	ASSERT(pMenu);
//	pMenu = pMenu->GetSubMenu(1);
//	ASSERT(pMenu);

	// TODO: Add your command handler code here
	if (CEditWnd::bDisableUndoRedo = !CEditWnd::bDisableUndoRedo)
	{
		// clear undo/redo menu label
//		pMenu->ModifyMenu(
//			0, MF_BYPOSITION|MF_GRAYED|MF_STRING, ID_EDIT_UNDO, UNDOINFO::UNDOLABELS[0]
//			);
//		pMenu->ModifyMenu(
//			1, MF_BYPOSITION|MF_GRAYED|MF_STRING, ID_EDIT_UNDO, UNDOINFO::REDOLABELS[0]
//			);
//		pMenu->ModifyMenu(
//			2, MF_BYPOSITION|MF_STRING, ID_EDIT_DISABLEUNDOREDO, "Enable Undo/Redo"
//			);

		// clear undo/redo stack
		CEditWnd::GLOBALUNDOSTACK.clear();
	}
//	else
//		pMenu->ModifyMenu(
//			2, MF_BYPOSITION|MF_STRING, ID_EDIT_DISABLEUNDOREDO, "Disable Undo/Redo"
//			);
}

void CMainFrame::OnEditCut()
{
	// TODO: Add your command handler code here
	if (!BRUSH::SELECTEDBRUSHLIST.empty())
	{
		OnEditCopy();
		BRUSH::DeleteSelectedBrushList();
		CEditWnd::RedrawWindow(WU_ALLEDIT);
	}
}

void CMainFrame::OnEditCopy()
{
	// TODO: Add your command handler code here
	if (!BRUSH::SELECTEDBRUSHLIST.empty())
	{
		// clear clipboard
		CEditWnd::CLIPBOARD.clear();
		// reserve clipboard size
		CEditWnd::CLIPBOARD.reserve(BRUSH::SELECTEDBRUSHLIST.size());
		// copy selected brushes to clipboard
		for (BRUSH::ptrbrushlist::const_iterator pB = BRUSH::SELECTEDBRUSHLIST.begin();
			pB != BRUSH::SELECTEDBRUSHLIST.end();
			++pB)
				CEditWnd::CLIPBOARD.push_back(**pB);
//		// enable "Paste" menu label
//		CMenu *pMenu = GetMenu();
//		ASSERT(pMenu);
//		pMenu = pMenu->GetSubMenu(1);
//		ASSERT(pMenu);
//		pMenu->EnableMenuItem(ID_EDIT_PASTE, MF_BYCOMMAND|MF_ENABLED);
//		pMenu->EnableMenuItem(ID_EDIT_PASTESPECIAL, MF_BYCOMMAND|MF_ENABLED);
	}
}

void CMainFrame::OnEditPaste()
{
	// TODO: Add your command handler code here
	if (!CEditWnd::CLIPBOARD.empty())
	{
		// update ID`s for clipboard`s brushes
		for (BRUSH::brushlist::iterator b = CEditWnd::CLIPBOARD.begin();
			b != CEditWnd::CLIPBOARD.end();
			++b)
				b->UpdateID();
		// clear all
		BRUSH::ClearSelectedBrushList();
		// insert brushes from clipboard to world
		BRUSH::WORLDBRUSHLIST.insert(
			BRUSH::WORLDBRUSHLIST.end(), CEditWnd::CLIPBOARD.begin(), CEditWnd::CLIPBOARD.end()
			);
		// record undo info
		if(!CEditWnd::bDisableUndoRedo)
		{
			UNDOINFO undo(UNDOINFO::OT_PASTE);
			// save end frame only!
			undo.EndFrame(CEditWnd::CLIPBOARD);
			// push in undo stack
			CEditWnd::GLOBALUNDOSTACK.push(undo);
			// update undo/redo labels in main menu
			::UpdateUndoRedoMenu();
		}
		// reinit selection
		BRUSH::ReInitSelectedBrushList();
		// redraw all edit window!
		CEditWnd::RedrawWindow(WU_ALLEDIT);
	}
}

void CMainFrame::OnEditPastespecial()
{
	// TODO: Add your command handler code here
	if (PASTESPECIALDIALOG.DoModal() == IDOK &&		// user press OK button
		!CEditWnd::CLIPBOARD.empty() &&				// clipboard not empty
		(PASTESPECIALDIALOG.m_fOffsetX ||
		PASTESPECIALDIALOG.m_fOffsetY ||
		PASTESPECIALDIALOG.m_fOffsetZ ||
		PASTESPECIALDIALOG.m_fRotationX ||
		PASTESPECIALDIALOG.m_fRotationY ||
		PASTESPECIALDIALOG.m_fRotationZ))			// non-zero inc offset or rotation
	{
		/* increments */
		const VECTOR3 ioffset(
			PASTESPECIALDIALOG.m_fOffsetX,
			PASTESPECIALDIALOG.m_fOffsetY,
			PASTESPECIALDIALOG.m_fOffsetZ
			);
		const VECTOR3 irotation(
			PASTESPECIALDIALOG.m_fRotationX,
			PASTESPECIALDIALOG.m_fRotationY,
			PASTESPECIALDIALOG.m_fRotationZ
			);
		/* bases */
		VECTOR3 offset(VECTOR3::O),
				rotation(VECTOR3::O);

		// undoinfo
		UNDOINFO undo(UNDOINFO::OT_PASTESPECIAL,
			0, CEditWnd::CLIPBOARD.size()*PASTESPECIALDIALOG.m_nNumCopies
			);

		/* clear all */
		BRUSH::ClearSelectedBrushList();
		/* prepare union bbox (for non-group paste) */
		BBOX3 unibox;
		for (BRUSH::brushlist::const_iterator b = CEditWnd::CLIPBOARD.begin();
			b != CEditWnd::CLIPBOARD.end();
			++b)
				unibox += b->bbox;

		/* process special paste */
		for (UINT i = 0; i < PASTESPECIALDIALOG.m_nNumCopies; ++i)
		{
			// make clipboard copy
			CEditWnd::brushlist temp(CEditWnd::CLIPBOARD);

			// set rotation/offset matrix
			MATRIX16 m, r(MATRIX16::I);
			if (!ioffset.isZero())	r.Translate(offset += ioffset);
			if (irotation.X)	r.RotationX(rotation.X += irotation.X);
			if (irotation.Y)	r.RotationY(rotation.Y += irotation.Y);
			if (irotation.Z)	r.RotationZ(rotation.Z += irotation.Z);

			if (PASTESPECIALDIALOG.m_bGroupPaste)
			{
				// prepare group transform matrix
				m.LoadTranslation(unibox.GetCenter());
				m = r * m;
				m.Translate(-unibox.GetCenter());

				// transform clipboard copy
				for (BRUSH::brushlist::iterator b = temp.begin(); b != temp.end(); ++b)
				{
					b->UpdateID();
					b->Translate(m, PREFERENCES::Current.bTextureLock);
				}
			}
			else
			{
				// transform clipboard copy
				for (BRUSH::brushlist::iterator b = temp.begin(); b != temp.end(); ++b)
				{
					// prepare individual brush transform matrix
					m.LoadTranslation(b->bbox.GetCenter());
					m = r * m;
					m.Translate(-b->bbox.GetCenter());

					b->UpdateID();
					b->Translate(m, PREFERENCES::Current.bTextureLock);
				}
			}

			// insert brushes from clipboard to world
			BRUSH::WORLDBRUSHLIST.insert(
				BRUSH::WORLDBRUSHLIST.end(), temp.begin(), temp.end()
				);

			// record undo info
			if(!CEditWnd::bDisableUndoRedo)
				// add to end frame only!
				undo.AddEndFrame(temp);
		}

		/* record undo info */
		if(!CEditWnd::bDisableUndoRedo)
		{	// push undo info in undo/redo-stack
			CEditWnd::GLOBALUNDOSTACK.push(undo);
			// update undo/redo labels in main menu
			::UpdateUndoRedoMenu();
		}

		/* reinit selection list */
		BRUSH::ReInitSelectedBrushList();
		/* final redraw */
		CEditWnd::RedrawWindow(WU_ALLEDIT);
	}
}

void CMainFrame::OnEditDelete()
{
	// TODO: Add your command handler code here
	CEditWnd::DeleteSelection();
}

void CMainFrame::OnEditClearSelection()
{
	// TODO: Add your command handler code here
	CEditWnd::ClearSelection();
}

void CMainFrame::OnEditSelectAll()
{
	// TODO: Add your command handler code here
	BRUSH::SelectAll();
	// and redraw all editable!
	CEditWnd::RedrawWindow(WU_ALLEDIT);
}

void CMainFrame::OnEditEntityinspector()
{
	// TODO: Add your command handler code here
	if(!BRUSH::SELECTEDBRUSHLIST.empty())
	{
		if(CEditWnd::ENTITYINSPECTORDIALOG.IsWindowVisible())
			CEditWnd::ENTITYINSPECTORDIALOG.ShowWindow(SW_HIDE);
		else
		{
			CEditWnd::ENTITYINSPECTORDIALOG.SetInfo();
			CEditWnd::ENTITYINSPECTORDIALOG.ShowWindow(SW_SHOW);
		}
	}
}

void CMainFrame::OnViewNextview()
{
	// TODO: Add your command handler code here
	if (CEditWnd::pActiveWindow)
		CEditWnd::pActiveWindow->NextViewMode();
}

void CMainFrame::OnViewChangeViewMode2(UINT nID)
{
	// TODO: Add your command handler code here
	ASSERT(nID - ID_ORTHO_FRONT >= VM_FRONT &&
			nID - ID_ORTHO_FRONT <= VM_TEXTURE);

	if (CEditWnd::pActiveWindow)
		CEditWnd::pActiveWindow->SetViewMode(VIEWMODE(nID - ID_ORTHO_FRONT));
}

void CMainFrame::OnViewZoom(UINT nID)
{
	if (CEditWnd::pActiveWindow)
	{
		switch (nID)
		{
		case ID_ZOOM_100:
			CEditWnd::pActiveWindow->Zoom(true, true);
			break;
		case ID_ZOOM_ZOOMIN:
			CEditWnd::pActiveWindow->Zoom(true);
			break;
		case ID_ZOOM_ZOOMOUT:
			CEditWnd::pActiveWindow->Zoom(false);
			break;
		case ID_ZOOM_CUBICCLIPZOOMIN:
			if (PREFERENCES::Current.bEnableCubicClipping)
			{
				CEditWnd::CUBICCLIPPER.ZoomIn(CEditWnd::CUBICCLIPPERDELTA);
				CEditWnd::pActiveWindow->RedrawWindow();
			}
			break;
		case ID_ZOOM_CUBICCLIPZOOMOUT:
			if (PREFERENCES::Current.bEnableCubicClipping)
			{
				CEditWnd::CUBICCLIPPER.ZoomOut(CEditWnd::CUBICCLIPPERDELTA);
				CEditWnd::pActiveWindow->RedrawWindow();
			}
			break;
		}
	}
}

void CMainFrame::OnViewEnableView(UINT nID)
{
	switch (nID)
	{
	case ID_ENABLE_ALL:
		for(unsigned w = 0; w < PREFERENCES::Current.nNumEditWindow; ++w)
			CEditWnd::pEditWindow[w]->EnableView(VT_TOTAL);
		break;
	case ID_ENABLE_ALLORTHO:
		for(unsigned w = 0; w < PREFERENCES::Current.nNumEditWindow; ++w)
			CEditWnd::pEditWindow[w]->EnableView(VT_2D);
		break;
	case ID_ENABLE_ALLPERSPECTIVE:
		for(unsigned w = 0; w < PREFERENCES::Current.nNumEditWindow; ++w)
			CEditWnd::pEditWindow[w]->EnableView(VT_3D);
		break;
	}
}

void CMainFrame::OnViewDisableView(UINT nID)
{
	switch (nID)
	{
	case ID_DISABLE_ALLORTHO:
		for(unsigned w = 0; w < PREFERENCES::Current.nNumEditWindow; ++w)
			CEditWnd::pEditWindow[w]->EnableView(VT_2D, false);
		break;
	case ID_DISABLE_ALLPERSPECTIVE:
		for(unsigned w = 0; w < PREFERENCES::Current.nNumEditWindow; ++w)
			CEditWnd::pEditWindow[w]->EnableView(VT_3D, false);
		break;
	case ID_DISABLE_CURRENT:
		if (CEditWnd::pActiveWindow)
			CEditWnd::pActiveWindow->EnableView(VT_TOTAL, false);
		break;
	}
}

void CMainFrame::OnViewShow(UINT nID)
{
	CMenu *pShowMenu = GetMenu();				// get main frame`s menu
	ASSERT(pShowMenu);
	pShowMenu = pShowMenu->GetSubMenu(2);		// get "View" submenu
	ASSERT(pShowMenu);
	pShowMenu = pShowMenu->GetSubMenu(9);		// get "View/Show" submenu
	ASSERT(pShowMenu);
	CMenu *pGridMenu = pShowMenu->GetSubMenu(0);// get "View/Show/Grid" submenu
	ASSERT(pGridMenu);

	switch (nID)
	{
	case ID_GRID_ALL:
		//if (pGridMenu->GetMenuState(ID_GRID_ALL, MF_BYCOMMAND) & MF_CHECKED)
		//{
		//	// uncheck "All" submenu
		//	pGridMenu->CheckMenuItem(ID_GRID_ALL, MF_UNCHECKED | MF_BYCOMMAND);
		//	// enable "Major" menu
		//	pGridMenu->EnableMenuItem(ID_GRID_MAJOR, MF_BYCOMMAND | MF_ENABLED);
		//	// enable "Minor" menu
		//	pGridMenu->EnableMenuItem(ID_GRID_MINOR, MF_BYCOMMAND | MF_ENABLED);
		//	// enable "Axis" menu
		//	pGridMenu->EnableMenuItem(ID_GRID_AXIS, MF_BYCOMMAND | MF_ENABLED);
		//}
		//else
		//{
		//	// check "All" submenu
		//	pGridMenu->CheckMenuItem(ID_GRID_ALL, MF_CHECKED | MF_BYCOMMAND);
		//	// "Major" menu: check + disable + change grid global preferences
		//	pGridMenu->CheckMenuItem(ID_GRID_MAJOR, MF_CHECKED | MF_BYCOMMAND);
		//	pGridMenu->EnableMenuItem(ID_GRID_MAJOR, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		//	PREFERENCES::Current.sGrid.Major.bShow = true;
		//	// "Minor" menu: check + disable + change grid global preferences
		//	pGridMenu->CheckMenuItem(ID_GRID_MINOR, MF_CHECKED | MF_BYCOMMAND);
		//	pGridMenu->EnableMenuItem(ID_GRID_MINOR, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		//	PREFERENCES::Current.sGrid.Minor.bShow = true;
		//	// "Axis" menu: check + disable + change grid global preferences
		//	pGridMenu->CheckMenuItem(ID_GRID_AXIS, MF_CHECKED | MF_BYCOMMAND);
		//	pGridMenu->EnableMenuItem(ID_GRID_AXIS, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		//	PREFERENCES::Current.sGrid.Axis.bShow = true;
		//}

		PREFERENCES::Current.sGrid.Major.bShow =
			PREFERENCES::Current.sGrid.Minor.bShow =
				PREFERENCES::Current.sGrid.Axis.bShow = 
				!(bool)(pGridMenu->GetMenuState(ID_GRID_ALL, MF_BYCOMMAND) & MF_CHECKED);
		break;
	case ID_GRID_MAJOR:
		//if (PREFERENCES::Current.sGrid.Major.bShow =
		//	!PREFERENCES::Current.sGrid.Major.bShow)
		//	pShowMenu->CheckMenuItem(ID_GRID_MAJOR, MF_CHECKED | MF_BYCOMMAND);
		//else
		//	pShowMenu->CheckMenuItem(ID_GRID_MAJOR, MF_UNCHECKED | MF_BYCOMMAND);

		PREFERENCES::Current.sGrid.Major.bShow =
			!PREFERENCES::Current.sGrid.Major.bShow;
		break;
	case ID_GRID_MINOR:
		//if (PREFERENCES::Current.sGrid.Minor.bShow =
		//	!PREFERENCES::Current.sGrid.Minor.bShow)
		//	pShowMenu->CheckMenuItem(ID_GRID_MINOR, MF_CHECKED | MF_BYCOMMAND);
		//else
		//	pShowMenu->CheckMenuItem(ID_GRID_MINOR, MF_UNCHECKED | MF_BYCOMMAND);

		PREFERENCES::Current.sGrid.Minor.bShow =
			!PREFERENCES::Current.sGrid.Minor.bShow;
		break;
	case ID_GRID_AXIS:
		//if (PREFERENCES::Current.sGrid.Axis.bShow =
		//	!PREFERENCES::Current.sGrid.Axis.bShow)
		//	pShowMenu->CheckMenuItem(ID_GRID_AXIS, MF_CHECKED | MF_BYCOMMAND);
		//else
		//	pShowMenu->CheckMenuItem(ID_GRID_AXIS, MF_UNCHECKED | MF_BYCOMMAND);

		PREFERENCES::Current.sGrid.Axis.bShow =
			!PREFERENCES::Current.sGrid.Axis.bShow;
		break;
	case ID_SHOW_WINDOWLABEL:
		//if (PREFERENCES::Current.bShowWindowLabel =
		//	!PREFERENCES::Current.bShowWindowLabel)
		//	pShowMenu->CheckMenuItem(ID_SHOW_WINDOWLABEL, MF_CHECKED | MF_BYCOMMAND);
		//else
		//	pShowMenu->CheckMenuItem(ID_SHOW_WINDOWLABEL, MF_UNCHECKED | MF_BYCOMMAND);

		PREFERENCES::Current.bShowWindowLabel =
			!PREFERENCES::Current.bShowWindowLabel;
		break;
	case ID_SHOW_ACTIVERECT:
		//if (PREFERENCES::Current.bShowActiveWindowRect =
		//	!PREFERENCES::Current.bShowActiveWindowRect)
		//	pShowMenu->CheckMenuItem(ID_SHOW_ACTIVERECT, MF_CHECKED | MF_BYCOMMAND);
		//else
		//	pShowMenu->CheckMenuItem(ID_SHOW_ACTIVERECT, MF_UNCHECKED | MF_BYCOMMAND);

		PREFERENCES::Current.bShowActiveWindowRect =
			!PREFERENCES::Current.bShowActiveWindowRect;
		break;
	case ID_SHOW_WORLDAXIS:
		//if (PREFERENCES::Current.bShowWorldAxis =
		//	!PREFERENCES::Current.bShowWorldAxis)
		//	pShowMenu->CheckMenuItem(ID_SHOW_WORLDAXIS, MF_CHECKED | MF_BYCOMMAND);
		//else
		//	pShowMenu->CheckMenuItem(ID_SHOW_WORLDAXIS, MF_UNCHECKED | MF_BYCOMMAND);

		PREFERENCES::Current.bShowWorldAxis =
			!PREFERENCES::Current.bShowWorldAxis;
		break;    
	case ID_SHOW_WORLDAXISICON:
		//if (PREFERENCES::Current.bShowCoordSpaceIcon =
		//	!PREFERENCES::Current.bShowCoordSpaceIcon)
		//	pShowMenu->CheckMenuItem(ID_SHOW_WORLDAXISICON, MF_CHECKED | MF_BYCOMMAND);
		//else
		//	pShowMenu->CheckMenuItem(ID_SHOW_WORLDAXISICON, MF_UNCHECKED | MF_BYCOMMAND);

		PREFERENCES::Current.bShowCoordSpaceIcon =
			!PREFERENCES::Current.bShowCoordSpaceIcon;
		break;
	}

	// and redraw all editable!
	CEditWnd::RedrawWindow(WU_ALLEDIT);
}

void CMainFrame::OnViewHideSelected()
{
	// TODO: Add your command handler code here
	if(!BRUSH::SELECTEDBRUSHLIST.empty())
	{
		BRUSH::HideSelectedBrushList();
		// and redraw all editable!
		CEditWnd::RedrawWindow(WU_ALLEDIT);
	}
}

void CMainFrame::OnViewHideUnselected()
{
	// TODO: Add your command handler code here
	BRUSH::HideUnselectedBrushList();
	// and redraw all editable!
	CEditWnd::RedrawWindow(WU_ALLEDIT);
}

void CMainFrame::OnViewShowHidden()
{
	// TODO: Add your command handler code here
	BRUSH::UnhideAll();
	// and redraw all editable!
	CEditWnd::RedrawWindow(WU_ALLEDIT);
}

void CMainFrame::OnViewBrushAs(UINT nID)
{
	// TODO: Add your command handler code here
	DRAWMODE dm;
	CMenu *pBrushAsMenu = GetMenu();			// get main frame`s menu
	ASSERT(pBrushAsMenu);
	pBrushAsMenu = pBrushAsMenu->GetSubMenu(2);	// get "View" submenu
	ASSERT(pBrushAsMenu);
	pBrushAsMenu = pBrushAsMenu->GetSubMenu(12);// get "View/Brush As..." submenu
	ASSERT(pBrushAsMenu);

	switch (nID)
	{
	case ID_BRUSHAS_FILLED:		dm = DM_SOLID;	break;
	case ID_BRUSHAS_TEXTURED:	dm = DM_TEXTURE;break;
	//case ID_BRUSHAS_BOX:		break;
	//case ID_BRUSHAS_WIREFRAME:break;
	default:					dm = DM_WIRE;	break;
	}

	pBrushAsMenu->CheckMenuRadioItem(ID_BRUSHAS_BOX, ID_BRUSHAS_TEXTURED, nID, MF_BYCOMMAND);
	CEditWnd::SetDrawMode(dm);
}

void CMainFrame::OnViewCubicClipping()
{
	// TODO: Add your command handler code here
	//CMenu *pCubicClipMenu = GetMenu();			// get main frame`s menu
	//ASSERT(pCubicClipMenu);
	//pCubicClipMenu = pCubicClipMenu->GetSubMenu(2);	// get "View" submenu
	//ASSERT(pCubicClipMenu);

	//if (PREFERENCES::Current.bEnableCubicClipping =
	//	!PREFERENCES::Current.bEnableCubicClipping)
	//	pCubicClipMenu->CheckMenuItem(ID_VIEW_CUBICCLIPPING, MF_CHECKED | MF_BYCOMMAND);
	//else
	//	pCubicClipMenu->CheckMenuItem(ID_VIEW_CUBICCLIPPING, MF_UNCHECKED | MF_BYCOMMAND);

	PREFERENCES::Current.bEnableCubicClipping =
		!PREFERENCES::Current.bEnableCubicClipping;
	// and redraw all editable!
	CEditWnd::RedrawWindow(WU_3D);
}

void CMainFrame::OnViewOpenGLLighting()
{
	// TODO: Add your command handler code here
	//CMenu *pOpenGLLightingMenu = GetMenu();			// get main frame`s menu
	//ASSERT(pOpenGLLightingMenu);
	//pOpenGLLightingMenu = pOpenGLLightingMenu->GetSubMenu(2);	// get "View" submenu
	//ASSERT(pOpenGLLightingMenu);

	//if (PREFERENCES::Current.bEnableOpenGLLighting =
	//	!PREFERENCES::Current.bEnableOpenGLLighting)
	//	pOpenGLLightingMenu->CheckMenuItem(ID_VIEW_OPENGLLIGHTING, MF_CHECKED | MF_BYCOMMAND);
	//else
	//	pOpenGLLightingMenu->CheckMenuItem(ID_VIEW_OPENGLLIGHTING, MF_UNCHECKED | MF_BYCOMMAND);

	PREFERENCES::Current.bEnableOpenGLLighting =
		!PREFERENCES::Current.bEnableOpenGLLighting;
	// and redraw all editable!
	CEditWnd::RedrawWindow(WU_3D);
}

void CMainFrame::OnMapSnaptogrid()
{
	// TODO: Add your command handler code here
	//CMenu *pSnapToGridMenu = GetMenu();					// get main frame`s menu
	//ASSERT(pSnapToGridMenu);
	//pSnapToGridMenu = pSnapToGridMenu->GetSubMenu(3);	// get "Map" submenu
	//ASSERT(pSnapToGridMenu);

	//if (PREFERENCES::Current.bSnapToGrid =
	//	!PREFERENCES::Current.bSnapToGrid)
	//	pSnapToGridMenu->CheckMenuItem(ID_MAP_SNAPTOGRID, MF_CHECKED | MF_BYCOMMAND);
	//else
	//	pSnapToGridMenu->CheckMenuItem(ID_MAP_SNAPTOGRID, MF_UNCHECKED | MF_BYCOMMAND);

	PREFERENCES::Current.bSnapToGrid = !PREFERENCES::Current.bSnapToGrid;

	// update status bar info
	CStatusBar *pStatusBar = (CStatusBar *)GetMessageBar();
	ASSERT(pStatusBar);
	CString str;
	str.Format("Snap: %s, Grid: %d",
		(PREFERENCES::Current.bSnapToGrid ? "On" : "Off"),
		PREFERENCES::Current.sGrid.Major.nStep);
	pStatusBar->SetPaneText(5, str);
}

void CMainFrame::OnMapShowgrid()
{
	// TODO: Add your command handler code here
	CMainFrame::OnViewShow(ID_GRID_ALL);

	CMenu *pSnapToGridMenu = GetMenu();					// get main frame`s menu
	ASSERT(pSnapToGridMenu);
	pSnapToGridMenu = pSnapToGridMenu->GetSubMenu(3);	// get "Map" submenu
	ASSERT(pSnapToGridMenu);
	if (pSnapToGridMenu->GetMenuState(ID_MAP_SHOWGRID, MF_BYCOMMAND) & MF_CHECKED)
		pSnapToGridMenu->CheckMenuItem(ID_MAP_SHOWGRID, MF_UNCHECKED | MF_BYCOMMAND);
	else
		pSnapToGridMenu->CheckMenuItem(ID_MAP_SHOWGRID, MF_CHECKED | MF_BYCOMMAND);
}

void CMainFrame::OnMapChangeGridSettings(UINT nID)
{
	if (nID == ID_GRIDSETTINGS_SMALLER)
		PREFERENCES::Current.sGrid.ChangeGridSize(false);
	else // ID_GRIDSETTINGS_BIGGER
		PREFERENCES::Current.sGrid.ChangeGridSize(true);
	
	// update status bar info
	CStatusBar *pStatusBar = (CStatusBar *)GetMessageBar();
	ASSERT(pStatusBar);
	CString str;
	str.Format("Snap: %s, Grid: %d",
		(PREFERENCES::Current.bSnapToGrid ? "On" : "Off"),
		PREFERENCES::Current.sGrid.Major.nStep);
	pStatusBar->SetPaneText(5, str);

	// redraw ortho (for grid)
	CEditWnd::RedrawWindow(WU_2D);
}


void CMainFrame::OnMapGotobrushnumber()
{
	// TODO: Add your command handler code here
	BRUSH* x;
	CGoToBrushDialog dlg;

	// if has find
	if ((dlg.DoModal() == IDOK) && (x = BRUSH::FindBrushByID(dlg.m_uBrushN)))
	{
		// select brush
		x->flags |= BRUSH::BF_SELECTED;
		BRUSH::SELECTEDBRUSHLIST.push_back(x);
		// centered views
		if (dlg.m_bSearchVisibleOnly)
			if (!(x->flags & BRUSH::BF_HIDDEN))
				CEditWnd::CenteredView(*x);
			else;
		else
			CEditWnd::CenteredView(*x);

		// redraw prespective only
		CEditWnd::RedrawWindow(WU_3D);
	}
}

void CMainFrame::OnMapShowinformation()
{
	// TODO: Add your command handler code here
	UINT nSolids, nFaces,
		 nPointEntities, nSolidEntities,
		 nUniqueTextures, nTextureMemory;
	CStringArray texpathes;

	// get world info
	BRUSH::GetWorldInfo(nSolids, nFaces, nPointEntities, nSolidEntities,
						nUniqueTextures, nTextureMemory, texpathes);

	// show dlg
	CMapInformationDialog dlg(nSolids, nFaces, nPointEntities, nSolidEntities,
							  nUniqueTextures, nTextureMemory, &texpathes);
	dlg.DoModal();
}

void CMainFrame::OnMapEntityreport()
{
	// TODO: Add your command handler code here
	CEntityReportDialog dlg;
	dlg.DoModal();
}

void CMainFrame::OnMapCheckforproblems()
{
	// TODO: Add your command handler code here
	CMapProblemsDialog dlg;
	dlg.DoModal();
}

void CMainFrame::OnMapMapproperties()
{
	// TODO: Add your command handler code here
	if (BRUSH* pWorldspawn = BRUSH::FindWorldspawn())
	{
		// clear all selection
		BRUSH::ClearSelectedBrushList();
		// select brush
		pWorldspawn->flags |= BRUSH::BF_SELECTED;
		// add to selected brush list
		BRUSH::SELECTEDBRUSHLIST.push_back(pWorldspawn);
		// and redraw all editable!
		CEditWnd::RedrawWindow(WU_ALLEDIT);

		// update "Entity Inspector" dlg items
		CEditWnd::ENTITYINSPECTORDIALOG.SetInfo();
		// show "Entity Inspector" dlg
		CEditWnd::ENTITYINSPECTORDIALOG.ShowWindow(SW_SHOW);
	}
}

void CMainFrame::OnToolsCarve()
{
	// TODO: Add your command handler code here
	CEditWnd::Carve();
}

void CMainFrame::OnToolsHollow()
{
	// TODO: Add your command handler code here
	CEditWnd::Hollow();
}

void CMainFrame::OnToolsUnion()
{
	// TODO: Add your command handler code here
	CEditWnd::Union();
}

void CMainFrame::OnMorphingVertices()
{
	// TODO: Add your command handler code here
	CEditWnd::MorphingVertices();
}

void CMainFrame::OnMorphingEdges()
{
	// TODO: Add your command handler code here
	CEditWnd::MorphingEdges();
}

void CMainFrame::OnToolsClone()
{
	// TODO: Add your command handler code here
	CEditWnd::CloneSelection();
}

void CMainFrame::OnToolsClip()
{
	// TODO: Add your command handler code here
	CEditWnd::ToggleClip();
}

void CMainFrame::OnToolsGroup()
{
	// TODO: Add your command handler code here
	// :TODO: ??? ??? ???
}

void CMainFrame::OnToolsUngroup()
{
	// TODO: Add your command handler code here
	OnToolsMovetoworld();
}

void CMainFrame::OnToolsTietoentity()
{
	// TODO: Add your command handler code here
	if (!BRUSH::SELECTEDBRUSHLIST.empty())
	{
		CEditWnd::ENTITYINSPECTORDIALOG.SetInfo();
		CEditWnd::ENTITYINSPECTORDIALOG.ShowWindow(SW_SHOW);
	}
}

void CMainFrame::OnToolsMovetoworld()
{
	// TODO: Add your command handler code here
	for (BRUSH::ptrbrushlist::iterator pB = BRUSH::SELECTEDBRUSHLIST.begin();
		pB != BRUSH::SELECTEDBRUSHLIST.end();
		++pB)
		if ((*pB)->owner)
			(*pB)->owner = 0;
}

void CMainFrame::OnToolsTransform()
{
	// TODO: Add your command handler code here
	if (BRUSH::SELECTEDBRUSHLIST.empty())
	{
		::AfxMessageBox(
			"You must select some objects before you can transform them.",
			MB_ICONWARNING | MB_OK
			);
		return;
	}

	CTransformationDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		UNDOINFO undo;
		// save start undo frame
		if(!CEditWnd::bDisableUndoRedo)
			undo.StartFrame(BRUSH::SELECTEDBRUSHLIST, UNDOINFO::OT_TRANSFORM);
		// transform selected brushes
		BRUSH::TransformSelectedBrushList(
			VECTOR3(dlg.m_fValue), dlg.m_uMode, PREFERENCES::Current.bTextureLock
			);
		// record undo info
		if(!CEditWnd::bDisableUndoRedo)
		{	// save end undo frame
			undo.EndFrame(BRUSH::SELECTEDBRUSHLIST);
			// push undo info in stack
			CEditWnd::GLOBALUNDOSTACK.push(undo);
			// update undo/redo labels in main menu
			::UpdateUndoRedoMenu();
		}
		// redraw all editable!
		CEditWnd::RedrawWindow(WU_ALLEDIT);
	}
}

void CMainFrame::OnToolsAlign(UINT nID)
{
	// TODO: Add your command handler code here
	UNDOINFO undo;

	if (BRUSH::SELECTEDBRUSHLIST.size() < 2)
	{
		::AfxMessageBox(
			"You must select some objects (>1) before you can align them.",
			MB_ICONWARNING | MB_OK
			);
		return;
	}

	// save start undo frame
	if(!CEditWnd::bDisableUndoRedo)
		undo.StartFrame(BRUSH::SELECTEDBRUSHLIST, UNDOINFO::OT_ALIGN);
	// align selection
	BRUSH::AlignSelectedBrushList(nID - ID_ALIGN_BYFRONT, PREFERENCES::Current.bTextureLock);
	// record undo info
	if(!CEditWnd::bDisableUndoRedo)
	{	// save end undo frame
		undo.EndFrame(BRUSH::SELECTEDBRUSHLIST);
		// push undo info in stack
		CEditWnd::GLOBALUNDOSTACK.push(undo);
		// update undo/redo labels in main menu
		::UpdateUndoRedoMenu();
	}
	// redraw all editable!
	CEditWnd::RedrawWindow(WU_ALLEDIT);
}

void CMainFrame::OnToolsFlip(UINT nID)
{
	// TODO: Add your command handler code here
	UNDOINFO undo;

	if (BRUSH::SELECTEDBRUSHLIST.empty())
	{
		::AfxMessageBox("You must select some objects before you can flip them.",
			MB_ICONWARNING | MB_OK);
		return;
	}

	// save start undo frame
	if(!CEditWnd::bDisableUndoRedo)
		undo.StartFrame(BRUSH::SELECTEDBRUSHLIST, UNDOINFO::OT_FLIP);
	// flip selection
	BRUSH::FlipSelectedBrushList(nID - ID_FLIP_BYX, PREFERENCES::Current.bTextureLock);
	// record undo info
	if(!CEditWnd::bDisableUndoRedo)
	{	// save end undo frame
		undo.EndFrame(BRUSH::SELECTEDBRUSHLIST);
		// push undo info in stack
		CEditWnd::GLOBALUNDOSTACK.push(undo);
		// update undo/redo labels in main menu
		::UpdateUndoRedoMenu();
	}
	// redraw all editable!
	CEditWnd::RedrawWindow(WU_ALLEDIT);
}

void CMainFrame::OnTexturesFaceinspector()
{	// *** SHOW "FACE INSPECTOR" DIALOG ***

	// TODO: Add your command handler code here
	if(FACE::SELECTED.pFace || !BRUSH::SELECTEDBRUSHLIST.empty())
	{
		if(CEditWnd::FACEINSPECTORDIALOG.IsWindowVisible())
			CEditWnd::FACEINSPECTORDIALOG.ShowWindow(SW_HIDE);
		else
		{
			CEditWnd::FACEINSPECTORDIALOG.SetInfo();
			CEditWnd::FACEINSPECTORDIALOG.ShowWindow(SW_SHOW);
		}
	}
}

void CMainFrame::OnTexturesFindReplace()
{
	// TODO: Add your command handler code here
	CFindReplaceTextureDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		if (BRUSH::ReplaceTextures(
			dlg.m_strFindPath, dlg.m_strReplacePath, dlg.m_bSelectedOnly
			))
			CEditWnd::RedrawWindow(WU_ALLEDIT);
	}
}

void CMainFrame::OnTexturesTexturelock()
{
	// TODO: Add your command handler code here
	//CMenu *pTexturesMenu = GetMenu();				// get main frame`s menu
	//ASSERT(pTexturesMenu);
	//pTexturesMenu = pTexturesMenu->GetSubMenu(4);	// get "Textures" submenu
	//ASSERT(pTexturesMenu);

	//if (PREFERENCES::Current.bTextureLock = !PREFERENCES::Current.bTextureLock)
	//	pTexturesMenu->CheckMenuItem(ID_TEXTURES_TEXTURELOCK, MF_CHECKED | MF_BYCOMMAND);
	//else
	//	pTexturesMenu->CheckMenuItem(ID_TEXTURES_TEXTURELOCK, MF_UNCHECKED | MF_BYCOMMAND);

	PREFERENCES::Current.bTextureLock = !PREFERENCES::Current.bTextureLock;
}

void CMainFrame::OnTexturesChangeWindowScale(UINT nID)
{
	// TODO: Add your command handler code here
	if (PREFERENCES::Current.uTextureWindowSize != nID - ID_TEXTUREWINDOWSCALE_100)
	{
//		CMenu *pSizeMenu = GetMenu();			// get main frame`s menu
//		ASSERT(pSizeMenu);
//		pSizeMenu = pSizeMenu->GetSubMenu(4);	// get "Textures" submenu
//		ASSERT(pSizeMenu);
//		pSizeMenu = pSizeMenu->GetSubMenu(8);	// get "Textures/Window Size" submenu
//		ASSERT(pSizeMenu);

		// set texture scale koeff.
		PREFERENCES::Current.uTextureWindowSize = nID - ID_TEXTUREWINDOWSCALE_100;
		ASSERT(PREFERENCES::Current.uTextureWindowSize >= 0 &&
				PREFERENCES::Current.uTextureWindowSize <= 2);
		// check correspond menu
//		pSizeMenu->CheckMenuRadioItem(
//			ID_TEXTUREWINDOWSCALE_100, ID_TEXTUREWINDOWSCALE_25, nID, MF_BYCOMMAND
//		);
		// redraw texture view
		CEditWnd::RedrawWindow(WU_TEXTURE);
	}
}

void CMainFrame::OnToolsOptions()
{
	// TODO: Add your command handler code here
	COptionsSheet shet("Options");
	if (shet.DoModal() == IDOK)
	{
		/* process change undo */
		if (PREFERENCES::Current.bEnableUndo != shet.m_GeneralPage.m_bEnableUndo)
		{
			PREFERENCES::Current.bEnableUndo = shet.m_GeneralPage.m_bEnableUndo;
			CEditWnd::bDisableUndoRedo = !PREFERENCES::Current.bEnableUndo;	// NOTE:: non need it!!!
			CMainFrame::OnEditDisableundoredo();
		}

		/* process change undo depth */
		if (PREFERENCES::Current.nUndoStackDepth != shet.m_GeneralPage.m_nUndoStackDepth &&
			PREFERENCES::Current.bEnableUndo)
		{
			PREFERENCES::Current.nUndoStackDepth = shet.m_GeneralPage.m_nUndoStackDepth;
			ASSERT(PREFERENCES::Current.nUndoStackDepth >= 5 &&
					PREFERENCES::Current.nUndoStackDepth <= 50);
			CEditWnd::GLOBALUNDOSTACK.size(PREFERENCES::Current.nUndoStackDepth);
		}

		/* process load scheme */
		if (shet.m_GeneralPage.m_nColorScheme != 3)
			PREFERENCES::Current.LoadColorScheme(shet.m_GeneralPage.m_nColorScheme);

		/* process use global clear color */
		PREFERENCES::Current.bUseGlobalClearColor = shet.m_GeneralPage.bUseGlobalClearColor;

		/* set new global clear color */
		// TODO: ... ... ...

		/* process change editview layout */
		if (PREFERENCES::Current.eWindowLayout != shet.m_ViewportPage.m_wl)
		{
			// TODO: change window layout
			CCreateContext cc;
			cc.m_pNewViewClass = RUNTIME_CLASS(CEditWnd);
			cc.m_pCurrentDoc = GetActiveDocument();
			cc.m_pNewDocTemplate = 0;
			cc.m_pLastView = 0;
			cc.m_pCurrentFrame = this;

			InitWindowLayout(shet.m_ViewportPage.m_wl, &cc);
		}

		/* process grid options */
		// TODO: ... ... ...


		::SavePreferencesToIni( EDITORENV_SETTINGS_PATH );
	}
}



//=============================================================================
//							U P D A T E   C M D  U I
//=============================================================================

void CMainFrame::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (const UNDOINFO* pUndoInfo = CEditWnd::GLOBALUNDOSTACK.undoinfo())
	{
		pCmdUI->Enable(TRUE);
		pCmdUI->SetText(UNDOINFO::UNDOLABELS[pUndoInfo->optype]);
	}
	else
	{
		pCmdUI->Enable(FALSE);
		pCmdUI->SetText(UNDOINFO::UNDOLABELS[0]);
	}
}

void CMainFrame::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (const UNDOINFO* pRedoInfo = CEditWnd::GLOBALUNDOSTACK.redoinfo())
	{
		pCmdUI->Enable(TRUE);
		pCmdUI->SetText(UNDOINFO::REDOLABELS[pRedoInfo->optype]);
	}
	else
	{
		pCmdUI->Enable(FALSE);
		pCmdUI->SetText(UNDOINFO::REDOLABELS[0]);
	}
}

void CMainFrame::OnUpdateEditDisableundoredo(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetText(
		CEditWnd::bDisableUndoRedo ? "Enable Undo/Redo" : "Disable Undo/Redo"
	);
}

void CMainFrame::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(CEditWnd::CLIPBOARD.empty() ? FALSE : TRUE);
}

void CMainFrame::OnUpdateEditPastespecial(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(CEditWnd::CLIPBOARD.empty() ? FALSE : TRUE);
}

void CMainFrame::OnUpdateGridAll(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(
		PREFERENCES::Current.sGrid.Major.bShow &&
		PREFERENCES::Current.sGrid.Minor.bShow &&
		PREFERENCES::Current.sGrid.Axis.bShow
		);
}

void CMainFrame::OnUpdateGridMajor(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(
		!(PREFERENCES::Current.sGrid.Major.bShow &&
		PREFERENCES::Current.sGrid.Minor.bShow &&
		PREFERENCES::Current.sGrid.Axis.bShow)
		);

	pCmdUI->SetCheck(PREFERENCES::Current.sGrid.Major.bShow);
}

void CMainFrame::OnUpdateGridMinor(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(
		!(PREFERENCES::Current.sGrid.Major.bShow &&
		PREFERENCES::Current.sGrid.Minor.bShow &&
		PREFERENCES::Current.sGrid.Axis.bShow)
		);

	pCmdUI->SetCheck(PREFERENCES::Current.sGrid.Minor.bShow);
}

void CMainFrame::OnUpdateGridAxis(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(
		!(PREFERENCES::Current.sGrid.Major.bShow &&
		PREFERENCES::Current.sGrid.Minor.bShow &&
		PREFERENCES::Current.sGrid.Axis.bShow)
		);

	pCmdUI->SetCheck(PREFERENCES::Current.sGrid.Axis.bShow);
}

void CMainFrame::OnUpdateShowWindowlabel(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(PREFERENCES::Current.bShowWindowLabel);
}

void CMainFrame::OnUpdateShowActiverect(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(PREFERENCES::Current.bShowActiveWindowRect);
}

void CMainFrame::OnUpdateShowWorldaxis(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(PREFERENCES::Current.bShowWorldAxis);
}

void CMainFrame::OnUpdateShowWorldaxisicon(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(PREFERENCES::Current.bShowCoordSpaceIcon);
}

void CMainFrame::OnUpdateViewCubicclipping(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(PREFERENCES::Current.bEnableCubicClipping);
}

void CMainFrame::OnUpdateViewOpengllighting(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(PREFERENCES::Current.bEnableOpenGLLighting);
}

void CMainFrame::OnUpdateMapSnaptogrid(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(PREFERENCES::Current.bSnapToGrid);
}

void CMainFrame::OnUpdateMapShowgrid(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(
		PREFERENCES::Current.sGrid.Major.bShow &&
		PREFERENCES::Current.sGrid.Minor.bShow &&
		PREFERENCES::Current.sGrid.Axis.bShow
		);
}

void CMainFrame::OnUpdateTexturesTexturelock(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(PREFERENCES::Current.bTextureLock);
}

void CMainFrame::OnUpdateTexturesTextureWindowScale(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(
		PREFERENCES::Current.uTextureWindowSize == pCmdUI->m_nID - ID_TEXTUREWINDOWSCALE_100
	);
}

void CMainFrame::OnUpdateChangeShapeType(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio(
		CEditWnd::CURRENTSHAPE == BRUSH::eShapeType(pCmdUI->m_nID - ID_BRUSH_BLOCK)
	);
}

void CMainFrame::OnUpdateMorphingVertices(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(CEditWnd::GetModifyMode() == CEditWnd::MM_VERTEXDRAG);
}

void CMainFrame::OnUpdateMorphingEdges(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(CEditWnd::GetModifyMode() == CEditWnd::MM_EDGEDRAG);
}
