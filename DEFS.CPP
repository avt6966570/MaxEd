#include "stdafx.h"
#include "defs.h"

const UBCOLOR3 UBCOLOR3::BLACK = {0, 0, 0};
const UBCOLOR3 UBCOLOR3::DARKGRAY = {64, 64, 64};
const UBCOLOR3 UBCOLOR3::LIGHTGRAY = {192, 192, 192};
const UBCOLOR3 UBCOLOR3::WHITE = {255, 255, 255};
const UBCOLOR3 UBCOLOR3::GRAY0 = {0, 0, 0};
const UBCOLOR3 UBCOLOR3::GRAY32 = {32, 32, 32};
const UBCOLOR3 UBCOLOR3::GRAY64 = {64, 64, 64};
const UBCOLOR3 UBCOLOR3::GRAY80 = {80, 80, 80};
const UBCOLOR3 UBCOLOR3::GRAY96 = {96, 96, 96};
const UBCOLOR3 UBCOLOR3::GRAY104 = {104, 104, 104};
const UBCOLOR3 UBCOLOR3::GRAY128 = {128, 128, 128};
const UBCOLOR3 UBCOLOR3::GRAY160 = {160, 160, 160};
const UBCOLOR3 UBCOLOR3::GRAY192 = {192, 192, 192};
const UBCOLOR3 UBCOLOR3::GRAY224 = {224, 224, 224};
const UBCOLOR3 UBCOLOR3::GRAY255 = {255, 255, 255};
const UBCOLOR3 UBCOLOR3::RED = {255, 0, 0};
const UBCOLOR3 UBCOLOR3::LIGHTRED = {255, 128, 128};
const UBCOLOR3 UBCOLOR3::GREEN = {0, 255, 0};
const UBCOLOR3 UBCOLOR3::LIGHTGREEN = {128, 255, 128};
const UBCOLOR3 UBCOLOR3::BLUE = {0, 0, 255};
const UBCOLOR3 UBCOLOR3::LIGHTBLUE = {128, 128, 255};
const UBCOLOR3 UBCOLOR3::YELLOW = {255, 255, 0};
const UBCOLOR3 UBCOLOR3::MARINE = {0, 128, 128};


const FCOLOR3 FCOLOR3::BLACK = {0, 0, 0};
const FCOLOR3 FCOLOR3::DARKGRAY = {.25F, .25F, .25F};
const FCOLOR3 FCOLOR3::LIGHTGRAY = {.75F, .75F, .75F};
const FCOLOR3 FCOLOR3::WHITE = {1, 1, 1};
const FCOLOR3 FCOLOR3::GRAY0 = {0, 0, 0};
const FCOLOR3 FCOLOR3::GRAY32 = {.125F, .125F, .125F};
const FCOLOR3 FCOLOR3::GRAY64 = {.25F, .25F, .25F};
const FCOLOR3 FCOLOR3::GRAY80 = {0.3125F, 0.3125F, 0.3125F};
const FCOLOR3 FCOLOR3::GRAY96 = {.375F, .375F, .375F};
const FCOLOR3 FCOLOR3::GRAY104 = {0.40625F, 0.40625F, 0.40625F};
const FCOLOR3 FCOLOR3::GRAY128 = {.5F, .5F, .5F};
const FCOLOR3 FCOLOR3::GRAY160 = {.625F, .625F, .625F};
const FCOLOR3 FCOLOR3::GRAY192 = {.75F, .75F, .75F};
const FCOLOR3 FCOLOR3::GRAY224 = {.875F, .875F, .875F};
const FCOLOR3 FCOLOR3::GRAY255 = {1, 1, 1};
const FCOLOR3 FCOLOR3::RED = {1, 0, 0};
const FCOLOR3 FCOLOR3::LIGHTRED = {1, .5F, .5F};
const FCOLOR3 FCOLOR3::GREEN = {0, 1, 0};
const FCOLOR3 FCOLOR3::LIGHTGREEN = {.5F, 1, .5F};
const FCOLOR3 FCOLOR3::BLUE = {0, 0, 1};
const FCOLOR3 FCOLOR3::LIGHTBLUE = {.5F, .5F, 1};
const FCOLOR3 FCOLOR3::YELLOW = {1, 1, 0};
const FCOLOR3 FCOLOR3::MARINE = {0, .5F, .5F};


const DCOLOR3 DCOLOR3::BLACK = {0, 0, 0};
const DCOLOR3 DCOLOR3::DARKGRAY = {.25, .25, .25};
const DCOLOR3 DCOLOR3::LIGHTGRAY = {.75, .75, .75};
const DCOLOR3 DCOLOR3::WHITE = {1, 1, 1};
const DCOLOR3 DCOLOR3::GRAY0 = {0, 0, 0};
const DCOLOR3 DCOLOR3::GRAY32 = {.125, .125, .125};
const DCOLOR3 DCOLOR3::GRAY64 = {.25, .25, .25};
const DCOLOR3 DCOLOR3::GRAY80 = {0.3125, 0.3125, 0.3125};
const DCOLOR3 DCOLOR3::GRAY96 = {.375, .375, .375};
const DCOLOR3 DCOLOR3::GRAY104 = {.40625, .40625, .40625};
const DCOLOR3 DCOLOR3::GRAY128 = {.5, .5, .5};
const DCOLOR3 DCOLOR3::GRAY160 = {.625, .625, .625};
const DCOLOR3 DCOLOR3::GRAY192 = {.75, .75, .75};
const DCOLOR3 DCOLOR3::GRAY224 = {.875, .875, .875};
const DCOLOR3 DCOLOR3::GRAY255 = {1, 1, 1};
const DCOLOR3 DCOLOR3::RED = {1, 0, 0};
const DCOLOR3 DCOLOR3::LIGHTRED = {1, .5, .5};
const DCOLOR3 DCOLOR3::GREEN = {0, 1, 0};
const DCOLOR3 DCOLOR3::LIGHTGREEN = {.5, 1, .5};
const DCOLOR3 DCOLOR3::BLUE = {0, 0, 1};
const DCOLOR3 DCOLOR3::LIGHTBLUE = {.5, .5, 1};
const DCOLOR3 DCOLOR3::YELLOW = {1, 1, 0};
const DCOLOR3 DCOLOR3::MARINE = {0, .5, .5};

// `unsigned char` specialization
template<>
colortypeinfo3<unsigned char>::colortypeinfo3(COLORREF clrref)
{
	red = clrref && 0xff;
	green = clrref && 0xff00;
	blue = clrref && 0xff0000;
}




const UBCOLOR4 UBCOLOR4::BLACK = {0, 0, 0, 255};
const UBCOLOR4 UBCOLOR4::DARKGRAY = {64, 64, 64, 255};
const UBCOLOR4 UBCOLOR4::LIGHTGRAY = {192, 192, 192, 255};
const UBCOLOR4 UBCOLOR4::WHITE = {255, 255, 255, 255};
const UBCOLOR4 UBCOLOR4::GRAY0 = {0, 0, 0, 255};
const UBCOLOR4 UBCOLOR4::GRAY32 = {32, 32, 32, 255};
const UBCOLOR4 UBCOLOR4::GRAY64 = {64, 64, 64, 255};
const UBCOLOR4 UBCOLOR4::GRAY80 = {80, 80, 80, 255};
const UBCOLOR4 UBCOLOR4::GRAY96 = {96, 96, 96, 255};
const UBCOLOR4 UBCOLOR4::GRAY104 = {104, 104, 104, 255};
const UBCOLOR4 UBCOLOR4::GRAY128 = {128, 128, 128, 255};
const UBCOLOR4 UBCOLOR4::GRAY160 = {160, 160, 160, 255};
const UBCOLOR4 UBCOLOR4::GRAY192 = {192, 192, 192, 255};
const UBCOLOR4 UBCOLOR4::GRAY224 = {224, 224, 224, 255};
const UBCOLOR4 UBCOLOR4::GRAY255 = {255, 255, 255, 255};
const UBCOLOR4 UBCOLOR4::RED = {255, 0, 0, 255};
const UBCOLOR4 UBCOLOR4::LIGHTRED = {255, 128, 128, 255};
const UBCOLOR4 UBCOLOR4::GREEN = {0, 255, 0, 255};
const UBCOLOR4 UBCOLOR4::LIGHTGREEN = {128, 255, 128, 255};
const UBCOLOR4 UBCOLOR4::BLUE = {0, 0, 255, 255};
const UBCOLOR4 UBCOLOR4::LIGHTBLUE = {128, 128, 255, 255};
const UBCOLOR4 UBCOLOR4::YELLOW = {255, 255, 0, 255};
const UBCOLOR4 UBCOLOR4::MARINE = {0, 128, 128, 255};


const FCOLOR4 FCOLOR4::BLACK = {0.F, 0.F, 0.F, 1.F};
const FCOLOR4 FCOLOR4::DARKGRAY = {.25F, .25F, .25F, 1.F};
const FCOLOR4 FCOLOR4::LIGHTGRAY = {.75F, .75F, .75F, 1.F};
const FCOLOR4 FCOLOR4::WHITE = {1.F, 1.F, 1.F, 1.F};
const FCOLOR4 FCOLOR4::GRAY0 = {0.F, 0.F, 0.F, 1.F};
const FCOLOR4 FCOLOR4::GRAY32 = {.125F, .125F, .125F, 1.F};
const FCOLOR4 FCOLOR4::GRAY64 = {.25F, .25F, .25F, 1.F};
const FCOLOR4 FCOLOR4::GRAY80 = {0.3125F, 0.3125F, 0.3125F, 1.F};
const FCOLOR4 FCOLOR4::GRAY96 = {.375F, .375F, .375F, 1.F};
const FCOLOR4 FCOLOR4::GRAY104 = {0.40625F, 0.40625F, 0.40625F, 1.F};
const FCOLOR4 FCOLOR4::GRAY128 = {.5F, .5F, .5F, 1.F};
const FCOLOR4 FCOLOR4::GRAY160 = {.625F, .625F, .625F, 1.F};
const FCOLOR4 FCOLOR4::GRAY192 = {.75F, .75F, .75F, 1.F};
const FCOLOR4 FCOLOR4::GRAY224 = {.875F, .875F, .875F, 1.F};
const FCOLOR4 FCOLOR4::GRAY255 = {1.F, 1.F, 1.F, 1.F};
const FCOLOR4 FCOLOR4::RED = {1.F, 0.F, 0.F, 1.F};
const FCOLOR4 FCOLOR4::LIGHTRED = {1.F, .5F, .5F, 1.F};
const FCOLOR4 FCOLOR4::GREEN = {0.F, 1.F, 0.F, 1.F};
const FCOLOR4 FCOLOR4::LIGHTGREEN = {.5F, 1.F, .5F, 1.F};
const FCOLOR4 FCOLOR4::BLUE = {0.F, 0.F, 1.F, 1.F};
const FCOLOR4 FCOLOR4::LIGHTBLUE = {.5F, .5F, 1.F, 1.F};
const FCOLOR4 FCOLOR4::YELLOW = {1.F, 1.F, 0, 1.F};
const FCOLOR4 FCOLOR4::MARINE = {0.F, .5F, .5F, 1.F};


const DCOLOR4 DCOLOR4::BLACK = {0., 0., 0., 1.};
const DCOLOR4 DCOLOR4::DARKGRAY = {.25, .25, .25, 1.};
const DCOLOR4 DCOLOR4::LIGHTGRAY = {.75, .75, .75, 1.};
const DCOLOR4 DCOLOR4::WHITE = {1., 1., 1., 1.};
const DCOLOR4 DCOLOR4::GRAY0 = {0., 0., 0., 1.};
const DCOLOR4 DCOLOR4::GRAY32 = {.125, .125, .125, 1.};
const DCOLOR4 DCOLOR4::GRAY64 = {.25, .25, .25, 1.};
const DCOLOR4 DCOLOR4::GRAY80 = {0.3125, 0.3125, 0.3125, 1.};
const DCOLOR4 DCOLOR4::GRAY96 = {.375, .375, .375, 1.};
const DCOLOR4 DCOLOR4::GRAY104 = {.40625, .40625, .40625, 1.};
const DCOLOR4 DCOLOR4::GRAY128 = {.5, .5, .5, 1.};
const DCOLOR4 DCOLOR4::GRAY160 = {.625, .625, .625, 1.};
const DCOLOR4 DCOLOR4::GRAY192 = {.75, .75, .75, 1.};
const DCOLOR4 DCOLOR4::GRAY224 = {.875, .875, .875, 1.};
const DCOLOR4 DCOLOR4::GRAY255 = {1., 1., 1., 1.};
const DCOLOR4 DCOLOR4::RED = {1., 0., 0., 1.};
const DCOLOR4 DCOLOR4::LIGHTRED = {1., .5, .5, 1.};
const DCOLOR4 DCOLOR4::GREEN = {0., 1., 0., 1.};
const DCOLOR4 DCOLOR4::LIGHTGREEN = {.5, 1., .5, 1.};
const DCOLOR4 DCOLOR4::BLUE = {0., 0., 1., 1.};
const DCOLOR4 DCOLOR4::LIGHTBLUE = {.5, .5, 1., 1.};
const DCOLOR4 DCOLOR4::YELLOW = {1, 1, 0, 1};
const DCOLOR4 DCOLOR4::MARINE = {0., .5, .5, 1.};

// `unsigned char` specialization
template<>
colortypeinfo4<unsigned char>::colortypeinfo4(COLORREF clrref)
{
	red = clrref && 0xff;
	green = clrref && 0xff00;
	blue = clrref && 0xff0000;
	alpha = 0;
}




template<> UBCOLOR3& UBCOLOR3::operator = (COLORREF rgb)
{
	red = GetRValue(rgb);
	green = GetGValue(rgb);
	blue = GetBValue(rgb);
	return *this;
}

template<> FCOLOR3& FCOLOR3::operator = (COLORREF rgb)
{
	red = GetRValue(rgb) / 255.F;
	green = GetGValue(rgb) / 255.F;
	blue = GetBValue(rgb) / 255.F;
	return *this;
}

template<> DCOLOR3& DCOLOR3::operator = (COLORREF rgb)
{
	red = GetRValue(rgb) / 255.0;
	green = GetGValue(rgb) / 255.0;
	blue = GetBValue(rgb) / 255.0;
	return *this;
}

template<> UBCOLOR4& UBCOLOR4::operator = (COLORREF rgb)
{
	red = GetRValue(rgb);
	green = GetGValue(rgb);
	blue = GetBValue(rgb);
	alpha = GetAValue(rgb);
	return *this;
}

template<> FCOLOR4& FCOLOR4::operator = (COLORREF rgb)
{
	red = GetRValue(rgb) / 255.F;
	green = GetGValue(rgb) / 255.F;
	blue = GetBValue(rgb) / 255.F;
	alpha = GetAValue(rgb) / 255.F;
	return *this;
}

template<> DCOLOR4& DCOLOR4::operator = (COLORREF rgb)
{
	red = GetRValue(rgb) / 255.0;
	green = GetGValue(rgb) / 255.0;
	blue = GetBValue(rgb) / 255.0;
	alpha = GetAValue(rgb) / 255.0;
	return *this;
}


template<> UBCOLOR3::operator const COLORREF () const
	{ return RGB(red, green, blue); }
template<> FCOLOR3::operator const COLORREF () const
	{ return RGB(red*255.F, green*255.F, blue*255.F); }
template<> DCOLOR3::operator const COLORREF () const
	{ return RGB(red*255.0, green*255.0, blue*255.0); }

template<> UBCOLOR4::operator const COLORREF () const
	{ return RGBA(red, green, blue, alpha); }
template<> FCOLOR4::operator const COLORREF () const
	{ return RGBA(red*255.F, green*255.F, blue*255.F, alpha*255.F); }
template<> DCOLOR4::operator const COLORREF () const
	{ return RGBA(red*255.0, green*255.0, blue*255.0, alpha*255.0); }




const VIEWMODEDIMS VIEWMODEDIMS::FRONT = { 0, 1 };
const VIEWMODEDIMS VIEWMODEDIMS::LEFT = { 2, 1 };
const VIEWMODEDIMS VIEWMODEDIMS::BOTTOM = { 0, 2 };
const VIEWMODEDIMS VIEWMODEDIMS::BACK = { 0, 1 };	// same as FRONT
const VIEWMODEDIMS VIEWMODEDIMS::RIGHT = { 2, 1 };	// same as LEFT
const VIEWMODEDIMS VIEWMODEDIMS::TOP = { 0, 2 };	// same as BOTTOM
const VIEWMODEDIMS VIEWMODEDIMS::DIM[VM_TOTALDIMS] = {
	VIEWMODEDIMS::FRONT,
	VIEWMODEDIMS::LEFT,
	VIEWMODEDIMS::BOTTOM,
	VIEWMODEDIMS::BACK,								// same as FRONT
	VIEWMODEDIMS::RIGHT,							// same as LEFT;
	VIEWMODEDIMS::TOP 								// same as BOTTOM
};



const char* VIEWLABEL::szStdLabels[VM_TOTALEDITVIEW] = {
	"Front/Disabled",
	"Left/Disabled",
	"Bottom/Disabled",
	"Back/Disabled",
	"Right/Disabled",
	"Top/Disabled",
	"Perspective/Disabled"
};

const VIEWLABEL VIEWLABEL::FRONT	= { VIEWLABEL::szStdLabels[VM_FRONT], 5 };
const VIEWLABEL VIEWLABEL::LEFT		= { VIEWLABEL::szStdLabels[VM_LEFT], 4 };
const VIEWLABEL VIEWLABEL::BOTTOM	= { VIEWLABEL::szStdLabels[VM_BOTTOM], 6 };
const VIEWLABEL VIEWLABEL::BACK		= { VIEWLABEL::szStdLabels[VM_BACK], 4 };
const VIEWLABEL VIEWLABEL::RIGHT	= { VIEWLABEL::szStdLabels[VM_RIGHT], 5 };
const VIEWLABEL VIEWLABEL::TOP		= { VIEWLABEL::szStdLabels[VM_TOP], 3 };
const VIEWLABEL VIEWLABEL::PERSPECTIVE = { VIEWLABEL::szStdLabels[VM_PERSPECTIVE], 11};

const VIEWLABEL VIEWLABEL::LABEL[VM_TOTALEDITVIEW] = {
	VIEWLABEL::FRONT,
	VIEWLABEL::LEFT,
	VIEWLABEL::BOTTOM,
	VIEWLABEL::BACK,
	VIEWLABEL::RIGHT,
	VIEWLABEL::TOP,
	VIEWLABEL::PERSPECTIVE
};



const RENDERINGOPTIONS RENDERINGOPTIONS::STD2D =
	{ DM_WIRE, GL_LINE_LOOP, {GL_LINE, GL_LINE}, GL_FLAT, false, FCOLOR4::GRAY128 };
const RENDERINGOPTIONS RENDERINGOPTIONS::STD3D =
	{ DM_TEXTURE, GL_POLYGON, {GL_FILL, GL_FILL}, GL_FLAT, true, FCOLOR4::GRAY128 };
const RENDERINGOPTIONS RENDERINGOPTIONS::STD[VT_TOTAL] = {
	RENDERINGOPTIONS::STD2D,
	RENDERINGOPTIONS::STD3D
};


bool WINDOWINFO::Set(VIEWMODE newViewMode)
{
	ASSERT(newViewMode >= 0 && newViewMode < VM_TOTAL);
	if(eMode != newViewMode)
	{
		if(WINDOWINFO::STD[eMode].eType == WINDOWINFO::STD[newViewMode].eType)
		{
			// save previous rendering options instead load standart
			RENDERINGOPTIONS prev = sRenderOptions;
			*this = WINDOWINFO::STD[newViewMode];
			sRenderOptions = prev;
		}
		else
			*this = WINDOWINFO::STD[newViewMode];
		return true;
	}
	else
		return false;
}

bool WINDOWINFO::Set(DRAWMODE newDrawMode)
{
	ASSERT(newDrawMode >= 0 && newDrawMode < DM_TOTAL);
	if(sRenderOptions.eMode != newDrawMode)
	{
		sRenderOptions.eMode = newDrawMode;
		if(newDrawMode  > DM_WIRE)
		{
			sRenderOptions.PolyRasterMethod[0] = GL_FILL;
			sRenderOptions.oglRenderMode = GL_POLYGON;
		}
		else
		{
			sRenderOptions.PolyRasterMethod[0] = GL_LINE;
			sRenderOptions.oglRenderMode = GL_LINE_LOOP;
		}

		sRenderOptions.bCullFace = eType != VT_2D;
		return true;
	}
	return false;
}

const WINDOWINFO WINDOWINFO::FRONT =
	{ VT_2D, VM_FRONT, VIEWMODEDIMS::FRONT, VIEWLABEL::FRONT,
	  WU_FRONT, WS_NORMAL, RENDERINGOPTIONS::STD2D, false };
const WINDOWINFO WINDOWINFO::LEFT =
	{ VT_2D, VM_LEFT, VIEWMODEDIMS::LEFT, VIEWLABEL::LEFT,
	  WU_LEFT, WS_NORMAL, RENDERINGOPTIONS::STD2D, false };
const WINDOWINFO WINDOWINFO::BOTTOM =
	{ VT_2D, VM_BOTTOM, VIEWMODEDIMS::BOTTOM, VIEWLABEL::BOTTOM,
	  WU_BOTTOM, WS_NORMAL, RENDERINGOPTIONS::STD2D, false };
const WINDOWINFO WINDOWINFO::BACK =
	{ VT_2D, VM_BACK, VIEWMODEDIMS::BACK, VIEWLABEL::BACK,
	  WU_BACK, WS_NORMAL, RENDERINGOPTIONS::STD2D, false };
const WINDOWINFO WINDOWINFO::RIGHT =
	{ VT_2D, VM_RIGHT, VIEWMODEDIMS::RIGHT,	VIEWLABEL::RIGHT,
	  WU_RIGHT, WS_NORMAL, RENDERINGOPTIONS::STD2D, false };
const WINDOWINFO WINDOWINFO::TOP =
	{ VT_2D, VM_TOP, VIEWMODEDIMS::TOP, VIEWLABEL::TOP,
	  WU_TOP, WS_NORMAL, RENDERINGOPTIONS::STD2D, false };
const WINDOWINFO WINDOWINFO::PERSPECTIVE =
	{ VT_3D, VM_PERSPECTIVE, VIEWMODEDIMS::TOP,	VIEWLABEL::PERSPECTIVE,
	  WU_PERSPECTIVE, WS_NORMAL, RENDERINGOPTIONS::STD3D, false };
const WINDOWINFO WINDOWINFO::TEXTURE =
	{ VT_TEXTURE, VM_TEXTURE, VIEWMODEDIMS::TOP, VIEWLABEL::PERSPECTIVE,
	  WU_TEXTURE, WS_NORMAL, RENDERINGOPTIONS::STD3D, false };

const WINDOWINFO WINDOWINFO::STD[VM_TOTAL] = {
	WINDOWINFO::FRONT,
	WINDOWINFO::LEFT,
	WINDOWINFO::BOTTOM,
	WINDOWINFO::BACK,
	WINDOWINFO::RIGHT,
	WINDOWINFO::TOP,
	WINDOWINFO::PERSPECTIVE,
	WINDOWINFO::TEXTURE
};

const unsigned WINDOWINFO::STDWISET[WL_TOTAL][WL_MAXWINDOW] = {
	{ VM_PERSPECTIVE, VM_FRONT, VM_TEXTURE, VM_TOP },			//WL_STANDART (WL_H2L2)
	{ VM_PERSPECTIVE, VM_FRONT, VM_LEFT },						//WL_H1L2		
	{ VM_PERSPECTIVE, VM_FRONT, VM_LEFT },						//WL_H2L1		
	{ VM_PERSPECTIVE, VM_FRONT, VM_LEFT, VM_TOP },				//WL_H1L3		
	{ VM_PERSPECTIVE, VM_FRONT, VM_LEFT, VM_TOP },				//WL_H3L1		
	{ VM_PERSPECTIVE, VM_FRONT, VM_LEFT, VM_TOP, VM_RIGHT },	//WL_H2L3		
	{ VM_PERSPECTIVE, VM_FRONT, VM_LEFT, VM_TOP, VM_RIGHT },	//WL_H3L2		
	{ VM_PERSPECTIVE, VM_FRONT, VM_LEFT },						//WL_L1R2		
	{ VM_PERSPECTIVE, VM_FRONT, VM_LEFT },						//WL_L2R1		
	{ VM_PERSPECTIVE, VM_FRONT, VM_LEFT, VM_TOP },				//WL_L1R3		
	{ VM_PERSPECTIVE, VM_FRONT, VM_LEFT, VM_TOP },				//WL_L3R1		
	{ VM_PERSPECTIVE, VM_FRONT, VM_LEFT, VM_TOP, VM_RIGHT },	//WL_L2R3		
	{ VM_PERSPECTIVE, VM_FRONT, VM_LEFT, VM_TOP, VM_RIGHT }		//WL_L3R2		
};


const GRIDINFO GRIDINFO::Default = {
	{ true, false, false, 1.0F, 16, COLOR3::GRAY104, 1, 0xFFFF },	// major grid
	{ true, false, false, 1.0F, 128, COLOR3::GRAY80, 1, 0xFFFF },	// minor grid
	{ true, false, false, 1.0F, 16, COLOR3::BLACK, 1, 0xFFFF }		// coord axis
};

void GRIDINFO::ChangeGridSize(bool bBigger)
{
	if (bBigger)
	{
		if (Minor.nStep < 512)
		{
			Minor.nStep <<= 1;
			if (Major.nStep < 512)
				Major.nStep <<= 1;
		}
	}
	else
	{
		if (Major.nStep > 2)
		{
			Major.nStep >>= 1;
			if (Minor.nStep > 2)
				Minor.nStep >>= 1;
		}
	}
}



const PREFERENCES PREFERENCES::Default = {
	WL_STANDART,				// current window layout
	WINDOWNUM[WL_STANDART],		// num window in layout (pEditWnd pointers)
	GRIDINFO::Default, true,	// gridinfo, snap-to-grid?
	true, FCOLOR4::GRAY128,		// use-global-clearcolor?, color
	true, COLOR3::GRAY224,		// show label?, labels color
	true,						// show coord space icon?
	true,						// show active window rect?
	false,						// force centered when scale mode?
	false,						// texture lock?
	true,						// show world axis?
	false,						// enable cubic clipping?
	false,						// enable OpenGL lighting?
	0,							// texture window size (0:100% - 1:50% - 2:25%)
	true,						// undo enable?
	10							// undo stack depth
};

PREFERENCES PREFERENCES::Current;

// color scheme num (0 - ala 3DMax, 1 - Black-and-Green, 2 - White-and-Black)
void PREFERENCES::LoadColorScheme(int scheme)
{
/*
*	color scheme = grid + clear_color + label
*/
	switch (scheme)
	{
	case 0:	// "ala 3DMax"
		sGrid.Major.cColor = COLOR3::GRAY104;
		sGrid.Minor.cColor = COLOR3::GRAY80;
		sGrid.Axis.cColor = COLOR3::BLACK;
		clrGlobalClearColor = FCOLOR4::GRAY128;
		cLabelColor = COLOR3::GRAY224;
		break;
	case 1:	// "Black-and-Green"
		sGrid.Major.cColor = COLOR3::GRAY80;
		sGrid.Minor.cColor = COLOR3::GRAY128;
		sGrid.Axis.cColor = COLORINFO3(0, 104, 104);
		clrGlobalClearColor = FCOLOR4::BLACK;
		cLabelColor = COLOR3::GRAY224;
		break;
	case 2:	// "White-and-Black"
		sGrid.Major.cColor = COLOR3::GRAY80;
		sGrid.Minor.cColor = COLOR3::GRAY32;
		sGrid.Axis.cColor = COLOR3::BLACK;
		clrGlobalClearColor = FCOLOR4::WHITE;
		cLabelColor = COLOR3::BLACK;
		break;
	}
}
