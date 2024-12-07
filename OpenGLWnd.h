// COpenGLWnd.h : interface of the COpenGLWnd class
// COpenGLWnd: - base class for OpenGL window functionality
//

#pragma once
#include "defs.h"

class COpenGLWnd : public CWnd
{
	DECLARE_DYNCREATE(COpenGLWnd)

public:
	COpenGLWnd();
	virtual ~COpenGLWnd();

// operations
public:
	// getting info
	HGLRC GetRenderingContext() const
		{ return m_hGLRC; }

	// ogl font
	BOOL PrepareOpenGLFont(DWORD first, DWORD count, DWORD listBase,
		int font, HFONT hFont = 0);
	BOOL ShareFontLists(HGLRC hglrc) const;

protected:
	int SetPixelFormat(HDC hDC);			// setup window pixel format
	int GetChoosePixelFormat() const
		{ return m_iChoosePFD; }
	int GetCurrentPixelFormat() const
		{ return m_iCurrentPFD; }

private:


// members
public:
protected:
	HGLRC	m_hGLRC;						// rendering context
	int		m_iWidth, m_iHeight;			// window area size width/height
	int		m_iChoosePFD, m_iCurrentPFD;	// choosing pixel format (choose/current)

private:


// messages
public:
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

private:
};