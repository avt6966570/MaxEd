#pragma once
#include "OpenGLWnd.h"
#include "EntityInspectorDialog.h"
#include "FaceInspectorDialog.h"
#include "BRUSH.h"
#include "Camera.h"

class CEditWnd : public COpenGLWnd
{
	DECLARE_DYNCREATE(CEditWnd)

	typedef vectorex<BRUSH>		brushlist;
	typedef vectorex<BRUSH*>	ptrbrushlist;


	enum eEditingStatus {
		ES_SELECTION,	// neutral
		ES_VIEWMOVE,
		ES_CREATE,
		ES_MODIFY
	};

	enum eViewMoveMode {
		VMM_2DMOVE,
		VMM_2DSCALE,
		VMM_3DFLY
	};

	enum eModifyMode {
		MM_DRAGCREATE,
		MM_TRANSLATE,
		MM_ROTATE,
		MM_RESIZE,
		MM_SHEAR,
		MM_VERTEXDRAG,
		MM_EDGEDRAG,
		MM_CLIPPING,
		MM_MOUSETEXTURESHIFT		// for texture mouse manipulation in 3D-view
	};

	enum eClippingState {
		CS_FIRSTCLIPPLANEDEF,	// 1`-2` points for clip plane definition
		CS_SECONDCLIPPLANEDEF,	// 3` point clip plane definition
		CS_COMPLETECLIPPLANEDEF
	};

	// max deep camera in orto proj for 2d selection (correspond to WORLD_SIZE)
	static const FLOAT ORTHOCAMDEEP[VM_TOTALDIMS];

// Attributes
public:
protected:
private:

// Operations
public:
	// window info
	void SetWindowInfo(const WINDOWINFO& wi);
	void SetViewMode(VIEWMODE vm);
	void NextViewMode();
	static void SetDrawMode(DRAWMODE dm);	// "static" - in all 6 view!
	void Zoom(bool bZoomIn, bool bReset = false);
	void EnableView(VIEWTYPE vt, bool bEnable = true);
	static void CenteredView(const BRUSH& brush);

	// editing info
	static const eModifyMode GetModifyMode();

	// update
	using COpenGLWnd::RedrawWindow;
	static void RedrawWindow(WINDOWUPDATE wu);

	// texture view
	static void ResetTextureScrollPos(bool bUpdateWindow = false);


	// edit
	static void ClearSelection();
	static void DeleteSelection();
	static void Carve();
	static void Hollow();
	static void Union();
	static void MorphingVertices();
	static void MorphingEdges();
	static void CloneSelection();
	static void ToggleClip();
	void MultiplySelection();


protected:
private:
	// utils
	void SetActiveView(bool wndDisabled = false);
	void ScreenToWorld(const CPoint& ptScreen, VECTOR2& vecWorld, bool snaptogrid) const;
	void ScreenToWorld(const CPoint& ptScreen, VECTOR3& vecWorld, bool snaptogrid) const;
	void LoadBoxSideTransMatrix(const VECTOR3& delta) const;
	void LoadBoxSideShearMatrix(const VECTOR3& delta) const;
	void LoadBoxSideRotationMatrix(const VECTOR2& origin, const VECTOR2& current) const;

	// drawing
	void DrawCoordSpaceOverlayIcon() const;
	void DrawCoordSpace() const;
	void DrawWindowLabelOverlay() const;
	void DrawGrid(float fViewLeft, float fViewRight,
				  float fViewBottom, float fViewTop) const;
	void DrawActiveWindowRect() const;
	void DrawBrush(const BRUSH& b) const;
	void DrawWiredBrush(const BRUSH& b) const;
	void DrawBrush2D(const BRUSH& b) const;
	void DrawFace(const FACE* face, const BRUSH* owner) const;

	// process mouse

	// texture
	bool HitTextureInView(const TEXTUREDEF* ptdf,
						  const CPoint& hit, int& ox, int& oy, int& maxHi);
	void SetActiveTexture(const TEXTUREDEF *ptdf);
	static void OnMouseTextureMove();
	static void OnKeyTextureMove(UINT nKey, UNDOINFO::eOpType op);

	// status bar info
	void UpdateStatusBarSelectionInfo() const;


// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);	// add vscroll to texture view
protected:
private:


// Implementation
public:
	CEditWnd();
	virtual ~CEditWnd();

	static CEditWnd* pEditWindow[WL_MAXWINDOW];	// global pointers to editing panes
	static CEditWnd* pActiveWindow;				// only one active window

	// create brush
	static BRUSH::eShapeType CURRENTSHAPE;
	static int				 CURRENTSHAPENSIDE;

	// texture menu draw
	static bool					bTexturesShowInUseOnly;

	// enable/disable undo/redo
	static undostack<UNDOINFO>	GLOBALUNDOSTACK;
	static bool					bDisableUndoRedo;

	// clipboard
	static brushlist			CLIPBOARD;

	// cubic clipping
	static BBOX3CLIPPER			CUBICCLIPPER;
	static VECTOR3				CUBICCLIPPERDELTA;


	/* DIALOGS */
	static CEntityInspectorDialog	ENTITYINSPECTORDIALOG;	// "Entity Inspector" dialog
	static CFaceInspectorDialog		FACEINSPECTORDIALOG;	// "Face Inspector" dialog

protected:
private:
	// window info
	WINDOWINFO		m_WindowInfo;			// (type/mode/state/label/render opt etc.)

	// base
	float			m_fScale;				// view scale
	VECTOR2			m_vOrigin;				// plane camera
	int				m_iTextureScrollPos;	// current position on texture scroll bar
	static int		iMaxTextureScrollHight;	// max texture scroll bar pos

	// editing status
	static eEditingStatus		EDITINGSTATUS;
	static eModifyMode			MODIFYMODE;
	static eViewMoveMode		VIEWMOVEMODE;
	static eClippingState		CLIPPINGSTATE;
	static VECTOR3				vClipPlanePoint[3];

	// editing (dragging box) units
	static MATRIX16				DRAGMODIFYMATRIX;
	static BBOX3				DRAGMODIFYBOX;
	static BBOX3::eBoxSideInfo	DRAGMODIFYSIDE;
	static BBOX2::eBoxSideInfo	DRAGMODIFY2DSIDE;

	// mouse capture info
	static CPoint			ptPrevPoint;		// mouse capture (2D point)
	static VECTOR2			vMouseOrigin,		// mouse press (2D vec)
							vMouseOriginNS,		// (not snapped)
							vMouseCurrent,		// mouse drag (2D vec)
							vMouseCurrentNS;	// (not snapped)

	// cursors
	static const HCURSOR	CURSOR_ARROW,
							CURSOR_SIZEALL,
							CURSOR_SIZENESW,
							CURSOR_SIZENS,
							CURSOR_SIZENWSE,
							CURSOR_SIZEWE,
							CURSOR_HIDE;
	static HCURSOR			CURSOR_ROTATE,
							hCurrentCursor;

	// 2d clip
	static BBOX2			_2DCLIPPER;

	// texture click/draw
	enum { TEXSKIN_XOFF = 4, TEXSKIN_YOFF = 8 };


// Generated message map functions
public:
protected:
	//{{AFX_MSG(CEditWnd)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg void OnChangeViewMode(UINT nID);
	afx_msg void OnChangeRenderingMode(UINT nID);
	afx_msg void OnChangeShadeModel(UINT nID);
	afx_msg void OnChangePolygonMode(UINT nID);
	afx_msg void OnAddEntityPopup(UINT nID);
	afx_msg void OnChangeCullFace();
	afx_msg void OnChangeDisableWindow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};

extern void UpdateUndoRedoMenu(bool bClearSelection = false);