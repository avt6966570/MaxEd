#include "StdAfx.h"
#include "BRUSH.h"

#include <sys/stat.h>		// get file time: _stat64()


//=============================================================================
//								ENTITY implementation
//=============================================================================
ENTITY ENTITY::GLOBALENTITYLIST[128];
ENTITY* ENTITY::GLOBALENTITYLISTPTR = ENTITY::GLOBALENTITYLIST;




//=============================================================================
//								ENTITYINFO implementation
//=============================================================================
const VECTOR3	ENTITYINFO::DEFAULTSIZE(32, 32, 32);
const COLORREF	ENTITYINFO::DEFAULTCOLOR = RGB(255, 0, 0);
const COLORREF	ENTITYINFO::BRUSHCOLOR = RGB(128, 0, 255);
CArray<ENTITYINFO>	ENTITYINFO::GLOBALENTITYINFOLIST;

bool ENTITYINFO::ParseDefFile(const CString& defdata)
{
/*
{
"ammo_box" (255 0 0)(32 32 32)
"targetname" "" "target" "" "wait" "0.0" "weapon" "7"
"suspended" "gravity" "real"
Description...
}
*/

	int pos = 0, entityn = 0, x = 0, y = 0, z = 0;
	CString token, key, value;

	ENTITYINFO::GLOBALENTITYINFOLIST.SetSize(0, 64);

	while(entityn < ENTITYINFO::MAX_ENTITY)
	{
		ENTITYINFO EI;

		// 1. GET OPENED BRACKETS
		if((pos = defdata.Find('{', pos)) == -1)
			return true;

		// 2. PARSE ENTITY NAME
		pos = defdata.Find('"', pos);
		EI.name = defdata.Tokenize("\" (", pos);

		// 3. PARSE ENTITY COLOR
		if(defdata[pos] != '(')
			pos = defdata.Find('(', pos);
		if((token = defdata.Tokenize(")", ++pos)) != "")
		{
			int tpos = 0;
			EI.color = RGB(
				::atoi(token.Tokenize(" ", tpos)),
				::atoi(token.Tokenize(" ", tpos)),
				::atoi(token.Tokenize(" ", tpos))
				);
		}

		// 4. PARSE ENTITY SIZE
		while(defdata[pos] == ' ')	// skip whitespace
			++pos;
		if(defdata[pos] == '?')		// solid entity
			++pos;
		else if((token = defdata.Tokenize("()", pos)) != "")
		{
			int tpos = 0;
			x = ::atoi(token.Tokenize(" ", tpos));
			y = ::atoi(token.Tokenize(" ", tpos));
			z = ::atoi(token.Tokenize(" ", tpos));
			EI.size(x, y, z);
		}


		// 5. PARSE ENTITY EPAIRS
		pos = defdata.Find('\n', pos);
		while(defdata[++pos] != '\n')
			if(defdata[pos] == '"')
			{
				key = defdata.Tokenize("\"", pos);
				while(defdata[++pos] == ' ')
					;
				if(defdata[pos] == '"' && defdata[++pos] == '"')
					value = "";
				else
					value = defdata.Tokenize("\"", pos);

				EI.epairs.SetAt(key, value);
			}

		// 6. PARSE ENTITY FLAGS
		while(defdata[++pos] != '\n')
			if(defdata[pos] == '"')
			{
				token = defdata.Tokenize("\"", pos);
				EI.flagnames.Add(token);
			}
		EI.flags = 0;	// + reset flag bits!

		// 7. PARSE ENTITY DESCRIPTION
		EI.desc = defdata.Tokenize("}", ++pos);

		// 8. GET CLOSED BRACKETS
		if((pos = defdata.Find('}', pos - 1)) == -1)
			return false;	// parsing error: expected closed brackets
		
		// 9. DO NEXT ENTITY
		ENTITYINFO::GLOBALENTITYINFOLIST.Add(EI);
		++entityn;
	}

	return false;
}

ENTITYINFO* ENTITYINFO::FindEntityByName(const CString& name)
{
	for(INT_PTR i = 0; i < ENTITYINFO::GLOBALENTITYINFOLIST.GetSize(); ++i)
		if(ENTITYINFO::GLOBALENTITYINFOLIST[i].name == name)
			return &ENTITYINFO::GLOBALENTITYINFOLIST[i];
	return 0;
}

ENTITYINFO* ENTITYINFO::FindEntityByMenuID(INT_PTR menuid)
{
	for(INT_PTR i = 0; i < ENTITYINFO::GLOBALENTITYINFOLIST.GetSize(); ++i)
		if(ENTITYINFO::GLOBALENTITYINFOLIST[i].menuID == menuid)
			return &ENTITYINFO::GLOBALENTITYINFOLIST[i];
	return 0;
}




//=============================================================================
//								TEXTUREDEF implementation
//=============================================================================
#include "bmp.h"	// loading BMP file

// default texture pixel data (RGB)
static TEXTUREDEF::pixeldata DEFAULTCHECKIMAGE[8 * 8 * 3] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

TEXTUREDEF TEXTUREDEF::DEFAULT("default", 8, 8, 24, 0,
							   DEFAULTCHECKIMAGE);				// default texture
TEXTUREDEF TEXTUREDEF::ZERO("zero", 8, 8, 24, 0, 0);			// zero texture (for point-based entity)
TEXTUREDEF *TEXTUREDEF::pACTIVETEXTURE = &TEXTUREDEF::DEFAULT;	// current


bool TEXTUREDEF::Load(const char* filename)
{
		// load texture data from file
	if(::LoadBMP(filename, false, &width, &height, &bpp, &data) == BMPER_OK)
	{
		// + set name
		name = filename;

		// + set filetime
		__stat64 buf;
		if(::_tstat64(filename, &buf) == 0)
			filetime = buf.st_mtime;

		// opengl bind texture
		::glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		texID ? ::glDeleteTextures(1, &texID) : ::glGenTextures(1, &texID);
		::glBindTexture(GL_TEXTURE_2D, texID);
		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		
		// delete data pixels
		delete [] data, data = 0;

		return true;
	}

	return false;
}

TEXTUREDEF::~TEXTUREDEF()
{
	::glDeleteTextures(1, &texID);
	texID = 0;
}

void TEXTUREDEF::InitDefaultTexture()
{
	// OpenGL bind texture
	::glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	::glGenTextures(1, &TEXTUREDEF::DEFAULT.texID);
	::glBindTexture(GL_TEXTURE_2D, TEXTUREDEF::DEFAULT.texID);
	::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 8, 8, 0, GL_RGB,
					GL_UNSIGNED_BYTE, TEXTUREDEF::DEFAULT.data);
}

//void TEXTUREDEF::InitTextureCash()
//{
//	TEXTUREDEF td;
//	WIN32_FIND_DATA FileData;
//
//	// change current directory to textures dir
//	if(!::SetCurrentDirectory("textures//"))
//		return;
//
//	// load & binding textures
//	HANDLE hSearch = ::FindFirstFile("*.bmp", &FileData); 
//	if(hSearch != INVALID_HANDLE_VALUE && td.Load(FileData.cFileName))
//		TEXTUREDEF::TEXTURECASH.push_back(td);
//
//	while(::FindNextFile(hSearch, &FileData))
//		if(td.Load(FileData.cFileName))
//			TEXTUREDEF::TEXTURECASH.push_back(td);
//
//	FindClose(hSearch);
//
//	// return root app dir
//	::SetCurrentDirectory("maxed//");
//
//	// avoid texture unbinding in destructor
//	td.texID = 0;
//}


void TEXTUREDEF::DrawSkin(int left, int top, int bitscale, bool selection) const
{
	// draw texture skin in "select texture" window
	::glColor3ubv(COLOR3::WHITE);
	::glLineWidth(1);
	::glBindTexture(GL_TEXTURE_2D, texID);
	::glBegin(GL_QUADS);
		::glTexCoord2i(0,0);
		::glVertex2i(left, top - (height>>bitscale));
		::glTexCoord2i(1,0);
		::glVertex2i(left + (width>>bitscale), top - (height>>bitscale));
		::glTexCoord2i(1,1);
		::glVertex2i(left + (width>>bitscale), top);
		::glTexCoord2i(0,1);
		::glVertex2i(left, top);
	::glEnd();

	::glBindTexture(GL_TEXTURE_2D, 0);	// reset texture :TODO: may be glDisable(GL_TEXTURE_2D)

	if(selection)
	{	// draw selection rect
		::glColor3ubv(COLOR3::RED);
		::glLineWidth(3);
		::glBegin(GL_LINE_LOOP);
			::glVertex2i(left, top - (height>>bitscale));
			::glVertex2i(left + (width>>bitscale), top - (height>>bitscale));
			::glVertex2i(left + (width>>bitscale), top);
			::glVertex2i(left, top);
		::glEnd();
	}

	// print texture name
	::glColor3ubv(COLOR3::WHITE);
	::glRasterPos2i(left, top);
	::glListBase(VIEWLABEL::VL_TEXNAMELISTINDEX);
	::glCallLists(name.GetLength() - 4, GL_UNSIGNED_BYTE, name);
}




//=============================================================================
//								  TEXTURE DIRECTORY
//=============================================================================

TEXTUREDIR::texturedirlist TEXTUREDIR::CACHE(8);
TEXTUREDIR* TEXTUREDIR::CURRENTCACHEDIR = 0;

TEXTUREDIR* TEXTUREDIR::FindDirInTDCache(const char* dirname,
										 const TEXTUREDEF* td, const char* tdname)
{
/*
if dirname ? find in this txdirectory : find in all txdirs +
	if td ? find by tdef`s pointer adresses : find by tdef`s texname
*/
	if (dirname)
	{
		TEXTUREDIR* dir = 0;
		for (texturedirlistiterator tdr = CACHE.begin(); tdr != CACHE.end(); ++tdr)
			if (tdr->name == dirname)
				dir = &*tdr;

		if (dir)
		{
			if (td)
				for (texturedeflistiterator tdf = dir->cache.begin(); tdf != dir->cache.end(); ++tdf)
					if (td == &*tdf)
						return dir;
					else;
			else if (tdname)
				for (texturedeflistiterator tdf = dir->cache.begin(); tdf != dir->cache.end(); ++tdf)
					if (tdf->name == tdname)
						return dir;
					else;
			else
				return dir;
		}
	}
	else if (td)
		for (texturedirlistiterator tdr = CACHE.begin(); tdr != CACHE.end(); ++tdr)
			for (texturedeflistiterator tdf = tdr->cache.begin(); tdf != tdr->cache.end(); ++tdf)
				if (td == &*tdf)
					return &*tdr;
				else;
	else
		for (texturedirlistiterator tdr = CACHE.begin(); tdr != CACHE.end(); ++tdr)
			for (texturedeflistiterator tdf = tdr->cache.begin(); tdf != tdr->cache.end(); ++tdf)
				if (tdf->name == tdname)
					return &*tdr;

	return 0;
}

TEXTUREDEF* TEXTUREDIR::FindTexDefInTDCache(const char* tname, const char* tdir)
{
	if (tdir)
		for (texturedirlistiterator tdr = CACHE.begin(); tdr != CACHE.end(); ++tdr)
			if (tdr->name.Find(tdir) != -1)
			{
				for (texturedeflistiterator tdf = tdr->cache.begin(); tdf != tdr->cache.end(); ++tdf)
					if (tdf->name.Find(tname) != -1)
						return &*tdf;
				return 0;
			}
			else;
	else
		for (texturedirlistiterator tdr = CACHE.begin(); tdr != CACHE.end(); ++tdr)
			for (texturedeflistiterator tdf = tdr->cache.begin(); tdf != tdr->cache.end(); ++tdf)
				if (tdf->name.Find(tname) != -1)
					return &*tdf;
	return 0;
}

const TEXTUREDEF* TEXTUREDIR::Find(const char* texname) const
{
	for (texturedeflist::const_iterator tdf = cache.begin(); tdf != cache.end(); ++tdf)
		if (tdf->name == texname)
			return &*tdf;
	return 0;
}

TEXTUREDIR* TEXTUREDIR::FindTexDirInTDCache(const char* dirname)
{
	for (texturedirlistiterator tdr = CACHE.begin(); tdr != CACHE.end(); ++tdr)
		if (tdr->name.CompareNoCase(dirname) == 0)
			return &*tdr;
	return 0;
}

TEXTUREDEF* TEXTUREDIR::Find2(const char* texname)
{
	for (texturedeflist::iterator tdf = cache.begin(); tdf != cache.end(); ++tdf)
		if (tdf->name.Find(texname) != -1)
			return &*tdf;
	return 0;
}


bool TEXTUREDIR::Init(const char* texturepath, const char* dirname)
{
	if (::SetCurrentDirectory(texturepath) &&	// set root texture path
		::SetCurrentDirectory(dirname))			// + set new texture folder
	{
		CFileFind finder;
		size_t filenum = 0;


		// calc num files (for reserve cache mem)
		BOOL bWorking = finder.FindFile(_T("*.bmp"));
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			++filenum;
		}

		if (filenum)
		{
			TEXTUREDEF td;

			// set dir name
			name = dirname;
			// reserve texture cache
			cache.reserve(filenum + 4);

			bWorking = finder.FindFile(_T("*.bmp"));
			while (bWorking)
			{
				bWorking = finder.FindNextFile();
				if (td.Load(finder.GetFileName()))
					cache.push_back(td);
				td.texID = 0;		// avoid texture unbinding in destructor
			}

			finder.Close();
			return true;
		}

		finder.Close();
	}

	return false;
}

bool TEXTUREDIR::Update(const char* texturepath, const char* dirname)
{
	bool bRedraw = false;

	if (::SetCurrentDirectory(texturepath) &&	// set root texture path
		::SetCurrentDirectory(dirname))			// + set new texture folder
	{
		CFileFind finder;
		CTime filetime;

		// update current textures
		for (texturedeflistiterator tdf = cache.begin(); tdf != cache.end(); ++tdf)
		{
			if (finder.FindFile(tdf->name))
			{
				finder.FindNextFile();
				finder.GetLastWriteTime(filetime);
				// if textures has difference modify time -> delete old & load new
				if (tdf->filetime != filetime.GetTime() &&
					tdf->Load(finder.GetFileName()))	// update texture file
					bRedraw = true;
			}
			else	// user delete/replace texture file
			{
				tdf->SetDefault();
				bRedraw = true;
			}
		}

		// find new texture files?
		BOOL bWorking = finder.FindFile(_T("*.bmp"));
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			if (!Find(finder.GetFileName()))		// if user add to dir new texture file
			{
				TEXTUREDEF td;
				if (td.Load(finder.GetFileName()))	// try load it
				{
					cache.push_back(td);			// + add it to texture cache line
					bRedraw = true;
				}
				td.texID = 0;						// avoid texture unbinding in destructor
			}
		}
	}

	return bRedraw;
}




//=============================================================================
//								FACE implementation
//=============================================================================
FACE::_SELECTED	FACE::SELECTED;

void FACE::GenerateTextureCoords(const vectorlist& vlist, unsigned bestpln)
{	// :NOTE: normal must be alredy defined!

	// get best texture dims from plane
	const unsigned D1 = ::BASEDIMS[bestpln][0], D2 = ::BASEDIMS[bestpln][1];

	// load texture matrix
	MATRIX6 tm(shift, 1.F/scale, rotate, texdef->width, texdef->height);

	// load inverse rotation matrix
	MATRIX16 irm;
	irm.LoadIdentity();
	irm.MM[0][D1] = V.X, irm.MM[1][D1] = V.Y, irm.MM[2][D1] = V.Z;
	irm.MM[0][D2] = W.X, irm.MM[1][D2] = W.Y, irm.MM[2][D2] = W.Z;

	// update texture coords
	for(indexedvertexlist::iterator ivl = ivlist.begin(); ivl != ivlist.end(); ++ivl)
		ivl->T = (vlist[ivl->I] * irm).Project(D1, D2) * tm;
}

bool FACE::Intersection(const VECTOR3& rayP0, const VECTOR3& rayV,
						vectorlist& vlist, VECTOR3* q, FLOAT* t) const
{
	VECTOR3 qq;
	FLOAT tt;
	if(IntersectionRay<OrientationConv>(MakePolygone3(vlist), N, rayP0, rayV, &qq, &tt) == 1)
	{
		if(q) *q = qq;
		if(t) *t = tt;
		return true;
	}
	else return false;
}



//=============================================================================
//								BRUSH implementation
//=============================================================================
BRUSH::brushlist BRUSH::WORLDBRUSHLIST(32);			// reserve 32 unit
BRUSH::ptrbrushlist BRUSH::SELECTEDBRUSHLIST(32);	// reserve 32 unit
BRUSH::ptrvectorlist BRUSH::SELECTEDVERTEXLIST(32);	// reserve 32 unit
unsigned BRUSH::GLOBALCOUNTER = 0;

void BRUSH::MakeBlock(const VECTOR3& origin, const VECTOR3& size)
{	// for make entity
	// --- BLOCK (4-6-12-8) VERTEX/INDEX DATA ---
	const VECTOR3 blockv[8] = {
		VECTOR3(origin.X + size.X,	origin.Y,			origin.Z + size.Z),
		VECTOR3(origin.X + size.X,	origin.Y + size.Y,	origin.Z + size.Z),
		VECTOR3(origin.X,			origin.Y + size.Y,	origin.Z + size.Z),
		VECTOR3(origin.X,			origin.Y,			origin.Z + size.Z),
		VECTOR3(origin.X + size.X,	origin.Y,			origin.Z),
		VECTOR3(origin.X + size.X,	origin.Y + size.Y,	origin.Z),
		VECTOR3(origin.X,			origin.Y + size.Y,	origin.Z),
		VECTOR3(origin.X,			origin.Y,			origin.Z)
	};

	static const vertexindex blocki[6][4] = {
		{ 0, 1, 2, 3 },		// front
		{ 3, 2, 6, 7 },		// left
		{ 0, 3, 7, 4 },		// bottom
		{ 7, 6, 5, 4 },		// back
		{ 4, 5, 1, 0 },		// right
		{ 1, 5, 6, 2 }		// top
	};


	// init vertex data
	vlist.clear();
	vlist.insert(vlist.begin(), blockv, blockv + 8);

	// init face lists
	flist.clear();
	flist.reserve(6);
	for(vertexindex i = 0; i < 6; ++i)
		flist.push_back(FACE(blockv, blocki[i], 4));

	// update brush stuff
	UpdateBBox();
	UpdateNormals();
	UpdateVWs();
	UpdateID();
}

void BRUSH::MakeShape(unsigned shapetype, const VECTOR3& origin, unsigned nside,
					  int gridsize)
{
	// --- BLOCK (4-6-12-8) VERTEX/INDEX DATA ---
	static const VECTOR3 blockv[8] = {
		VECTOR3(1.0F, 0.0F, 1.0F),
		VECTOR3(1.0F, 1.0F, 1.0F),
		VECTOR3(0.0F, 1.0F, 1.0F),
		VECTOR3(0.0F, 0.0F, 1.0F),
		VECTOR3(1.0F, 0.0F, 0.0F),
		VECTOR3(1.0F, 1.0F, 0.0F),
		VECTOR3(0.0F, 1.0F, 0.0F),
		VECTOR3(0.0F, 0.0F, 0.0F)
	};

	static const vertexindex blocki[6][4] = {
		{ 0, 1, 2, 3 },		// front
		{ 3, 2, 6, 7 },		// left
		{ 0, 3, 7, 4 },		// bottom
		{ 7, 6, 5, 4 },		// back
		{ 4, 5, 1, 0 },		// right
		{ 1, 5, 6, 2 }		// top
	};


	// --- WEDGE (X-5-9-6) VERTEX/INDEX DATA ---
	static const VECTOR3 wedgev[6] = {
		VECTOR3(1.0F, 0.0F, 1.0F),
		VECTOR3(1.0F, 1.0F, 1.0F),
		VECTOR3(0.0F, 1.0F, 1.0F),
		VECTOR3(0.0F, 0.0F, 1.0F),
		VECTOR3(1.0F, 0.0F, 0.0F),
		VECTOR3(1.0F, 1.0F, 0.0F)
	};

	static const vertexindex wedgei1[3][4] = {
		{ 0, 1, 2, 3 },		// front
		{ 4, 5, 1, 0 },		// right
		{ 3, 2, 5, 4 }		// cap
	};

	static const vertexindex wedgei2[2][3] = {
		{ 0, 3, 4 },		// bottom3
		{ 1, 5, 2 }			// top3
	};


	// --- SPIKE3 (3-4-6-4) VERTEX/INDEX DATA ---
	static VECTOR3 spike3v[4] = {
		VECTOR3(1, 0, 1),
		VECTOR3(0.5F, 1, 0.5F),
		VECTOR3(0, 0, 1),
		VECTOR3(0.5F, 0, 0)
	};

	static const vertexindex spike3i[4][3] = {
		{ 1, 2, 0 },
		{ 1, 3, 2 },
		{ 1, 0, 3 },
		{ 0, 2, 3 }
	};


	// --- SPIKE4 (3-4-8-5) VERTEX/INDEX DATA ---
	static VECTOR3 spike4v[5] = {
		VECTOR3(1, 0, 1),
		VECTOR3(0.5F, 1, 0.5F),
		VECTOR3(0, 0, 1),
		VECTOR3(0, 0, 0),
		VECTOR3(1, 0, 0)
	};

	static const vertexindex spike4i[4][3] = {
		{ 1, 2, 0 },
		{ 1, 3, 2 },
		{ 1, 4, 3 },
		{ 1, 0, 4 }
	};

	static const vertexindex spike4icap[1][4] = {
		{ 0, 2, 3, 4 }
	};


	// --- DSPIKE3 (3-6-10-5) VERTEX/INDEX DATA ---
	static const VECTOR3 dspike3v[5] = {
		VECTOR3(1.0F, 0.5F, 1.0F),
		VECTOR3(0.5F, 1.0F, 0.5F),
		VECTOR3(0.0F, 0.5F, 1.0F),
		VECTOR3(0.5F, 0.5F, 0.0F),
		VECTOR3(0.5F, 0.0F, 0.5F)
	};

	static const vertexindex dspike3i[6][3] = {
		{ 1, 2, 0 },
		{ 1, 3, 2 },
		{ 1, 0, 3 },
		{ 4, 0, 2 },
		{ 4, 2, 3 },
		{ 4, 3, 0 }
	};


	// --- DSPIKE4 (3-8-12-6) VERTEX/INDEX DATA ---
	static const VECTOR3 dspike4v[6] = {
		VECTOR3(1.0F, 0.5F, 1.0F),
		VECTOR3(0.5F, 1.0F, 0.5F),
		VECTOR3(0.0F, 0.5F, 1.0F),
		VECTOR3(0.0F, 0.5F, 0.0F),
		VECTOR3(1.0F, 0.5F, 0.0F),
		VECTOR3(0.5F, 0.0F, 0.5F)
	};

	static const vertexindex dspike4i[8][3] = {
		{ 1, 2, 0 },
		{ 1, 3, 2 },
		{ 1, 4, 3 },
		{ 1, 0, 4 },
		{ 5, 0, 2 },
		{ 5, 2, 3 },
		{ 5, 3, 4 },
		{ 5, 4, 0 }
	};

	// ###################################################

	// for static (non user changeable sided) shapes
	struct _SHAPEINFO
	{
		unsigned vecnum,
				 edgenum1, vedgenum1,
				 edgenum2, vedgenum2;
		const VECTOR3 *vdata;
		const vertexindex *idata1, *idata2;
	};

	static const _SHAPEINFO SHAPEINFO[6] = {
		{ 8, 6, 4, 0, 0, blockv,	(const vertexindex*)blocki, 0 },
		{ 6, 3, 4, 2, 3, wedgev,	(const vertexindex*)wedgei1, (const vertexindex*)wedgei2 },
		{ 4, 4, 3, 0, 0, spike3v,	(const vertexindex*)spike3i, 0 },
		{ 5, 4, 3, 1, 4, spike4v,	(const vertexindex*)spike4i, (const vertexindex*)spike4icap },
		{ 5, 6, 3, 0, 0, dspike3v,	(const vertexindex*)dspike3i, 0 },
		{ 6, 8, 3, 0, 0, dspike4v,  (const vertexindex*)dspike4i }
	};

	ASSERT(shapetype >= ST_BLOCK && shapetype <= ST_DSPIKEN);

	_SHAPEINFO current;
	vectorlist vdata;
	vertexindexlist idata, cap;

	switch(shapetype)
	{
	case ST_CYLINDERN:
		ASSERT(nside > 2);
		vdata.reserve(nside*2);
		idata.reserve(nside*4);
		cap.reserve(nside*2);

		// init cylinder vertex data
		for(unsigned v = 0; v < nside; ++v)
		{
			FLOAT fsin, fcos;
			::sincos(DEG2RADF(360.F/nside*v), &fsin, &fcos);
			vdata.push_back(VECTOR3(fcos, -1, fsin));
			vdata.push_back(VECTOR3(fcos, 1, fsin));
		}

		// init cylinder index data
		for(vertexindex e = 0; e < nside-1; ++e)
		{
			const vertexindex voff = e * 2;
			idata.push_back(voff + 0);
			idata.push_back(voff + 1);
			idata.push_back(voff + 3);
			idata.push_back(voff + 2);
		}
		// + last grain
		{
			const vertexindex voff = (nside-1) * 2;
			idata.push_back(voff + 0);
			idata.push_back(voff + 1);
			idata.push_back(1);
			idata.push_back(0);
		}

		// + bottom cap
		for(vertexindex v = 0; v < nside*2; v += 2)
			cap.push_back(v);
		// + top cap
		for(vertexindex v = nside*2 - 1; v != (vertexindex)-1; v -= 2)
			cap.push_back(v);

		// init shape info
		current.vecnum = nside*2;
		current.edgenum1 = nside;
		current.vedgenum1 = 4; 
		current.edgenum2 = 2;
		current.vedgenum2 = nside;
		current.vdata = &vdata[0];
		current.idata1 = &idata[0];
		current.idata2 = &cap[0];
		break;
	case ST_SPIKEN:
		ASSERT(nside > 2);
		vdata.reserve(nside + 1);
		idata.reserve(nside*3);
		cap.reserve(nside);

		// init spike vertex data
		for(unsigned v = 0; v < nside; ++v)
		{
			FLOAT fsin, fcos;
			::sincos(DEG2RADF(360.F/nside*v), &fsin, &fcos);
			vdata.push_back(VECTOR3(fcos, -1, fsin));
		}
		// + capped vert
			vdata.push_back(VECTOR3(0, 1, 0));

		// init spike index data
		for(vertexindex e = 0; e < nside-1; ++e)
		{
			idata.push_back(e);
			idata.push_back(nside);
			idata.push_back(e + 1);
		}
		// + last grain
			idata.push_back(nside - 1);
			idata.push_back(nside);
			idata.push_back(0);
		// + bottom cap
			for(vertexindex e = 0; e < nside; ++e)
				cap.push_back(e);

		// init shape info
		current.vecnum = nside + 1;
		current.edgenum1 = nside;
		current.vedgenum1 = 3; 
		current.edgenum2 = 1;
		current.vedgenum2 = nside;
		current.vdata = &vdata[0];
		current.idata1 = &idata[0];
		current.idata2 = &cap[0];
		break;
	case ST_DSPIKEN:
		ASSERT(nside > 2);
		vdata.reserve(nside + 2);
		idata.reserve(nside*2*3);

		// init dspike vertex data
		for(unsigned v = 0; v < nside; ++v)
		{
			FLOAT fsin, fcos;
			::sincos(DEG2RADF(360.F/nside*v), &fsin, &fcos);
			vdata.push_back(VECTOR3(fcos, 0, fsin));
		}
		// + capped vert
			vdata.push_back(VECTOR3(0, 1, 0));
			vdata.push_back(VECTOR3(0, -1, 0));

		// init dspike index data (hi grain)
		for(vertexindex e = 0; e < nside-1; ++e)
		{
			idata.push_back(e);
			idata.push_back(nside);
			idata.push_back(e + 1);
		}
		// + last grain (hi grain)
			idata.push_back(nside - 1);
			idata.push_back(nside);
			idata.push_back(0);

		// init dspike index data (low grain)
		for(vertexindex e = 0; e < nside-1; ++e)
		{
			idata.push_back(e);
			idata.push_back(e + 1);
			idata.push_back(nside + 1);
		}
		// + last grain (low grain)
			idata.push_back(nside - 1);
			idata.push_back(0);
			idata.push_back(nside + 1);

		// init shape info
		current.vecnum = nside + 2;
		current.edgenum1 = nside*2;
		current.vedgenum1 = 3; 
		current.edgenum2 = 0;
		current.vedgenum2 = 0;
		current.vdata = &vdata[0];
		current.idata1 = &idata[0];
		break;
	default:
		ASSERT(SHAPEINFO[shapetype].vdata);
		ASSERT(SHAPEINFO[shapetype].idata1);
		current = SHAPEINFO[shapetype];
		break;
	}


	// init vertex data array
	vlist.clear();
	vlist.insert(vlist.begin(), current.vdata, current.vdata + current.vecnum);

		// translate [-1 1] -> [0 1] if changeable shape
		if(shapetype >= eShapeType::TOTAL_NONCHANGEABLE)
		{
			MATRIX16 m;
			m.LoadScale(0.5F, 0.5F, 0.5F);
			m.Translate(1.0F, 1.0F, 1.0F);
			for(vectorlist::iterator v = vlist.begin(); v != vlist.end(); ++v)
				*v *= m;
		}

	// init face lists
	flist.clear();
	flist.reserve(current.edgenum1 + current.edgenum2);
	for(vertexindex i = 0; i < current.edgenum1; ++i)
		flist.push_back(
			FACE(current.vdata, &current.idata1[i*current.vedgenum1], current.vedgenum1)
			);

	// init cap faces
	if(current.edgenum2)	// if need cap...
	{
		ASSERT(current.idata2);
		for(vertexindex i = 0; i < current.edgenum2; ++i)
			flist.push_back(
				FACE(current.vdata, &current.idata2[i*current.vedgenum2], current.vedgenum2)
				);
	}

#ifdef _DEBUG
	switch(shapetype)
	{
	case ST_BLOCK:
		ASSERT(vlist.size() == 8);
		ASSERT(flist.size() == 6);
		for(facelist::const_iterator f = flist.begin(); f != flist.end(); ++f)
			ASSERT(f->ivlist.size() == 4);
		break;
	case ST_WEDGE:
		ASSERT(vlist.size() == 6);
		ASSERT(flist.size() == 5);
		for(facelist::const_iterator f = flist.begin(); f != flist.end(); ++f)
			ASSERT(f->ivlist.size() == 4 || f->ivlist.size() == 3);
		break;
	case ST_SPIKE3:
		ASSERT(vlist.size() == 4);
		ASSERT(flist.size() == 4);
		for(facelist::const_iterator f = flist.begin(); f != flist.end(); ++f)
			ASSERT(f->ivlist.size() == 3);
		break;
	case ST_SPIKE4:
		ASSERT(vlist.size() == 5);
		ASSERT(flist.size() == 5);
		for(facelist::const_iterator f = flist.begin(); f != flist.end(); ++f)
			ASSERT(f->ivlist.size() == 4 || f->ivlist.size() == 3);
		break;
	case ST_DSPIKE3:
		ASSERT(vlist.size() == 5);
		ASSERT(flist.size() == 6);
		for(facelist::const_iterator f = flist.begin(); f != flist.end(); ++f)
			ASSERT(f->ivlist.size() == 3);
		break;
	case ST_DSPIKE4:
		ASSERT(vlist.size() == 6);
		ASSERT(flist.size() == 8);
		for(facelist::const_iterator f = flist.begin(); f != flist.end(); ++f)
			ASSERT(f->ivlist.size() == 3);
		break;
	case ST_CYLINDERN:
		ASSERT(vlist.size() == nside*2);
		ASSERT(flist.size() == nside + 2);
		for(facelist::const_iterator f = flist.begin(); f != flist.end(); ++f)
			ASSERT(f->ivlist.size() == 4 || f->ivlist.size() == nside);
		break;
	case ST_SPIKEN:
		ASSERT(vlist.size() == nside + 1);
		ASSERT(flist.size() == nside + 1);
		for(facelist::const_iterator f = flist.begin(); f != flist.end(); ++f)
			ASSERT(f->ivlist.size() == 3 || f->ivlist.size() == nside);
		break;
	case ST_DSPIKEN:
		ASSERT(vlist.size() == nside + 2);
		ASSERT(flist.size() == nside*2);
		for(facelist::const_iterator f = flist.begin(); f != flist.end(); ++f)
			ASSERT(f->ivlist.size() == 3);
		break;
	default:
		ASSERT(0);
		break;
	}
#endif
}

void BRUSH::RebuildShape(MATRIX16& m,
						 const VECTOR3& origin, const VECTOR3& current, int gridsize)
{
	//// resize width
	//if(size.X > origin.X) {
	//	if(size.X - origin.X < gridsize)
	//		size.X = origin.X + gridsize;
	//}
	//else if(size.X - origin.X > -gridsize)
	//	size.X = origin.X - gridsize;

	//// resize height
	//if(size.Y > origin.Y) {
	//	if(size.Y - origin.Y < gridsize)
	//		size.Y = origin.Y + gridsize;
	//}
	//else if(size.Y - origin.Y > -gridsize)
	//	size.Y = origin.Y - gridsize;

	//// resize depth
	//if(size.Z > origin.Z) {
	//	if(size.Z - origin.Z < gridsize)
	//		size.Z = origin.Z + gridsize;
	//}
	//else if(size.Z - origin.Z > -gridsize)
	//	size.Z = origin.Z - gridsize;

	VECTOR3 size(current - origin);
	if(size.X < 0) {
		if(size.X > -gridsize)
			size.X = -gridsize;
	}
	else if(size.X < gridsize)
		size.X = gridsize;

	if(size.Y < 0) {
		if(size.Y > -gridsize)
			size.Y = -gridsize;
	}
	else if(size.Y < gridsize)
		size.Y = gridsize;

	if(size.Z < 0) {
		if(size.Z > -gridsize)
			size.Z = -gridsize;
	}
	else if(size.Z < gridsize)
		size.Z = gridsize;

	m.LoadTranslation(origin);
	m.Scale(size);
}

FACE* BRUSH::Intersection(const VECTOR3& rayP0, const VECTOR3& rayV,
						  VECTOR3* q, FLOAT* t) const
{
	FLOAT tt, tnear = FLT_MAX;
	VECTOR3 qq, qnear;
	FACE* pNearFace = 0;
	for(facelist::const_iterator f = flist.begin(); f != flist.end(); ++f)
	{
		const POLYGONE3 poly(f->MakePolygone3(vlist));
		if(::IntersectionRay<OrientationRad>(poly, f->N, rayP0, rayV, &qq, &tt) == 1 &&
			tt > 0 && tt < tnear)
		{
			pNearFace = const_cast<FACE*>(&*f);
			qnear = qq;
			tnear = tt;
		}
	}

	if(q) *q = qnear;
	if(t) *t = tnear;
	return pNearFace;
}


// bbox`s simple gabarite test
BRUSH* BRUSH::GetSelectionBoxInWorldBrushList(const VECTOR3& rayP0, const VECTOR3& rayV)
{
	FLOAT t, tnear = FLT_MAX;
	BRUSH* pNearBrush = 0;
	for(brushlist::iterator b = WORLDBRUSHLIST.begin(); b != WORLDBRUSHLIST.end(); ++b)
	{
		if(::IntersectionRay(b->bbox, rayP0, rayV, &t) && t > 0 && t < tnear)
		{
			pNearBrush = &*b;
			tnear = t;
		}
	}

	return pNearBrush;
}

template<int (*ORIENTATIONPOINTPFN)(const POLYGONE2&, const VECTOR2&)>
FACE* IntersectionRay(const BRUSH& brush, const VECTOR3& rayP0, const VECTOR3& rayV,
					  VECTOR3* q, FLOAT* t)
{
	FLOAT tt, tnear = FLT_MAX;
	VECTOR3 qq, qnear;
	FACE* pNearFace = 0;
	for(BRUSH::facelist::const_iterator f = brush.flist.begin();
		f != brush.flist.end();
		++f)
	{
		const POLYGONE3 poly(f->MakePolygone3(brush.vlist));
		if(::IntersectionRay<ORIENTATIONPOINTPFN>(poly, f->N, rayP0, rayV, &qq, &tt) == 1 &&
			tt > 0 && tt < tnear)
		{
			pNearFace = const_cast<FACE*>(&*f);
			qnear = qq;
			tnear = tt;
		}
	}

	if(q) *q = qnear;
	if(t) *t = tnear;
	return pNearFace;
}

// face `exact` ray variant
BRUSH* BRUSH::GetSelectionInWorldBrushList(const VECTOR3& rayP0, const VECTOR3& rayV,
											FACE** f)
{
	FLOAT t, tnear = FLT_MAX;
	BRUSH *pNearBrush = 0;
	FACE *pNearFace = 0, *ff;

	for(brushlist::iterator b = WORLDBRUSHLIST.begin(); b != WORLDBRUSHLIST.end(); ++b)
	{
		if(::IntersectionRay(b->bbox, rayP0, rayV, &t) &&
			t > 0 && t < tnear && 
			(ff = ::IntersectionRay<OrientationConv>(*b, rayP0, rayV, 0, &t)) &&
			t > 0 && t < tnear)
		{
			pNearBrush = &*b;
			pNearFace = ff;
			tnear = t;
		}
	}

	if(f) *f = pNearFace;
	return pNearBrush;
}

bool BRUSH::isConvex() const
{
	for(facelist::const_iterator f = flist.begin(); f != flist.end(); ++f)
	{
		bool l = false, r = false;
		const POLYGONE3 poly(f->MakePolygone3(vlist));
		for(vectorlist::const_iterator v = vlist.begin(); v != vlist.end(); ++v)
		{
			switch(::OrientationConv(poly, *v))
			{
			case 1:		r = true; break;	// at right
			case -1:	l = true; break;	// at left
			}

			if(l && r)
				return false;
		}
	}
	return true;
}


static inline
bool MergeFaceInBrush(const FACE::vectorlist& vlist, const FACE& f1, const FACE& f2,
					  FACE& dst)
{
	if(::equal(f1.N, f2.N))
	{
		// merge...
		LINESEG3LIST mpl(f1.ivlist.size() + f2.ivlist.size());
		const POLYGONE3 f1p(f1.MakePolygone3(vlist)), f2p(f2.MakePolygone3(vlist));
		mpl.Add(f1p, false);
		mpl.Add(f2p, false);
		mpl.OptimizeChain();
		if(mpl.BuildClosedCircuit())
		{
			const POLYGONE3 mpoly(mpl);
			dst = FACE(vlist, mpoly, f1);
			return true;
		}
	}
	return false;
}

void BRUSH::Optimize()
{
	// find coplanar face
start:
	for(facelist::iterator fs = flist.begin(); fs != flist.end(); ++fs)
		for(facelist::iterator fd = flist.begin(); fd != flist.end(); ++fd)
			if(fs != fd)
			{
				FACE newf;
				if(::MergeFaceInBrush(vlist, *fs, *fd, newf))
				{
					flist.erase(fd);
					flist.erase(fs);
					flist.push_back(newf);
					goto start;
				}
			}

	// skip unused vdata
	for(vertexindex i = 0; i < vlist.size(); )
	{
		facelist::const_iterator f;
		for(f = flist.begin(); f != flist.end(); ++f)
			for(indexedvertexlist::const_iterator ivl = f->ivlist.begin();
				ivl != f->ivlist.end();
				++ivl)
					if(i == ivl->I)
						goto mainloop;

		if(f == flist.end())	// has unused index
		{
			vlist.erase(i);
			for(facelist::iterator x = flist.begin(); x != flist.end(); ++x)
				for(indexedvertexlist::iterator ivlx = x->ivlist.begin();
					ivlx != x->ivlist.end();
					++ivlx)
					if(ivlx->I > i)
						--ivlx->I;
			i = 0;
		}
		else
		{
mainloop:;
			++i;
		}
	}
}



//=============================================================================
//								UNDO implementation
//=============================================================================

const char* UNDOINFO::UNDOLABELS[UNDOINFO::eOpType::SIZE] = 
{
	"Can`t &Undo\tCtrl+Z",
	"&Undo New Object\tCtrl+Z",
	"&Undo New Entity\tCtrl+Z",
	"&Undo Paste\tCtrl+Z",
	"&Undo Paste Special\tCtrl+Z",
	"&Undo Clone\tCtrl+Z",

	"&Undo Delete\tCtrl+Z",

	"&Undo Clipping\tCtrl+Z",
	"&Undo Union\tCtrl+Z",
	"&Undo Carve\tCtrl+Z",
	"&Undo Hollow\tCtrl+Z",

	"&Undo Resize\tCtrl+Z",
	"&Undo Translate\tCtrl+Z",
	"&Undo Rotate\tCtrl+Z",
	"&Undo Shear\tCtrl+Z",
	"&Undo Transform\tCtrl+Z",
	"&Undo Align\tCtrl+Z",
	"&Undo Flip\tCtrl+Z",
	"&Undo Morfing\tCtrl+Z",

	"&Undo Apply Texture\tCtrl+Z",
	"&Undo Texture Shift\tCtrl+Z",
	"&Undo Texture Scale\tCtrl+Z",
	"&Undo Texture Rotate\tCtrl+Z",
	"&Undo Texture Fit\tCtrl+Z",
	"&Undo Texture Axial\tCtrl+Z"
};

const char* UNDOINFO::REDOLABELS[UNDOINFO::eOpType::SIZE]=
{
	"Can`t &Redo\tCtrl+Y",
	"&Redo New Object\tCtrl+Y",
	"&Redo New Entity\tCtrl+Y",
	"&Redo Paste\tCtrl+Y",
	"&Redo Paste Special\tCtrl+Y",
	"&Redo Clone\tCtrl+Y",

	"&Redo Delete\tCtrl+Y",

	"&Redo Clipping\tCtrl+Y",
	"&Redo Union\tCtrl+Y",
	"&Redo Carve\tCtrl+Y",
	"&Redo Hollow\tCtrl+Y",

	"&Redo Resize\tCtrl+Y",
	"&Redo Translate\tCtrl+Y",
	"&Redo Rotate\tCtrl+Y",
	"&Redo Shear\tCtrl+Y",
	"&Redo Transform\tCtrl+Y",
	"&Redo Align\tCtrl+Y",
	"&Redo Flip\tCtrl+Y",
	"&Redo Morfing\tCtrl+Y",

	"&Redo Apply Texture\tCtrl+Y",
	"&Redo Texture Shift\tCtrl+Y",
	"&Redo Texture Scale\tCtrl+Y",
	"&Redo Texture Rotate\tCtrl+Y",
	"&Redo Texture Fit\tCtrl+Y",
	"&Redo Texture Axial\tCtrl+Y"
};