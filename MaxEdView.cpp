// MaxEdView.cpp : implementation of the CMaxEdView class
//

#include "stdafx.h"
#include "MaxEd.h"

#include "MaxEdDoc.h"
#include "MaxEdView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMaxEdView

IMPLEMENT_DYNCREATE(CMaxEdView, CView)

BEGIN_MESSAGE_MAP(CMaxEdView, CView)
END_MESSAGE_MAP()

// CMaxEdView construction/destruction

CMaxEdView::CMaxEdView()
{
	// TODO: add construction code here

}

CMaxEdView::~CMaxEdView()
{
}

BOOL CMaxEdView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMaxEdView drawing

void CMaxEdView::OnDraw(CDC* /*pDC*/)
{
	CMaxEdDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CMaxEdView diagnostics

#ifdef _DEBUG
void CMaxEdView::AssertValid() const
{
	CView::AssertValid();
}

void CMaxEdView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMaxEdDoc* CMaxEdView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMaxEdDoc)));
	return (CMaxEdDoc*)m_pDocument;
}
#endif //_DEBUG


// CMaxEdView message handlers
