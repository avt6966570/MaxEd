// EditWnd.cpp : implementation file
//

#include "StdAfx.h"
#include "MaxEd.h"
#include "EditWnd.h"


/* DEBUG */ static void DebugDrawSimpleParallelepiped()
{
	glBegin(GL_QUADS);
	glVertex3i(0,0,0);
	glVertex3i(128,0,0);
	glVertex3i(128,64,0);
	glVertex3i(0,64,0);

	glVertex3i(0,0,0);
	glVertex3i(0,64,0);
	glVertex3i(0,64,-512);
	glVertex3i(0,0,-512);

	glVertex3i(0,0,0);
	glVertex3i(0,0,-512);
	glVertex3i(128,0,-512);
	glVertex3i(128,0,0);


	glVertex3i(0,0,-512);
	glVertex3i(0,64,-512);
	glVertex3i(128,64,-512);
	glVertex3i(128,0,-512);

	glVertex3i(128,0,0);
	glVertex3i(128,0,-512);
	glVertex3i(128,64,-512);
	glVertex3i(128,64,0);

	glVertex3i(0,64,0);
	glVertex3i(128,64,0);
	glVertex3i(128,64,-512);
	glVertex3i(0,64,-512);
	glEnd();
}



// draw as glRect() but wire
static inline void myglWireRectf(float x1, float y1, float x2, float y2) {
	glBegin(GL_LINE_LOOP);
		glVertex2f(x1, y1);
		glVertex2f(x2, y1);
		glVertex2f(x2, y2);
		glVertex2f(x1, y2);
	glEnd();
}

// throw cursor position as 3ds max
static inline bool ThrowingCursorPos(POINT& cursorpos)
{
	const int xmax = ::GetSystemMetrics(SM_CXSCREEN) - 1,
			  ymax = ::GetSystemMetrics(SM_CYSCREEN) - 1;
	if(cursorpos.x == 0)	{ cursorpos.x = xmax - 1; return true; }
	if(cursorpos.y == 0)	{ cursorpos.y = ymax - 1; return true; }
	if(cursorpos.x == xmax)	{ cursorpos.x = 1; return true; }
	if(cursorpos.y == ymax)	{ cursorpos.y = 1; return true; }
	return false;
}




//*****************************************************************************
//							STATIC CEditWnd MEMBERS
//*****************************************************************************
const FLOAT CEditWnd::ORTHOCAMDEEP[VM_TOTALDIMS] = { 
	524287,	// VM_FRONT
	-524288,// VM_LEFT
	-524288,// VM_BOTTOM
	-524288,// VM_BACK
	524287,	// VM_RIGHT
	524287	// VM_TOP
};


// mouse capture info
CEditWnd* CEditWnd::pEditWindow[WL_MAXWINDOW];
CEditWnd* CEditWnd::pActiveWindow;

CEditWnd::eViewMoveMode	CEditWnd::VIEWMOVEMODE;
CEditWnd::eEditingStatus CEditWnd::EDITINGSTATUS = CEditWnd::eEditingStatus::ES_SELECTION;
CEditWnd::eModifyMode CEditWnd::MODIFYMODE = CEditWnd::eModifyMode::MM_TRANSLATE;
CEditWnd::eClippingState CEditWnd::CLIPPINGSTATE;

VECTOR3	CEditWnd::vClipPlanePoint[3];			// plane definition three point

BBOX3 CEditWnd::DRAGMODIFYBOX;
BBOX3::eBoxSideInfo	CEditWnd::DRAGMODIFYSIDE;
BBOX2::eBoxSideInfo	CEditWnd::DRAGMODIFY2DSIDE;
MATRIX16 CEditWnd::DRAGMODIFYMATRIX(1,0,0,0,
									0,1,0,0,
									0,0,1,0,
									0,0,0,1);

BRUSH::eShapeType	CEditWnd::CURRENTSHAPE = BRUSH::eShapeType::ST_BLOCK;
int CEditWnd::CURRENTSHAPENSIDE = 8;


CPoint	CEditWnd::ptPrevPoint;
VECTOR2	CEditWnd::vMouseOrigin;
VECTOR2	CEditWnd::vMouseOriginNS;
VECTOR2	CEditWnd::vMouseCurrent;
VECTOR2	CEditWnd::vMouseCurrentNS;


bool	CEditWnd::bTexturesShowInUseOnly;

undostack<UNDOINFO> CEditWnd::GLOBALUNDOSTACK(10);
bool	CEditWnd::bDisableUndoRedo;


CEditWnd::brushlist	CEditWnd::CLIPBOARD;

BBOX3CLIPPER		CEditWnd::CUBICCLIPPER(VECTOR3::O, 2048.F);
VECTOR3				CEditWnd::CUBICCLIPPERDELTA(1024.F);


// cursors
const HCURSOR CEditWnd::CURSOR_ARROW = ::LoadCursor(0, IDC_ARROW);
const HCURSOR CEditWnd::CURSOR_SIZEALL = ::LoadCursor(0, IDC_SIZEALL);
const HCURSOR CEditWnd::CURSOR_SIZENESW = ::LoadCursor(0, IDC_SIZENESW);
const HCURSOR CEditWnd::CURSOR_SIZENS = ::LoadCursor(0, IDC_SIZENS);
const HCURSOR CEditWnd::CURSOR_SIZENWSE = ::LoadCursor(0, IDC_SIZENWSE);
const HCURSOR CEditWnd::CURSOR_SIZEWE = ::LoadCursor(0, IDC_SIZEWE);
const HCURSOR CEditWnd::CURSOR_HIDE = 0;
HCURSOR	CEditWnd::CURSOR_ROTATE = 0;
HCURSOR	CEditWnd::hCurrentCursor = CURSOR_ARROW;


CEntityInspectorDialog CEditWnd::ENTITYINSPECTORDIALOG;
CFaceInspectorDialog CEditWnd::FACEINSPECTORDIALOG;



// texture scroll bar
int		CEditWnd::iMaxTextureScrollHight = 1024;


// clip 2d in ortho edit wnd
BBOX2	CEditWnd::_2DCLIPPER;




//*****************************************************************************
//								CEditWnd MESSAGE MAP
//*****************************************************************************

BEGIN_MESSAGE_MAP(CEditWnd, COpenGLWnd)
	//{{AFX_MSG_MAP(CEditorWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_WM_SYSKEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_VSCROLL()
	ON_COMMAND_RANGE(ID_POPUPMENU_WINDOWPROPERTIES_FRONT,
					 ID_POPUPMENU_WINDOWPROPERTIES_TEXTURE,
					 OnChangeViewMode)
	ON_COMMAND_RANGE(ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_MODE_BOX,
					 ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_MODE_LIGHTWIRE,
					 OnChangeRenderingMode)
	ON_COMMAND_RANGE(ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_SHADEMODEL_GLFLAT,
					 ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_SHADEMODEL_GLSMOOTH,
					 OnChangeShadeModel)
	ON_COMMAND_RANGE(ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_POLYGONMODE_FRONT_GLPOINT,
					 ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_POLYGONMODE_BACK_GLFILL,
					 OnChangePolygonMode)
	ON_COMMAND(ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_CULLFACE, OnChangeCullFace)
	ON_COMMAND(ID_POPUPMENU_WINDOWPROPERTIES_DISABLEWINDOW, OnChangeDisableWindow)
	ON_COMMAND_RANGE(ID_POPUPMENU_ADDENTITY_STARTID,
					 ID_POPUPMENU_ADDENTITY_STARTID + ENTITYINFO::MAX_ENTITY,
					 OnAddEntityPopup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




//*****************************************************************************
//						CEditWnd MEMBER IMPLEMENTATION
//*****************************************************************************

IMPLEMENT_DYNCREATE(CEditWnd, COpenGLWnd)

CEditWnd::CEditWnd()
	: m_fScale(1), m_vOrigin(VECTOR2::O)	/* DEBUG */
{
	// TODO: add member initialization code here
	m_iTextureScrollPos = 0;
}

CEditWnd::~CEditWnd()
{
}

void CEditWnd::SetWindowInfo(const WINDOWINFO& wi)
{
	m_WindowInfo = wi;

	// show texture scrollbar if current viewmode is "texture"
	CWnd::SetScrollRange(SB_VERT, 0, 1024, 0);
	CWnd::SetScrollPos(SB_VERT, 0, 0);
	CWnd::ShowScrollBar(SB_VERT, m_WindowInfo.eType == VT_TEXTURE);
}

BOOL CEditWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_VSCROLL |		// + texture view scroll bar (show if we in texture mode)
				WS_CLIPCHILDREN | WS_CLIPSIBLINGS;	// + spec OpenGL window style
	return COpenGLWnd::PreCreateWindow(cs);
}

void CEditWnd::DrawCoordSpaceOverlayIcon() const
{	// don`t forget load "standart window-ortho matrix (0-w-0-h)"!
	::glPushMatrix();
		::glTranslatef(PREFERENCES::CSI_WINDOWSHIFT, PREFERENCES::CSI_WINDOWSHIFT, 0);
		switch(m_WindowInfo.eMode)
		{
		case VM_LEFT:	::glRotatef( 90, 0, 1, 0); break;
		case VM_BOTTOM: ::glRotatef(-90, 1, 0, 0); break;
		case VM_BACK:	::glRotatef(180, 0, 1, 0); break;
		case VM_RIGHT:	::glRotatef(-90, 0, 1, 0); break;
		case VM_TOP:	::glRotatef( 90, 1, 0, 0); break;
		case VM_PERSPECTIVE:
						::glRotatef( Camera::pCurrent->m_vAngles[0], 1, 0, 0),
						::glRotatef(-Camera::pCurrent->m_vAngles[1], 0, 1, 0);
						break;
		//case VM_FRONT:
		}
		::glTranslatef(-PREFERENCES::CSI_WINDOWSHIFT, -PREFERENCES::CSI_WINDOWSHIFT, 0);

		::glLineWidth(1);
		::glBegin(GL_LINES);
			// DRAW X-AXIS
			::glColor3ubv(COLOR3::RED);
			::glVertex2i(PREFERENCES::CSI_WINDOWSHIFT, PREFERENCES::CSI_WINDOWSHIFT),
			::glVertex2i(PREFERENCES::CSI_WINDOWSHIFT + PREFERENCES::CSI_SIZE, PREFERENCES::CSI_WINDOWSHIFT);

			// DRAW Y-AXIS
			::glColor3ubv(COLOR3::GREEN);
			::glVertex2i(PREFERENCES::CSI_WINDOWSHIFT, PREFERENCES::CSI_WINDOWSHIFT),
			::glVertex2i(PREFERENCES::CSI_WINDOWSHIFT, PREFERENCES::CSI_WINDOWSHIFT + PREFERENCES::CSI_SIZE);

			// DRAW Z-AXIS
			::glColor3ubv(COLOR3::BLUE);
			::glVertex3i(PREFERENCES::CSI_WINDOWSHIFT, PREFERENCES::CSI_WINDOWSHIFT, 0),
			::glVertex3i(PREFERENCES::CSI_WINDOWSHIFT, PREFERENCES::CSI_WINDOWSHIFT, PREFERENCES::CSI_SIZE);
		::glEnd();
	::glPopMatrix();
}

void CEditWnd::DrawCoordSpace() const
{
	::glLineWidth(3);
	::glBegin(GL_LINES);
	// DRAW X-AXIS
	::glColor3ubv(COLOR3::RED);
	::glVertex3i(0, 0, 0), ::glVertex3i(64, 0, 0);

	// DRAW Y-AXIS
	::glColor3ubv(COLOR3::GREEN);
	::glVertex3i(0, 0, 0), ::glVertex3i(0, 64, 0);

	// DRAW Z-AXIS
	::glColor3ubv(COLOR3::BLUE);
	::glVertex3i(0, 0, 0), ::glVertex3i(0, 0, 64);
	::glEnd();
}

void CEditWnd::DrawWindowLabelOverlay() const
{	// don`t forget load "standart window-ortho matrix (0-w-0-h)"!
	::glColor3ubv(PREFERENCES::Current.cLabelColor);
	::glRasterPos2i(VIEWLABEL::VL_LEFTSHIFT, m_iHeight - VIEWLABEL::VL_TOPSHIFT);
	::glListBase(VIEWLABEL::VL_BASELISTINDEX);
	::glCallLists(m_WindowInfo.sLabel.uLabelSize, GL_UNSIGNED_BYTE,
		m_WindowInfo.sLabel.szLabel);
}

void CEditWnd::DrawGrid(float fViewLeft, float fViewRight,
						float fViewBottom, float fViewTop) const
{
	/*** CALC WINDOW GRID BOUNDS ***/
	const float	xb = 512 * ::floor(fViewLeft/512),
				xe = 512 * ::ceil(fViewRight/512),
				yb = 512 * ::floor(fViewBottom/512),
				ye = 512 * ::ceil(fViewTop/512);

	/*** DRAW MAJOR GRID ***/
	if(PREFERENCES::Current.sGrid.Major.bShow && 
	   PREFERENCES::Current.sGrid.Major.nStep * m_fScale >= 4)
	{
		::glColor3ubv(PREFERENCES::Current.sGrid.Major.cColor);
		if(PREFERENCES::Current.sGrid.Major.bDotted)
		{
			::glPointSize(PREFERENCES::Current.sGrid.Major.fWidth);
			::glBegin(GL_POINTS);
			for (float x = xb; x < xe; x += PREFERENCES::Current.sGrid.Major.nStep)
				//if ((int)x & theEditorOptions.sMinorGridInfo.nStep - 1)
					for (float y = yb; y < ye; y += PREFERENCES::Current.sGrid.Major.nStep)
						//if ((int)y & theEditorOptions.sMinorGridInfo.nStep - 1)
							::glVertex2f(x, y);
		}
		else
		{
			if(PREFERENCES::Current.sGrid.Major.bStipple)
			{
				::glEnable(GL_LINE_STIPPLE);
				::glLineStipple(PREFERENCES::Current.sGrid.Major.nStippleFactor,
								PREFERENCES::Current.sGrid.Major.uStippleMask);
			}

			::glLineWidth(PREFERENCES::Current.sGrid.Major.fWidth);
			::glBegin(GL_LINES);
			for (float x = xb; x < xe; x += PREFERENCES::Current.sGrid.Major.nStep)
				if ((int)x & PREFERENCES::Current.sGrid.Minor.nStep - 1)
					::glVertex2f(x, yb), ::glVertex2f(x, ye);
			for (float y = yb; y < ye; y += PREFERENCES::Current.sGrid.Major.nStep)
				if ((int)y & PREFERENCES::Current.sGrid.Minor.nStep - 1)
					::glVertex2f(xb, y), ::glVertex2f(xe, y);
		}
		::glEnd();
		::glDisable(GL_LINE_STIPPLE);
	}

	/*** DRAW MINOR GRID ***/
	if(PREFERENCES::Current.sGrid.Minor.bShow &&
	   PREFERENCES::Current.sGrid.Minor.nStep * m_fScale >= 4)
	{
		::glColor3ubv(PREFERENCES::Current.sGrid.Minor.cColor);
		if(PREFERENCES::Current.sGrid.Minor.bDotted)
		{
			::glPointSize(PREFERENCES::Current.sGrid.Minor.fWidth);

			::glBegin(GL_POINTS);
			for (float x = xb; x < xe; x += PREFERENCES::Current.sGrid.Minor.nStep)
				for (float y = yb; y < ye; y += PREFERENCES::Current.sGrid.Minor.nStep)
					glVertex2f(x, y);
		}
		else
		{
			if(PREFERENCES::Current.sGrid.Minor.bStipple)
			{
				::glEnable(GL_LINE_STIPPLE);
				::glLineStipple(PREFERENCES::Current.sGrid.Minor.nStippleFactor,
								PREFERENCES::Current.sGrid.Minor.uStippleMask);
			}

			::glLineWidth(PREFERENCES::Current.sGrid.Minor.fWidth);
			::glBegin(GL_LINES);
			for (float x = xb; x < xe; x += PREFERENCES::Current.sGrid.Minor.nStep)
				::glVertex2f(x, yb), ::glVertex2f(x, ye);
			for (float y = yb; y < ye; y += PREFERENCES::Current.sGrid.Minor.nStep)
				::glVertex2f(xb, y), ::glVertex2f(xe, y);
		}
		::glEnd();
		::glDisable(GL_LINE_STIPPLE);
	}

	/*** DRAW COORDINATE AXIS ***/
	if(PREFERENCES::Current.sGrid.Axis.bShow)
	{
		::glColor3ubv(PREFERENCES::Current.sGrid.Axis.cColor);

		if(PREFERENCES::Current.sGrid.Axis.bDotted)
		{
			::glPointSize(PREFERENCES::Current.sGrid.Axis.fWidth);
			::glBegin(GL_POINTS);
			for (float x = xb; x < xe; x += PREFERENCES::Current.sGrid.Axis.nDotStep)
				::glVertex2f(x, 0);
			for (float y = yb; y < ye; y += PREFERENCES::Current.sGrid.Axis.nDotStep)
				::glVertex2f(0, y);
		}
		else 
		{
			if(PREFERENCES::Current.sGrid.Axis.bStipple)
			{
				::glEnable(GL_LINE_STIPPLE);
				::glLineStipple(PREFERENCES::Current.sGrid.Axis.nStippleFactor,
								PREFERENCES::Current.sGrid.Axis.uStippleMask);
			}

			::glLineWidth(PREFERENCES::Current.sGrid.Axis.fWidth);
			::glBegin(GL_LINES);
			::glVertex2f(xb, 0), ::glVertex2f(xe, 0);	// Ox-axis
			::glVertex2f(0, yb), ::glVertex2f(0, ye);	// Oy-axis
		}
		::glEnd();
		::glDisable(GL_LINE_STIPPLE);
	}
}

void CEditWnd::DrawActiveWindowRect() const
{	// don`t forget load "standart window-ortho matrix (0-w-0-h)"!
	::glColor3ubv(COLOR3::YELLOW);
	::glLineWidth(6);
	::glBegin(GL_LINE_LOOP);
		::glVertex2i(0, 0);
		::glVertex2i(m_iWidth, 0);
		::glVertex2i(m_iWidth, m_iHeight);
		::glVertex2i(0, m_iHeight);
	::glEnd();
}

void CEditWnd::DrawBrush(const BRUSH& b) const
{
	// for all brush faces...
	for(BRUSH::facelist::const_iterator f = b.flist.begin(); f != b.flist.end(); ++f)
	{
		// if texture mode -- bind face texture
		if(m_WindowInfo.sRenderOptions.eMode > DM_SOLID_WIRE)
			::glBindTexture(GL_TEXTURE_2D, f->texdef->texID);

		// draw face
		::glBegin(m_WindowInfo.sRenderOptions.oglRenderMode);
		::glNormal3fv(f->N);
		for(FACE::indexedvertexlist::const_iterator ivl = f->ivlist.begin(); ivl != f->ivlist.end(); ++ivl)
		{
			::glTexCoord2fv(ivl->T);
			::glVertex3fv(b.vlist[ivl->I]);
		}
		::glEnd();
	}
}

void CEditWnd::DrawWiredBrush(const BRUSH& b) const
{
	// for all brush faces...
	for(BRUSH::facelist::const_iterator f = b.flist.begin(); f != b.flist.end(); ++f)
	{
		// draw face
		::glBegin(GL_LINE_LOOP);
		for(FACE::indexedvertexlist::const_iterator ivl = f->ivlist.begin();
			ivl != f->ivlist.end();
			++ivl)
				::glVertex3fv(b.vlist[ivl->I]);
		::glEnd();
	}
}

void CEditWnd::DrawBrush2D(const BRUSH& b) const
{
	// for all brush faces...
	for(BRUSH::facelist::const_iterator f = b.flist.begin(); f != b.flist.end(); ++f)
	{
		switch(m_WindowInfo.eMode)
		{
		case VM_FRONT:	if(f->N.Z <= 0) continue; break;
		case VM_LEFT:	if(f->N.X >= 0) continue; break;
		case VM_BOTTOM:	if(f->N.Y >= 0) continue; break;
		case VM_BACK:	if(f->N.Z >= 0) continue; break;
		case VM_RIGHT:	if(f->N.X <= 0) continue; break;
		case VM_TOP:	if(f->N.Y <= 0) continue; break;
		}

		// if texture mode -- bind face texture
		if(m_WindowInfo.sRenderOptions.eMode > DM_SOLID_WIRE)
			::glBindTexture(GL_TEXTURE_2D, f->texdef->texID);

		// draw face
		::glBegin(m_WindowInfo.sRenderOptions.oglRenderMode);
		::glNormal3fv(f->N);
		for(FACE::indexedvertexlist::const_iterator ivl = f->ivlist.begin(); ivl != f->ivlist.end(); ++ivl)
		{
			::glTexCoord2fv(ivl->T);
			::glVertex3fv(b.vlist[ivl->I]);
		}
		::glEnd();
	}
}

void CEditWnd::DrawFace(const FACE* face, const BRUSH* owner) const
{
	ASSERT(face);
	ASSERT(owner);

	if(m_WindowInfo.eType == VT_2D)
		switch(m_WindowInfo.eMode)
		{
		case VM_FRONT:	if(face->N.Z <= 0) return; break;
		case VM_LEFT:	if(face->N.X >= 0) return; break;
		case VM_BOTTOM:	if(face->N.Y >= 0) return; break;
		case VM_BACK:	if(face->N.Z >= 0) return; break;
		case VM_RIGHT:	if(face->N.X <= 0) return; break;
		case VM_TOP:	if(face->N.Y <= 0) return; break;
		}

	// if texture mode -- bind face texture
	if(m_WindowInfo.sRenderOptions.eMode > DM_SOLID_WIRE)
		::glBindTexture(GL_TEXTURE_2D, face->texdef->texID);

	// draw face
	::glBegin(m_WindowInfo.sRenderOptions.oglRenderMode);
	::glNormal3fv(face->N);
	for(FACE::indexedvertexlist::const_iterator ivl = face->ivlist.begin(); ivl != face->ivlist.end(); ++ivl)
	{
		::glTexCoord2fv(ivl->T);
		::glVertex3fv(owner->vlist[ivl->I]);
	}
	::glEnd();
}

bool CEditWnd::HitTextureInView(const TEXTUREDEF* ptdf,
								const CPoint& hit, int& ox, int& oy, int& maxHi)
{
	if(CEditWnd::bTexturesShowInUseOnly && !ptdf->bInUse)
		return false;

	const int tw = ptdf->width >> PREFERENCES::Current.uTextureWindowSize,
			  th = ptdf->height >> PREFERENCES::Current.uTextureWindowSize;

	// go to next texture skin row?
	if(ox != TEXSKIN_XOFF && ox + tw > m_iWidth)
		ox = TEXSKIN_XOFF, oy -= maxHi + TEXSKIN_YOFF, maxHi = 0;

	// hit test
	if(hit.x > ox && hit.x < ox + tw && hit.y < oy && hit.y > oy - th
	/* && TEXTUREDEF::pACTIVETEXTURE != (TEXTUREDEF*)&*tdf */)	// :NOTE: apply always!
		return true;

	// update skin pos
	maxHi = max(maxHi, th);
	ox += tw + TEXSKIN_XOFF;
	return false;
}

void CEditWnd::SetActiveTexture(const TEXTUREDEF *ptdf)
{
	UNDOINFO undo;

	// set new active texture
	TEXTUREDEF::pACTIVETEXTURE = const_cast<TEXTUREDEF *>(ptdf);
	// if has selected single face...
	if (FACE::SELECTED.pFace &&
		FACE::SELECTED.pFace->texdef != TEXTUREDEF::pACTIVETEXTURE)
	{
		// save undo start frame
		if(!CEditWnd::bDisableUndoRedo)
			undo.StartFrame(*FACE::SELECTED.pFaceOwner, UNDOINFO::OT_APPLY_TEXTURE);
		// apply new texture on selected face
		FACE::SELECTED.pFace->texdef = TEXTUREDEF::pACTIVETEXTURE;
		// save undo end frame
		if(!CEditWnd::bDisableUndoRedo)
			undo.EndFrame(*FACE::SELECTED.pFaceOwner);
		// and redraw all
		RedrawWindow(WU_ALL);
	}
	// if has selected multiply brushes...
	else if (!BRUSH::SELECTEDBRUSHLIST.empty())
	{
		// save undo start frame
		if(!CEditWnd::bDisableUndoRedo)
			undo.StartFrame(BRUSH::SELECTEDBRUSHLIST, UNDOINFO::OT_APPLY_TEXTURE);
		// apply new texture on selected brushes
		BRUSH::UpdateTexDefInSelectionBrushList(TEXTUREDEF::pACTIVETEXTURE);
		// save undo end frame
		if(!CEditWnd::bDisableUndoRedo)
			undo.EndFrame(BRUSH::SELECTEDBRUSHLIST);								
		// and redraw all
		RedrawWindow(WU_ALL);
	}
	else
	{
		// redraw only texture view
		// (TODO: may be try specific update: RedrawWindow(WU_TEXTURE)?)
		Invalidate();
		return;
	}

	// record undo info
	if(!CEditWnd::bDisableUndoRedo)
	{
		// push undoinfo in undo-stack
		GLOBALUNDOSTACK.push(undo);
		// update undo/redo labels in main menu
		::UpdateUndoRedoMenu();
	}
}

void CEditWnd::SetActiveView(bool wndDisabled)
{
	if(this != CEditWnd::pActiveWindow && !wndDisabled)
	{
		CEditWnd* prevActiveWindow = CEditWnd::pActiveWindow;
		CEditWnd::pActiveWindow = this;
		prevActiveWindow->COpenGLWnd::RedrawWindow();
					this->COpenGLWnd::RedrawWindow();
	}
}


// CEditWnd message handlers

void CEditWnd::OnPaint()
{
	float w, h, left, right, bottom, top;

	CPaintDC dc(this);

	// TODO: Add your message handler code here
	// *** MAKE OPENGL RENDERING CONTEXT CURRENT ***
	if (!::wglMakeCurrent(dc, m_hGLRC))
	{
		TRACE0("UpdateWindow() -> COGLWnd::OnPaint() -> wglMakeCurrent(): failed\n"
			   "cannot make rendering context current\n");
		return;
	}

	// *** SETUP MODELVIEW MATRIX ***
	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();

	// *** SETUP VIEWPORT ***
	::glViewport(0, 0, m_iWidth, m_iHeight);

	// *** CLEAR BACKGROUND ***
	PREFERENCES::Current.bUseGlobalClearColor ?
		::glClearColor(PREFERENCES::Current.clrGlobalClearColor.red,
						PREFERENCES::Current.clrGlobalClearColor.green,
						PREFERENCES::Current.clrGlobalClearColor.blue,
						PREFERENCES::Current.clrGlobalClearColor.alpha) :
		::glClearColor(m_WindowInfo.sRenderOptions.clrClearColor.red,
						m_WindowInfo.sRenderOptions.clrClearColor.green,
						m_WindowInfo.sRenderOptions.clrClearColor.blue,
						m_WindowInfo.sRenderOptions.clrClearColor.alpha);

	::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(m_WindowInfo.bDisable)
		goto disable_window;

	// *** SETUP RENDERING OPTIONS - DRAWMODE ***
	switch(m_WindowInfo.sRenderOptions.eMode)
	{
	case DM_BBOX:
		::glDisable(GL_ALPHA_TEST);
		::glDisable(GL_BLEND);
		::glDisable(GL_DEPTH_TEST);
		::glDisable(GL_LIGHTING);
		::glDisable(GL_LINE_SMOOTH);
		::glDisable(GL_LINE_STIPPLE);
		::glDisable(GL_POINT_SMOOTH);
		::glDisable(GL_TEXTURE_1D);
		::glDisable(GL_TEXTURE_2D);
		break;
	case DM_WIRE:
		::glDisable(GL_ALPHA_TEST);
		::glDisable(GL_BLEND);
		::glDisable(GL_DEPTH_TEST);
		::glDisable(GL_LIGHTING);
		::glDisable(GL_LINE_SMOOTH);
		::glDisable(GL_LINE_STIPPLE);
		::glDisable(GL_POINT_SMOOTH);
		::glDisable(GL_TEXTURE_1D);
		::glDisable(GL_TEXTURE_2D);
		break;
	case DM_SOLID:
	case DM_SOLID_WIRE:
		::glDisable(GL_ALPHA_TEST);
		::glDisable(GL_BLEND);
		::glDisable(GL_DEPTH_TEST);
		::glDisable(GL_LIGHTING);
		::glDisable(GL_LINE_SMOOTH);
		::glDisable(GL_LINE_STIPPLE);
		::glDisable(GL_POINT_SMOOTH);
		::glDisable(GL_TEXTURE_1D);
		::glDisable(GL_TEXTURE_2D);
		::glShadeModel(m_WindowInfo.sRenderOptions.ShadeModel);
		break;

	case DM_TEXTURE:
	case DM_TEXTURE_WIRE:
		::glEnable(GL_TEXTURE_2D);
		::glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		::glDisable(GL_BLEND);
//off	::glEnable(GL_DEPTH_TEST);
//off	::glDepthFunc(GL_LEQUAL);
		::glShadeModel(m_WindowInfo.sRenderOptions.ShadeModel);
		break;

	case DM_BLEND:
	case DM_BLEND_WIRE:
		// TODO: ???
		ASSERT(0);
		break;

	case DM_LIGHT:
	case DM_LIGHT_WIRE:
		// TODO: ???
		ASSERT(0);
		break;
	}

	// + setup Polygon Mode
	::glPolygonMode(GL_FRONT, m_WindowInfo.sRenderOptions.PolyRasterMethod[0]);
	::glPolygonMode(GL_BACK,  m_WindowInfo.sRenderOptions.PolyRasterMethod[1]);

	// + setup Cull Face
	m_WindowInfo.sRenderOptions.bCullFace ?
		::glEnable(GL_CULL_FACE) : ::glDisable(GL_CULL_FACE);


	// *** CHECK VIEWTYPE & VIEWMODE ***
	switch(m_WindowInfo.eType)
	{
	case VT_2D:
		// SETUP CENTERED `FRONT` ORTHO PROJECTION MATRIX
		w  = (m_iWidth>>1) / m_fScale;
		h  = (m_iHeight>>1)/ m_fScale;
		left	= m_vOrigin.X - w;
		right	= m_vOrigin.X + w;
		bottom	= m_vOrigin.Y - h;
		top		= m_vOrigin.Y + h;
		::glMatrixMode(GL_PROJECTION);
		::glLoadIdentity();
		::glOrtho(left, right, bottom, top, -FLT_MAX, FLT_MAX);

		// DRAW GRID
		DrawGrid(left, right, bottom, top);

		// SETUP PROJECTION MODE
		switch(m_WindowInfo.eMode)
		{
		case VM_LEFT:	::glRotatef( 90, 0, 1, 0); break;
		case VM_BOTTOM: ::glRotatef(-90, 1, 0, 0); break;
		case VM_BACK:	::glRotatef(180, 0, 1, 0); break;
		case VM_RIGHT:	::glRotatef(-90, 0, 1, 0); break;
		case VM_TOP:	::glRotatef( 90, 1, 0, 0); break;
		default:		break; //case VM_FRONT:
		}

		if(m_WindowInfo.sRenderOptions.eMode > DM_WIRE)	// enable z-test if textured mode
			::glEnable(GL_DEPTH_TEST);

		::glLineWidth(1);

// ### INIT 2D CLIPPER ###
switch(m_WindowInfo.eMode)
{
case VM_BACK:
case VM_RIGHT:
	CEditWnd::_2DCLIPPER.BBOX2::BBOX2(-right, bottom, -left, top);	// NOTE: add BBOX2::operator()() for reinit!
	break;
case VM_TOP:
	CEditWnd::_2DCLIPPER.BBOX2::BBOX2(left, -top, right, -bottom);	// NOTE: add BBOX2::operator()() for reinit!
	break;
default:
	CEditWnd::_2DCLIPPER.BBOX2::BBOX2(left, bottom, right, top);	// NOTE: add BBOX2::operator()() for reinit!
	break;
}

		// *** DRAW SELECTED FACE ***
		if(FACE::SELECTED.pFace)
		{
			::glColor3ubv(COLOR3::RED);
			DrawFace(FACE::SELECTED.pFace, FACE::SELECTED.pFaceOwner);
		}

		// *** DRAW ALL NON-SELECTED BRUSH LIST ***
		for(BRUSH::brushlist::const_iterator b = BRUSH::WORLDBRUSHLIST.begin();
			b != BRUSH::WORLDBRUSHLIST.end();
			++b)
		{
			if(!(b->flags & (BRUSH::BF_SELECTED | BRUSH::BF_HIDDEN)) &&
				b->IsInsideIn(CEditWnd::_2DCLIPPER, m_WindowInfo.sDims.D1, m_WindowInfo.sDims.D2))
			{	// draw only non-selected or hidden brush & in ortho-window
				::glColor3ubv(b->owner ? (const GLubyte*)&b->owner->color : COLOR3::WHITE);
				DrawBrush2D(*b);
			}
		}

		// *** DRAW SELECTED BRUSH LIST ***
		::glColor3ubv(COLOR3::RED);						//	setup selection brush color (red)

		::glEnable(GL_LINE_STIPPLE);					//	enable stippled line (in otho only)
		::glLineStipple(1, 0xF0F0);
		::glLineWidth(2);

		if(CEditWnd::EDITINGSTATUS == ES_MODIFY)
		{
			::glDisable(GL_TEXTURE_2D);					//	disable texturing (no recalc texture coords!)
			::glDisable(GL_CULL_FACE);					//	no cull face
			::glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//	draw all as line
			::glMultMatrixf(DRAGMODIFYMATRIX);			//  and multiply view with specific modify matrix
		}

		for(BRUSH::ptrbrushlist::const_iterator b = BRUSH::SELECTEDBRUSHLIST.begin();
			b != BRUSH::SELECTEDBRUSHLIST.end();
			++b)
				DrawBrush2D(**b);

		::glDisable(GL_LINE_STIPPLE);					//	disable stippled line

		if(CEditWnd::MODIFYMODE == MM_CLIPPING)
		{
			::glColor3ubv(COLOR3::BLUE);
			::glPointSize(5);
			switch(CEditWnd::CLIPPINGSTATE)
			{
			case CS_FIRSTCLIPPLANEDEF:
				break;
			case CS_SECONDCLIPPLANEDEF:
				::glBegin(GL_POINTS);
				::glVertex3fv(CEditWnd::vClipPlanePoint[0]);
				::glEnd();
				break;
			case CS_COMPLETECLIPPLANEDEF:
				::glLineWidth(3);
				::glBegin(GL_LINES);
				::glVertex3fv(CEditWnd::vClipPlanePoint[0]);
				::glVertex3fv(CEditWnd::vClipPlanePoint[2]);
				::glEnd();

				::glBegin(GL_POINTS);
				::glVertex3fv(CEditWnd::vClipPlanePoint[0]);
				::glVertex3fv(CEditWnd::vClipPlanePoint[2]);
				::glEnd();
				break;
			}
		}
	break;
	case VT_3D:
		// SETUP PROJECTION MATRIX
		::glMatrixMode(GL_PROJECTION);
		::glLoadIdentity();
		::gluPerspective(
			RAD2DEGF(2 * ::atan((float)m_iHeight / m_iWidth)),	//yFov
			(float)m_iWidth / m_iHeight,						// screenaspect
			2,  8192);
		::glRotatef(Camera::pCurrent->m_vAngles[0],  1, 0, 0);
		::glRotatef(-Camera::pCurrent->m_vAngles[1],  0, 1, 0);
		::glTranslatef(-Camera::pCurrent->m_vOrigin[0],
					   -Camera::pCurrent->m_vOrigin[1],
					   -Camera::pCurrent->m_vOrigin[2]);

		// 3D DRAW COORD SPACE AXIS (OX/OY/OZ)
		if (PREFERENCES::Current.bShowWorldAxis)
			DrawCoordSpace();

		if(m_WindowInfo.sRenderOptions.eMode > DM_WIRE)	// enable z-test if textured mode
			::glEnable(GL_DEPTH_TEST);

		::glLineWidth(1);								// reset line width

		// *** DRAW SELECTED FACE ***
		if(FACE::SELECTED.pFace)
		{
			::glColor3ubv(COLOR3::RED);
			DrawFace(FACE::SELECTED.pFace, FACE::SELECTED.pFaceOwner);
		}

		// *** DRAW ALL NON-SELECTED BRUSH LIST ***

// + OGL LIGHTNING
if (PREFERENCES::Current.bEnableOpenGLLighting)
{
const GLfloat RGBAamb[4] = { 0.35F, 0.35F, 0.35F, 1.0F },
			  lightpos[4] = { 0.8017F, 0.5345F, 0.2672F, 0.0F };
							//{ Camera::pCurrent->m_vOrigin[0],
							//  Camera::pCurrent->m_vOrigin[1],
							//  Camera::pCurrent->m_vOrigin[2],
							//  0.0F };
::glLightfv(GL_LIGHT0, GL_AMBIENT, RGBAamb);	// all ambient light
::glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

::glEnable(GL_LIGHTING);
::glEnable(GL_LIGHT0);
::glEnable(GL_COLOR_MATERIAL);
}
		for(BRUSH::brushlist::const_iterator b = BRUSH::WORLDBRUSHLIST.begin();
			b != BRUSH::WORLDBRUSHLIST.end();
			++b)
		{
			if (b->flags & (BRUSH::BF_SELECTED | BRUSH::BF_HIDDEN))	// if brush selected or hidden
				continue;
			if (PREFERENCES::Current.bEnableCubicClipping &&		// if enable cubic clipping
				!::isInside(b->bbox, CEditWnd::CUBICCLIPPER))		// but not inside in clipper
				continue;

			::glColor3ubv(b->owner && b->owner->color != ENTITYINFO::BRUSHCOLOR ?
				(const GLubyte*)&b->owner->color :
				COLOR3::WHITE);// if color != ENTITYINFO::BRUSHCOLOR -- this is point entity
			DrawBrush(*b);
		}
::glDisable(GL_COLOR_MATERIAL);
::glDisable(GL_LIGHT0);
::glDisable(GL_LIGHTING);

		// *** DRAW SELECTED BRUSH LIST ***
		::glColor3ubv(COLOR3::RED);						// setup selection color (current -- red)
		if(CEditWnd::EDITINGSTATUS == ES_MODIFY)
		{	// DRAW BRUSHES WIRED MODIFYING OVERLAY
			::glColor3ubv(COLOR3::YELLOW);
			::glDisable(GL_DEPTH_TEST);					//	disable depth test
			::glDisable(GL_TEXTURE_2D);					//	disable texturing (no recalc texture coords!),
			::glDisable(GL_CULL_FACE);					//	no cull face,
			::glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//	draw all as line,
			::glMultMatrixf(DRAGMODIFYMATRIX);			//  and multiply view with specific modify matrix
		}

		for(BRUSH::ptrbrushlist::const_iterator b = BRUSH::SELECTEDBRUSHLIST.begin();
			b != BRUSH::SELECTEDBRUSHLIST.end();
			++b)
				DrawBrush(**b);

		if(CEditWnd::EDITINGSTATUS != ES_MODIFY)
		{	// DRAW BRUSHES WIRED SELECTION OVERLAY
			::glColor3ubv(COLOR3::YELLOW);
			::glDisable(GL_DEPTH_TEST);					//	disable depth test
			::glDisable(GL_TEXTURE_2D);					//	disable texturing (no recalc texture coords!),
			::glDisable(GL_CULL_FACE);					//	no cull face,
			::glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//	draw all as line,

			for(BRUSH::ptrbrushlist::const_iterator b = BRUSH::SELECTEDBRUSHLIST.begin();
				b != BRUSH::SELECTEDBRUSHLIST.end();
				++b)
					DrawBrush(**b);
		}

	break;
	case VT_TEXTURE:
		int cx = TEXSKIN_XOFF, cy = m_iHeight - TEXSKIN_YOFF,
			maxHi = 0, iOld = CEditWnd::iMaxTextureScrollHight;

		// LOAD "STANDART WINDOW-ORTHO MATRIX (0-w-0-h)"
		::glMatrixMode(GL_PROJECTION);
		::glLoadIdentity();
		::glOrtho(0, m_iWidth, m_iTextureScrollPos, m_iTextureScrollPos + m_iHeight, -1, 1);
		::glDisable(GL_DEPTH_TEST);
		::glDisable(GL_BLEND);
		::glEnable(GL_TEXTURE_2D);

		// DRAW TEXTURE SKIN
		::glColor3ubv(COLOR3::WHITE);
		if (TEXTUREDIR::CURRENTCACHEDIR)
		{	// draw current texture cache dir
			for (TEXTUREDIR::texturedeflistiterator tdf = TEXTUREDIR::CURRENTCACHEDIR->cache.begin();
				tdf != TEXTUREDIR::CURRENTCACHEDIR->cache.end();
				++tdf)
			{
				// if "show-in-use" mode & texture realy not use
				if(CEditWnd::bTexturesShowInUseOnly && !tdf->bInUse)
					continue;
				// calc scale koeff.
				const int tw = tdf->width >> PREFERENCES::Current.uTextureWindowSize,
						  th = tdf->height >> PREFERENCES::Current.uTextureWindowSize;
				// go to next skin row? (reset row pos, inc column pos)
				if (cx != TEXSKIN_XOFF && cx + tw > m_iWidth)
					cx = TEXSKIN_XOFF, cy -= maxHi + TEXSKIN_YOFF, maxHi = 0;
				// if is realy show?
				if (!(cy < m_iTextureScrollPos ||
					cy - (th>>PREFERENCES::Current.uTextureWindowSize) >
					m_iTextureScrollPos + m_iHeight))
						// draw it
						tdf->DrawSkin(
							cx, cy,
							PREFERENCES::Current.uTextureWindowSize,
							&*tdf == TEXTUREDEF::pACTIVETEXTURE
						);
				// update row & column pos
				maxHi = max(maxHi, th);
				cx += tw + TEXSKIN_XOFF;
			}
		}
		else
		{
			for (TEXTUREDIR::texturedirlistiterator tdr = TEXTUREDIR::CACHE.begin();
				tdr != TEXTUREDIR::CACHE.end();
				++tdr)
				for (TEXTUREDIR::texturedeflistiterator tdf = tdr->cache.begin();
					tdf != tdr->cache.end();
					++tdf)
				{
					// if "show-in-use" mode & texture realy not use
					if(CEditWnd::bTexturesShowInUseOnly && !tdf->bInUse)
						continue;
					// calc scale koeff.
					const int tw = tdf->width >> PREFERENCES::Current.uTextureWindowSize,
							  th = tdf->height >> PREFERENCES::Current.uTextureWindowSize;
					// go to next skin row? (reset row pos, inc column pos)
					if (cx != TEXSKIN_XOFF && cx + tw > m_iWidth)
						cx = TEXSKIN_XOFF, cy -= maxHi + TEXSKIN_YOFF, maxHi = 0;

					// is realy show?
					if (!(cy < m_iTextureScrollPos ||
						cy - (th>>PREFERENCES::Current.uTextureWindowSize) >
						m_iTextureScrollPos + m_iHeight))
							tdf->DrawSkin(
								cx, cy,
								PREFERENCES::Current.uTextureWindowSize,
								&*tdf == TEXTUREDEF::pACTIVETEXTURE
							);
					maxHi = max(maxHi, th);
					cx += tw + TEXSKIN_XOFF;
				}
		}

		// SWAP BUFFERS
		::glFlush();
		::SwapBuffers(dc);

		// UPDATE TEXTURE SCROLL RANGE
		CEditWnd::iMaxTextureScrollHight = ::abs(cy) + maxHi;
		if(CEditWnd::iMaxTextureScrollHight != iOld)
			CWnd::SetScrollRange(SB_VERT, 0, CEditWnd::iMaxTextureScrollHight, TRUE);
		return;		// das ist alles!
	break;
	}

	// reset & disable texture
	::glDisable(GL_DEPTH_TEST);
	::glBindTexture(GL_TEXTURE_2D, 0);
	::glDisable(GL_TEXTURE_2D);

	// *** DRAW VERTICES IF WE IN VERTEX-DRAG MODE ***
	if(CEditWnd::MODIFYMODE == MM_VERTEXDRAG)
	{
		::glColor3ubv(COLOR3::BLUE);	// NOTE: draw all points
		::glPointSize(5);
		::glBegin(GL_POINTS);
		for(BRUSH::ptrbrushlist::const_iterator b = BRUSH::SELECTEDBRUSHLIST.begin();
			b != BRUSH::SELECTEDBRUSHLIST.end();
			++b)
				(*b)->DrawVertices();
		::glEnd();
	}
	else if(CEditWnd::MODIFYMODE == MM_EDGEDRAG)
	{
		::glColor3ubv(COLOR3::GREEN);
		::glPointSize(5);
		::glBegin(GL_POINTS);
		for(BRUSH::ptrbrushlist::const_iterator b = BRUSH::SELECTEDBRUSHLIST.begin();
			b != BRUSH::SELECTEDBRUSHLIST.end();
			++b)
				(*b)->DrawEdgeMarkers();
		::glEnd();
	}

	// *** DRAW 2D OVERLAYS (3D-AXIS ICON/WINDOW LABEL/ACTIVE WND RECT) ***
disable_window:
	// LOAD "STANDART WINDOW-ORTHO MATRIX (0-w-0-h)"
	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();
	::glOrtho(0, m_iWidth, 0, m_iHeight, -PREFERENCES::CSI_SIZE, PREFERENCES::CSI_SIZE);
	
	// *** DRAW COORD SPACE ICON ***
	if(PREFERENCES::Current.bShowCoordSpaceIcon)
		DrawCoordSpaceOverlayIcon();

	// *** DRAW WINDOW LABEL ***
	if(PREFERENCES::Current.bShowWindowLabel)
		DrawWindowLabelOverlay();

	// *** DRAW ACTIVE WINDOW`s YELLOW RECT ***
	if(PREFERENCES::Current.bShowActiveWindowRect &&
		this == CEditWnd::pActiveWindow)
		DrawActiveWindowRect();

	// *** SWAP COLOR BUFFERS ***
	::glFlush();
	::SwapBuffers(dc);

	// Do not call COpenGLWnd::OnPaint() for painting messages
}

void CEditWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if(m_WindowInfo.eType != VT_TEXTURE)
		return;

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);

	int n = CWnd::GetScrollPos(SB_VERT);
	switch (nSBCode)
	{
	case SB_LINEUP :
		n = (n - 15 >  0) ? n - 15 : 0; 
		break;
	case SB_LINEDOWN :
		n = (n + 15 < CEditWnd::iMaxTextureScrollHight) ? n + 15 : n; 
		break;
	case SB_PAGEUP :
		n = (n - m_iHeight >  0) ? n - m_iHeight : 0; 
		break;
	case SB_PAGEDOWN :
		n = (n + m_iHeight < CEditWnd::iMaxTextureScrollHight) ? n + m_iHeight : n; 
		break;
	case SB_THUMBPOSITION :
	case SB_THUMBTRACK :
		n = nPos;
		break;
	}
	CWnd::SetScrollPos(SB_VERT, n);
	m_iTextureScrollPos = -n;
	Invalidate();
	UpdateWindow();	// TODO: may be specific update: RedrawWindow(WU_TEXTURE)?
}

void CEditWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_WindowInfo.bDisable)
		return COpenGLWnd::OnLButtonDown(nFlags, point);	// exit if disable

	// *** SET CAPTURE/MOUSE INFO ***
	ptPrevPoint = point;
	ScreenToWorld(point, vMouseOrigin, PREFERENCES::Current.bSnapToGrid);
	ScreenToWorld(point, vMouseOriginNS, false);
	CWnd::SetCapture();
	CWnd::SetFocus();
	SetActiveView();

	// *** DRAG EDITVIEW MODE ***
	if(::GetKeyState('Z') & 0x8000)		// Z
	{
		switch(m_WindowInfo.eType)
		{
		case VT_2D:
			::SetCursor(hCurrentCursor = CURSOR_SIZEALL);	// set new cursor
			CEditWnd::EDITINGSTATUS = ES_VIEWMOVE;			// set status
			CEditWnd::VIEWMOVEMODE = VMM_2DMOVE;			// set 2d move cam mode
			break;
		case VT_3D:
			// ENABLE `FLY` MODE
			if(CEditWnd::EDITINGSTATUS != ES_VIEWMOVE)
			{
				::SetCursor(hCurrentCursor = CURSOR_HIDE);	// hide cursor
				CEditWnd::EDITINGSTATUS = ES_VIEWMOVE;		// set new status
				CEditWnd::VIEWMOVEMODE = VMM_3DFLY;			// set 3d `fly` cam mode
			}
			// DISABLE `FLY` MODE
			else
			{
				::SetCursor(hCurrentCursor = CURSOR_ARROW);
				CEditWnd::EDITINGSTATUS = ES_SELECTION;		// reset status
			}
			break;
		}
	}
	// *** SELECTION MODE (LMB + SHIFT (+ CTRL)) ***
	else if(nFlags & MK_SHIFT)
	{
		switch(m_WindowInfo.eType)
		{
		case VT_2D:
			// 2D SELECTION
			if(BRUSH* pSelBrush = BRUSH::GetSelectionInWorldBrushList(
				VECTOR3(vMouseOriginNS, m_WindowInfo.eMode, ORTHOCAMDEEP[m_WindowInfo.eMode]),
				VECTOR3::INVIEW[m_WindowInfo.eMode])
				)
			{
				// now we need clear face selection
				if(FACE::SELECTED.pFace) {
					FACE::SELECTED.pFace = 0;
					FACE::SELECTED.pFaceOwner = 0;
				}

				// toggle select / deselect brush
				if(pSelBrush->flags ^= BRUSH::BF_SELECTED)
				{
					if(::GetKeyState(VK_MENU) & 0x8000)		// SHIFT + ALT -- force single selection
					{
						if(!(nFlags & MK_CONTROL))			
							BRUSH::ClearSelectedBrushList();
						BRUSH::SELECTEDBRUSHLIST.push_back(pSelBrush);	// SHIFT + ALT + CTRL -- ignore grouping
					}
					else if(pSelBrush->owner && BRUSH::SelectByOwner(pSelBrush->owner))	// check owner for group selection
						BRUSH::ReInitSelectedBrushList();
					else
						BRUSH::SELECTEDBRUSHLIST.push_back(pSelBrush);
				}
				else
					BRUSH::SELECTEDBRUSHLIST.erase(BRUSH::SELECTEDBRUSHLIST.find(pSelBrush));
					// TODO: and exit from MM_VERTEXDRAG/MM_EDGEDRAG mode here???

				// we must UPDATE "ENTITY INSPECTOR" dialog
				// NOTE: update dlg only if: it was first selected brush && dlg is visible
				if (BRUSH::SELECTEDBRUSHLIST.size() == 1 && ENTITYINSPECTORDIALOG.IsWindowVisible())
					ENTITYINSPECTORDIALOG.SetInfo();

				RedrawWindow(WU_ALLEDIT);
			}
			break;
		// 3D SELECTION
		case VT_3D:
			{{{
			FACE* pSelFace = 0;
			// calc ray direction (dir)
			float u = (float)(m_iHeight - point.y - 1 - (m_iHeight>>1)) / (m_iWidth>>1),
				  r = (float)(point.x - (m_iWidth>>1)) / (m_iWidth>>1);
			VECTOR3 dir(-Camera::pCurrent->m_vForward * 1 +
						Camera::pCurrent->m_vRight * r +
						(Camera::pCurrent->m_vForward ^ Camera::pCurrent->m_vRight) * u);
			dir.Normalize();

			if(BRUSH* pSelBrush =
				BRUSH::GetSelectionInWorldBrushList(Camera::pCurrent->m_vOrigin, dir, &pSelFace))
			{
				// FACE SELECTION
				if(nFlags & MK_CONTROL)
				{
					// clear brush selection
					BRUSH::ClearSelectedBrushList();

					// clear vertex-drag list & reset modify mode
					BRUSH::SELECTEDVERTEXLIST.clear();
					CEditWnd::MODIFYMODE = MM_TRANSLATE;

					if(FACE::SELECTED.pFace == pSelFace)
					{
						FACE::SELECTED.pFace = 0;
						FACE::SELECTED.pFaceOwner = 0;
					}
					else
					{
						FACE::SELECTED.pFace = pSelFace;
						FACE::SELECTED.pFaceOwner = pSelBrush;
					}
				}
				// BRUSH SELECTION
				else
				{
					// now we need clear face selection
					if(FACE::SELECTED.pFace) {
						FACE::SELECTED.pFace = 0;
						FACE::SELECTED.pFaceOwner = 0;
					}

					if(pSelBrush->flags ^= BRUSH::BF_SELECTED)
					{
						//if(::GetKeyState(VK_MENU) & 0x8000)	// SHIFT + ALT -- force single selection
						//	BRUSH::ClearSelectedBrushList();
						//BRUSH::SELECTEDBRUSHLIST.push_back(pSelBrush);
						if(::GetKeyState(VK_MENU) & 0x8000)		// SHIFT + ALT -- force single selection
						{
							if(!(nFlags & MK_CONTROL))			
								BRUSH::ClearSelectedBrushList();
							BRUSH::SELECTEDBRUSHLIST.push_back(pSelBrush);	// SHIFT + ALT + CTRL -- ignore grouping
						}
						else if(pSelBrush->owner && BRUSH::SelectByOwner(pSelBrush->owner))	// check owner for group selection
							BRUSH::ReInitSelectedBrushList();
						else
							BRUSH::SELECTEDBRUSHLIST.push_back(pSelBrush);
					}
					else
						BRUSH::SELECTEDBRUSHLIST.erase(BRUSH::SELECTEDBRUSHLIST.find(pSelBrush));
						// TODO: and exit from MM_VERTEXDRAG/MM_EDGEDRAG mode here???

					// we must UPDATE "ENTITY INSPECTOR" dialog
					// NOTE: update dlg only if: it was first selected brush && dlg is visible
					if (BRUSH::SELECTEDBRUSHLIST.size() == 1 && ENTITYINSPECTORDIALOG.IsWindowVisible())
						ENTITYINSPECTORDIALOG.SetInfo();
				}

				// we must UPDATE "FACE INSPECTOR" dialog
				// NOTE: update only if have selected brush or single face & dlg is visible
				if ((FACE::SELECTED.pFace || !BRUSH::SELECTEDBRUSHLIST.empty()) &&
						FACEINSPECTORDIALOG.IsWindowVisible())
					FACEINSPECTORDIALOG.SetInfo();

				RedrawWindow(WU_ALLEDIT);
			}
			}}}
			break;
		}

		// *** UPDATE STATUS BAR SELECTION INFO ***
		UpdateStatusBarSelectionInfo();

	}// nFlags & MK_SHIFT
	else
	{
		switch (m_WindowInfo.eType)
		{
		// *** CREATE & MODIFY MODE (2D) ***
		case VT_2D:
			if (BRUSH::SELECTEDBRUSHLIST.empty())
			{
				// if has selected face & press ALT
				if (FACE::SELECTED.pFace && ::GetKeyState(VK_MENU) & 0x8000)
				{	// +++ START MOUSE TEXTURE MOVE +++
					UNDOINFO undo;
					// record undo info
					if(!bDisableUndoRedo)
					{	// save undo start frame
						undo.StartFrame(*FACE::SELECTED.pFaceOwner, UNDOINFO::OT_TEXTURE_SHIFT);
						// :NOTE: no save end frame -- only after LMB-Up!
						/* undo.EndFrame(*FACE::SELECTED.pFaceOwner); */
						// push undoinfo in undo-stack
						GLOBALUNDOSTACK.push(undo);
					}
					// set new modify mode
					CEditWnd::MODIFYMODE = MM_MOUSETEXTURESHIFT;
				}
				else
					// +++ DO CREATE NEW BRUSH +++
					CEditWnd::EDITINGSTATUS = ES_CREATE;
			}
			else
			{
				// +++ DRAGGING VERTICES/EDGES +++
				if(CEditWnd::MODIFYMODE == MM_VERTEXDRAG || CEditWnd::MODIFYMODE == MM_EDGEDRAG)
				{
					// build selected VERTEX list
					if( CEditWnd::MODIFYMODE == MM_VERTEXDRAG &&
						BRUSH::BuildSelectedVertexList(vMouseOriginNS, m_WindowInfo.sDims.D1, m_WindowInfo.sDims.D2)
						)
					{
						if(!bDisableUndoRedo)
						{
							// save undo start frame
							UNDOINFO undo;
							undo.StartFrame(BRUSH::SELECTEDBRUSHLIST, UNDOINFO::OT_MORFING);
							// NOTE:: no save end frame -- only after LMB-Up!
							/* undo.EndFrame(BRUSH::SELECTEDBRUSHLIST); */
							// push undoinfo in undo-stack
							GLOBALUNDOSTACK.push(undo);
						}
						// set new editing statue
						CEditWnd::EDITINGSTATUS = ES_MODIFY;
					}

					// build selected EDGE list
					if( CEditWnd::MODIFYMODE == MM_EDGEDRAG &&
						BRUSH::BuildSelectedEdgeList(vMouseOriginNS, m_WindowInfo.sDims.D1, m_WindowInfo.sDims.D2)
						)
					{
						if(!bDisableUndoRedo)
						{
							// save undo start frame
							UNDOINFO undo;
							undo.StartFrame(BRUSH::SELECTEDBRUSHLIST, UNDOINFO::OT_MORFING);
							// NOTE:: no save end frame -- only after LMB-Up!
							/* undo.EndFrame(BRUSH::SELECTEDBRUSHLIST); */
							// push undoinfo in undo-stack
							GLOBALUNDOSTACK.push(undo);
						}
						// set new editing statue
						CEditWnd::EDITINGSTATUS = ES_MODIFY;
					}
				}
				// +++ CLIP BRUSH +++
				else if(CEditWnd::MODIFYMODE == MM_CLIPPING)
					switch(CEditWnd::CLIPPINGSTATE)
					{
					case CS_FIRSTCLIPPLANEDEF:
					case CS_COMPLETECLIPPLANEDEF:
						vClipPlanePoint[0] = VECTOR3(vMouseOrigin, m_WindowInfo.eMode);
						vClipPlanePoint[1] = vClipPlanePoint[0] + VECTOR3::INVIEW[m_WindowInfo.eMode];
						CEditWnd::CLIPPINGSTATE = CS_SECONDCLIPPLANEDEF;		// do next clip state
						break;
					case CS_SECONDCLIPPLANEDEF:
						vClipPlanePoint[2] = VECTOR3(vMouseOrigin, m_WindowInfo.eMode);
						CEditWnd::CLIPPINGSTATE = vClipPlanePoint[2] == vClipPlanePoint[0] ?
								CS_FIRSTCLIPPLANEDEF : CS_COMPLETECLIPPLANEDEF;
						break;
					}
				// +++ LMB + ALT & !CTRL (TEXTURE MOUSE MANIPULATION IN 2D-VIEW) +++
				else if (!(nFlags & MK_CONTROL|MK_SHIFT) && ::GetKeyState(VK_MENU) & 0x8000)
					CEditWnd::MODIFYMODE = MM_MOUSETEXTURESHIFT;
				// +++ LMB & SELECTED (TRANS/ROT/SHEAR/RESIZE BRUSHES) +++
				else
				{
					// build dragbox-modify unibox
					DRAGMODIFYBOX.Clear();
					for(BRUSH::ptrbrushlist::const_iterator s = BRUSH::SELECTEDBRUSHLIST.begin();
						s != BRUSH::SELECTEDBRUSHLIST.end();
						++s)
							DRAGMODIFYBOX += (*s)->bbox;

					// get dragbox-modify 2dside
					DRAGMODIFY2DSIDE = DRAGMODIFYBOX.Get2dSide(vMouseOriginNS, m_WindowInfo.eMode);
					
					// set new editing status & modify mode
					CEditWnd::EDITINGSTATUS = ES_MODIFY;

					if (DRAGMODIFY2DSIDE == BBOX2::eBoxSideInfo::BSI_INSIDE)
						CEditWnd::MODIFYMODE = MM_TRANSLATE;		// if inside -- translate
					else if (nFlags & MK_CONTROL)
					{
						if (::GetKeyState(VK_MENU) & 0x8000)
							CEditWnd::MODIFYMODE = MM_ROTATE;		// CTRL + ALT -- rotate
						else
							CEditWnd::MODIFYMODE = MM_SHEAR;		// CTRL -- shear
					}
					else
						CEditWnd::MODIFYMODE = MM_RESIZE;			// resize
				}
			}
			break;
		/*** TEXTURE MOUSE MANIPULATION IN 3D-VIEW ***/
		case VT_3D:
			if (::GetKeyState(VK_MENU) & 0x8000 &&
				(FACE::SELECTED.pFace || !BRUSH::SELECTEDBRUSHLIST.empty()))
			{
				UNDOINFO undo;
				// record undo info
				if(!bDisableUndoRedo)
				{	// save undo start frame
					if (FACE::SELECTED.pFace)
						undo.StartFrame(*FACE::SELECTED.pFaceOwner, UNDOINFO::OT_TEXTURE_SHIFT);
					else
						undo.StartFrame(BRUSH::SELECTEDBRUSHLIST, UNDOINFO::OT_TEXTURE_SHIFT);
					// :NOTE: no save end frame -- only after LMB-Up!
					/* undo.EndFrame(BRUSH::SELECTEDBRUSHLIST); */
					// push undoinfo in undo-stack
					GLOBALUNDOSTACK.push(undo);
				}
				// set new modify mode
				CEditWnd::MODIFYMODE = MM_MOUSETEXTURESHIFT;
			}
			break;
		// *** SELECT TEXTURE ***
		case VT_TEXTURE:
			{
			// hit on texture skin
			CPoint hit(point.x, m_iHeight - point.y + m_iTextureScrollPos);
			// initial skin left-top pos
			int ox = TEXSKIN_XOFF, oy = m_iHeight - TEXSKIN_YOFF, maxHi = 0;
			// find selection in current cache dir
			if (TEXTUREDIR::CURRENTCACHEDIR)
			{
				for (TEXTUREDIR::texturedeflistiterator tdf = TEXTUREDIR::CURRENTCACHEDIR->cache.begin();
					tdf != TEXTUREDIR::CURRENTCACHEDIR->cache.end();
					++tdf)
						if (HitTextureInView(&*tdf, hit, ox, oy, maxHi))
						{
							SetActiveTexture(&*tdf);
							break;
						}
			}
			// find selection in ALL cache dir
			else
			{
				for (TEXTUREDIR::texturedirlistiterator tdr = TEXTUREDIR::CACHE.begin();
					tdr != TEXTUREDIR::CACHE.end();
					++tdr)
					for (TEXTUREDIR::texturedeflistiterator tdf = tdr->cache.begin();
						tdf != tdr->cache.end();
						++tdf)
							if (HitTextureInView(&*tdf, hit, ox, oy, maxHi))
							{
								SetActiveTexture(&*tdf);
								goto exit;
							}
			}
exit:;
			}
			break;
		}
	}

	COpenGLWnd::OnLButtonDown(nFlags, point);
}

void CEditWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_WindowInfo.bDisable)
		return COpenGLWnd::OnLButtonUp(nFlags, point);
	
	// *** SET MOUSE CAPTURE INFO ***
	ptPrevPoint = point;
	ScreenToWorld(point, vMouseCurrent, PREFERENCES::Current.bSnapToGrid);
	ScreenToWorld(point, vMouseCurrentNS, false);

	// *** RELEASE MOUSE CAPTURE ***
	UNDOINFO undo;

	switch(m_WindowInfo.eType)
	{
	case VT_2D:
		if(CEditWnd::EDITINGSTATUS == ES_MODIFY)
		{
			switch (CEditWnd::MODIFYMODE)
			{
			case MM_DRAGCREATE:
				// set undo action type
				undo.SetAction(UNDOINFO::OT_NEWOBJECT);
				// resize new object
				BRUSH::WORLDBRUSHLIST.back().Translate(DRAGMODIFYMATRIX, false, true, true);
				// record undo info
				if(!bDisableUndoRedo)
				{	// save undo end frame
					undo.EndFrame(BRUSH::WORLDBRUSHLIST.back());
					// push undoinfo in undo-stack
					GLOBALUNDOSTACK.push(undo);
				}
				break;
			case MM_TRANSLATE:
				// save undo start frame
				if(!bDisableUndoRedo)
					undo.StartFrame(BRUSH::SELECTEDBRUSHLIST, UNDOINFO::OT_TRANSLATE);
				// translate selection objects
				BRUSH::TranslateSelectedBrushList(
					DRAGMODIFYMATRIX, PREFERENCES::Current.bTextureLock
					);
				// record undo info
				if(!bDisableUndoRedo)
				{	// save undo end frame
					undo.EndFrame(BRUSH::SELECTEDBRUSHLIST);
					// push undoinfo in undo-stack
					GLOBALUNDOSTACK.push(undo);
				}
				break;
			case MM_ROTATE:
				// save undo start frame
				if(!bDisableUndoRedo)
					undo.StartFrame(BRUSH::SELECTEDBRUSHLIST, UNDOINFO::OT_ROTATE);
				// rotate selection objects
				BRUSH::TranslateSelectedBrushList(
					DRAGMODIFYMATRIX, PREFERENCES::Current.bTextureLock
					);
				// record undo info
				if(!bDisableUndoRedo)
				{	// save undo end frame
					undo.EndFrame(BRUSH::SELECTEDBRUSHLIST);
					// push undoinfo in undo-stack
					GLOBALUNDOSTACK.push(undo);
				}
				break;
			case MM_RESIZE:
				// save undo start frame
				if(!bDisableUndoRedo)
					undo.StartFrame(BRUSH::SELECTEDBRUSHLIST, UNDOINFO::OT_RESIZE);
				// resize selection objects
				BRUSH::TranslateSelectedBrushList(DRAGMODIFYMATRIX, false);
				// record undo info
				if(!bDisableUndoRedo)
				{	// save undo end frame
					undo.EndFrame(BRUSH::SELECTEDBRUSHLIST);
					// push undoinfo in undo-stack
					GLOBALUNDOSTACK.push(undo);
				}
				break;
			case MM_SHEAR:
				// save undo start frame
				if(!bDisableUndoRedo)
					undo.StartFrame(BRUSH::SELECTEDBRUSHLIST, UNDOINFO::OT_SHEAR);
				// shear selection objects
				BRUSH::TranslateSelectedBrushList(DRAGMODIFYMATRIX, false);
				// record undo info
				if(!bDisableUndoRedo)
				{	// save undo end frame
					undo.EndFrame(BRUSH::SELECTEDBRUSHLIST);
					// push undoinfo in undo-stack
					GLOBALUNDOSTACK.push(undo);
				}
				break;
			case MM_VERTEXDRAG:
			case MM_EDGEDRAG:
				BRUSH::TranslateSelectedBrushList(MATRIX16::I, false);
				if(!bDisableUndoRedo)
				{
					ASSERT(GLOBALUNDOSTACK.current());
					ASSERT(GLOBALUNDOSTACK.current()->optype == UNDOINFO::OT_MORFING);
					GLOBALUNDOSTACK.current()->EndFrame(BRUSH::SELECTEDBRUSHLIST);
				}
				break;
			}
			
			// + update undo/redo labels in main menu
			if(!bDisableUndoRedo)
				::UpdateUndoRedoMenu();
			// reset main modify matrix
			DRAGMODIFYMATRIX.LoadIdentity();
			// reset editing status
			CEditWnd::EDITINGSTATUS = ES_SELECTION;
			// update status bar
			UpdateStatusBarSelectionInfo();
			// and redraw result!
			RedrawWindow(WU_ALLEDIT);
		}// if CEditWnd::EDITINGSTATUS == ES_MODIFY
		else
			switch (CEditWnd::MODIFYMODE)
			{
			case MM_CLIPPING:
				// update status bar
				UpdateStatusBarSelectionInfo();
				// and redraw result!
				CWnd::RedrawWindow();
				break;
			case MM_MOUSETEXTURESHIFT:
				OnMouseTextureMove();
				break;
			default:
				// reset editing status
				CEditWnd::EDITINGSTATUS = ES_SELECTION;
				break;
			}

		if(hCurrentCursor != CURSOR_ARROW)
			::SetCursor(hCurrentCursor = CURSOR_ARROW);		// set standart cursor
		::ReleaseCapture();									// unset window capture
		break;
	case VT_3D:
		if(CEditWnd::EDITINGSTATUS != ES_VIEWMOVE)
		{
			if (CEditWnd::MODIFYMODE == MM_MOUSETEXTURESHIFT)
				OnMouseTextureMove();

			::SetCursor(hCurrentCursor = CURSOR_ARROW);		// set standart cursor
			::ReleaseCapture();								// unset window capture
			CEditWnd::EDITINGSTATUS = ES_SELECTION;			// reset status
		}
		break;
	case VT_TEXTURE:
		CEditWnd::EDITINGSTATUS = ES_SELECTION;				// reset editing status
		if(hCurrentCursor != CURSOR_ARROW)
			::SetCursor(hCurrentCursor = CURSOR_ARROW);		// set standart cursor
		::ReleaseCapture();									// unset window capture
		break;
	}

	COpenGLWnd::OnLButtonUp(nFlags, point);
}

void CEditWnd::OnMouseTextureMove()
{
	if(!bDisableUndoRedo)
	{
		ASSERT(GLOBALUNDOSTACK.current());
		ASSERT(GLOBALUNDOSTACK.current()->optype == UNDOINFO::OT_TEXTURE_SHIFT);
		if(FACE::SELECTED.pFace)
		{
			GLOBALUNDOSTACK.current()->EndFrame(*FACE::SELECTED.pFaceOwner);
			// + update "Face Inspector" dialog (if visible)
			if (FACEINSPECTORDIALOG.IsWindowVisible())
				FACEINSPECTORDIALOG.SetTexMod(FACE::SELECTED.pFace);
		}
		else
		{
			GLOBALUNDOSTACK.current()->EndFrame(BRUSH::SELECTEDBRUSHLIST);
			// + update "Face Inspector" dialog (if visible)
			if (FACEINSPECTORDIALOG.IsWindowVisible())
				FACEINSPECTORDIALOG.SetTexMod(&BRUSH::SELECTEDBRUSHLIST[0]->flist[0]);
		}

		::UpdateUndoRedoMenu();
	}
	// reset modify mode
	CEditWnd::MODIFYMODE = MM_TRANSLATE;
}

void CEditWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_WindowInfo.bDisable)
		return COpenGLWnd::OnLButtonDblClk(nFlags, point);

	switch(m_WindowInfo.eType)
	{
	case VT_2D:

		// if LMB + Z
		if(::GetKeyState('Z') & 0x8000)
		{
			// *** SET CAPTURE/MOUSE INFO ***
			CWnd::SetCapture();
			CWnd::SetFocus();
			::SetCursor(hCurrentCursor = CURSOR_SIZENS);

			// *** SET 2D ZOOM CAM MODE ***
			CEditWnd::EDITINGSTATUS = ES_VIEWMOVE;			// set status to viewmove
			CEditWnd::VIEWMOVEMODE = VMM_2DSCALE;			// set 2d zoom cam mode

			if(PREFERENCES::Current.bForceCenteredWhenScaleMode)
			{
				m_vOrigin = vMouseOriginNS;
				COpenGLWnd::RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
			}
		}
		break;
	case VT_3D:
		// ...
		break;
	}

	COpenGLWnd::OnLButtonDblClk(nFlags, point);
}

void CEditWnd::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_WindowInfo.bDisable)
		return COpenGLWnd::OnMButtonDown(nFlags, point);

	// *** SET CAPTURE ***
	ptPrevPoint = point;
	CWnd::SetCapture();
	CWnd::SetFocus();
	SetActiveView();

	// *** PROCESS MOUSE ***
	switch(m_WindowInfo.eType)
	{
	case VT_2D:
		CEditWnd::EDITINGSTATUS = ES_VIEWMOVE;			// set status to viewmove
		CEditWnd::VIEWMOVEMODE = VMM_2DMOVE;			// set 2d move cam mode
		::SetCursor(hCurrentCursor = CURSOR_SIZEALL);
		break;
	case VT_3D:
		if(CEditWnd::EDITINGSTATUS == ES_VIEWMOVE && CEditWnd::VIEWMOVEMODE == VMM_3DFLY)
		{
			CEditWnd::EDITINGSTATUS = ES_SELECTION;
			::SetCursor(hCurrentCursor = CURSOR_ARROW);
			::ReleaseCapture();
		}
		else
		{
			CEditWnd::EDITINGSTATUS = ES_VIEWMOVE;
			CEditWnd::VIEWMOVEMODE = VMM_3DFLY;
			::SetCursor(hCurrentCursor = CURSOR_HIDE);
		}
		break;
	}

	COpenGLWnd::OnMButtonDown(nFlags, point);
}

void CEditWnd::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_WindowInfo.bDisable)
		return COpenGLWnd::OnMButtonUp(nFlags, point);

	// *** RELEASE MOUSE CAPTURE ***
	if(m_WindowInfo.eType != VT_3D)
	{
		CEditWnd::EDITINGSTATUS = ES_SELECTION;			// reset editing status
		::SetCursor(hCurrentCursor = CURSOR_ARROW);
		::ReleaseCapture();
	}

	COpenGLWnd::OnMButtonUp(nFlags, point);
}

void CEditWnd::OnMButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_WindowInfo.bDisable)
		return COpenGLWnd::OnMButtonDblClk(nFlags, point);

	switch(m_WindowInfo.eType)
	{
	case VT_2D:
		// *** SET 2D ZOOM CAM MODE ***
		CEditWnd::EDITINGSTATUS = ES_VIEWMOVE;			// set status to viewmove
		CEditWnd::VIEWMOVEMODE = VMM_2DSCALE;			// set 2d zoom cam mode

		// *** MOUSE CAPTURE ***
		CWnd::SetCapture();
		::SetCursor(hCurrentCursor = CURSOR_SIZENS);

		if(PREFERENCES::Current.bForceCenteredWhenScaleMode)
		{
			m_vOrigin = vMouseOriginNS;
			COpenGLWnd::RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		}
		break;
	case VT_3D:
		// ... ... ...
		break;
	}

	COpenGLWnd::OnMButtonDblClk(nFlags, point);
}

void CEditWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	
	// *** SET MOUSE CAPTURE INFO ***
	ptPrevPoint = point;
	ScreenToWorld(point, vMouseOrigin, PREFERENCES::Current.bSnapToGrid);
	ScreenToWorld(point, vMouseOriginNS, false);
	//CWnd::SetCapture();
	//CWnd::SetFocus();

	// *** SET ACTIVE WINDOW ***
	SetActiveView(m_WindowInfo.bDisable);

	// *** PREPARE WINDOW OPTIONS POPUP MENU ***

	// show if clicked in window-label area
	if(point.x < 100 && point.y < 25)
	{
		CMenu mnuWindowOptions, mnuRenderOptions, mnuRenderMode,
			  mnuPolygonMode, mnuPolygonModeFront, mnuPolygonModeBack,
			  mnuShadeModel;

		// create Popup->Render Options->Render Mode menu
		mnuRenderMode.CreateMenu();
		mnuRenderMode.AppendMenu(MF_STRING,
			ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_MODE_BOX, "Box");
		mnuRenderMode.AppendMenu(MF_STRING,
			ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_MODE_WIRE, "Wire");
		mnuRenderMode.AppendMenu(MF_STRING,
			ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_MODE_SOLID, "Solid");
		mnuRenderMode.AppendMenu(MF_STRING,
			ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_MODE_SOLIDWIRE, "Solid + Wire");
		mnuRenderMode.AppendMenu(MF_STRING,
			ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_MODE_TEXTURE, "Texture");
		mnuRenderMode.AppendMenu(MF_STRING,
			ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_MODE_TEXTUREWIRE, "Texture + Wire");
		mnuRenderMode.AppendMenu(MF_STRING,
			ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_MODE_BLEND, "Blend");
		mnuRenderMode.AppendMenu(MF_STRING,
			ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_MODE_BLENDWIRE, "Blend + Wire");
		mnuRenderMode.AppendMenu(MF_STRING,
			ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_MODE_LIGHT, "Light");
		mnuRenderMode.AppendMenu(MF_STRING,
			ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_MODE_LIGHTWIRE, "Light + Wire");
		mnuRenderMode.CheckMenuRadioItem(0, 9, m_WindowInfo.sRenderOptions.eMode,
			MF_BYPOSITION);

		// create Popup->Render Options->Polygone Mode->Front menu
		mnuPolygonModeFront.CreateMenu();
		mnuPolygonModeFront.AppendMenu(MF_STRING,
			ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_POLYGONMODE_FRONT_GLPOINT,
			"GL_POINT");
		mnuPolygonModeFront.AppendMenu(MF_STRING,
			ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_POLYGONMODE_FRONT_GLLINE,
			"GL_LINE");
		mnuPolygonModeFront.AppendMenu(MF_STRING,
			ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_POLYGONMODE_FRONT_GLFILL,
			"GL_FILL");
		mnuPolygonModeFront.CheckMenuRadioItem(0, 2,
			m_WindowInfo.sRenderOptions.PolyRasterMethod[0] == GL_POINT ? 0 :
			m_WindowInfo.sRenderOptions.PolyRasterMethod[0] == GL_LINE ? 1 : 2,
			MF_BYPOSITION);

		// create Popup->Render Options->Polygone Mode->Back menu
		mnuPolygonModeBack.CreateMenu();
		mnuPolygonModeBack.AppendMenu(MF_STRING,
			ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_POLYGONMODE_BACK_GLPOINT,
			"GL_POINT");
		mnuPolygonModeBack.AppendMenu(MF_STRING,
			ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_POLYGONMODE_BACK_GLLINE,
			"GL_LINE");
		mnuPolygonModeBack.AppendMenu(MF_STRING,
			ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_POLYGONMODE_BACK_GLFILL,
			"GL_FILL");
		mnuPolygonModeBack.CheckMenuRadioItem(0, 2,
			m_WindowInfo.sRenderOptions.PolyRasterMethod[1] == GL_POINT ? 0 :
			m_WindowInfo.sRenderOptions.PolyRasterMethod[1] == GL_LINE ? 1 : 2,
			MF_BYPOSITION);

		// create Popup->Render Options->Polygone Mode menu
		mnuPolygonMode.CreateMenu();
		mnuPolygonMode.AppendMenu(MF_POPUP, (UINT_PTR)mnuPolygonModeFront.m_hMenu, "Front");
		mnuPolygonMode.AppendMenu(MF_POPUP, (UINT_PTR)mnuPolygonModeBack.m_hMenu, "Back");

		// create Popup->Render Options->Shade Model menu
		mnuShadeModel.CreateMenu();
		mnuShadeModel.AppendMenu(MF_STRING,
			ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_SHADEMODEL_GLFLAT,
			"GL_FLAT");
		mnuShadeModel.AppendMenu(MF_STRING,
			ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_SHADEMODEL_GLSMOOTH,
			"GL_SMOOTH");
		mnuShadeModel.CheckMenuRadioItem(0, 1,
			m_WindowInfo.sRenderOptions.ShadeModel == GL_FLAT ? 0 : 1, MF_BYPOSITION);

		// create Popup->Render Options menu
		mnuRenderOptions.CreateMenu();
		mnuRenderOptions.AppendMenu(MF_POPUP, (UINT_PTR)mnuRenderMode.m_hMenu, "Mode");
		mnuRenderOptions.AppendMenu(MF_POPUP, (UINT_PTR)mnuPolygonMode.m_hMenu, "Polygon Mode");
		mnuRenderOptions.AppendMenu(MF_POPUP, (UINT_PTR)mnuShadeModel.m_hMenu, "ShadeModel");
		mnuRenderOptions.AppendMenu(MF_STRING,
			ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_CULLFACE, "Cull Face");
		if(m_WindowInfo.sRenderOptions.bCullFace)
			mnuRenderOptions.CheckMenuItem(
			ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_CULLFACE, MF_CHECKED|MF_BYCOMMAND);

		// create Popup->Window Options menu
		UINT_PTR nID = 60000;
		mnuWindowOptions.CreatePopupMenu();
		mnuWindowOptions.AppendMenu(MF_STRING, ID_POPUPMENU_WINDOWPROPERTIES_FRONT, "Front");
		mnuWindowOptions.AppendMenu(MF_STRING, ID_POPUPMENU_WINDOWPROPERTIES_BACK, "Back");
		mnuWindowOptions.AppendMenu(MF_STRING, ID_POPUPMENU_WINDOWPROPERTIES_LEFT, "Left");
		mnuWindowOptions.AppendMenu(MF_STRING, ID_POPUPMENU_WINDOWPROPERTIES_RIGHT, "Right");
		mnuWindowOptions.AppendMenu(MF_STRING, ID_POPUPMENU_WINDOWPROPERTIES_BOTTOM, "Bottom");
		mnuWindowOptions.AppendMenu(MF_STRING, ID_POPUPMENU_WINDOWPROPERTIES_TOP, "Top");
		mnuWindowOptions.AppendMenu(MF_STRING, ID_POPUPMENU_WINDOWPROPERTIES_PERSPECTIVE, "Perspective");
		mnuWindowOptions.AppendMenu(MF_STRING, ID_POPUPMENU_WINDOWPROPERTIES_TEXTURE, "Texture");
		mnuWindowOptions.AppendMenu(MF_SEPARATOR, nID++, "");
		mnuWindowOptions.AppendMenu(MF_POPUP, (UINT_PTR)mnuRenderOptions.m_hMenu, "Render Options");
		mnuWindowOptions.AppendMenu(MF_SEPARATOR, nID++, "");
		mnuWindowOptions.AppendMenu(MF_STRING, ID_POPUPMENU_WINDOWPROPERTIES_DISABLEWINDOW, "Disable Window");
		const UINT vmtable[VM_TOTALEDITVIEW] = { 0, 2, 4, 1, 3, 5, 6 };
		mnuWindowOptions.CheckMenuRadioItem(0, 6, vmtable[m_WindowInfo.eMode],
			MF_CHECKED|MF_BYPOSITION);
		if(m_WindowInfo.bDisable)
		{
			mnuWindowOptions.CheckMenuItem(ID_POPUPMENU_WINDOWPROPERTIES_DISABLEWINDOW,
				MF_CHECKED|MF_BYCOMMAND);

			// grayed all
			for(UINT i = 0; i < 11; ++i)
				mnuWindowOptions.EnableMenuItem(i, MF_GRAYED|MF_BYPOSITION);
		}

		// show Window Options menu
		ClientToScreen(&point);
		mnuWindowOptions.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}

	// *** PREPARE "ADD ENTITY" POPUP MENU ***
	else if(m_WindowInfo.eType == VT_2D)
	{
		CMenu mnuAddEntity,
			  mnuAmmo, mnuInfo, mnuItem,
			  mnuMisc, mnuPatch,
			  mnuTarget, mnuTeam, mnuTrigger, mnuWeapon, mnuOther;

		mnuAddEntity.CreatePopupMenu();
		mnuAmmo.CreateMenu();
		mnuInfo.CreateMenu();
		mnuItem.CreateMenu();
		mnuMisc.CreateMenu();
		mnuPatch.CreateMenu();
		mnuTarget.CreateMenu();
		mnuTeam.CreateMenu();
		mnuTrigger.CreateMenu();
		mnuWeapon.CreateMenu();
		mnuOther.CreateMenu();


		// fill "add entity" popup-menu info
		for(INT_PTR i = 0, nID = ID_POPUPMENU_ADDENTITY_STARTID;
			i < ENTITYINFO::GLOBALENTITYINFOLIST.GetSize();
			++i)
		{
			if(ENTITYINFO::GLOBALENTITYINFOLIST[i].name.Find("func_") == -1)	// no "func_" in menu!
			{
				if(ENTITYINFO::GLOBALENTITYINFOLIST[i].name.Find("ammo_") != -1)
					mnuAmmo.AppendMenu(MF_STRING, nID, ENTITYINFO::GLOBALENTITYINFOLIST[i].name);
				else if(ENTITYINFO::GLOBALENTITYINFOLIST[i].name.Find("info_") != -1)
					mnuInfo.AppendMenu(MF_STRING, nID, ENTITYINFO::GLOBALENTITYINFOLIST[i].name);
				else if(ENTITYINFO::GLOBALENTITYINFOLIST[i].name.Find("item_") != -1)
					mnuItem.AppendMenu(MF_STRING, nID, ENTITYINFO::GLOBALENTITYINFOLIST[i].name);
				else if(ENTITYINFO::GLOBALENTITYINFOLIST[i].name.Find("misc_") != -1)
					mnuMisc.AppendMenu(MF_STRING, nID, ENTITYINFO::GLOBALENTITYINFOLIST[i].name);
				else if(ENTITYINFO::GLOBALENTITYINFOLIST[i].name.Find("patch_") != -1)
					mnuPatch.AppendMenu(MF_STRING, nID, ENTITYINFO::GLOBALENTITYINFOLIST[i].name);
				else if(ENTITYINFO::GLOBALENTITYINFOLIST[i].name.Find("target_") != -1)
					mnuTarget.AppendMenu(MF_STRING, nID, ENTITYINFO::GLOBALENTITYINFOLIST[i].name);
				else if(ENTITYINFO::GLOBALENTITYINFOLIST[i].name.Find("team_") != -1)
					mnuTeam.AppendMenu(MF_STRING, nID, ENTITYINFO::GLOBALENTITYINFOLIST[i].name);
				else if(ENTITYINFO::GLOBALENTITYINFOLIST[i].name.Find("trigger_") != -1)
					mnuTrigger.AppendMenu(MF_STRING, nID, ENTITYINFO::GLOBALENTITYINFOLIST[i].name);
				else if(ENTITYINFO::GLOBALENTITYINFOLIST[i].name.Find("weapon_") != -1)
					mnuWeapon.AppendMenu(MF_STRING, nID, ENTITYINFO::GLOBALENTITYINFOLIST[i].name);
				else
					mnuOther.AppendMenu(MF_STRING, nID, ENTITYINFO::GLOBALENTITYINFOLIST[i].name);

				ENTITYINFO::GLOBALENTITYINFOLIST[i].menuID = nID++;
			}
		}

		mnuAddEntity.AppendMenu(MF_POPUP, (UINT_PTR)mnuAmmo.m_hMenu, "ammo");
		mnuAddEntity.AppendMenu(MF_POPUP, (UINT_PTR)mnuInfo.m_hMenu, "info");
		mnuAddEntity.AppendMenu(MF_POPUP, (UINT_PTR)mnuItem.m_hMenu, "item");
		mnuAddEntity.AppendMenu(MF_STRING, ID_POPUPMENU_ADDENTITY_LIGHT, "light");
		mnuAddEntity.AppendMenu(MF_POPUP, (UINT_PTR)mnuMisc.m_hMenu, "misk");
		mnuAddEntity.AppendMenu(MF_POPUP, (UINT_PTR)mnuPatch.m_hMenu, "patch");
		mnuAddEntity.AppendMenu(MF_POPUP, (UINT_PTR)mnuTarget.m_hMenu, "target");
		mnuAddEntity.AppendMenu(MF_POPUP, (UINT_PTR)mnuTeam.m_hMenu, "team");
		mnuAddEntity.AppendMenu(MF_POPUP, (UINT_PTR)mnuTrigger.m_hMenu, "trigger");
		mnuAddEntity.AppendMenu(MF_POPUP, (UINT_PTR)mnuWeapon.m_hMenu, "weapon");
		mnuAddEntity.AppendMenu(MF_POPUP, (UINT_PTR)mnuOther.m_hMenu, "other");

		// show Add Entity menu
		ClientToScreen(&point);
		mnuAddEntity.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}

	COpenGLWnd::OnRButtonDown(nFlags, point);
}

void CEditWnd::OnAddEntityPopup(UINT nID)
{
	ASSERT(nID >= ID_POPUPMENU_ADDENTITY_STARTID &&
		   nID <= ID_POPUPMENU_ADDENTITY_STARTID + ENTITYINFO::MAX_ENTITY);

	if(ENTITYINFO* pEntityInfo = ENTITYINFO::FindEntityByMenuID(nID))
	{
		// make new entity
		BRUSH brush;
		brush.MakeBlock(VECTOR3(vMouseOrigin, m_WindowInfo.eMode), pEntityInfo->size);
		brush.UpdateTexDef(&TEXTUREDEF::ZERO);	// "ZERO" - spec texture for point-entity objects
		BRUSH::WORLDBRUSHLIST.push_back(brush);
		BRUSH::WORLDBRUSHLIST.back().MakeEntity(*pEntityInfo);
		// record undo info
		if(!bDisableUndoRedo)
		{	// save undo end frame
			UNDOINFO undo(UNDOINFO::OT_NEWENTITY);
			undo.EndFrame(BRUSH::WORLDBRUSHLIST.back());
			// push undoinfo in undo-stack
			GLOBALUNDOSTACK.push(undo);
		}
		// reinit selection
		BRUSH::ReInitSelectedBrushList();
		// return focus
		ASSERT(CEditWnd::pActiveWindow);
		CEditWnd::pActiveWindow->SetFocus();
		// redraw all editview
		RedrawWindow(WU_ALLEDIT);
	}
	else
		ASSERT(0);
}

void CEditWnd::SetViewMode(VIEWMODE vm)
{
	if(m_WindowInfo.Set(vm))
	{
		// TODO: reset m_vOrigin to zero?
		CWnd::ShowScrollBar(SB_VERT, m_WindowInfo.eType == VT_TEXTURE);
		COpenGLWnd::RedrawWindow();
	}
}

void CEditWnd::NextViewMode()
{
	VIEWMODE currentvm = m_WindowInfo.eMode;
	SetViewMode(++currentvm);
}

void CEditWnd::OnChangeViewMode(UINT nID)
{
	ASSERT(nID >= ID_POPUPMENU_WINDOWPROPERTIES_FRONT &&
		   nID <= ID_POPUPMENU_WINDOWPROPERTIES_TEXTURE);
	ASSERT((ID_POPUPMENU_WINDOWPROPERTIES_TEXTURE - nID) >= 0 &&
		   (ID_POPUPMENU_WINDOWPROPERTIES_TEXTURE - nID) < VM_TOTAL);

	const VIEWMODE	vmtable[VM_TOTAL] = {
						VM_TEXTURE, VM_PERSPECTIVE, VM_TOP, VM_BOTTOM,
						VM_RIGHT, VM_LEFT, VM_BACK, VM_FRONT
	};

	SetViewMode(VIEWMODE(vmtable[ID_POPUPMENU_WINDOWPROPERTIES_TEXTURE - nID]));
}

void CEditWnd::OnChangeRenderingMode(UINT nID)
{
	if(m_WindowInfo.Set(
		DRAWMODE(nID - ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_MODE_BOX)))
		COpenGLWnd::RedrawWindow();
}

void CEditWnd::OnChangeShadeModel(UINT nID)
{
	ASSERT(nID == ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_SHADEMODEL_GLFLAT ||
		   nID == ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_SHADEMODEL_GLSMOOTH);

	const UINT newsm =
		nID == ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_SHADEMODEL_GLFLAT ?
																	GL_FLAT : GL_SMOOTH;
	if(m_WindowInfo.sRenderOptions.ShadeModel != newsm)
	{
		m_WindowInfo.sRenderOptions.ShadeModel = newsm;
		COpenGLWnd::RedrawWindow();
	}
}

void CEditWnd::OnChangePolygonMode(UINT nID)
{
	ASSERT(nID >= ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_POLYGONMODE_FRONT_GLPOINT &&
		   nID <= ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_POLYGONMODE_BACK_GLFILL);
	ASSERT(nID - ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_POLYGONMODE_FRONT_GLPOINT >= 0 &&
		   nID - ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_POLYGONMODE_FRONT_GLPOINT < 6);

	const UINT pmtable[6] =
		{ GL_POINT, GL_LINE, GL_FILL, GL_POINT, GL_LINE, GL_FILL };
	const UINT newpm =
		pmtable[nID - ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_POLYGONMODE_FRONT_GLPOINT];

	if(nID < ID_POPUPMENU_WINDOWPROPERTIES_RENDEROPTIONS_POLYGONMODE_BACK_GLPOINT)
	{	// work with FRONT poly side
		if(m_WindowInfo.sRenderOptions.PolyRasterMethod[0] != newpm)
		{
			m_WindowInfo.sRenderOptions.PolyRasterMethod[0] = newpm;
			COpenGLWnd::RedrawWindow();
		}
	}
	else
	{	// work with BACK poly side
		if(m_WindowInfo.sRenderOptions.PolyRasterMethod[1] != newpm)
		{
			m_WindowInfo.sRenderOptions.PolyRasterMethod[1] = newpm;
			COpenGLWnd::RedrawWindow();
		}
	}
}

void CEditWnd::OnChangeCullFace()
{
	m_WindowInfo.sRenderOptions.bCullFace = !m_WindowInfo.sRenderOptions.bCullFace;
	COpenGLWnd::RedrawWindow();
}

void CEditWnd::OnChangeDisableWindow()
{
	m_WindowInfo.bDisable = !m_WindowInfo.bDisable;
	m_WindowInfo.bDisable ?
		m_WindowInfo.sLabel.uLabelSize += 9 :
		m_WindowInfo.sLabel.uLabelSize -= 9;
	COpenGLWnd::RedrawWindow();
}

void CEditWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	COpenGLWnd::OnRButtonUp(nFlags, point);
}

void CEditWnd::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	COpenGLWnd::OnRButtonDblClk(nFlags, point);
}

void CEditWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_WindowInfo.bDisable)
		return COpenGLWnd::OnMouseMove(nFlags, point);		// exit

	// *** SET MOUSE CAPTURE INFO ***
	ScreenToWorld(point, vMouseCurrent, PREFERENCES::Current.bSnapToGrid);
	ScreenToWorld(point, vMouseCurrentNS, false);

	// *** PROCESS MOUSE ACTION ***
	switch(CEditWnd::EDITINGSTATUS)
	{
	case ES_SELECTION:
		// SET STATUS BAR COORD INFO
		if(!nFlags)
		{
			CStatusBar *pStatusBar = (CStatusBar*)CWnd::GetParentFrame()->GetMessageBar();
			ASSERT(pStatusBar);
			if(m_WindowInfo.eType == VT_2D)
			{
				CString coord2D;
				coord2D.Format("%.0f, %.0f", vMouseCurrentNS.X, vMouseCurrentNS.Y);
				pStatusBar->SetPaneText(2, coord2D);
			}
			//else
			//	pStatusBar->SetPaneText(2, "");
		}
		// MOUSE MOVE TEXTURE	(LMB + ALT)
		else if(nFlags == MK_LBUTTON && ::GetKeyState(VK_MENU) & 0x8000)
		{
			const VECTOR2 texshift((FLOAT)(point.x - ptPrevPoint.x),
									(FLOAT)-(point.y - ptPrevPoint.y));
			// move texture on all brush face
			if(!BRUSH::SELECTEDBRUSHLIST.empty())
			{
				BRUSH::MoveTextureCoordsInSelectionBrushList(texshift, VECTOR2::O, 0);
				RedrawWindow(WU_ALLEDIT);
			}
			// move texture on selected face
			else if(FACE::SELECTED.pFace)
			{
				FACE::SELECTED.pFace->MoveTextureCoords(
					FACE::SELECTED.pFaceOwner->vlist, texshift, VECTOR2::O, 0);
				RedrawWindow(WU_ALLEDIT);
			}
		}
		break;
	case ES_VIEWMOVE:
		{{{
		// throwing?
		POINT cursorpos;
		::GetCursorPos(&cursorpos);
		if (ThrowingCursorPos(cursorpos))
		{
			::SetCursorPos(cursorpos.x, cursorpos.y);
			ptPrevPoint = cursorpos;
			CWnd::ScreenToClient(&ptPrevPoint);
			return COpenGLWnd::OnMouseMove(nFlags, point);			// exit
		}

		const VECTOR2 mdelta((point.x - ptPrevPoint.x) / m_fScale,
								(point.y - ptPrevPoint.y) / m_fScale);

		switch(CEditWnd::VIEWMOVEMODE)
		{
		case VMM_2DMOVE:
			m_vOrigin.X -= mdelta.X, m_vOrigin.Y += mdelta.Y;
			break;
		case VMM_2DSCALE:
			{{{
			// calc delta zoom
			const float deltazoom = mdelta.Magnitude() / 256 * m_fScale * m_fScale;
			// define inc/deg
			mdelta.Y > 0 ? m_fScale += deltazoom : m_fScale -= deltazoom;
			// check hi-low bounds
			if(m_fScale < 0.1F) m_fScale = 0.1F;
			if(m_fScale > 16)	m_fScale = 16;
			// set new zoom factor in status bar
			CStatusBar *pStatusBar = (CStatusBar*)CWnd::GetParentFrame()->GetMessageBar();
			ASSERT(pStatusBar);
			CString str;
			str.Format("Zoom: %.1f", m_fScale);
			pStatusBar->SetPaneText(4, str);
			}}}
			break;
		case VMM_3DFLY:
			ASSERT(Camera::pCurrent);
			Camera::pCurrent->m_vAngles[0] += mdelta.Y / 2;
			Camera::pCurrent->m_vAngles[1] -= mdelta.X / 2;
			// update forward-right-up cam vectors
			Camera::pCurrent->BuildMatrix();
			break;
		}

		// redraw current window
		COpenGLWnd::RedrawWindow();
		}}}
		break;
	case ES_CREATE:
		{{{
		const VECTOR3 origin(vMouseOrigin, m_WindowInfo.eMode),
					  current(vMouseCurrent, m_WindowInfo.eMode);
		BRUSH brush;
		brush.MakeShape(
			CURRENTSHAPE, origin,
			CURRENTSHAPE >= BRUSH::eShapeType::TOTAL_NONCHANGEABLE ? CURRENTSHAPENSIDE : 0,
			PREFERENCES::Current.sGrid.Major.nStep
			);

		// if size() == capacity() next push get realloc vector
		// and all pointer in SELECTEDBRUSHLIST would be nonvalid 
		if(BRUSH::WORLDBRUSHLIST.size() == BRUSH::WORLDBRUSHLIST.capacity()) {
			BRUSH::WORLDBRUSHLIST.push_back(brush);
			BRUSH::ReInitSelectedBrushList();
		} else {
			BRUSH::WORLDBRUSHLIST.push_back(brush);
			BRUSH::SELECTEDBRUSHLIST.push_back(&BRUSH::WORLDBRUSHLIST.back());
		}

		CEditWnd::EDITINGSTATUS = ES_MODIFY;
		CEditWnd::MODIFYMODE = MM_DRAGCREATE;
		RedrawWindow(WU_ALLEDIT);
		}}}
		break;
	case ES_MODIFY:
		{{{
		VECTOR3 delta(vMouseCurrent - vMouseOrigin, m_WindowInfo.eMode);
		switch(CEditWnd::MODIFYMODE)
		{
		case MM_DRAGCREATE:
			{{{
			const VECTOR3 origin(vMouseOrigin, m_WindowInfo.eMode),
						  current(vMouseCurrent, m_WindowInfo.eMode);
			BRUSH::WORLDBRUSHLIST.back().RebuildShape(CEditWnd::DRAGMODIFYMATRIX,
				origin, current, PREFERENCES::Current.sGrid.Major.nStep
				);
			}}}
			break;
		case MM_TRANSLATE:
			DRAGMODIFYMATRIX.LoadTranslation(delta);
			break;
		case MM_ROTATE:
			LoadBoxSideRotationMatrix(vMouseOriginNS, vMouseCurrentNS);
			break;
		case MM_RESIZE:
			LoadBoxSideTransMatrix(delta);
			break;
		case MM_SHEAR:
			LoadBoxSideShearMatrix(delta);
			break;
		case MM_VERTEXDRAG:
		case MM_EDGEDRAG:
			if(!BRUSH::SELECTEDVERTEXLIST.empty())
			{
				VECTOR3 cur, prev;
				ScreenToWorld(point, cur, PREFERENCES::Current.bSnapToGrid);
				ScreenToWorld(ptPrevPoint, prev, PREFERENCES::Current.bSnapToGrid);
				BRUSH::DragSelectedVertexList(
					cur - prev, m_WindowInfo.sDims.D1, m_WindowInfo.sDims.D2,
					PREFERENCES::Current.sGrid.Major.nStep, PREFERENCES::Current.bSnapToGrid
					);
			}
			break;
		case MM_CLIPPING:
			switch(CLIPPINGSTATE)
			{
			case CS_FIRSTCLIPPLANEDEF:
				break;
			case CS_SECONDCLIPPLANEDEF:
				break;
			case CS_COMPLETECLIPPLANEDEF:
				break;
			}
			break;
		}
		RedrawWindow(WU_ALLEDIT);
		}}}
		break;
	}

	ptPrevPoint = point;
	COpenGLWnd::OnMouseMove(nFlags, point);
}

void CEditWnd::LoadBoxSideRotationMatrix(const VECTOR2& origin, const VECTOR2& current) const
{
	float drot = 0;
	if(DRAGMODIFY2DSIDE & BBOX2::eBoxSideInfo::BSI_TOP)
		drot += current.X - origin.X;
	if(DRAGMODIFY2DSIDE & BBOX2::eBoxSideInfo::BSI_BOTTOM)
		drot -= current.X - origin.X;
	if(DRAGMODIFY2DSIDE & BBOX2::eBoxSideInfo::BSI_LEFT)
		drot += current.Y - origin.Y;
	if(DRAGMODIFY2DSIDE & BBOX2::eBoxSideInfo::BSI_RIGHT)
		drot -= current.Y - origin.Y;
	drot = ::floor(drot/m_fScale / 15.0F + 0.5F) * 15.0F;	// :TODO: if PREFERENCES::iRotationRoundFactor == 15

	VECTOR3 bbcenter(DRAGMODIFYBOX.GetCenter());
	DRAGMODIFYMATRIX.LoadIdentity();
	DRAGMODIFYMATRIX.Translate(bbcenter);
	DRAGMODIFYMATRIX.RotationAxis(
		VECTOR3::INVIEW[m_WindowInfo.eMode < 3 ? m_WindowInfo.eMode : m_WindowInfo.eMode - 3], drot);
	DRAGMODIFYMATRIX.Translate(-bbcenter);
}

void CEditWnd::LoadBoxSideTransMatrix(const VECTOR3& delta) const
{
	const VECTOR3	bsize(DRAGMODIFYBOX.Size()),
					k((bsize + delta)/bsize),
					k2((bsize - delta)/bsize);
		  VECTOR3	t1(VECTOR3::O), sc(VECTOR3::I), t2(VECTOR3::O);
	const FLOAT& mins = (FLOAT)PREFERENCES::Current.sGrid.Major.nStep;
	const unsigned &d1 = m_WindowInfo.sDims.D1, &d2 = m_WindowInfo.sDims.D2;

	if(DRAGMODIFY2DSIDE & BBOX2::eBoxSideInfo::BSI_TOP) {
		t1[d2] = DRAGMODIFYBOX.Min[d2];
		sc[d2] = k[d2] > 0 ? k[d2] : mins/bsize[d2];
		t2[d2] = -DRAGMODIFYBOX.Min[d2];
	}
	if(DRAGMODIFY2DSIDE & BBOX2::eBoxSideInfo::BSI_RIGHT) {
		t1[d1] = DRAGMODIFYBOX.Min[d1];
		sc[d1] = k[d1] > 0 ? k[d1] : mins/bsize[d1];
		t2[d1] = -DRAGMODIFYBOX.Min[d1];
	}
	if(DRAGMODIFY2DSIDE & BBOX2::eBoxSideInfo::BSI_BOTTOM) {
		t1[d2] = DRAGMODIFYBOX.Max[d2];
		sc[d2] = k2[d2] > 0 ? k2[d2] : mins/bsize[d2];
		t2[d2] = -DRAGMODIFYBOX.Max[d2];
	}
	if(DRAGMODIFY2DSIDE & BBOX2::eBoxSideInfo::BSI_LEFT) {
		t1[d1] = DRAGMODIFYBOX.Max[d1];
		sc[d1] = k2[d1] > 0 ? k2[d1] : mins/bsize[d1];
		t2[d1] = -DRAGMODIFYBOX.Max[d1];
	}

	// prepare modifying matrix
	DRAGMODIFYMATRIX.LoadIdentity();
	DRAGMODIFYMATRIX.Translate(t1);
	DRAGMODIFYMATRIX.Scale(sc);
	DRAGMODIFYMATRIX.Translate(t2);
}

void CEditWnd::LoadBoxSideShearMatrix(const VECTOR3& delta) const
{
	const VECTOR3	bsize(DRAGMODIFYBOX.Size()),
					k((bsize + delta)/bsize),
					k2((bsize - delta)/bsize);
		  VECTOR3	t1(VECTOR3::O), sc(VECTOR3::I), t2(VECTOR3::O),
					shiftaxis[3] = { VECTOR3::O, VECTOR3::O, VECTOR3::O };

	const FLOAT& mins = (FLOAT)PREFERENCES::Current.sGrid.Major.nStep;
	const unsigned &d1 = m_WindowInfo.sDims.D1, &d2 = m_WindowInfo.sDims.D2;

	if(DRAGMODIFY2DSIDE & BBOX2::eBoxSideInfo::BSI_TOP) {
		t1[d2] = DRAGMODIFYBOX.Min[d2];
		sc[d2] = k[d2] > 0 ? k[d2] : mins/bsize[d2];
		shiftaxis[d1][d2] = delta[d1]/bsize[d2];
		t2[d2] = -DRAGMODIFYBOX.Min[d2];
	}
	if(DRAGMODIFY2DSIDE & BBOX2::eBoxSideInfo::BSI_RIGHT) {
		t1[d1] = DRAGMODIFYBOX.Min[d1];
		sc[d1] = k[d1] > 0 ? k[d1] : mins/bsize[d1];
		shiftaxis[d2][d1] = delta[d2]/bsize[d1];
		t2[d1] = -DRAGMODIFYBOX.Min[d1];
	}
	if(DRAGMODIFY2DSIDE & BBOX2::eBoxSideInfo::BSI_BOTTOM) {
		t1[d2] = DRAGMODIFYBOX.Max[d2];
		sc[d2] = k2[d2] > 0 ? k2[d2] : mins/bsize[d2];
		shiftaxis[d1][d2] = -delta[d1]/bsize[d2];
		t2[d2] = -DRAGMODIFYBOX.Max[d2];
	}
	if(DRAGMODIFY2DSIDE & BBOX2::eBoxSideInfo::BSI_LEFT) {
		t1[d1] = DRAGMODIFYBOX.Max[d1];
		sc[d1] = k2[d1] > 0 ? k2[d1] : mins/bsize[d1];
		shiftaxis[d2][d1] = -delta[d2]/bsize[d1];
		t2[d1] = -DRAGMODIFYBOX.Max[d1];
	}

	DRAGMODIFYMATRIX.LoadTranslation(t1);
	// disable translation if has "double shift" mode
	if(!(DRAGMODIFY2DSIDE &~
			(BBOX2::eBoxSideInfo::BSI_TOP|BBOX2::eBoxSideInfo::BSI_BOTTOM)) ||
		!(DRAGMODIFY2DSIDE &~
			(BBOX2::eBoxSideInfo::BSI_LEFT|BBOX2::eBoxSideInfo::BSI_RIGHT)))
		DRAGMODIFYMATRIX.Scale(sc);
	DRAGMODIFYMATRIX.Shift(shiftaxis[0], shiftaxis[1], shiftaxis[2]);
	DRAGMODIFYMATRIX.Translate(t2);
}

void CEditWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(m_WindowInfo.eType == VT_3D && CEditWnd::EDITINGSTATUS == ES_VIEWMOVE)
	{
		ASSERT(Camera::pCurrent);
		switch(nChar)
		{
		case 'W': Camera::pCurrent->Move(-10); break;
		case 'S': Camera::pCurrent->Move(10); break;
		case 'A': Camera::pCurrent->Strafe(-10); break;
		case 'D': Camera::pCurrent->Strafe(10); break;
		case VK_SPACE:	Camera::pCurrent->m_vOrigin[1] += 5; break;
		}
		CEditWnd::CUBICCLIPPER.Update(Camera::pCurrent->m_vOrigin);
		RedrawWindow();
	}
	else
		switch(nChar)
		{
		case VK_HOME:
			if(!BRUSH::SELECTEDBRUSHLIST.empty())
			{
				// MULTIPLY CLIP BRUSH:
				if(CEditWnd::MODIFYMODE == MM_CLIPPING &&
					CEditWnd::CLIPPINGSTATE == CS_COMPLETECLIPPLANEDEF)
				{
					//UNDOINFO undoclip(
					//	BRUSH::SELECTEDBRUSHLIST.size(),
					//	BRUSH::SELECTEDBRUSHLIST.size()*2,
					//	UNDOINFO::OT_CLIPPING);
					//BRUSH::brushlist clipblist(BRUSH::SELECTEDBRUSHLIST.size()*2);

					//for(BRUSH::ptrbrushlist::const_iterator pb = BRUSH::SELECTEDBRUSHLIST.begin();
					//	pb != BRUSH::SELECTEDBRUSHLIST.end();
					//	++pb)
					//{
					//	BRUSH b1, b2;
					//	if(::SplitBrush(**pb,
					//		vClipPlanePoint[0], vClipPlanePoint[1], vClipPlanePoint[2], b1, b2))
					//	{
					//		// assign new ID & push in cliplist for new clipped brushes
					//		b1.UpdateID();
					//		b2.UpdateID();
					//		clipblist.push_back(b1);
					//		clipblist.push_back(b2);

					//		// undo/redo
					//		if(!bDisableUndoRedo)
					//		{
					//			undoclip.blist1.push_back(**pb);	// save source clipped brushes in undo info
					//			undoclip.blist2.push_back(b1);		// save new clipped brushes in undo info
					//			undoclip.blist2.push_back(b2);		// save new clipped brushes in undo info
					//		}

					//		(**pb).flags |= BRUSH::BF_DELETED;	// source brush mark as deleted
					//	}
					//}

					//// insert new clipped brushes
					//BRUSH::WORLDBRUSHLIST.insert(
					//	BRUSH::WORLDBRUSHLIST.end(), clipblist.begin(), clipblist.end());

					//if(!bDisableUndoRedo)
					//{	// push undo info in undo/redo-stack
					//	GLOBALUNDOSTACK.push(undoclip);
					//	// + update undo/redo labels in main menu
					//	::UpdateUndoRedoMenu();
					//}
				
					//// reset clipping state
					//CEditWnd::CLIPPINGSTATE = CS_FIRSTCLIPPLANEDEF;
				}
				else	// MULTIPLY SELECTION:
					for(BRUSH::ptrbrushlist::const_iterator pb = BRUSH::SELECTEDBRUSHLIST.begin();
						pb != BRUSH::SELECTEDBRUSHLIST.end();
						++pb)
					{
						// get selection sum
						BRUSH::GroupSelectionInWorldBrushList((*pb)->bbox,
							m_WindowInfo.eMode, PREFERENCES::Current.sGrid.Major.nStep);
						// mark selector as deleted
						(**pb).flags |= BRUSH::BF_DELETED;
					}

				// delete clipped brushes/selectors
				BRUSH::WORLDBRUSHLIST.erase(
					std::remove_if(BRUSH::WORLDBRUSHLIST.begin(), BRUSH::WORLDBRUSHLIST.end(), BRUSH::IsDeleted),
					BRUSH::WORLDBRUSHLIST.end()
					);

				// reinit selection list
				BRUSH::ReInitSelectedBrushList();

				// final redraw
				RedrawWindow(WU_ALLEDIT);
			}
			break;
		// shift texture...
		case VK_LEFT:
		case VK_UP:
		case VK_RIGHT:
		case VK_DOWN:
			CEditWnd::OnKeyTextureMove(nChar, UNDOINFO::OT_TEXTURE_SHIFT);
			break;
//		case 'C':	// *** GO TO CLIP BRUSH MODE ***
//			if(!BRUSH::SELECTEDBRUSHLIST.empty())
//			{
//				if(CEditWnd::MODIFYMODE != MM_CLIPPING)					// toggle mode
//				{
//					CEditWnd::MODIFYMODE = MM_CLIPPING;
//					CEditWnd::CLIPPINGSTATE = CS_FIRSTCLIPPLANEDEF;
//				}
//				else
//					CEditWnd::MODIFYMODE = MM_TRANSLATE;
//				//RedrawWindow(WU_ALLEDIT);		// :NOTE: nothing redraw!
//			}
//			break;
		}
	
	COpenGLWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CEditWnd::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{	// ALT + <any key>
	if(nChar != VK_MENU)
		switch(nChar)
		{
		// scale texture
		case VK_UP:
		case VK_DOWN:
		case VK_RIGHT:
		case VK_LEFT:
			CEditWnd::OnKeyTextureMove(nChar, UNDOINFO::OT_TEXTURE_SCALE);
			break;
		// rotate texture
		case VK_PRIOR:
		case VK_NEXT:
			CEditWnd::OnKeyTextureMove(nChar, UNDOINFO::OT_TEXTURE_ROTATE);
			break;
		}

	COpenGLWnd::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

void CEditWnd::OnKeyTextureMove(UINT nKey, UNDOINFO::eOpType op)
{
	UNDOINFO undo;
	VECTOR2 shift(VECTOR2::O), scale(VECTOR2::O);
	float rot = 0;

	switch (op)
	{
	case UNDOINFO::OT_TEXTURE_SHIFT:
		switch (nKey)
		{
		case VK_LEFT:
			shift(-(FLOAT)PREFERENCES::Current.sGrid.Major.nStep, 0);
			break;
		case VK_UP:
			shift(0, (FLOAT)PREFERENCES::Current.sGrid.Major.nStep);
			break;
		case VK_RIGHT:
			shift((FLOAT)PREFERENCES::Current.sGrid.Major.nStep, 0);
			break;
		case VK_DOWN:
			shift(0, -(FLOAT)PREFERENCES::Current.sGrid.Major.nStep);
			break;
		}
		break;
	case UNDOINFO::OT_TEXTURE_SCALE:
		switch (nKey)
		{
		case VK_LEFT:
			scale(-0.1F, 0);
			break;
		case VK_UP:
			scale(0, 0.1F);
			break;
		case VK_RIGHT:
			scale(0.1F, 0);
			break;
		case VK_DOWN:
			scale(0, -0.1F);
			break;
		}
		break;
	case UNDOINFO::OT_TEXTURE_ROTATE:
		switch (nKey)
		{
		case VK_PRIOR:
			rot = 15.F;
			break;
		case VK_NEXT:
			rot = -15.F;
			break;
		}
		break;
	}

	if(!BRUSH::SELECTEDBRUSHLIST.empty())
	{
		// save undo start frame
		if(!bDisableUndoRedo)
			undo.StartFrame(BRUSH::SELECTEDBRUSHLIST, op);
		// move texture on selected brushes
		BRUSH::MoveTextureCoordsInSelectionBrushList(shift, scale, rot);
		// record undo info
		if(!bDisableUndoRedo)
		{	// save undo end frame
			undo.EndFrame(BRUSH::SELECTEDBRUSHLIST);
			// push undoinfo in undo-stack
			GLOBALUNDOSTACK.push(undo);
			// update undo/redo labels in main menu
			::UpdateUndoRedoMenu();
		}
		// update "Face Inspector" dialog (if visible)
		if (FACEINSPECTORDIALOG.IsWindowVisible())
			FACEINSPECTORDIALOG.SetTexMod(&BRUSH::SELECTEDBRUSHLIST[0]->flist[0]);
		// and redraw all!
		RedrawWindow(WU_ALLEDIT);
	}
	else if(FACE::SELECTED.pFace)
	{
		// save undo start frame
		if(!bDisableUndoRedo)
			undo.StartFrame(*FACE::SELECTED.pFaceOwner, op);
		// move texture on selection face
		FACE::SELECTED.pFace->MoveTextureCoords(
			FACE::SELECTED.pFaceOwner->vlist, shift, scale, rot);
		// record undo info
		if(!bDisableUndoRedo)
		{	// save undo end frame
			undo.EndFrame(*FACE::SELECTED.pFaceOwner);
			// push undoinfo in undo-stack
			GLOBALUNDOSTACK.push(undo);
			// update undo/redo labels in main menu
			::UpdateUndoRedoMenu();
		}
		// update "Face Inspector" dialog (if visible)
		if (FACEINSPECTORDIALOG.IsWindowVisible())
			FACEINSPECTORDIALOG.SetTexMod(FACE::SELECTED.pFace);
		// and redraw all!
		RedrawWindow(WU_ALLEDIT);
	}
}

void CEditWnd::ScreenToWorld(const CPoint& ptScreen, VECTOR2& vecWorld, bool snaptogrid) const
{
	vecWorld.X = m_vOrigin.X + (ptScreen.x - (m_iWidth>>1)) / m_fScale;
	vecWorld.Y = m_vOrigin.Y - (ptScreen.y - (m_iHeight>>1)) / m_fScale;
	if(snaptogrid)														// need floor?
		vecWorld.Floor(PREFERENCES::Current.sGrid.Major.nStep);
	if(m_WindowInfo.eMode == VM_BACK || m_WindowInfo.eMode == VM_RIGHT)
		vecWorld.X = -vecWorld.X;			// invert x/z-axis
	else if(m_WindowInfo.eMode == VM_TOP)
		vecWorld.Y = -vecWorld.Y;			// invert z-axis
}

void CEditWnd::ScreenToWorld(const CPoint& ptScreen, VECTOR3& vecWorld, bool snaptogrid) const
{	// NOTE: неправильно -- m_vOrigin[m_WindowInfo.sDims.D1] -> m_vOrigin.X !!! !!! !!!
	vecWorld[m_WindowInfo.sDims.D1] = m_vOrigin[m_WindowInfo.sDims.D1] +
										(ptScreen.x - (m_iWidth>>1)) / m_fScale;
	vecWorld[m_WindowInfo.sDims.D2] = m_vOrigin[m_WindowInfo.sDims.D2] -
										(ptScreen.y - (m_iHeight>>1)) / m_fScale;
	vecWorld[3 - m_WindowInfo.sDims.D1 - m_WindowInfo.sDims.D2] = 0;
	if(snaptogrid)
	{
		vecWorld[m_WindowInfo.sDims.D1] = ::floor(vecWorld[m_WindowInfo.sDims.D1] /
											PREFERENCES::Current.sGrid.Major.nStep + 0.5F) *
												PREFERENCES::Current.sGrid.Major.nStep;
		vecWorld[m_WindowInfo.sDims.D2] = ::floor(vecWorld[m_WindowInfo.sDims.D2] /
											PREFERENCES::Current.sGrid.Major.nStep + 0.5F) *
												PREFERENCES::Current.sGrid.Major.nStep;
	}

	if(m_WindowInfo.eMode == VM_BACK)
		vecWorld.X = -vecWorld.X;			// invert x-axis
	else if(m_WindowInfo.eMode == VM_TOP || m_WindowInfo.eMode == VM_RIGHT)
		vecWorld.Z = -vecWorld.Z;			// invert z-axis
}

void CEditWnd::RedrawWindow(WINDOWUPDATE wu)
{
	for(unsigned w = 0; w < PREFERENCES::Current.nNumEditWindow; ++w)
	{
		ASSERT(CEditWnd::pEditWindow[w]);
		if(CEditWnd::pEditWindow[w]->m_WindowInfo.eUpdate & wu)
			CEditWnd::pEditWindow[w]->COpenGLWnd::RedrawWindow();
	}
}

void CEditWnd::ResetTextureScrollPos(bool bUpdateWindow)
{
	for(unsigned w = 0; w < PREFERENCES::Current.nNumEditWindow; ++w)
	{
		ASSERT(CEditWnd::pEditWindow[w]);
		if(CEditWnd::pEditWindow[w]->m_WindowInfo.eType == VT_TEXTURE)
		{
			CEditWnd::pEditWindow[w]->m_iTextureScrollPos = 0;
			CEditWnd::pEditWindow[w]->SetScrollPos(SB_VERT, 0);
			if(bUpdateWindow)
			{
				CEditWnd::pEditWindow[w]->Invalidate();
				CEditWnd::pEditWindow[w]->UpdateWindow();
			}
		}
	}
}

void CEditWnd::ClearSelection()
{
	if(!BRUSH::SELECTEDBRUSHLIST.empty())
	{
		// clear brush selection
		BRUSH::ClearSelectedBrushList();
		// clear selected vertices (for vertex-drag modify mode)
		BRUSH::SELECTEDVERTEXLIST.clear();
		// reset modify mode
		CEditWnd::MODIFYMODE = MM_TRANSLATE;
		// reset clipping state
		CEditWnd::CLIPPINGSTATE = CS_FIRSTCLIPPLANEDEF;
		CEditWnd::RedrawWindow(WU_ALLEDIT);
	}

	if(FACE::SELECTED.pFace)
	{
		// clear face selection
		FACE::SELECTED.pFace = 0;
		FACE::SELECTED.pFaceOwner = 0;
		CEditWnd::RedrawWindow(WU_ALLEDIT);
	}

	// reset status tab selection info
	CFrameWnd *pMainFrame = (CFrameWnd *)::AfxGetMainWnd();
	ASSERT(pMainFrame);
	CStatusBar *pStatusBar = (CStatusBar *)pMainFrame->GetMessageBar();
	ASSERT(pStatusBar);
	pStatusBar->SetPaneText(3, "");				// selection size
	pStatusBar->SetPaneText(1, "no selection");	// selection information

	// reset editing status
	CEditWnd::EDITINGSTATUS = ES_SELECTION;
}

void CEditWnd::DeleteSelection()
{
	if(!BRUSH::SELECTEDBRUSHLIST.empty())
	{
		// record undo info
		if(!bDisableUndoRedo)
		{
			UNDOINFO undo;
			// save undo start frame
			undo.StartFrame(BRUSH::SELECTEDBRUSHLIST, UNDOINFO::OT_DELETE);
			// push undoinfo in undo-stack
			GLOBALUNDOSTACK.push(undo);
			// update undo/redo labels in main menu
			::UpdateUndoRedoMenu();
		}
		// delete selected brushes
		BRUSH::DeleteSelectedBrushList();
		// reset editing status
		CEditWnd::EDITINGSTATUS = ES_SELECTION;
		// and redraw all!
		RedrawWindow(WU_ALLEDIT);
	}
}

void CEditWnd::Carve()
{
	if(BRUSH::SELECTEDBRUSHLIST.size() == 1)
	{
		BRUSH::brushlist cliplist(6), alllist(16);
		const BRUSH clipper(*BRUSH::SELECTEDBRUSHLIST[0]);
		const size_t old = BRUSH::WORLDBRUSHLIST.size();
		UNDOINFO undo(UNDOINFO::OT_CARVE, 4, 0);

		// start carve
		for(BRUSH::brushlist::iterator b = BRUSH::WORLDBRUSHLIST.begin();
			b != BRUSH::WORLDBRUSHLIST.end();
			)
		{
			if(!(b->flags & BRUSH::BF_SELECTED) && ::CarveBrush(clipper, *b, cliplist))
			{
				// record undo info
				if(!CEditWnd::bDisableUndoRedo)
					undo.StartFrame(*b);
				// erase carved brush
				b = BRUSH::WORLDBRUSHLIST.erase(b);
				// insert new clipped brushes
				alllist.insert(alllist.end(), cliplist.begin(), cliplist.end());
				// erase clipped brushes list
				cliplist.resize(0);
			}
			else
				++b;
		}

		if (old != BRUSH::WORLDBRUSHLIST.size())
		{
			// insert carved brushes in map
			BRUSH::WORLDBRUSHLIST.insert(BRUSH::WORLDBRUSHLIST.end(),
				alllist.begin(), alllist.end());
			// record undo info
			if (!CEditWnd::bDisableUndoRedo)
			{	// save end undo frame
				undo.EndFrame(alllist);
				// push undo info in stack
				GLOBALUNDOSTACK.push(undo);
				// update undo/redo labels in main menu
				::UpdateUndoRedoMenu();
			}
			// reinit selection list
			BRUSH::ReInitSelectedBrushList();
			// and redraw all!
			CEditWnd::RedrawWindow(WU_ALLEDIT);
		}
	}
}

void CEditWnd::Hollow()
{
	if(!BRUSH::SELECTEDBRUSHLIST.empty())
	{
		if(BRUSH::SELECTEDBRUSHLIST.size() > 1 &&
			::AfxMessageBox(
				"Do you want to turn each of the selected solids into a hollow room?",
				MB_ICONWARNING | MB_YESNO) == IDNO
			)
			return;


		BRUSH tmp;
		MATRIX16 m;
		VECTOR3 wall(PREFERENCES::Current.sGrid.Major.nStep * 2.F),
				bcenter,
				bsize;
		BRUSH::brushlist walllist(6),
						 alllist(BRUSH::SELECTEDBRUSHLIST.size() * 6);
		for(BRUSH::ptrbrushlist::const_iterator sb = BRUSH::SELECTEDBRUSHLIST.begin();
			sb != BRUSH::SELECTEDBRUSHLIST.end();
			++sb)
		{
			// prepare hollow scale matrix
			bcenter = (**sb).bbox.GetCenter(), bsize = (**sb).bbox.Size();
			m.LoadTranslation(bcenter);
			m.Scale((bsize + wall) / bsize);
			m.Translate(-bcenter);

			// make hollow (scale & carve)
			tmp = **sb;
			tmp.Translate(m, false);
			if(::CarveBrush(**sb, tmp, walllist))	// TODO:: make walllist as group?
				alllist.insert(alllist.end(), walllist.begin(), walllist.end());
		}

		// record undo info
		if(!CEditWnd::bDisableUndoRedo)
		{	// save undo info
			UNDOINFO undo(UNDOINFO::OT_HOLLOW, BRUSH::SELECTEDBRUSHLIST, alllist);
			// push undo info in stack
			GLOBALUNDOSTACK.push(undo);
			// update undo/redo labels in main menu
			::UpdateUndoRedoMenu();
		}

		// delete selected in world-brush list
		BRUSH::WORLDBRUSHLIST.erase(
			std::remove_if(BRUSH::WORLDBRUSHLIST.begin(), BRUSH::WORLDBRUSHLIST.end(), BRUSH::IsSelected),
			BRUSH::WORLDBRUSHLIST.end()
			);
		// insert new walls in world-brush list
		BRUSH::WORLDBRUSHLIST.insert(BRUSH::WORLDBRUSHLIST.end(),
			alllist.begin(), alllist.end());
		// reinit selection list
		BRUSH::ReInitSelectedBrushList();
		// final redraw
		RedrawWindow(WU_ALLEDIT);
	}
}

void CEditWnd::Union()
{
	if(BRUSH::SELECTEDBRUSHLIST.size() == 2)
	{
		BRUSH dst;
		if(::UnionBrush(*BRUSH::SELECTEDBRUSHLIST[0], *BRUSH::SELECTEDBRUSHLIST[1], dst))
		{	// record undo info
			if(!CEditWnd::bDisableUndoRedo)
			{	// save for undo/redo
				UNDOINFO undo(UNDOINFO::OT_UNION, BRUSH::SELECTEDBRUSHLIST, dst);
				// push in undo stack
				GLOBALUNDOSTACK.push(undo);
				// update undo/redo labels in main menu
				::UpdateUndoRedoMenu();
			}

			// delete selection brushes
			BRUSH::DeleteSelectedBrushList();
			// add new unioning brush
			BRUSH::WORLDBRUSHLIST.push_back(dst);
			// update selection brush list
			BRUSH::SELECTEDBRUSHLIST.push_back(&BRUSH::WORLDBRUSHLIST.back());
			// and redraw all!
			RedrawWindow(WU_ALLEDIT);
		}
	}
	else if(BRUSH::SELECTEDBRUSHLIST.size() > 2)
	{
		BRUSH::brushlist selblist(BRUSH::SELECTEDBRUSHLIST.size());
		for(BRUSH::ptrbrushlist::const_iterator i = BRUSH::SELECTEDBRUSHLIST.begin();
			i != BRUSH::SELECTEDBRUSHLIST.end();
			++i)
			selblist.push_back(**i);

		for(BRUSH::brushlist::iterator sb1 = selblist.begin();
			sb1 != selblist.end();
			++sb1)
		{
			for(BRUSH::brushlist::iterator sb2 = selblist.begin();
				sb2 != selblist.end();
				++sb2)
			{
				if(sb1 != sb2)
				{
					BRUSH dst;
					if(::UnionBrush(*sb1, *sb2, dst))
					{
						sb1->flags |= BRUSH::BF_DELETED;
						sb2->flags |= BRUSH::BF_DELETED;
						selblist.erase(std::remove_if(
							selblist.begin(), selblist.end(), BRUSH::IsDeleted),
							selblist.end()
							);
						selblist.push_back(dst);
						sb1 = sb2 = selblist.begin();
					}
				}
			}
		}

		if(selblist.size() != BRUSH::SELECTEDBRUSHLIST.size())
		{	// record undo info
			if(!CEditWnd::bDisableUndoRedo)
			{	// save for undo/redo
				UNDOINFO undo(UNDOINFO::OT_UNION, BRUSH::SELECTEDBRUSHLIST, selblist);
				// push in undo stack
				GLOBALUNDOSTACK.push(undo);
				// update undo/redo labels in main menu
				::UpdateUndoRedoMenu();
			}

			// delete selected in world-brush list
			BRUSH::WORLDBRUSHLIST.erase(
				std::remove_if(BRUSH::WORLDBRUSHLIST.begin(), BRUSH::WORLDBRUSHLIST.end(), BRUSH::IsSelected),
				BRUSH::WORLDBRUSHLIST.end()
				);
			// insert new (merged) brushes
			BRUSH::WORLDBRUSHLIST.insert(
				BRUSH::WORLDBRUSHLIST.end(), selblist.begin(), selblist.end());
			// reinit selection list
			BRUSH::ReInitSelectedBrushList();
			// final redraw
			RedrawWindow(WU_ALLEDIT);
		}
	}
}

void CEditWnd::MorphingVertices()
{
	if(!BRUSH::SELECTEDBRUSHLIST.empty())
	{
		CEditWnd::MODIFYMODE = CEditWnd::MODIFYMODE == MM_VERTEXDRAG ?	// toggle mode
														MM_TRANSLATE : MM_VERTEXDRAG;
		RedrawWindow(WU_ALLEDIT);
	}
}

void CEditWnd::MorphingEdges()
{
	if(!BRUSH::SELECTEDBRUSHLIST.empty())
	{
		CEditWnd::MODIFYMODE = CEditWnd::MODIFYMODE == MM_EDGEDRAG ?	// toggle mode
													  MM_TRANSLATE : MM_EDGEDRAG;
		RedrawWindow(WU_ALLEDIT);
	}
}

void CEditWnd::CloneSelection()
{
	if(!BRUSH::SELECTEDBRUSHLIST.empty() &&
		CEditWnd::pActiveWindow &&
		CEditWnd::pActiveWindow->m_WindowInfo.eType == VT_2D)
	{
		// clone selection
		BRUSH::CloneSelectedBrushList(
			VECTOR3(VECTOR2((FLOAT)PREFERENCES::Current.sGrid.Major.nStep),
			CEditWnd::pActiveWindow->m_WindowInfo.eMode),
			1,
			PREFERENCES::Current.bTextureLock
			);
		// record undo info
		if(!bDisableUndoRedo)
		{
			UNDOINFO undo(UNDOINFO::OT_CLONE);
			// save end frame only!
			undo.EndFrame(BRUSH::SELECTEDBRUSHLIST);
			// push in undo stack
			GLOBALUNDOSTACK.push(undo);
			// update undo/redo labels in main menu
			::UpdateUndoRedoMenu();
		}
		// and redraw all!
		RedrawWindow(WU_ALLEDIT);
	}
}

void CEditWnd::ToggleClip()
{
	if (!BRUSH::SELECTEDBRUSHLIST.empty())
	{
		if(CEditWnd::MODIFYMODE != MM_CLIPPING)							// toggle mode
		{
			CEditWnd::MODIFYMODE = MM_CLIPPING;
			CEditWnd::CLIPPINGSTATE = CS_FIRSTCLIPPLANEDEF;
		}
		else if (CEditWnd::CLIPPINGSTATE == CS_COMPLETECLIPPLANEDEF)
		{
			UNDOINFO undoclip(UNDOINFO::OT_CLIPPING,
				BRUSH::SELECTEDBRUSHLIST.size(),
				BRUSH::SELECTEDBRUSHLIST.size()*2
				);
			BRUSH::brushlist clipblist(BRUSH::SELECTEDBRUSHLIST.size()*2);
			
			// for all selected brushes
			for(BRUSH::ptrbrushlist::const_iterator pb = BRUSH::SELECTEDBRUSHLIST.begin();
				pb != BRUSH::SELECTEDBRUSHLIST.end();
				++pb)
			{
				BRUSH b1, b2;
				// if split source brush
				if(::SplitBrush(**pb, vClipPlanePoint[0], vClipPlanePoint[1], vClipPlanePoint[2], b1, b2))
				{
					// assign new ID for clipped
					b1.UpdateID();
					b2.UpdateID();
					// push in cliplist for clipped
					clipblist.push_back(b1);
					clipblist.push_back(b2);
					// record undo info
					if(!bDisableUndoRedo)
						// push: (source, clipped1, clipped2)
						undoclip.Push(**pb, b1, b2);
					// mark source brush as deleted
					(**pb).flags |= BRUSH::BF_DELETED;
				}
			}

			// insert new clipped brushes
			BRUSH::WORLDBRUSHLIST.insert(
				BRUSH::WORLDBRUSHLIST.end(), clipblist.begin(), clipblist.end());
			// record undo info
			if(!bDisableUndoRedo)
			{	// push undo info in undo stack
				GLOBALUNDOSTACK.push(undoclip);
				// update undo/redo labels in main menu
				::UpdateUndoRedoMenu();
			}
			// reset clipping state (NOTE: call before CEditWnd::RedrawWindow()!)
			CEditWnd::CLIPPINGSTATE = CS_FIRSTCLIPPLANEDEF;
			// reset modify mode
			CEditWnd::MODIFYMODE = MM_TRANSLATE;
			// delete clipped brushes/selectors
			BRUSH::WORLDBRUSHLIST.erase(
				std::remove_if(BRUSH::WORLDBRUSHLIST.begin(), BRUSH::WORLDBRUSHLIST.end(), BRUSH::IsDeleted),
				BRUSH::WORLDBRUSHLIST.end()
				);
			// reinit selection list
			BRUSH::ReInitSelectedBrushList();
			// final redraw
			RedrawWindow(WU_ALLEDIT);
		}
	}
}

void CEditWnd::MultiplySelection()
{
	if(BRUSH::SELECTEDBRUSHLIST.empty())
		return;

	// MULTIPLY SELECTION:
	for(BRUSH::ptrbrushlist::const_iterator pb = BRUSH::SELECTEDBRUSHLIST.begin();
		pb != BRUSH::SELECTEDBRUSHLIST.end();
		++pb)
	{
		// get selection sum
		BRUSH::GroupSelectionInWorldBrushList((*pb)->bbox,
			m_WindowInfo.eMode, PREFERENCES::Current.sGrid.Major.nStep);
		// mark selector as deleted
		(**pb).flags |= BRUSH::BF_DELETED;
	}

	// delete clipped brushes/selectors
	BRUSH::WORLDBRUSHLIST.erase(
		std::remove_if(BRUSH::WORLDBRUSHLIST.begin(), BRUSH::WORLDBRUSHLIST.end(), BRUSH::IsDeleted),
		BRUSH::WORLDBRUSHLIST.end()
		);

	// reinit selection list
	BRUSH::ReInitSelectedBrushList();

	// final redraw
	RedrawWindow(WU_ALLEDIT);
}

void CEditWnd::Zoom(bool bZoomIn, bool bReset)
{
	if (m_WindowInfo.eType == VT_2D)
	{
		if (bReset)
			m_fScale = 1.0F;
		else
		{
			if (bZoomIn)
				m_fScale += 0.1F;
			else
				m_fScale -= 0.1F;

			if(m_fScale < 0.1F)
				m_fScale = 0.1F;
			if(m_fScale > 16.F)
				m_fScale = 16.F;
		}

		// redraw current window
		COpenGLWnd::RedrawWindow();
	}
}

void CEditWnd::EnableView(VIEWTYPE vt, bool bEnable)
{
	if (m_WindowInfo.bDisable == bEnable &&
		(m_WindowInfo.eType == vt || vt == VT_TOTAL))
	{
		m_WindowInfo.bDisable = !bEnable;
		m_WindowInfo.bDisable ?
			m_WindowInfo.sLabel.uLabelSize += 9 :
			m_WindowInfo.sLabel.uLabelSize -= 9;

		// redraw current window
		COpenGLWnd::RedrawWindow();
	}
}

void CEditWnd::SetDrawMode(DRAWMODE dm)
{
	for (unsigned w = 0; w < PREFERENCES::Current.nNumEditWindow; ++w)
	{
		ASSERT(CEditWnd::pEditWindow[w]);
		if (CEditWnd::pEditWindow[w]->m_WindowInfo.eType == VT_2D ||
			CEditWnd::pEditWindow[w]->m_WindowInfo.eType == VT_3D)
		{
			CEditWnd::pEditWindow[w]->m_WindowInfo.Set(dm);
			CEditWnd::pEditWindow[w]->COpenGLWnd::RedrawWindow();
		}
	}
}

void CEditWnd::CenteredView(const BRUSH& brush)
{
	const VECTOR3 center(brush.bbox.GetCenter());

	for (unsigned w = 0; w < PREFERENCES::Current.nNumEditWindow; ++w)
	{
		ASSERT(CEditWnd::pEditWindow[w]);
		if (CEditWnd::pEditWindow[w]->m_WindowInfo.eType == VT_2D)// for ortho view only
		{
			// centered ortho camera on brush
			CEditWnd::pEditWindow[w]->m_vOrigin(
				center[CEditWnd::pEditWindow[w]->m_WindowInfo.sDims.D1],
				center[CEditWnd::pEditWindow[w]->m_WindowInfo.sDims.D2]
			);
			// correct axis sign
			if(CEditWnd::pEditWindow[w]->m_WindowInfo.eMode == VM_BACK ||
				CEditWnd::pEditWindow[w]->m_WindowInfo.eMode == VM_RIGHT)
				CEditWnd::pEditWindow[w]->m_vOrigin.X =
					-CEditWnd::pEditWindow[w]->m_vOrigin.X;			// invert x/z-axis
			else if(CEditWnd::pEditWindow[w]->m_WindowInfo.eMode == VM_TOP)
				CEditWnd::pEditWindow[w]->m_vOrigin.Y = 
					-CEditWnd::pEditWindow[w]->m_vOrigin.Y;			// invert z-axis
			// redraw
			CEditWnd::pEditWindow[w]->COpenGLWnd::RedrawWindow();
		}
	}
}

const CEditWnd::eModifyMode CEditWnd::GetModifyMode()
{
	return CEditWnd::MODIFYMODE;
}


void CEditWnd::UpdateStatusBarSelectionInfo() const
{
	// get status bar pointer
	CStatusBar *pStatusBar = (CStatusBar*)CWnd::GetParentFrame()->GetMessageBar();
	ASSERT(pStatusBar);
	// set label
	VECTOR3 selsize;
	const unsigned numsel = BRUSH::GetSelectionSize(selsize);
	CString str, ename;
	str.Format("L: %.0f H: %.0f W: %.0f", selsize.X, selsize.Y, selsize.Z);
	switch (numsel)
	{
	case 0:
		pStatusBar->SetPaneText(3, "");				// selection size
		pStatusBar->SetPaneText(1, "no selection");	// selection information
		break;
	case 1:
		// set selection size
		pStatusBar->SetPaneText(3, str);
		// if it is entity
		if (BRUSH::SELECTEDBRUSHLIST[0]->owner)
		{
			// has a targetname
			if (BRUSH::SELECTEDBRUSHLIST[0]->owner->Lookup("targetname", ename) &&
				!ename.IsEmpty())
			{
				str.Format("%s - %s", ename, BRUSH::SELECTEDBRUSHLIST[0]->owner->name);
				pStatusBar->SetPaneText(1, str);				// selection information
			}
			else
				pStatusBar->SetPaneText(1, BRUSH::SELECTEDBRUSHLIST[0]->owner->name);// selection information
		}
		else
			pStatusBar->SetPaneText(1, "1 object selected");	// selection information

		break;
	default:
		pStatusBar->SetPaneText(3, str);		// selection size
		str.Format("%u objects selected", numsel);
		pStatusBar->SetPaneText(1, str);		// selection information
		break;
	}
}






//
//	UPDATE MAIN FRAME`S UNDO/REDO MENU:
//
void UpdateUndoRedoMenu(bool bClearSelection)
{
	if(CWnd* pMain = ::AfxGetMainWnd())
		if(CMenu* pMainMenu = pMain->GetMenu())
			if(CMenu* pEditMenu = pMainMenu->GetSubMenu(1))
			{
				// update undo menu info label
				if(const UNDOINFO* pInfo = CEditWnd::GLOBALUNDOSTACK.undoinfo())
				{
					//pEditMenu->ModifyMenu(
					//	0, MF_BYPOSITION|MF_ENABLED|MF_STRING, ID_EDIT_UNDO, UNDOINFO::UNDOLABELS[pInfo->optype]
					//	);
				}
				else
				{
					//pEditMenu->ModifyMenu(
					//	0, MF_BYPOSITION|MF_STRING, ID_EDIT_UNDO, UNDOINFO::UNDOLABELS[0]
					//	);

					if(bClearSelection)
					{
						BRUSH::ClearSelectedBrushList();
					//	pEditMenu->EnableMenuItem(0, MF_BYPOSITION|MF_GRAYED);	// gray "Undo" menu label
					}
				}

				//// update redo menu info label
				//if(const UNDOINFO* pInfo = CEditWnd::GLOBALUNDOSTACK.redoinfo())
				//	pEditMenu->ModifyMenu(
				//		1, MF_BYPOSITION|MF_ENABLED|MF_STRING, ID_EDIT_REDO, UNDOINFO::REDOLABELS[pInfo->optype]
				//		);
				//else
				//	pEditMenu->ModifyMenu(
				//		1, MF_BYPOSITION|MF_GRAYED|MF_STRING, ID_EDIT_REDO, UNDOINFO::REDOLABELS[0]
				//		);
			}
}