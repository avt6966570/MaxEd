// ExSplitterWnd.h : interface of the CExSplitterWnd class
#pragma once

class CExSplitterWnd : public CSplitterWnd
{
public:
	CExSplitterWnd() {}
	void ReInit()
	{
		DestroyWindow();
		delete[] m_pRowInfo;
		delete[] m_pColInfo;
		m_pRowInfo = m_pColInfo = 0;	// :NOTE: override Microsoft`s ASSERT
		m_nRows = m_nCols = 0;			// :NOTE: override Microsoft`s ASSERT
		CSplitterWnd::CSplitterWnd();
	}
};