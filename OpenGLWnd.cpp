// OpenGLWnd.cpp : implementation file

#include "stdafx.h"
#include "MaxEd.h"
#include "OpenGLWnd.h"


IMPLEMENT_DYNCREATE(COpenGLWnd, CWnd)

BEGIN_MESSAGE_MAP(COpenGLWnd, CWnd)
	//{{AFX_MSG_MAP(COpenGLWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


COpenGLWnd::COpenGLWnd()
{
}

COpenGLWnd::~COpenGLWnd()
{
}

int COpenGLWnd::SetPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize		= sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion	= 1;
	pfd.iPixelType	= PFD_TYPE_RGBA;
	pfd.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.cColorBits		= 32;
	pfd.cRedBits		= 0;
	pfd.cRedShift		= 0;
	pfd.cGreenBits		= 0;
	pfd.cGreenShift		= 0; 
	pfd.cBlueBits		= 0;
	pfd.cBlueShift		= 0;
	pfd.cAlphaBits		= 0;
	pfd.cAlphaShift		= 0;
	pfd.cAccumBits		= 0;
	pfd.cAccumRedBits	= 0;
	pfd.cAccumGreenBits = 0;
	pfd.cAccumBlueBits	= 0;
	pfd.cAccumAlphaBits = 0;
	pfd.cDepthBits		= 32;
	pfd.cStencilBits	= 0;
	pfd.cAuxBuffers		= 0;
	pfd.iLayerType		= PFD_MAIN_PLANE;
	pfd.bReserved		= 0;
	pfd.dwLayerMask		= 0;
	pfd.dwVisibleMask	= 0;
	pfd.dwDamageMask	= 0;

	m_iChoosePFD = ::ChoosePixelFormat(hDC, &pfd);
	m_iCurrentPFD = ::SetPixelFormat(hDC, m_iChoosePFD, &pfd);
	return m_iCurrentPFD;
}

BOOL COpenGLWnd::PrepareOpenGLFont(DWORD first, DWORD count, DWORD listBase,
									int font, HFONT hFont)
{
	// GET DEVICE CONTEXT
	CClientDC dc(this);

	if (!dc)
	{
		TRACE0(	"GET DEVICE CONTEXT:\n"
				"COpenGLWnd::OnCreate() -> CreateOGLFont() -> wglGetCurrentDC(): failed\n"
				"there is no current device context");
		return 0;
	}

	// MAKE RENDRING CONTEXT CURRENT
	if (::wglMakeCurrent(dc, m_hGLRC) == FALSE)
	{
		TRACE0("MAKE RENDRING CONTEXT CURRENT:\n"
			   "COpenGLWnd::OnCreate -> create paint context -> ::wglMakeCurrent(HDC, HGLRC):\n"
			   "cannot make rendering context current\n");
		return 0;
	}

	// SELECT OBJECT FONT
	if(::SelectObject(dc, font ? ::GetStockObject(font) : hFont) == HGDI_ERROR)
	{
		TRACE0("COpenGLWnd::OnCreate() -> CreateOGLFont() -> SelectObject(HDC, HFONT): failed\n"
			   "cannot select system font in this device context");
		return 0;
	}

	// MAKE OGL BITMAP FONT
	if (!::wglUseFontBitmaps(dc, first, count, listBase))
	{
		TRACE0("COpenGLWnd::OnCreate() -> CreateOGLFont() -> wglUseFontBitmaps(): failed\n"
			   "cannot create bitmap font");
		return 0;
	}

	return 1;
}

BOOL COpenGLWnd::ShareFontLists(HGLRC hglrc) const
{
	BOOL result = ::wglShareLists(hglrc, m_hGLRC);
	ASSERT(result);
	return result;
}


// COpenGLWnd message handlers

int COpenGLWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	/*** SETUP OGL PIXEL FORMAT ***/
	CPaintDC dc(this);
	if (SetPixelFormat(dc) == 0)
	{
		TRACE0("SETUP OGL PIXEL FORMAT:\n"
			   "COpenGLWnd::OnCreate -> create paint context -> COpenGLWnd::SetPixelFormat(HDC):\n"
			   "cannot setup pixel format\n");
		return -1;
	}

	/*** CREATE OGL RENDRING CONTEXT ***/
	m_hGLRC = wglCreateContext(dc);
	if (m_hGLRC == 0)
	{
		TRACE0("CREATE OGL RENDRING CONTEXT:\n"
			   "COpenGLWnd::OnCreate -> create paint context -> ::wglCreateContext(HDC):\n"
			   "cannot create rendering context\n");
		return -1;
	}

	/*** MAKE RENDRING CONTEXT CURRENT ***/
	if (wglMakeCurrent(dc, m_hGLRC) == FALSE)
	{
		TRACE0("MAKE RENDRING CONTEXT CURRENT:\n"
			   "COpenGLWnd::OnCreate -> create paint context -> ::wglMakeCurrent(HDC, HGLRC):\n"
			   "cannot make rendering context current\n");
		return -1;
	}
	
	return 0;
}

void COpenGLWnd::OnDestroy() 
{
	// TODO: Add your message handler code here

	/*** DELETE FONTS ***/
	::glDeleteLists(VIEWLABEL::VL_BASELISTINDEX, 1);

	/*** MAKE RENDRING CONTEXT UNUSED ***/
	if (wglGetCurrentContext())
		wglMakeCurrent(0, 0);

	/*** DELETE RENDRING CONTEXT ***/
	if (m_hGLRC)
		wglDeleteContext(m_hGLRC), m_hGLRC = 0;

	CWnd::OnDestroy();	
}

void COpenGLWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	m_iWidth = cx, m_iHeight = cy;
}