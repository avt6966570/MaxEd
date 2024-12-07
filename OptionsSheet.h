#pragma once
#include "GeneralPage.h"
#include "ViewportPage.h"
#include "GridPage.h"
#include "PathesPage.h"


// COptionsSheet

class COptionsSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(COptionsSheet)

public:
	COptionsSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	COptionsSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~COptionsSheet();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CGeneralPage  m_GeneralPage;
	CViewportPage m_ViewportPage;
	CGridPage	  m_GridPage;
	CPathesPage	  m_PathesPage;
};


