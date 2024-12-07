// MaxEdDoc.cpp : implementation of the CMaxEdDoc class
//

#include "stdafx.h"
#include "MaxEd.h"

#include "MaxEdDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMaxEdDoc

IMPLEMENT_DYNCREATE(CMaxEdDoc, CDocument)

BEGIN_MESSAGE_MAP(CMaxEdDoc, CDocument)
END_MESSAGE_MAP()


// CMaxEdDoc construction/destruction

CMaxEdDoc::CMaxEdDoc()
{
	// TODO: add one-time construction code here

}

CMaxEdDoc::~CMaxEdDoc()
{
}

BOOL CMaxEdDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CMaxEdDoc serialization

void CMaxEdDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CMaxEdDoc diagnostics

#ifdef _DEBUG
void CMaxEdDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMaxEdDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMaxEdDoc commands
