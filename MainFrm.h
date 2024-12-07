// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "ExSplitterWnd.h"
#include "EditWnd.h"
#include "PasteSpecialDialog.h"

class CMainFrame : public CFrameWnd
{
	enum eSplitter {
		NOT_USE = -1,
		SPLITTER0,
		SPLITTER1,
		SPLITTER2,
		SPLITTERS_TOTAL,
		FRAMEWND = SPLITTERS_TOTAL,						// parent - main frame
		EXCLUDEROWCOLTABLESIZE = SPLITTERS_TOTAL - 1	// 
	};

	struct SplitterInfo {
		int	Name, Parent, Row, Col,
			ParentsRow, ParentsCol;
	};

	static const SplitterInfo
		SplitterLayoutInfo[WL_TOTAL][SPLITTERS_TOTAL];	// splitter layout info

protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:
protected:
	BOOL InitWindowLayout(WINDOWLAYOUT newLayout, CCreateContext* pContext);
	void AlignWindowLayout(int cx, int cy);
private:
	int BuildRowColETable(int splitname, int extable[SPLITTERS_TOTAL-1][2]);
	BOOL CheckExcludedRowCol(int extable[SPLITTERS_TOTAL-1][2],
							 int deep, int row, int col);

	void FillTextureDirInMenu() const;

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpCreateStruct, CCreateContext* pContext);

// Implementation
public:
	virtual ~CMainFrame();

	/* DIALOGS */
	CPasteSpecialDialog PASTESPECIALDIALOG;


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar		m_wndStatusBar;
	CToolBar		m_wndToolBar;
	CExSplitterWnd	m_wndSplitter[SPLITTERS_TOTAL];

// Generated message map functions
protected:
	//{{AFX_MSG(CEditWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChangeShapeType(UINT nID);
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnEditDisableundoredo();
	afx_msg void OnTexturesShowAll();
	afx_msg void OnTexturesShowInUse();
	afx_msg void OnTexturesFaceinspector();
	afx_msg void OnTexturesFindReplace();
	afx_msg void OnTexturesTexturelock();
	afx_msg void OnTexturesChangeWindowScale(UINT nID);
	afx_msg void OnLoadTexturesFromDir(UINT nID);
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditPastespecial();
	afx_msg void OnEditDelete();
	afx_msg void OnEditClearSelection();
	afx_msg void OnEditSelectAll();
	afx_msg void OnEditEntityinspector();
	afx_msg void OnViewNextview();
	afx_msg void OnViewChangeViewMode2(UINT nID);
	afx_msg void OnViewZoom(UINT nID);
	afx_msg void OnViewEnableView(UINT nID);
	afx_msg void OnViewDisableView(UINT nID);
	afx_msg void OnViewShow(UINT nID);
	afx_msg void OnViewHideSelected();
	afx_msg void OnViewHideUnselected();
	afx_msg void OnViewShowHidden();
	afx_msg void OnViewBrushAs(UINT nID);
	afx_msg void OnViewCubicClipping();
	afx_msg void OnViewOpenGLLighting();
	afx_msg void OnMapSnaptogrid();
	afx_msg void OnMapShowgrid();
	afx_msg void OnMapChangeGridSettings(UINT nID);
	afx_msg void OnMapGotobrushnumber();
	afx_msg void OnMapShowinformation();
	afx_msg void OnMapEntityreport();
	afx_msg void OnMapCheckforproblems();
	afx_msg void OnMapMapproperties();
	afx_msg void OnToolsCarve();
	afx_msg void OnToolsHollow();
	afx_msg void OnToolsUnion();
	afx_msg void OnMorphingVertices();
	afx_msg void OnMorphingEdges();
	afx_msg void OnToolsClone();
	afx_msg void OnToolsClip();
	afx_msg void OnToolsGroup();
	afx_msg void OnToolsUngroup();
	afx_msg void OnToolsTietoentity();
	afx_msg void OnToolsMovetoworld();
	afx_msg void OnToolsTransform();
	afx_msg void OnToolsAlign(UINT nID);
	afx_msg void OnToolsFlip(UINT nID);
	afx_msg void OnToolsOptions();
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditDisableundoredo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditPastespecial(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGridAll(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGridMajor(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGridMinor(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGridAxis(CCmdUI *pCmdUI);
	afx_msg void OnUpdateShowWindowlabel(CCmdUI *pCmdUI);
	afx_msg void OnUpdateShowActiverect(CCmdUI *pCmdUI);
	afx_msg void OnUpdateShowWorldaxis(CCmdUI *pCmdUI);
	afx_msg void OnUpdateShowWorldaxisicon(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewCubicclipping(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewOpengllighting(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMapSnaptogrid(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMapShowgrid(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTexturesTexturelock(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTexturesTextureWindowScale(CCmdUI *pCmdUI);
	afx_msg void OnUpdateChangeShapeType(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMorphingVertices(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMorphingEdges(CCmdUI *pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
