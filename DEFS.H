#pragma once

#ifndef RGBA
#define RGBA(r,g,b,a) ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)|(((DWORD)(BYTE)(a))<<24)))
#endif	// RGBA

#ifndef GetAValue
#define GetAValue(rgb) ((BYTE)((rgb)>>24))
#endif

//
//	color imp
//
template<typename T>
struct colortype3 {
	T red, green, blue;

	colortype3& operator = (COLORREF rgb);
	operator const COLORREF () const;

	operator const T * () const
		{ return &red; }
	operator T * ()
		{ return &red; }

	static const colortype3
		BLACK,
		DARKGRAY,
		LIGHTGRAY,
		WHITE,
		GRAY0, GRAY32, GRAY64, GRAY80, GRAY96, GRAY104, GRAY128,
			GRAY160, GRAY192, GRAY224, GRAY255,
		RED, LIGHTRED,
		GREEN, LIGHTGREEN,
		BLUE, LIGHTBLUE,
		YELLOW,
		MARINE;
};

typedef colortype3<unsigned char> UBCOLOR3;
typedef UBCOLOR3 COLOR3;
typedef colortype3<float> FCOLOR3;
typedef colortype3<double> DCOLOR3;


template<typename T>
struct colortypeinfo3 : colortype3<T> {
	colortypeinfo3()
	{}

	colortypeinfo3(COLORREF clrref);

	colortypeinfo3(T r, T g, T b)
	{
		red = r;
		green = g;
		blue = b;
	}
};

typedef colortypeinfo3<unsigned char> UBCOLORINFO3;
typedef UBCOLORINFO3 COLORINFO3;
typedef colortypeinfo3<float> FCOLORINFO3;
typedef colortypeinfo3<double> DCOLORINFO3;



template<typename T>
struct colortype4 {
	T red, green, blue, alpha;

	colortype4& operator = (COLORREF rgb);
	operator const COLORREF () const;

	operator const T * () const
		{ return &red; }
	operator T * ()
		{ return &red; }

	static const colortype4
		BLACK,
		DARKGRAY,
		LIGHTGRAY,
		WHITE,
		GRAY0, GRAY32, GRAY64, GRAY80, GRAY96, GRAY104, GRAY128,
			GRAY160, GRAY192, GRAY224, GRAY255,
		RED, LIGHTRED,
		GREEN, LIGHTGREEN,
		BLUE, LIGHTBLUE,
		YELLOW,
		MARINE;
};

typedef colortype4<unsigned char> UBCOLOR4;
typedef UBCOLOR4 COLOR4;
typedef colortype4<float> FCOLOR4;
typedef colortype4<double> DCOLOR4;


template<typename T>
struct colortypeinfo4 : colortype4<T> {
	colortypeinfo4()
	{}

	colortypeinfo4(COLORREF clrref);

	colortypeinfo4(T r, T g, T b, T a)
	{
		red = r;
		green = g;
		blue = b;
		alpha = a;
	}
};

typedef colortypeinfo4<unsigned char> UBCOLORINFO4;
typedef UBCOLORINFO4 COLORINFO4;
typedef colortypeinfo4<float> FCOLORINFO4;
typedef colortypeinfo4<double> DCOLORINFO4;



//
//	Window layout:
//
enum WINDOWLAYOUT {
	WL_STANDART,
	WL_H2L2 = WL_STANDART,
	/*
	+++++++++
	+	|	+
	+---|---+
	+	|	+
	+++++++++
	*/

	WL_H1L2, WL_H2L1,
	/*
	+++++++++	+++++++++
	+		+	+	|	+
	+---|---+	+---|---+
	+	|	+	+		+
	+++++++++	+++++++++
	*/

	WL_H1L3, WL_H3L1,
	/*
	---------	---------
	|		|	|  | |	|
	|-------|	|-------|
	|  | |	|	|		|
	---------	---------
	*/

	WL_H2L3, WL_H3L2,
	/*
	---------	---------
	|	|	|	|  | |	|
	|-------|	|-------|
	|  | |	|	|	|	|
	---------	---------
	*/

	WL_L1R2, WL_L2R1,
	/*
	---------	---------
	|	|	|	|	|	|
	|	|---|	|---|	|
	|	|	|	|	|	|
	---------	---------
	*/

	WL_L1R3, WL_L3R1,
	/*
	---------	---------
	|	|___|	|___|	|
	|	|	|	|	|	|
	|	|---|	|---|	|
	---------	---------
	*/

	WL_L2R3, WL_L3R2,
	/*
	---------	---------
	|	|___|	|___|	|
	|---|	|	|	|---|
	|	|---|	|---|	|
	---------	---------
	*/

	WL_TOTAL,
	WL_MAXWINDOW = 5	// max window in layout 3+2=2+3=5
};



//
// Num window in selected layout
//
const unsigned WINDOWNUM[WL_TOTAL] = {
	4,
	3, 3,
	4, 4,
	5, 5,

	3, 3,
	4, 4,
	5, 5
};



//
//	View type: (TODO: VM_TEXTURE?)
//
enum VIEWTYPE {
	VT_2D,
	VT_3D,
	VT_TEXTURE,
	  VT_TOTAL
};



//
//	View mode: (TODO: VM_TEXTURE?)
//
enum VIEWMODE {
	VM_FRONT, VM_XY = VM_FRONT,
	VM_LEFT, VM_ZY = VM_LEFT,
	VM_BOTTOM, VM_XZ = VM_BOTTOM,
	VM_BACK,
	VM_RIGHT,
	VM_TOP,
	  VM_TOTALDIMS,
	VM_PERSPECTIVE = VM_TOTALDIMS,
	  VM_TOTALEDITVIEW,
	VM_TEXTURE = VM_TOTALEDITVIEW,
	  VM_TOTAL
};

inline VIEWMODE& operator++ (VIEWMODE& e)
	{ return e = e == VM_TEXTURE ? VM_FRONT : VIEWMODE(e + 1); }
inline VIEWMODE operator++ (VIEWMODE& e, int)
	{ VIEWMODE tmp = e; ++e; return tmp; }
inline VIEWMODE& operator-- (VIEWMODE& e)
	{ return e = e == VM_FRONT ? VM_TEXTURE : VIEWMODE(e - 1); }
inline VIEWMODE operator-- (VIEWMODE& e, int)
	{ VIEWMODE tmp = e; --e; return tmp; }



//
//	Viewmode dims: (VECTOR3( [d],[d],[d] ))
//
struct VIEWMODEDIMS {
	unsigned D1, D2;

	static const VIEWMODEDIMS
		FRONT,
		LEFT,
		BOTTOM,
		BACK,
		RIGHT,
		TOP;

	static const VIEWMODEDIMS DIM[VM_TOTALDIMS];
};



//
//	VIEW LABEL:
//
struct VIEWLABEL {
	const char*	szLabel;
	unsigned uLabelSize;	// for glCallLists()

	enum { VL_LEFTSHIFT = 4, VL_TOPSHIFT = 15,
			VL_BASELISTINDEX = 1000, VL_TEXNAMELISTINDEX = 1256 };
	static const char* szStdLabels[VM_TOTALEDITVIEW];
	static const VIEWLABEL 
		FRONT,
		LEFT,
		BOTTOM,
		BACK,
		RIGHT,
		TOP,
		PERSPECTIVE;

	static const VIEWLABEL LABEL[VM_TOTALEDITVIEW];
};



//
//	Window update:
//
enum WINDOWUPDATE {
	WU_FRONT = 1<<0,
	WU_BACK = 1<<1,
	WU_XY = WU_FRONT|WU_BACK,		// front + back

	WU_LEFT = 1<<2,
	WU_RIGHT = 1<<3,
	WU_ZY = WU_LEFT|WU_RIGHT,		// left + right

	WU_BOTTOM = 1<<4,
	WU_TOP = 1<<5,
	WU_XZ = WU_BOTTOM|WU_TOP,		// bottom + top

	WU_2D = WU_XY|WU_ZY|WU_XZ,		// all 2D

	WU_PERSPECTIVE = 1<<10,
	WU_3D = WU_PERSPECTIVE,			// all 3D

	WU_ALLEDIT = WU_2D|WU_3D,

	WU_TEXTURE = 1<<11,				// `select texture` pane

	WU_ALL = WU_ALLEDIT|WU_TEXTURE
};



//
//	Window state:
//
enum WINDOWSTATE {
	WS_NORMAL,
	WS_HIDE,
	WS_MAXIMIZED,
	WS_DISABLE
};



//
//	Draw modes:
//
enum DRAWMODE {
	DM_BBOX,
	DM_WIRE,
	DM_SOLID,
	DM_SOLID_WIRE,
	DM_TEXTURE,
	DM_TEXTURE_WIRE,
	DM_BLEND,
	DM_BLEND_WIRE,
	DM_LIGHT,
	DM_LIGHT_WIRE,
	  DM_TOTAL
};



//
//	Rendering options:
//
struct RENDERINGOPTIONS {
	DRAWMODE eMode;
	GLenum	oglRenderMode;				// for glBegin() draw brushes as: GL_POLYGON/GL_LINE_LOOP
	GLenum	PolyRasterMethod[2];		// glPolygonMode(GL_FRONT/GL_BACK): GL_POINT, GL_LINE, GL_FILL
	GLenum	ShadeModel;					// glShadeModel: GL_FLAT/GL_SMOOTH
	bool	bCullFace;
	FCOLOR4	clrClearColor;				// individual clear color

	static const RENDERINGOPTIONS
		STD2D,
		STD3D;

	static const RENDERINGOPTIONS STD[VT_TOTAL];
};



//
//	Editor window information:
//
struct WINDOWINFO {
	VIEWTYPE		eType;
	VIEWMODE		eMode;
	VIEWMODEDIMS	sDims;
	VIEWLABEL		sLabel;
	WINDOWUPDATE	eUpdate;
	WINDOWSTATE		eState;
	RENDERINGOPTIONS sRenderOptions;
	bool			bDisable;

	bool Set(VIEWMODE newViewMode);
	bool Set(DRAWMODE newDrawMode);

	static const WINDOWINFO
		FRONT,
		LEFT,
		BOTTOM,
		BACK,
		RIGHT,
		TOP,
		PERSPECTIVE,
		TEXTURE;

	static const WINDOWINFO STD[VM_TOTAL];

	// STanDart WIndow info SET:
	// nNumEditWindow = WINDOWNUM[eWindowLayout];
	// for(int i = 0; i < nNumEditWindow; ++i)
	//	curwin[i].WndInfo = WINDOWINFO::STD[WINDOWINFO::STDWISET[layout][i]];
	static const unsigned STDWISET[WL_TOTAL][WL_MAXWINDOW];
};



//
// GRID
//
struct GRID
{
	bool			bShow;			// show grid?
	bool			bDotted;		// draw dotted line?
	bool			bStipple;		// draw stippled line?

	float			fWidth;			// line - width, dotted - dot size
	int				nStep;
	COLOR3			cColor;

	int				nStippleFactor;
	unsigned short	uStippleMask;
};



//
//	SCREEN AXIS x,y:
//
struct AXIS {
	bool			bShow;			// show grid?
	bool			bDotted;		// draw dotted line?
	bool			bStipple;		// draw stippled line?

	float			fWidth;			// line - width, dotted - dot size
	int				nDotStep;
	COLOR3			cColor;

	int				nStippleFactor;
	unsigned short	uStippleMask;
};



//
//	GRID INFO: (major/minor grids, axis...)
//
struct GRIDINFO {
	GRID Major, Minor;
	AXIS Axis;


	void ChangeGridSize(bool bBigger);

	static const GRIDINFO Default;
};



//
// EDITOR PREFERENCES:
//
struct PREFERENCES {
	// window layout
	WINDOWLAYOUT	eWindowLayout;
	unsigned		nNumEditWindow;

	// grid
	GRIDINFO		sGrid;
	bool			bSnapToGrid;

	// clear color (background)
	bool			bUseGlobalClearColor;
	FCOLOR4			clrGlobalClearColor;	// global clear color

	// label
	bool			bShowWindowLabel;
	COLOR3			cLabelColor;

	// coordinate-space icon
	bool			bShowCoordSpaceIcon;
	enum			{ CSI_WINDOWSHIFT = 24, CSI_SIZE = CSI_WINDOWSHIFT - 4 };

	// active window rect (active - yellow, non-active - black)
	bool			bShowActiveWindowRect;

	// edit-view scale mode options
	bool			bForceCenteredWhenScaleMode;

	bool			bTextureLock;

	// world axis
	bool			bShowWorldAxis;

	// cubic clipping
	bool			bEnableCubicClipping;

	// OpenGL lighting
	bool			bEnableOpenGLLighting;

	// texture window size (100% - 50% - 25%)
	unsigned		uTextureWindowSize;

	// undo
	bool			bEnableUndo;
	int				nUndoStackDepth;

// load scheme
	// color scheme num (0 - ala 3DMax, 1 - Black-and-Green, 2 - White-and-Black)
	void LoadColorScheme(int scheme);

	static const PREFERENCES Default;
	static PREFERENCES Current;
};