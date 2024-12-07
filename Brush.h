#pragma once
#include <algorithm>
#include "defs.h"


//=============================================================================
//								  ENTITY
//=============================================================================
struct ENTITY
{
	ENTITY()
		{
		}

	ENTITY(const CString& _Name, const CMapStringToString& _Epairs,
		UINT _Flags = 0, COLORREF _Color = 0xFF0000)
			: name(_Name)
			, flags(_Flags)
			, color(_Color)
		{
			// copy epairs
			CString key, value;
			for(POSITION pos = _Epairs.GetStartPosition(); pos != NULL; )
			{
				_Epairs.GetNextAssoc(pos, key, value);
				epairs.SetAt(key, value);
			}
		}

	ENTITY(const ENTITY& e)
		: name(e.name)
		, flags(e.flags)
		, color(e.color)
		{
			// copy epairs
			CString key, value;
			for(POSITION pos = e.epairs.GetStartPosition(); pos != NULL; )
			{
				e.epairs.GetNextAssoc(pos, key, value);
				epairs.SetAt(key, value);
			}
		}

	ENTITY& operator = (const ENTITY& e)
	{
		name = e.name;
		flags = e.flags;
		color = e.color;

		// copy epairs
		CString key, value;
		for(POSITION pos = e.epairs.GetStartPosition(); pos != NULL; )
		{
			e.epairs.GetNextAssoc(pos, key, value);
			epairs.SetAt(key, value);
		}

		return *this;
	}

	BOOL Lookup(LPCTSTR key, CString& value) const
		{	// find & get value for key
		return epairs.Lookup(key, value);
		}

	void UpdateKey(const CString& key, const CString& newvalue)
		{	// Inserts an element into the epair-map;
			// replaces an existing value if a matching key is found
		epairs.SetAt(key, newvalue);
		}

	BOOL RemoveKey(const CString& key)
		{
		return epairs.RemoveKey(key);
		}


	CString				name;
	CMapStringToString	epairs;
	UINT				flags;
	COLORREF			color;


	static ENTITY GLOBALENTITYLIST[128];
	static ENTITY* GLOBALENTITYLISTPTR;
};


struct ENTITYINFO : ENTITY
{
	CStringArray	flagnames;
	CString			desc;
	VECTOR3			size;
	INT_PTR			menuID;
	static const VECTOR3		DEFAULTSIZE;
	static const COLORREF		DEFAULTCOLOR, BRUSHCOLOR;
	static CArray<ENTITYINFO>	GLOBALENTITYINFOLIST;

	enum { MAX_ENTITY = 256 };

	static bool ParseDefFile(const CString& defdata);
	static ENTITYINFO* FindEntityByName(const CString& name);
	static ENTITYINFO* FindEntityByMenuID(INT_PTR menuid);

	ENTITYINFO& operator = (const ENTITYINFO& ei)
	{
		ENTITY::operator =(ei);
		flagnames.Copy(ei.flagnames);
		desc = ei.desc;
		size = ei.size;
		return *this;
	}
};





//=============================================================================
//								  TEXTUREDEF
//=============================================================================

struct TEXTUREDEF
{
	typedef unsigned char	pixeldata;
	typedef int				textureproperties;

	TEXTUREDEF()
		: width(0)
		, height(0)
		, bpp(0)
		, texID(0)
		, data(0)
		, filetime(0)
		, bInUse(false)
	{
	}

	TEXTUREDEF(const char* filename,
				textureproperties width, textureproperties height,
				textureproperties bpp, textureproperties texID,
				pixeldata *data)
		: name(filename)
		, width(width)
		, height(height)
		, bpp(bpp)
		, texID(texID)
		, data(data)
		, filetime(0)
		, bInUse(false)
	{
	}

	~TEXTUREDEF();

	// load & register texture from image file (.BMP just yet)
	bool Load(const char* filename);

	// set to default tex (if tex file was replaced or deleted)
	void SetDefault()
		{
		if (texID != TEXTUREDEF::DEFAULT.texID)
			{
			::glDeleteTextures(1, &texID);			// delete old texture resource
			texID	= TEXTUREDEF::DEFAULT.texID;	// make default
			filetime = TEXTUREDEF::DEFAULT.filetime;// reset filetime (so that can update on next rescanning)
			}
		}

	// draw skin in "select texture" window
	void DrawSkin(int left, int top, int bitscale = 0, bool selection = false) const;


	CString				name;				// name (texture file name with ext!)
	textureproperties	width, height, bpp;
	GLuint				texID;				// gl texture bind number
	pixeldata			*data;
	__int64				filetime;			// last modify file time (for update)
	bool				bInUse;				// really use in map?

	static TEXTUREDEF *pACTIVETEXTURE;		// current active texture
	static TEXTUREDEF ZERO;					// zero texture (for point-based entity)
	static TEXTUREDEF DEFAULT;				// default texture (checkers)
	static void InitDefaultTexture();
};




//=============================================================================
//								  TEXTURE FOLDER
//=============================================================================
struct TEXTUREDIR
{
	typedef vectorex<TEXTUREDEF>		texturedeflist;
	typedef texturedeflist::iterator	texturedeflistiterator;
	typedef vectorex<TEXTUREDIR>		texturedirlist;
	typedef texturedirlist::iterator	texturedirlistiterator;


	TEXTUREDIR() {}
	bool Init(const char* texturepath, const char* dirname);
	bool Update(const char* texturepath, const char* dirname);
	const TEXTUREDEF* Find(const char* texname) const;

	void Add(const TEXTUREDEF& td)	{ cache.push_back(td); }
	TEXTUREDEF* GetBack()			{ return &cache.back(); }

	CString			name;
	texturedeflist	cache;



	static texturedirlist CACHE;
	static TEXTUREDIR* CURRENTCACHEDIR;
	/* strong finding */
	static TEXTUREDIR* FindDirInTDCache(const char* dirname,
				const TEXTUREDEF* td = 0, const char* tdname = 0);

	/* utils for "Find/Replace" dialog */
	// find tex-dir by name (case insensitive)
	static TEXTUREDIR* FindTexDirInTDCache(const char* dirname);
	// ... + non-strong find in dir
	TEXTUREDEF* Find2(const char* texname);
	// or scan all texdir...
	static TEXTUREDEF* FindTexDefInTDCache(const char* tname, const char* tdir = 0);
};










//=============================================================================
//								  IVERTEX3
//=============================================================================

// INDEXED VERTEX
struct IVERTEX3
{
	typedef unsigned char vertexindex;

	IVERTEX3() {}
	explicit IVERTEX3(vertexindex vi) : I(vi) {}
	IVERTEX3(vertexindex vi, const VECTOR2& tc) : I(vi), T(tc) {}


	vertexindex I;			// index vertex in vertex array
	VECTOR2		T;			// texture coords
};




//=============================================================================
//									FACE
//=============================================================================
struct FACE
{
	typedef IVERTEX3::vertexindex vertexindex;
	typedef vectorex<vertexindex> vertexindexlist;
	typedef vectorex<VECTOR3>	vectorlist;
	typedef vectorex<IVERTEX3>	indexedvertexlist;


// constructors
	FACE()
		: texdef(TEXTUREDEF::pACTIVETEXTURE ? TEXTUREDEF::pACTIVETEXTURE : &TEXTUREDEF::DEFAULT),
		  V(VECTOR3::O),
		  W(VECTOR3::O),
		  N(VECTOR3::O),
		  shift(VECTOR2::O),
		  scale(VECTOR2::I),
		  rotate(0)
		{
		}

	FACE(const vertexindex viarr[], vertexindex num)
		: ivlist(num),
		  texdef(TEXTUREDEF::pACTIVETEXTURE ? TEXTUREDEF::pACTIVETEXTURE : &TEXTUREDEF::DEFAULT),
		  V(VECTOR3::O),
		  W(VECTOR3::O),
		  N(VECTOR3::O),
		  shift(VECTOR2::O),
		  scale(VECTOR2::I),
		  rotate(0)
		{
		for(vertexindex i = 0; i < num; ++i)
			ivlist.push_back(IVERTEX3(viarr[i]));
		}

	FACE(const VECTOR3 varr[], const vertexindex viarr[], vertexindex num)
		: ivlist(num),
		  texdef(TEXTUREDEF::pACTIVETEXTURE ? TEXTUREDEF::pACTIVETEXTURE : &TEXTUREDEF::DEFAULT),
		  N(varr[viarr[1]] - varr[viarr[0]] ^ varr[viarr[2]] - varr[viarr[1]]),
		  shift(VECTOR2::O),
		  scale(VECTOR2::I),
		  rotate(0)
		{
		// get best projection plane for faces normal
		const unsigned	vm = ::ViewForPlane(N),
						D1 = ::BASEDIMS[vm][0], D2 = ::BASEDIMS[vm][1];

		// normalize normal
		N.Normalize();

		// init texture directional vectors
		V = ::TEXTUREAXIS[vm][1], W = ::TEXTUREAXIS[vm][2];

		// init index data & generate texture coords (load inverse rotation matrix)
		MATRIX16 irm;
		irm.LoadIdentity();
		irm.MM[0][D1] = V.X/texdef->width, irm.MM[1][D1] = V.Y/texdef->width, irm.MM[2][D1] = V.Z/texdef->width;
		irm.MM[0][D2] = W.X/texdef->height, irm.MM[1][D2] = W.Y/texdef->height, irm.MM[2][D2] = W.Z/texdef->height;
		for(vertexindex i = 0; i < num; ++i)
			ivlist.push_back(IVERTEX3(viarr[i], (varr[viarr[i]] * irm).Project(D1, D2)));
		}

	FACE(const vectorlist& all, const vectorlist& vdata, const FACE& old)
		: ivlist(vdata.size()),
		  texdef(old.texdef),
		  V(old.V), W(old.W), N(old.N),
		  shift(old.shift),
		  scale(old.scale),
		  rotate(old.rotate)
		{
			// init face vertexindex list
			for(vectorlist::const_iterator v = vdata.begin(), pos; v != vdata.end(); ++v)
				if((pos = all.find(*v)) != 0)
					ivlist.push_back(IVERTEX3(std::distance(all.begin(), pos)));
				else
					ASSERT(0);

			// generate texture coords
			GenerateTextureCoords(all, ::ViewForPlane(N));
		}

	// cap face (for clip-brush case)
	FACE(const vectorlist& all, const vectorlist& vdata, TEXTUREDEF* td)
		: ivlist(vdata.size()),
		  texdef(td),
		  V(VECTOR3::O),
		  W(VECTOR3::O),
		  N(vdata[1] - vdata[0] ^ vdata[2] - vdata[1]),
		  shift(VECTOR2::O),
		  scale(VECTOR2::I),
		  rotate(0)
		{
			// get best projection plane for faces normal
			const unsigned	vm = ::ViewForPlane(N),
							D1 = ::BASEDIMS[vm][0], D2 = ::BASEDIMS[vm][1];
			// normalize normal
			N.Normalize();
			// init texture directional vectors
			V = ::TEXTUREAXIS[vm][1], W = ::TEXTUREAXIS[vm][2];

			// init face`s vertex indices from brushes vlist &
			// generate texture coords "in place" (load inverse rotation matrix)
			MATRIX16 irm;
			irm.LoadIdentity();
			irm.MM[0][D1] = V.X/texdef->width, irm.MM[1][D1] = V.Y/texdef->width, irm.MM[2][D1] = V.Z/texdef->width;
			irm.MM[0][D2] = W.X/texdef->height, irm.MM[1][D2] = W.Y/texdef->height, irm.MM[2][D2] = W.Z/texdef->height;

			for(vectorlist::const_iterator v = vdata.begin(), pos; v != vdata.end(); ++v)
				if((pos = all.find(*v)) != 0)
					ivlist.push_back(
					IVERTEX3(std::distance(all.begin(), pos), (*v * irm).Project(D1, D2))
					);
				else
					ASSERT(0);
		}


// updates (normal/VW/tex def/tex coord)
	void UpdateNormal(const vectorlist& vlist)
		{
		N = vlist[ivlist[1].I] - vlist[ivlist[0].I] ^ vlist[ivlist[2].I] - vlist[ivlist[1].I];
		N.Normalize();
		}

	void UpdateVW()
		{
		const unsigned	vm = ::ViewForPlane(N);
		V = ::TEXTUREAXIS[vm][1], W = ::TEXTUREAXIS[vm][2];
		}

	void UpdateTexDef(TEXTUREDEF *newtd)
		{
		texdef = newtd;
		}

	void GenerateTextureCoords(const vectorlist& vlist, unsigned bestpln);
	void UpdateTextureCoords(const vectorlist& vlist, const MATRIX16& m, bool texturelock)
		{
		if(texturelock)
		{
			V.Rotate(m), V.Normalize();						// update V, W
			W.Rotate(m), W.Normalize();
			shift.S -= V.X*m[12] + V.Y*m[13] + V.Z*m[14],	// update shift
			shift.T -= W.X*m[12] + W.Y*m[13] + W.Z*m[14];
			shift.S = (int)shift.S % texdef->width;			// normalize shift
			shift.T = (int)shift.T % texdef->height;
		}
		else
		{
			const unsigned bp = ::ViewForPlane(N);				// get best projection plane
			V = ::TEXTUREAXIS[bp][1], W = ::TEXTUREAXIS[bp][2];	// reinit texture directional
			GenerateTextureCoords(vlist, bp);					// update texture coords
		}
		}

	void MoveTextureCoords(const vectorlist& vlist,
							const VECTOR2& dshift, const VECTOR2& dscale, float drot)
		{
		// inc texture props
		shift += dshift, scale += dscale, rotate += drot;
		GenerateTextureCoords(vlist, ::ViewForPlane(N));
		}

	void JustifyTexture(const vectorlist& vlist, unsigned mode)
		{	// mode: 0 - left, 1 - right, 2 - bottom, 3 - top, 4 - center, 5 - fit
		// get best projection plane
		const int bp = ::ViewForPlane(N), k = bp > 2 ? 1 : -1;

		// build texture box
		POLYGONE3 poly(MakePolygone3(vlist));
		if (!VWHasWorldAxialDir())
		{
			MATRIX16 tvm;
			const VECTOR3 raxis(::TEXTUREAXIS[bp][0]^N);
			if (!raxis.isZero())
			{
				tvm.LoadRotationAxis(raxis, RAD2DEGF(-::Angle(::TEXTUREAXIS[bp][0], N)));
				poly *= tvm;
			}
		}
		const BBOX2 texturebox(&poly[0], poly.size(), ::BASEDIMS[bp][0], ::BASEDIMS[bp][1], rotate);

		// process mode
		switch(mode)
		{
		case 0:	/* left align */
			shift.S = (int)(texturebox.Min.X/scale.S*k) % texdef->width;
			break;
		case 1:	/* right align */
			shift.S = (int)(texturebox.Max.X/scale.S*k) % texdef->width;
			break;
		case 2:	/* bottom align */
			shift.T = -(int)(texturebox.Min.Y/scale.T) % texdef->height;
			break;
		case 3:	/* top align */
			shift.T = -(int)(texturebox.Max.Y/scale.T) % texdef->height;
			break;
		case 4:	/* center align */
			shift.S = (int)( (texturebox.Min.X + (texturebox.Max.X - texturebox.Min.X -
				texdef->width*scale.S)/2 )/scale.S*k) % texdef->width;
			shift.T = -(int)( (texturebox.Min.Y + (texturebox.Max.Y - texturebox.Min.Y -
				texdef->height*scale.T)/2 )/scale.T) % texdef->height;
			break;
		default:/* fit texture */
			// calc S/T scale
			scale.S = (texturebox.Max.X - texturebox.Min.X) / texdef->width;
			scale.T = (texturebox.Max.Y - texturebox.Min.Y) / texdef->height;

			// calc S/T shift
			shift.S = (int)(texturebox.Min.X/scale.S*k) % texdef->width;
			shift.T = (int)(texturebox.Min.Y*scale.T) % texdef->height;
			break;
		}

		GenerateTextureCoords(vlist, bp);
		}

	void AlignTexture(const vectorlist& vlist, unsigned mode)
		{	// mode: 0 - world, 1 - face aligning

			// get best projection plane
			const unsigned bp = ::ViewForPlane(N);

			// set texture axial vectors
			if (mode)
			{
				MATRIX16 tvm(MATRIX16::I);
				const VECTOR3 raxis(::TEXTUREAXIS[bp][0]^N);
				if(!raxis.isZero())
					tvm.LoadRotationAxis(raxis, RAD2DEGF(::Angle(::TEXTUREAXIS[bp][0], N)));
				V = ::TEXTUREAXIS[bp][1] * tvm;
				W = ::TEXTUREAXIS[bp][2] * tvm;
			}
			else
			{
				// reinit texture directional
				V = ::TEXTUREAXIS[bp][1];
				W = ::TEXTUREAXIS[bp][2];
			}

			// update texture coords
			GenerateTextureCoords(vlist, bp);
		}

	bool VWHasWorldAxialDir() const
		{
		for (unsigned i = 0; i < 6; ++i)
			if (::equal(V, ::TEXTUREAXIS[i][1]) && ::equal(W, ::TEXTUREAXIS[i][2]))
				return true;
		return false;
		}


// convert to simple polygon3
	POLYGONE3 MakePolygone3(const vectorlist& vlist) const
		{
		POLYGONE3 ret(ivlist.size());
		for(indexedvertexlist::const_iterator i = ivlist.begin(); i != ivlist.end(); ++i)
			ret.push_back(vlist[i->I]);
		return ret;
		}

// getting params
	BBOX3 GetBBox(const vectorlist& vlist) const
		{	// get real face bbox
		BBOX3 ret;
		for(indexedvertexlist::const_iterator i = ivlist.begin(); i != ivlist.end(); ++i)
			ret.CalcBounds(vlist[i->I]);
		return ret;
		}

	VECTOR3 GetInPoint(const vectorlist& vlist) const
		{	// get inner convex face point
		return VECTOR3( (vlist[ivlist[0].I] +
						((vlist[ivlist[1].I] + vlist[ivlist[2].I]) * 0.5F))
						* 0.5F );
		}

	FLOAT GetArea(const vectorlist& vlist) const
		{	// get convex face NON-SIGNED area
		FLOAT S = 0;
		for(indexedvertexlist::const_iterator a = ivlist.begin(), b = a + 1, c = b + 1;
			c != ivlist.end();
			++b, ++c)
				S += ::abs(::TriangleArea(vlist[a->I], vlist[b->I], vlist[c->I]))
				;
		return S;
		}

	VECTOR3 GetCenter(const vectorlist& vlist) const
		{	// get convex face center
		VECTOR3 g(VECTOR3::O);
		for(indexedvertexlist::const_iterator a = ivlist.begin(), b = a + 1, c = b + 1;
			c != ivlist.end();
			++b, ++c)
				g += ::abs(::TriangleArea(vlist[a->I], vlist[b->I], vlist[c->I])) *
						::TriangleCenter(vlist[a->I], vlist[b->I], vlist[c->I])
				;
		return g /= GetArea(vlist);
		}


// Ray - face intersection
	bool Intersection(const VECTOR3& rayP0, const VECTOR3& rayV,
					  vectorlist& vlist, VECTOR3* q = 0, FLOAT* t = 0) const;

// drawing
	void Draw(const vectorlist& vlist) const
		{
		::glBegin(GL_POLYGON);
		for(indexedvertexlist::const_iterator ivl = ivlist.begin();
			ivl != ivlist.end();
			++ivl)
			::glVertex3fv(vlist[ivl->I]);
		::glEnd();
		}

	void Draw2D(const vectorlist& vlist, VIEWMODE viewmode) const
		{
		switch(viewmode)
		{
		case VM_FRONT:	if(N.Z >= 0) return;
		case VM_LEFT:	if(N.X >= 0) return;
		case VM_BOTTOM:	if(N.Y >= 0) return;
		case VM_BACK:	if(N.Z <= 0) return;
		case VM_RIGHT:	if(N.X <= 0) return;
		case VM_TOP:	if(N.Y <= 0) return;
		}

		// draw polygon
		::glBegin(GL_LINE_LOOP);
		for(indexedvertexlist::const_iterator ivl = ivlist.begin();
			ivl != ivlist.end();
			++ivl)
			::glVertex3fv(vlist[ivl->I]);
		::glEnd();
		}

	void DrawEdgeMarkers(const vectorlist& vlist) const
	{
		//	::glColor3ubv(COLOR3::GREEN);	// NOTE: draw all points
		//	::glBegin(GL_POINTS);
		for(indexedvertexlist::const_iterator ivl1 = ivlist.begin(), ivl2 = ivlist.begin() + 1;
			ivl1 != ivlist.end() && ivl2 != ivlist.end();
			++ivl1, ++ivl2)
				::glVertex3fv((vlist[ivl1->I] + vlist[ivl2->I]) * 0.5F);
		//	::glVertex3fv((vlist[ivlist.front().I] - vlist[ivlist.back().I])/2);	// draw last marker
		//	::glEnd();
	}

	void Clear()
		{
		ivlist.clear();
		}


	indexedvertexlist	ivlist;		// vertex data index list
	TEXTUREDEF			*texdef;	// texture definition
	VECTOR3				V, W, N;	// V, W - directional vectors, N - normal
	VECTOR2				shift;		// texture shift
	VECTOR2				scale;		// texture scale
	FLOAT				rotate;		// texture rotation

	struct _SELECTED
	{
		FACE*			pFace;
		class BRUSH*	pFaceOwner;
	};
	static _SELECTED	SELECTED;		// only one selected face
};




//=============================================================================
//								  BRUSH
//=============================================================================

class BRUSH
{
public:
	typedef FACE::vertexindex vertexindex;
	typedef vectorex<vertexindex> vertexindexlist;
	typedef vectorex<VECTOR3> vectorlist;
	typedef vectorex<VECTOR3*> ptrvectorlist;
	typedef vectorex<IVERTEX3>	indexedvertexlist;
	typedef vectorex<FACE>	facelist;
	typedef vectorex<FACE*>	ptrfacelist;
	typedef vectorex<BRUSH>	brushlist;
	typedef vectorex<BRUSH*> ptrbrushlist;


	enum // brush flags
	{
		BF_SELECTED = 1<<0,
		BF_DELETED = 1<<1,
		BF_HIDDEN = 1<<2,
		BF_IGNORE = 1<<3
	};

	enum eShapeType
	{
		ST_BLOCK,
		ST_WEDGE,
		ST_SPIKE3,
		ST_SPIKE4,
		ST_DSPIKE3,
		ST_DSPIKE4,
			TOTAL_NONCHANGEABLE,
		ST_CYLINDERN = TOTAL_NONCHANGEABLE,
		ST_SPIKEN,
		ST_DSPIKEN,
			CHANGEABLE,
			TOTAL_CHANGEABLE = CHANGEABLE - TOTAL_NONCHANGEABLE
	};


	BRUSH(ENTITY* _Owner = 0, unsigned char _Flag = BF_SELECTED)
		: owner(_Owner), /*ID(++BRUSH::GLOBALCOUNTER),*/ flags(_Flag)
		{
		}

	~BRUSH()
		{
		}

// Initialization
public:
	void InitVertexList(const VECTOR3* varr, vertexindex num)
		{
		vlist.clear();
		vlist.reserve(num);
		for(vertexindex i = 0; i < num; ++i)
			vlist.push_back(varr[i]);
		}

	void AddFace(const vertexindex* viarr, vertexindex num)
		{
		flist.push_back(FACE(viarr, num));
		}

	void AddVertexList(const vectorlist& all)
	{
		for(vectorlist::const_iterator vnew = all.begin(); vnew != all.end(); ++vnew)
		{
			vectorlist::const_iterator vold;
			for(vold = vlist.begin(); vold != vlist.end(); ++vold)
				if(::equal(*vnew, *vold))
					break;
			if(vold == vlist.end())			// if is unique vertex...
				vlist.push_back(*vnew);		// ...add to vertex list
		}
	}

	void AddFace(const vectorlist& vdata, const FACE& old)
		{	// for corner face (cllip-brush case)
		AddVertexList(vdata);
		flist.push_back(FACE(vlist, vdata, old));
		}

	void AddFace(const vectorlist& vdata, TEXTUREDEF* td)
		{	// for capping face (cllip-brush case)
		AddVertexList(vdata);
		flist.push_back(FACE(vlist, vdata, td));
		}

// Ray intersection
public:
	FACE* Intersection(const VECTOR3& rayP0, const VECTOR3& rayV,
						VECTOR3* q = 0, FLOAT* t = 0) const;


// Drawing
	void Draw() const;
	void Draw2D(VIEWMODE viewmode) const;

	void DrawVertices() const		// vertex drag
		{
		//	::glColor3ubv(COLOR3::BLUE);	// NOTE: draw all points
		//	::glBegin(GL_POINTS);
			for(vectorlist::const_iterator v = vlist.begin(); v != vlist.end(); ++v)
				::glVertex3fv(*v);
		//	::glEnd();
		}

	void DrawEdgeMarkers() const	// edge drag
		{
		//	::glColor3ubv(COLOR3::GREEN);
		//	::glBegin(GL_POINTS);
		for(facelist::const_iterator f = flist.begin(); f != flist.end(); ++f)
			f->DrawEdgeMarkers(vlist);
		//	::glEnd();
		}

// Make shapes
	void MakeBlock(const VECTOR3& origin, const VECTOR3& size);
	void MakeShape(unsigned shapetype, const VECTOR3& origin, unsigned nside,
					int gridsize);
	void RebuildShape(MATRIX16& m, const VECTOR3& origin, const VECTOR3& current,
					int gridsize);


// Translation
	void UpdateBBox()
		{
		bbox.Clear();
		for(vectorlist::iterator v = vlist.begin(); v != vlist.end(); ++v)
			bbox.CalcBounds(*v);
		}

	void UpdateNormals()
		{
		for(facelist::iterator f = flist.begin(); f != flist.end(); ++f)
			f->UpdateNormal(vlist);
		}

	void UpdateVWs()
		{
		for(facelist::iterator f = flist.begin(); f != flist.end(); ++f)
			f->UpdateVW();
		}

	void UpdateTexDef(TEXTUREDEF *newtd)
		{
		for(facelist::iterator f = flist.begin(); f != flist.end(); ++f)
			f->texdef = newtd;
		}

	void UpdateID()
		{
		ID = ++BRUSH::GLOBALCOUNTER;
		}

	void Translate(const MATRIX16& m, bool texturelock,
				   bool checkface = false, bool updateID = false)
		{
		for(vectorlist::iterator v = vlist.begin(); v != vlist.end(); ++v)
			*v *= m;
		UpdateBBox();
		UpdateNormals();
		if (checkface)
			CheckFaceOrientation();
		UpdateVWs();
		UpdateTextureCoords(m, texturelock);
		if (updateID)
			UpdateID();
		}



	void Align(const BBOX3& unibox, unsigned mode, bool texturelock)
		{	// mode: 0 - front, 1 - back, 2 - left, 3 - right, 4 - bottom, 5 - top
			MATRIX16 m;

			switch (mode)
			{
			case 0:	/* front */
				m.LoadTranslation(0, 0, unibox.Max.Z - bbox.Max.Z);
				break;
			case 1:	/* back */
				m.LoadTranslation(0, 0, unibox.Min.Z - bbox.Min.Z);
				break;
			case 2:	/* left */
				m.LoadTranslation(unibox.Min.X - bbox.Min.X, 0, 0);
				break;
			case 3:	/* right */
				m.LoadTranslation(unibox.Max.X - bbox.Max.X, 0, 0);
				break;
			case 4:	/* bottom */
				m.LoadTranslation(0, unibox.Min.Y - bbox.Min.Y, 0);
				break;
			default:/* 5 - top */
				m.LoadTranslation(0, unibox.Max.Y - bbox.Max.Y, 0);
				break;
			}

			Translate(m, texturelock);
		}

	void Flip(unsigned mode, bool texturelook)
		{	// mode: 0 - by X, 1 - by Y, 2 - by Z
			MATRIX16 m;

			switch (mode)
			{
			case 0:	/* by X */
				m.LoadMirrorPlaneXY(bbox.GetCenter());
				break;
			case 1:	/* by Y */
				m.LoadMirrorPlaneZY(bbox.GetCenter());
				break;
			default:/* 2 - by Z */
				m.LoadMirrorPlaneXZ(bbox.GetCenter());
				break;
			}

			Translate(m, texturelook, true); // NOTE: (for flipping NEED CheckFaceOrientation()!)
		}

	void CheckFaceOrientation()
		{
		const VECTOR3 inpoint(GetInPoint());
		for(facelist::iterator f = flist.begin(); f != flist.end(); ++f)
			if((inpoint - vlist[f->ivlist[0].I] | f->N) > 0)
			{
				f->ivlist.reverse();
				f->N = -f->N;			// do correct normals!
			}
		}

// Texturing
	void GenerateTextureCoords()
		{	// :NOTE: faces normals must be alredy calc!
		for(facelist::iterator f = flist.begin(); f != flist.end(); ++f)
			f->GenerateTextureCoords(vlist, ::ViewForPlane(f->N));
		}

	void UpdateTextureCoords(const MATRIX16& m, bool texturelock)
		{	// :NOTE: faces normals must be alredy calc!
		for(facelist::iterator f = flist.begin(); f != flist.end(); ++f)
			f->UpdateTextureCoords(vlist, m, texturelock);
		}

	void MoveTextureCoords(const VECTOR2& newshift, const VECTOR2& newscale, float newrot)
		{
		for(facelist::iterator f = flist.begin(); f != flist.end(); ++f)
			f->MoveTextureCoords(vlist, newshift, newscale, newrot);
		}

	// get inner brush point (convex version!)
	VECTOR3 GetInPoint() const
		{
		return VECTOR3((flist[0].GetInPoint(vlist) + flist[1].GetInPoint(vlist)) * 0.5F);
		}

	void Clear()
		{
		vlist.clear();
		for(facelist::iterator f = flist.begin(); f != flist.end(); ++f)
			f->Clear();
		flist.clear();
		}

// Make/unmake entity
	void MakeEntity(const ENTITYINFO& einfo)
	{
		if(owner)
			*owner = einfo;
		else
			owner = new ENTITY(einfo);
	}

	void UnmakeEntity()
	{
		delete owner;
		owner = 0;
	}

// 2d ortho test
	bool IsInsideIn(const BBOX2& bov, unsigned nDim1, unsigned nDim2) const
	{
		return !(bbox.Max[nDim1] < bov.Min.X || bov.Max.X < bbox.Min[nDim1] ||
				 bbox.Max[nDim2] < bov.Min.Y || bov.Max.Y < bbox.Min[nDim2]);
	}



// Check brush error
	bool isConvex() const;
	void Optimize();



// Implementation
public:
	vectorlist		vlist;
	facelist		flist;
	BBOX3			bbox;
	ENTITY			*owner;
	unsigned		ID;
	unsigned char	flags;


// static
public:
//=============================== WORLD BRUSH LIST ===========================
	static brushlist WORLDBRUSHLIST;

	// gabarite test
	static BRUSH*
	GetSelectionBoxInWorldBrushList(const VECTOR3& rayP0, const VECTOR3& rayV);
	// exact polygonal test
	static BRUSH*
	GetSelectionInWorldBrushList(const VECTOR3& rayP0, const VECTOR3& rayV, FACE** f = 0);
	static bool SelectByOwner(const ENTITY* pOwner);

	// find
	static BRUSH* FindBrushByID(unsigned _ID);

	// delete
	static void DeleteBrushByID(unsigned _ID);

	// check face texture inuse (TEXTUREDEF::bInUse)
	static void SetTextureInUse();
	static bool ReplaceTextures(const CString& findpath, const CString& replacepath,
								BOOL selonly);

	// hide/show
	static void UnhideAll();

	// statistics
	static void GetWorldInfo(UINT& nSolids, UINT& nFaces,
							UINT& nPointEntities, UINT& nSolidEntities,
							UINT& nUniqueTextures, UINT& nTextureMemory,
							CStringArray& texpathes);

	// check fo errors
	static bool FindPlayerStart();
	static bool FindInvalidBrushes(ptrbrushlist& blist);
	static BRUSH* FindWorldspawn();
//=============================================================================

//============================ SELECTED PTR BRUSH LIST ========================
	static ptrbrushlist SELECTEDBRUSHLIST;

	static void ClearSelectedBrushList();
	static void SelectAll();
	static void ReInitSelectedBrushList();	// if BRUSH*-pointers has non-valid
	static void TransformSelectedBrushList(const VECTOR3& val, unsigned mode, bool texturelock);
	static void TranslateSelectedBrushList(const MATRIX16& m, bool texturelock);
	static bool AlignSelectedBrushList(unsigned mode, bool texturelock);
	static void FlipSelectedBrushList(unsigned mode, bool texturelock);
	static void DeleteSelectedBrushList();
	static void HideSelectedBrushList();
	static void HideUnselectedBrushList();

	static bool IsSelected(const BRUSH& b)	// predicat for std::remove_if (for delete brushes)
		{ return b.flags & BF_SELECTED; }
	static bool IsDeleted(const BRUSH& b)	// predicat for std::remove_if (for delete brushes)
		{ return b.flags & BF_DELETED; }
	static void CloneSelectedBrushList(const VECTOR3& move, unsigned num, bool texturelock);
	static void GroupSelectionInWorldBrushList(const BBOX3& selbox,
												unsigned projplane, int gridsize);
	static void MoveTextureCoordsInSelectionBrushList(
		const VECTOR2& newshift, const VECTOR2& newscale, float newrot
		);
	static void UpdateTexDefInSelectionBrushList(TEXTUREDEF *newtd);
	static unsigned GetSelectionUnibox(BBOX3& unibox);
	static unsigned GetSelectionSize(VECTOR3& size);
//=============================================================================

//============================= SELECTED VERTEX LIST ==========================
	static ptrvectorlist SELECTEDVERTEXLIST;
	static bool BuildSelectedVertexList(const VECTOR2& mousehit, unsigned d1, unsigned d2);
	static bool BuildSelectedEdgeList(const VECTOR2& mousehit, unsigned d1, unsigned d2);
	static void DragSelectedVertexList(const VECTOR3& offset,
										unsigned d1, unsigned d2, int gridstep, bool snaptogrid);
//=============================================================================


//==================================== UNDO ===================================
	static unsigned GLOBALCOUNTER;
//=============================================================================


// friends
	template<int (*ORIENTATIONPOINTPFN)(const POLYGONE2&, const VECTOR2&)>
	friend FACE* IntersectionRay(const BRUSH& brush,
								 const VECTOR3& rayP0, const VECTOR3& rayV,
								 VECTOR3* q, FLOAT* t);
};

template<int (*ORIENTATIONPOINTPFN)(const POLYGONE2&, const VECTOR2&)>
FACE* IntersectionRay(const BRUSH& brush, const VECTOR3& rayP0, const VECTOR3& rayV,
							 VECTOR3* q = 0, FLOAT* t = 0);


inline void BRUSH::Draw2D(VIEWMODE viewmode) const
{
	for(facelist::size_type f = 0, fsize = flist.size(); f < fsize; ++f)
	{
		switch(viewmode)
		{
		case VM_FRONT:	if(flist[f].N.Z >= 0) continue; break;
		case VM_LEFT:	if(flist[f].N.X >= 0) continue; break;
		case VM_BOTTOM:	if(flist[f].N.Y >= 0) continue; break;
		case VM_BACK:	if(flist[f].N.Z <= 0) continue; break;
		case VM_RIGHT:	if(flist[f].N.X <= 0) continue; break;
		case VM_TOP:	if(flist[f].N.Y <= 0) continue; break;
		}

		::glBegin(GL_LINE_LOOP);
		for(vectorlist::size_type v = 0, vsize = flist[f].ivlist.size(); v < vsize; ++v)
			::glVertex3fv(vlist[flist[f].ivlist[v].I]);
		::glEnd();
	}
}

inline void BRUSH::Draw() const
{
	for(facelist::size_type f = 0, fsize = flist.size(); f < fsize; ++f)
	{
		::glBegin(GL_POLYGON);
		for(vectorlist::size_type v = 0, vsize = flist[f].ivlist.size(); v < vsize; ++v)
			::glVertex3fv(vlist[flist[f].ivlist[v].I]);
		::glEnd();
	}
}

inline void BRUSH::ClearSelectedBrushList()
{
	for(ptrbrushlist::iterator b = SELECTEDBRUSHLIST.begin();
		b != SELECTEDBRUSHLIST.end();
		++b)
			(*b)->flags &= ~BF_SELECTED;
	BRUSH::SELECTEDBRUSHLIST.clear();
}

inline void BRUSH::SelectAll()
{
	// first clear selected brush list
	BRUSH::SELECTEDBRUSHLIST.clear();
	BRUSH::SELECTEDBRUSHLIST.reserve(WORLDBRUSHLIST.size());

	for(brushlist::iterator b = WORLDBRUSHLIST.begin();
		b != WORLDBRUSHLIST.end();
		++b)
	{
		// set flag "selected" on brush
		b->flags |= BF_SELECTED;
		SELECTEDBRUSHLIST.push_back(&*b);
	}
}

inline void BRUSH::ReInitSelectedBrushList()
{
	SELECTEDBRUSHLIST.clear();
	for(brushlist::iterator b = WORLDBRUSHLIST.begin();
		b != WORLDBRUSHLIST.end();
		++b)
	{
		if(b->flags & BF_SELECTED)
		{
			SELECTEDBRUSHLIST.push_back(&*b);
		}
	}
}

inline void BRUSH::TranslateSelectedBrushList(const MATRIX16& m, bool texturelock)
{
	for(ptrbrushlist::iterator b = SELECTEDBRUSHLIST.begin();
		b != SELECTEDBRUSHLIST.end();
		++b)
			(*b)->Translate(m, texturelock);
}

inline void BRUSH::TransformSelectedBrushList(const VECTOR3& val,
											  unsigned mode, bool texturelock)
{
	if (!val.isZero())
	{
		// calc union brush-box
		BBOX3 unibox;
		for (BRUSH::ptrbrushlist::iterator pB = BRUSH::SELECTEDBRUSHLIST.begin();
			pB != BRUSH::SELECTEDBRUSHLIST.end();
			++pB)
				unibox += (*pB)->bbox;

		MATRIX16 m;

        m.LoadTranslation(unibox.GetCenter());
		switch (mode)
		{
		case 0:	/* rotate */
			if (val[0])	m.RotationX(val[0]);
			if (val[1])	m.RotationY(val[1]);
			if (val[2])	m.RotationZ(val[2]);
			break;
		case 1:	/* scale */
			m.Scale(val[0] ? val[0] : 1, val[1] ? val[1] : 1, val[2] ? val[2] : 1);
			break;
		default:/* 2 - move */
			m.Translate(val);
			break;
		}
		m.Translate(-unibox.GetCenter());

		BRUSH::TranslateSelectedBrushList(m, texturelock);
	}
}

inline bool BRUSH::AlignSelectedBrushList(unsigned mode, bool texturelock)
{
	if (BRUSH::SELECTEDBRUSHLIST.size() > 1)
	{
		// calc union brush-box
		BBOX3 unibox;
		for (BRUSH::ptrbrushlist::iterator pB = BRUSH::SELECTEDBRUSHLIST.begin();
			pB != BRUSH::SELECTEDBRUSHLIST.end();
			++pB)
				unibox += (*pB)->bbox;

		// align selected brush list for unibox
		for (BRUSH::ptrbrushlist::iterator pB = BRUSH::SELECTEDBRUSHLIST.begin();
			pB != BRUSH::SELECTEDBRUSHLIST.end();
			++pB)
				(*pB)->Align(unibox, mode, texturelock);

		return true;	// need redraw
	}

	return false;	// no redraw
}

inline void BRUSH::FlipSelectedBrushList(unsigned mode, bool texturelock)
{
	for (BRUSH::ptrbrushlist::iterator pB = BRUSH::SELECTEDBRUSHLIST.begin();
		pB != BRUSH::SELECTEDBRUSHLIST.end();
		++pB)
			(*pB)->Flip(mode, texturelock);
}

inline void BRUSH::DeleteSelectedBrushList()
{
	WORLDBRUSHLIST.erase(
		std::remove_if(WORLDBRUSHLIST.begin(), WORLDBRUSHLIST.end(), BRUSH::IsSelected),
		WORLDBRUSHLIST.end()
		);
	SELECTEDBRUSHLIST.clear();
}

inline void BRUSH::HideSelectedBrushList()
{
	for(ptrbrushlist::iterator b = SELECTEDBRUSHLIST.begin();
		b != SELECTEDBRUSHLIST.end();
		++b)
	{
		(*b)->flags |= BRUSH::BF_HIDDEN;	// set "hidden" flag
		(*b)->flags &= ~BF_SELECTED;		// unset "selected" flag
	}

	BRUSH::SELECTEDBRUSHLIST.clear();
}

inline void BRUSH::HideUnselectedBrushList()
{
	for(brushlist::iterator b = WORLDBRUSHLIST.begin();
		b != WORLDBRUSHLIST.end();
		++b)
			if (!(b->flags & BRUSH::BF_SELECTED))	// if non-select
				b->flags |= BRUSH::BF_HIDDEN;
}

inline void BRUSH::UnhideAll()
{
	for(brushlist::iterator b = WORLDBRUSHLIST.begin();
		b != WORLDBRUSHLIST.end();
		++b)
			if (b->flags & BRUSH::BF_HIDDEN)
				b->flags &= ~BRUSH::BF_HIDDEN;		// unset "hidden" flag
}

inline
void BRUSH::CloneSelectedBrushList(const VECTOR3& move, unsigned num, bool texturelock)
{
	// *** SET WORLDBRUSHLIST SIZE ***
	// if size() == capacity() next push get realloc vector
	// and all pointer in SELECTEDBRUSHLIST would be nonvalid
	const size_t needsize = SELECTEDBRUSHLIST.size()*num;
	if(WORLDBRUSHLIST.capacity() - WORLDBRUSHLIST.size() <= needsize)
	{
		WORLDBRUSHLIST.reserve(WORLDBRUSHLIST.size() + needsize);
		BRUSH::ReInitSelectedBrushList();
	}

	// *** MAKE CLONE BRUSHES ***
	MATRIX16 m;
	m.LoadTranslation(move);
	for(unsigned i = 0; i < num; ++i)
		for(ptrbrushlist::iterator b = SELECTEDBRUSHLIST.begin();
			b != SELECTEDBRUSHLIST.end();
			++b)
		{
			WORLDBRUSHLIST.push_back(**b);
			WORLDBRUSHLIST.back().UpdateID();
			WORLDBRUSHLIST.back().flags |= BF_SELECTED;
			WORLDBRUSHLIST.back().Translate(m, texturelock);
		}

	// *** CLEAR SELECTED BRUSHLIST ***
	BRUSH::ClearSelectedBrushList();
	BRUSH::ReInitSelectedBrushList();
}

inline
void BRUSH::GroupSelectionInWorldBrushList(const BBOX3& selbox,
										   unsigned projplane, int gridsize)
{
	// 2D INFINITY SELECTION MODE
	if(selbox.Size()[::BASEDIMS[projplane][2]] <= gridsize)
	{	// if selection box depth <= gridsize: ortho-depth infinity selection
		const BBOX2 selbox2d(selbox.Project((eProjectPlane)projplane));
		for(brushlist::iterator b = WORLDBRUSHLIST.begin();
			b != WORLDBRUSHLIST.end();
			++b)
				if(::isIntersect2D(b->bbox, selbox2d, projplane, true))
					b->flags |= BF_SELECTED
					;
	}
	// 3D BBOX REAL INSIDE SELECTION MODE
	else	// else: real gabarite selection
		for(brushlist::iterator b = WORLDBRUSHLIST.begin();
			b != WORLDBRUSHLIST.end();
			++b)
				if(::isIntersect(b->bbox, selbox, true))
					b->flags |= BF_SELECTED
					;
}

inline
void BRUSH::MoveTextureCoordsInSelectionBrushList(const VECTOR2& newshift,
												  const VECTOR2& newscale, float newrot)
{
	for(ptrbrushlist::iterator b = SELECTEDBRUSHLIST.begin();
		b != SELECTEDBRUSHLIST.end();
		++b)
			(*b)->MoveTextureCoords(newshift, newscale, newrot);
}

inline
void BRUSH::UpdateTexDefInSelectionBrushList(TEXTUREDEF *newtd)
{
	for(ptrbrushlist::iterator b = SELECTEDBRUSHLIST.begin();
		b != SELECTEDBRUSHLIST.end();
		++b)
			(*b)->UpdateTexDef(newtd);
}

inline
unsigned BRUSH::GetSelectionUnibox(BBOX3& unibox)
{
	for(ptrbrushlist::iterator b = SELECTEDBRUSHLIST.begin();
		b != SELECTEDBRUSHLIST.end();
		++b)
			unibox += (*b)->bbox;
	return SELECTEDBRUSHLIST.size();
}

inline
unsigned BRUSH::GetSelectionSize(VECTOR3& size)
{
	BBOX3 unibox;
	for(ptrbrushlist::iterator b = SELECTEDBRUSHLIST.begin();
		b != SELECTEDBRUSHLIST.end();
		++b)
			unibox += (*b)->bbox;
	size = unibox.Size();
	return SELECTEDBRUSHLIST.size();
}

inline
bool BRUSH::BuildSelectedVertexList(const VECTOR2& mousehit, unsigned d1, unsigned d2)
{
	BRUSH::SELECTEDVERTEXLIST.clear();
	for(ptrbrushlist::const_iterator b = SELECTEDBRUSHLIST.begin();
		b != SELECTEDBRUSHLIST.end();
		++b)
		for(vectorlist::const_iterator v = (*b)->vlist.begin(); v != (*b)->vlist.end(); ++v)
		{
			const VECTOR2 v2d((*v)[d1], (*v)[d2]);
			if(::isIntersect(mousehit, v2d, false, 5.F))
				BRUSH::SELECTEDVERTEXLIST.push_back(const_cast<VECTOR3*>(&*v));
		}

	return !BRUSH::SELECTEDVERTEXLIST.empty();
}

inline
bool BRUSH::BuildSelectedEdgeList(const VECTOR2& mousehit, unsigned d1, unsigned d2)
{
	BRUSH::SELECTEDVERTEXLIST.clear();

	// for all selected brush
	for(ptrbrushlist::const_iterator b = SELECTEDBRUSHLIST.begin();
		b != SELECTEDBRUSHLIST.end();
		++b)
		// for all brush face
		for(facelist::const_iterator f = (*b)->flist.begin(); f != (*b)->flist.end(); ++f)
			// for all ivertex in face
			for(indexedvertexlist::const_iterator ivl1 = f->ivlist.begin(), ivl2 = f->ivlist.begin() + 1;
				ivl1 != f->ivlist.end() && ivl2 != f->ivlist.end();
				++ivl1, ++ivl2)
			{
				const VECTOR3	*v1 = &(*b)->vlist[ivl1->I],
								*v2 = &(*b)->vlist[ivl2->I],
								mid((*v1 + *v2) * 0.5F);
				const VECTOR2	mid2d(mid[d1], mid[d2]);

				if(::isIntersect(mousehit, mid2d, false, 5.F))
				{
					// check unique in selected vertexlist
					for(ptrvectorlist::const_iterator pv = BRUSH::SELECTEDVERTEXLIST.begin();
						pv != BRUSH::SELECTEDVERTEXLIST.end();
						++pv)
						if(::equal(**pv, *v1) || ::equal(**pv, *v2))
							goto outerloop;

					// push if unique
					BRUSH::SELECTEDVERTEXLIST.push_back(const_cast<VECTOR3*>(v1));
					BRUSH::SELECTEDVERTEXLIST.push_back(const_cast<VECTOR3*>(v2));
				}
			outerloop:;
			}

	return !BRUSH::SELECTEDVERTEXLIST.empty();
}

inline
void BRUSH::DragSelectedVertexList(const VECTOR3& offset,
									unsigned d1, unsigned d2, int gridstep, bool snaptogrid)
{
	for(ptrvectorlist::iterator pv = BRUSH::SELECTEDVERTEXLIST.begin();
		pv != BRUSH::SELECTEDVERTEXLIST.end();
		++pv)
	{
		**pv += offset;
		if(snaptogrid)
		{
			(**pv)[d1] = ::floor((**pv)[d1] / gridstep + 0.5F) * gridstep;
			(**pv)[d2] = ::floor((**pv)[d2] / gridstep + 0.5F) * gridstep;
		}
	}
}

inline
bool BRUSH::SelectByOwner(const ENTITY* pOwner)
{
	bool bResult = false;
	for(brushlist::iterator b = WORLDBRUSHLIST.begin();
		b != WORLDBRUSHLIST.end();
		++b)
	{
		if(pOwner == b->owner)
		{
			b->flags |= BRUSH::BF_SELECTED;
			bResult = true;
		}
	}

	return bResult;
}


// find
inline BRUSH* BRUSH::FindBrushByID(unsigned _ID)
{
	for(brushlist::iterator b = WORLDBRUSHLIST.begin();
		b != WORLDBRUSHLIST.end();
		++b)
			if(b->ID == _ID)
				return &*b;
	return 0;
}

// delete
inline void BRUSH::DeleteBrushByID(unsigned _ID)
{
	if(BRUSH* pb = BRUSH::FindBrushByID(_ID))
		WORLDBRUSHLIST.erase(pb);
}

// check face texture inuse (TEXTUREDEF::bInUse)
inline void BRUSH::SetTextureInUse()
{
	// for all brush
	for (brushlist::iterator b = WORLDBRUSHLIST.begin();
		b != WORLDBRUSHLIST.end();
		++b)
		// for all faces in brush
		for (facelist::iterator f = b->flist.begin(); f != b->flist.end(); ++f)
			if(!f->texdef->bInUse)
				f->texdef->bInUse = true;
}

inline bool BRUSH::ReplaceTextures(const CString& findpath, const CString& replacepath,
									BOOL selonly)
{
	TEXTUREDEF *pFindDef, *pReplaceDef;
	CString strDirName, strTexName;
	bool bRedraw = false;

	/* 1. FIND FINDING TEXDEFINITION */
	// if has dir & name -- parse it
	if (findpath.Find('//') != -1)
	{
		int nPos = 0;
		// parse dir
		strDirName = findpath.Tokenize("//", nPos);
		ASSERT(!strDirName.IsEmpty());
		// parse name
		strTexName = findpath.Mid(nPos);
		ASSERT(!strTexName.IsEmpty());
		// find finding texturedef by name and texdir-name
		if (!(pFindDef = TEXTUREDIR::FindTexDefInTDCache(strTexName, strDirName)))
			return false;
	}
	// try find by name only
	else if (!(pFindDef = TEXTUREDIR::FindTexDefInTDCache(findpath, 0)))
		return false;

	/* 2. FIND REPLACE TEXDEFINITION */
	// if has dir & name -- parse it
	if (replacepath.Find('//') != -1)
	{
		int nPos = 0;
		// parse dir
		strDirName = replacepath.Tokenize("//", nPos);
		ASSERT(!strDirName.IsEmpty());
		// parse name
		strTexName = replacepath.Mid(nPos);
		ASSERT(!strTexName.IsEmpty());
		// find finding texturedef by name and texdir-name
		if (TEXTUREDIR *pReplaceDir = TEXTUREDIR::FindTexDirInTDCache(strDirName))
		{
			if (pReplaceDef = pReplaceDir->Find2(strTexName))
				;
			else
			{
				// create new empty texture in this texture directory
				TEXTUREDEF newTDef(TEXTUREDEF::DEFAULT);
				newTDef.name = strTexName;
				// add to dir
				pReplaceDir->Add(newTDef);
				// get replace-def pointer
				pReplaceDef = pReplaceDir->GetBack();
				// avoid texture unbinding in destructor
				newTDef.texID = 0;
			}
		}
		else
		{
			// TODO: create new texture directory & empty texture
			return false;
		}
	}
	// try find by name only
	else if (!(pReplaceDef = TEXTUREDIR::FindTexDefInTDCache(replacepath, 0)))
		return false;

	/* 3. FIND & REPLACE */
	// for all brush
	for (brushlist::iterator b = WORLDBRUSHLIST.begin();
		b != WORLDBRUSHLIST.end();
		++b)
	{
		if (selonly && !(b->flags & BRUSH::BF_SELECTED))
			continue;

		// for all faces in brush
		for (facelist::iterator f = b->flist.begin(); f != b->flist.end(); ++f)
			if (f->texdef == pFindDef)
			{
				f->UpdateTexDef(pReplaceDef);
				bRedraw = true;
			}
	}

	return bRedraw;
}

inline void BRUSH::GetWorldInfo(UINT& nSolids, UINT& nFaces,
								UINT& nPointEntities, UINT& nSolidEntities,
								UINT& nUniqueTextures, UINT& nTextureMemory,
								CStringArray& texpathes)
{
	// RESET
	nSolids = nFaces = nPointEntities = nSolidEntities =
	nUniqueTextures = nTextureMemory = 0;

	// GET GEOMETRY INFO
	for (brushlist::iterator b = WORLDBRUSHLIST.begin();
		b != WORLDBRUSHLIST.end();
		++b)
	{
		// inc solids
		++nSolids;
		// inc faces
		nFaces += b->flist.size();
		// calc entity
		if (b->owner)
			b->owner->color == ENTITYINFO::BRUSHCOLOR ? ++nSolidEntities : ++nPointEntities;
	}


	// GET TEXTURE INFO
	BRUSH::SetTextureInUse();	// check "inuse" props for all textures

	for (TEXTUREDIR::texturedirlistiterator tdr = TEXTUREDIR::CACHE.begin();
		tdr != TEXTUREDIR::CACHE.end();
		++tdr)
		for (TEXTUREDIR::texturedeflistiterator tdf = tdr->cache.begin();
			tdf != tdr->cache.end();
			++tdf)
		{
			if (tdf->bInUse)
			{
				++nUniqueTextures;
				nTextureMemory += tdf->width*tdf->height*3;
				texpathes.Add(tdr->name + '/' + tdf->name);
			}
		}
}

inline bool BRUSH::FindPlayerStart()
{
	for (brushlist::const_iterator b = WORLDBRUSHLIST.begin();
		b != WORLDBRUSHLIST.end();
		++b)
		if (b->owner &&
			b->owner->color != ENTITYINFO::BRUSHCOLOR &&
			b->owner->name == "info_player_start")
			return true;
	return false;
}

inline bool BRUSH::FindInvalidBrushes(ptrbrushlist& blist)
{
	for (brushlist::iterator b = WORLDBRUSHLIST.begin();
		b != WORLDBRUSHLIST.end();
		++b)
		if (!b->isConvex())
			blist.push_back(&*b);
	return !blist.empty();
}

inline BRUSH* BRUSH::FindWorldspawn()
{
	for (brushlist::iterator b = WORLDBRUSHLIST.begin();
		b != WORLDBRUSHLIST.end();
		++b)
		if (b->owner &&
			b->owner->color != ENTITYINFO::BRUSHCOLOR &&
			b->owner->name == "worldspawn")
			return &*b;
	return 0;
}


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//								BRUSH - PLANE CLIPPING
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
inline
POLYGONE3 BuildBrushCap(const BRUSH& clipped, const IMPLICITPLANE& clipplane)
{
	LINESEG3LIST seglist(4);

	// BUILD COINCIDED CLIPPLANE EDGE-CHAIN
	for(BRUSH::facelist::const_iterator f = clipped.flist.begin(); f != clipped.flist.end(); ++f)
		for(FACE::indexedvertexlist::const_iterator ivl1 = f->ivlist.begin(), ivl2 = f->ivlist.begin() + 1;
			ivl1 != f->ivlist.end();
			++ivl1, ++ivl2 == f->ivlist.end() ? ivl2 = f->ivlist.begin() : ivl2)
		{
			const float d1 = (clipped.vlist[ivl1->I]|clipplane.N) + clipplane.D;
			if(FLOAT_EQ(d1, 0.F))
			{
				const float d2 = (clipped.vlist[ivl2->I]|clipplane.N) + clipplane.D;
				if(FLOAT_EQ(d2, 0.F))
					seglist.push_back(LINESEG3(clipped.vlist[ivl1->I], clipped.vlist[ivl2->I]));
			}
		}

	// BUILD CLOSED CIRCUIT FROM EDGE-CHAIN
	LINESEG3LIST tmp(seglist), dstlist(4);
	dstlist.push_back(*tmp.begin()), tmp.erase(tmp.begin());	// move seg
	for(LINESEG3LIST::iterator i = tmp.begin(); i != tmp.end() && !tmp.empty(); ++i)
	{
		switch(dstlist.isConnect(*i))
		{
		case 2: i->Swap();
		case 1:
			dstlist.push_back(*i), tmp.erase(i);				// move seg
			i = tmp.begin() - 1;
			break;
		}
	}

	// BUILD POLYGONE FROM CLOSED CIRCUIT
	return POLYGONE3(dstlist);
}

inline
bool SplitBrush(const BRUSH& b, const VECTOR3& pa, const VECTOR3& pb, const VECTOR3& pc,
				BRUSH& dst1, BRUSH& dst2)
{
	const IMPLICITPLANE plane(pa, pb, pc, false);
	if(::isIntersect(plane, b.vlist))
	{
		// for all brush faces
		for(BRUSH::facelist::const_iterator f = b.flist.begin(); f != b.flist.end(); ++f)
		{
			POLYGONE3 poly(f->MakePolygone3(b.vlist)), p1(4), p2(4);
			if(::Intersection(poly, plane, p1, p2))
			{
				dst1.AddFace(p1, *f);
				dst2.AddFace(p2, *f);
			}
			else
				switch(::OrientationConv(plane, poly.Center()))
				{
				case -1: dst1.AddFace(poly, *f); break;
				case 1:  dst2.AddFace(poly, *f); break;
				}
		}

		// UPDATES
		dst1.UpdateNormals();
		dst1.UpdateBBox();
		dst1.GenerateTextureCoords();
		dst1.UpdateID();
		dst2.UpdateNormals();
		dst2.UpdateBBox();
		dst2.GenerateTextureCoords();
		dst2.UpdateID();

		// BUILD CAPS
		POLYGONE3 cap(::BuildBrushCap(dst1, plane));
		if((dst1.GetInPoint() - cap[0] | cap.GetNormal()) < 0)	// NOTE: define cap orientation
		{
			dst1.AddFace(cap, TEXTUREDEF::pACTIVETEXTURE);
			cap.reverse();
			dst2.AddFace(cap, TEXTUREDEF::pACTIVETEXTURE);
		}
		else
		{
			dst2.AddFace(cap, TEXTUREDEF::pACTIVETEXTURE);
			cap.reverse();
			dst1.AddFace(cap, TEXTUREDEF::pACTIVETEXTURE);
		}
		return true;
	}
	else
		return false;
}


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//								UNION BRUSH
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
inline bool UnionBrush(const BRUSH& b1, const BRUSH& b2, BRUSH& dst)
{
	if(!::isIntersect(b1.bbox, b2.bbox, false, EPSILON))
		return false;

	dst.vlist.reserve(b1.vlist.size() + 4);
	dst.flist.reserve(b1.flist.size() + b2.flist.size());

	for(BRUSH::facelist::const_iterator f1 = b1.flist.begin();
		f1 != b1.flist.end();
		++f1)
	{
		for(BRUSH::facelist::const_iterator f2 = b2.flist.begin();
			f2 != b2.flist.end();
			++f2)
				if(f1->ivlist.size() == f2->ivlist.size() && ::equal(f1->N|f2->N, -1.F))
				{
					size_t eq = f1->ivlist.size();
					for(FACE::indexedvertexlist::const_iterator ivl1 = f1->ivlist.begin();
						ivl1 != f1->ivlist.end();
						++ivl1)
						for(FACE::indexedvertexlist::const_iterator ivl2 = f2->ivlist.begin();
							ivl2 != f2->ivlist.end();
							++ivl2)
								if(::equal(b1.vlist[ivl1->I], b2.vlist[ivl2->I]))
									--eq;

					if(eq == 0)
						goto nextface1;
				}

		dst.AddFace(f1->MakePolygone3(b1.vlist), *f1);
nextface1:;
	}


	for(BRUSH::facelist::const_iterator f2 = b2.flist.begin();
		f2 != b2.flist.end();
		++f2)
	{
		for(BRUSH::facelist::const_iterator f1 = b1.flist.begin();
			f1 != b1.flist.end();
			++f1)
				if(f1->ivlist.size() == f2->ivlist.size() && ::equal(f1->N|f2->N, -1.F))
				{
					size_t eq = f2->ivlist.size();
					for(FACE::indexedvertexlist::const_iterator ivl2 = f2->ivlist.begin();
						ivl2 != f2->ivlist.end();
						++ivl2)
						for(FACE::indexedvertexlist::const_iterator ivl1 = f1->ivlist.begin();
							ivl1 != f1->ivlist.end();
							++ivl1)
								if(::equal(b1.vlist[ivl1->I], b2.vlist[ivl2->I]))
									--eq;

					if(eq == 0)
						goto nextface2;
				}

		dst.AddFace(f2->MakePolygone3(b2.vlist), *f2);
nextface2:;
	}

	if(dst.flist.size() != b1.flist.size() + b2.flist.size())
	{
		dst.Optimize();
		dst.UpdateBBox();
		dst.UpdateID();
		return true;
	}

	return false;
}



// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//								CARVE BRUSH
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// тест ориентации точки p относительно грани браша
inline int OrientationConv(const FACE& face, const VECTOR3& f0, const VECTOR3& p)
	{	// return: 1 - forward, -1 - backward, 0 - on face
	const FLOAT d = (p - f0) | face.N;
	return	-EPSILON < d && d < EPSILON ? 0 :	// on poly-plane
	d > 0 ? 1 :									// at right
	-1;											// at left
	}
// тест ориентации точки p относительно ¬џѕ” Ћќ√ќ браша
inline int OrientationConv(const BRUSH& b, const VECTOR3& p)
	{	// return: 1 - outside, -1 - inside, 0 - on brush edge
	int LER[3] = { 0, 0, 0 };
	for(BRUSH::facelist::const_iterator f = b.flist.begin(); f != b.flist.end(); ++f)
	{
		LER[::OrientationConv(*f, b.vlist[f->ivlist[0].I], p) + 1] = 1;
		if(LER[0] && LER[2])
			return 1;
	}
	return LER[1] - 1;
	}

inline bool CarveBrush(const BRUSH& eff, const BRUSH& trg, BRUSH::brushlist& cliplist)
{
	// check brushes bbox`s intersection
	if(	eff.bbox.Min.X >= trg.bbox.Max.X - EPSILON ||
			eff.bbox.Max.X <= trg.bbox.Min.X + EPSILON ||
		eff.bbox.Min.Y >= trg.bbox.Max.Y - EPSILON ||
			eff.bbox.Max.Y <= trg.bbox.Min.Y + EPSILON ||
		eff.bbox.Min.Z >= trg.bbox.Max.Z - EPSILON ||
			eff.bbox.Max.Z <= trg.bbox.Min.Z + EPSILON )
		return false;

	// check that trg inside eff (per point test!)
	BRUSH::vectorlist::const_iterator v;
	for(v = trg.vlist.begin(); v != trg.vlist.end(); ++v)
		if(::OrientationConv(eff, *v) == 1)		// v outside eff
			break;
	if(v == trg.vlist.end())
		return true;

	// start splitting brush (splitting brushes move to cliplist)
	BRUSH dst(trg);
	for(BRUSH::facelist::const_iterator f = eff.flist.begin();
		f != eff.flist.end();
		++f)
	{
		BRUSH b1(0), b2(0);
		if(::SplitBrush(dst,
			eff.vlist[f->ivlist[0].I], eff.vlist[f->ivlist[1].I], eff.vlist[f->ivlist[2].I],
			b1, b2))
		{
			cliplist.push_back(b2);
			dst = b1;
		}
	}

	if(::OrientationConv(eff, dst.GetInPoint()) != -1)
	{
		dst.UpdateID();
		cliplist.push_back(dst);
	}

	return true;
}






//=============================================================================
//									UNDO
//=============================================================================

struct UNDOINFO
{
	typedef vectorex<BRUSH>		brushlist;
	typedef vectorex<BRUSH*>	ptrbrushlist;

	enum eOpType
	{
		// cannot undo/redo
		OT_CANNOT,

		// common primitive op
		OT_NEWOBJECT,
		OT_NEWENTITY,
		OT_PASTE,
		OT_PASTESPECIAL,
		OT_CLONE,

		OT_DELETE,

		// complex op
		OT_CLIPPING,
		OT_UNION,
		OT_CARVE,
		OT_HOLLOW,

		// affine transformation/morfing
		OT_RESIZE,
		OT_TRANSLATE,
		OT_ROTATE,
		OT_SHEAR,
		OT_TRANSFORM,
		OT_ALIGN,
		OT_FLIP,
		OT_MORFING,

		// texture translation
		OT_APPLY_TEXTURE,
		OT_TEXTURE_SHIFT,
		OT_TEXTURE_SCALE,
		OT_TEXTURE_ROTATE,
		OT_TEXTURE_FIT,
		OT_TEXTURE_AXIAL,
			SIZE
	};

	static const char *UNDOLABELS[eOpType::SIZE],
					  *REDOLABELS[eOpType::SIZE];

	UNDOINFO()
		{
		}

	UNDOINFO(eOpType op)
		: optype(op)
		, sframe()
		, eframe()
		{
		}

	UNDOINFO(eOpType op, unsigned bl1size, unsigned bl2size)
		: optype(op)
		{	// reserve size only
		sframe.reserve(bl1size);
		eframe.reserve(bl2size);
		}

	UNDOINFO(eOpType op, const ptrbrushlist& startframe, const brushlist& endframe)
		: optype(op)
		{	// USE: "multiply union" "hollow"
		// init start frame
		sframe.reserve(startframe.size());
		for (ptrbrushlist::const_iterator pb = startframe.begin(); pb != startframe.end(); ++pb)
			sframe.push_back(**pb);
		// init end frame
		eframe = endframe;
		}

	UNDOINFO(eOpType op, const ptrbrushlist& startframe, const BRUSH& brush)
		: optype(op)
		{	// USE: "single union"
		// init start frame
		sframe.reserve(startframe.size());
		for (ptrbrushlist::const_iterator pb = startframe.begin(); pb != startframe.end(); ++pb)
			sframe.push_back(**pb);
		// init end frame
		eframe.push_back(brush);
		}

	void Push(const BRUSH& sbrush, const BRUSH& ebrush1, const BRUSH& ebrush2)
		{	// USE: "clipping"
		sframe.push_back(sbrush);
		eframe.push_back(ebrush1);
		eframe.push_back(ebrush2);
		}

	void StartFrame(const BRUSH& brush)
		{
		sframe.push_back(brush);
		}

	void StartFrame(const BRUSH& brush, eOpType op)
		{
		optype = op;
		sframe.push_back(brush);
		}

	void EndFrame(const BRUSH& brush)
		{
		eframe.push_back(brush);
		}

	void StartFrame(const brushlist& bl, eOpType op)
		{
		optype = op;
		sframe = bl;
		}

	void EndFrame(const brushlist& bl)
		{
		eframe = bl;
		}

	void AddEndFrame(const brushlist& bl)
		{
		eframe.insert(eframe.end(), bl.begin(), bl.end());
		}

	void StartFrame(const ptrbrushlist& ptrbl, eOpType op)
		{
		optype = op;
		sframe.reserve(ptrbl.size());
		for (ptrbrushlist::const_iterator pb = ptrbl.begin(); pb != ptrbl.end(); ++pb)
			sframe.push_back(**pb);
		}

	void EndFrame(const ptrbrushlist& ptrbl)
		{
		eframe.reserve(ptrbl.size());
		for (ptrbrushlist::const_iterator pb = ptrbl.begin(); pb != ptrbl.end(); ++pb)
			eframe.push_back(**pb);
		}

	void SetAction(eOpType op)
		{
		optype = op;
		}

	eOpType			optype;
	brushlist		sframe, eframe;
};
