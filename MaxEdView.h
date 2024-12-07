// MaxEdView.h : interface of the CMaxEdView class
//


#pragma once


class CMaxEdView : public CView
{
protected: // create from serialization only
	CMaxEdView();
	DECLARE_DYNCREATE(CMaxEdView)

// Attributes
public:
	CMaxEdDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CMaxEdView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MaxEdView.cpp
inline CMaxEdDoc* CMaxEdView::GetDocument() const
   { return reinterpret_cast<CMaxEdDoc*>(m_pDocument); }
#endif

