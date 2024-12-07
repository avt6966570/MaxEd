#include "stdafx.h"
//#include "3d.h"


const VECTOR1 VECTOR1::O(0.0F);

const VECTOR2 VECTOR2::O(0.0F, 0.0F);
const VECTOR2 VECTOR2::OX(1.0F, 0.0F);
const VECTOR2 VECTOR2::OY(0.0F, 1.0F);
const VECTOR2 VECTOR2::Q1(1.0F, 1.0F);
const VECTOR2 VECTOR2::Q2(1.0F, -1.0F);
const VECTOR2 VECTOR2::Q3(-1.0F, -1.0F);
const VECTOR2 VECTOR2::Q4(-1.0F, 1.0F);
const VECTOR2 VECTOR2::I(1.0F, 1.0F);

const VECTOR3 VECTOR3::O(0.0F, 0.0F, 0.0F);
const VECTOR3 VECTOR3::OX(1.0F, 0.0F, 0.0F);
const VECTOR3 VECTOR3::OY(0.0F, 1.0F, 0.0F);
const VECTOR3 VECTOR3::OZ(0.0F, 0.0F, 1.0F);
const VECTOR3 VECTOR3::NOX(-1.0F, 0.0F, 0.0F);
const VECTOR3 VECTOR3::NOY(0.0F, -1.0F, 0.0F);
const VECTOR3 VECTOR3::NOZ(0.0F, 0.0F, -1.0F);
const VECTOR3 VECTOR3::I(1.0F, 1.0F, 1.0F);

//// normals/inverted normals for based 6 view
//// same as VIEWTYPE
//const VECTOR3 VECTOR3::NVIEW[6] = {
//	VECTOR3::OZ,	// VM_FRONT
//	VECTOR3::NOX,	// VM_LEFT
//	VECTOR3::NOY,	// VM_BOTTOM
//	VECTOR3::NOZ,	// VM_BACK
//	VECTOR3::OX,	// VM_RIGHT
//	VECTOR3::OY		// VM_TOP
//};

const VECTOR3 VECTOR3::INVIEW[6] = {
	VECTOR3::NOZ,	// VM_FRONT
	VECTOR3::OX,	// VM_LEFT
	VECTOR3::OY,	// VM_BOTTOM
	VECTOR3::OZ,	// VM_BACK
	VECTOR3::NOX,	// VM_RIGHT
	VECTOR3::NOY	// VM_TOP
};

/*
standart base-axis vectors (N(normal), V, W)
for based 6 view (front, left, bottom, back, right, top)
(correspond to enums VIEWTYPE, eProjectPlane)
*/
const VECTOR3 TEXTUREAXIS[6][3] =
{
	//		N			 V(S)		  W(T)
	{ VECTOR3::OZ,  VECTOR3::OX,  VECTOR3::OY },	// front(south wall)
	{ VECTOR3::NOX, VECTOR3::OZ,  VECTOR3::OY },	// left(west wall)
	{ VECTOR3::NOY, VECTOR3::OX,  VECTOR3::OZ },	// bottom(floor)
	{ VECTOR3::NOZ, VECTOR3::NOX, VECTOR3::OY },	// back(north wall)
	{ VECTOR3::OX,  VECTOR3::NOZ, VECTOR3::OY },	// right(east wall)
	{ VECTOR3::OY,  VECTOR3::NOX,  VECTOR3::OZ }	// top(ceiling)
};

/*
vector components for standart base-axis plane (1, 2, 3(BN))
*/
const unsigned BASEDIMS[6][3] = 
{
	// first	two		threed
	{	0,		 1,		 2 },	// front
	{	2,		 1,		 0 },	// left
	{	0,		 2,		 1 },	// bottom
	{	0,		 1,		 2 },	// back
	{	2,		 1,		 0 },	// right
	{	0,		 2,		 1 }	// top
};


const BBOX1 BBOX1::C(FLT_MAX, -FLT_MAX);
const BBOX2 BBOX2::C(FLT_MAX, FLT_MAX, -FLT_MAX, -FLT_MAX);
const BBOX3 BBOX3::C(FLT_MAX, FLT_MAX, FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX);
const BBOX3::boxsideinfo_ BBOX3::BSI[BBOX3::BSI_TOTAL] = {
	{ 1.F,  2, 0, 1 },		// front
	{ -1.F, 0, 2, 1 },		// left
	{ -1.F, 1, 0, 2 },		// bottom
	{ -1.F, 2, 0, 1 },		// back
	{ 1.F,  0, 2, 1 },		// right
	{ 1.F,  1, 0, 2 }		// top
};




const MATRIX16 MATRIX16::I(1.F, 0.F, 0.F, 0.F,
						   0.F, 1.F, 0.F, 0.F,
						   0.F, 0.F, 1.F, 0.F,
						   0.F, 0.F, 0.F, 1.F);


/*
=============================
VECTOREX IMPL
=============================
*/





/*
=============================
BBox-Ray Intersection
=============================
*/
bool BBOX3::Intersection(const VECTOR3& rayP0, const VECTOR3& rayV, FLOAT* t) const
	{
	for(unsigned bside = 0; bside < 5; ++bside)
		if(rayV[BSI[bside].bnindex])
		{
			// find intersection plane-line
			const VECTOR3& p0 = bside < 3 ? Max : Min;
			const FLOAT tt = (p0[BSI[bside].bnindex] - rayP0[BSI[bside].bnindex]) *
					BSI[bside].boxNormal / rayV[BSI[bside].bnindex] * BSI[bside].boxNormal;
			const VECTOR3 q = rayP0 + tt * rayV;
			// check on accessory _BBox
			if(	q[BSI[bside].windex] > Min[BSI[bside].windex] &&
				q[BSI[bside].windex] < Max[BSI[bside].windex] && 
				q[BSI[bside].hindex] > Min[BSI[bside].hindex] &&
				q[BSI[bside].hindex] < Max[BSI[bside].hindex])
			{
				if(t) *t = tt;
				return true;
			}
		}
	return false;
	}


BBOX3::eBoxSideInfo  BBOX3::Intersection(const VECTOR3& rayP0, const VECTOR3& rayV,
										VECTOR3* q, FLOAT* t) const
{
	eBoxSideInfo iNearSide = BBOX3::BSI_NONE;
	float tnear = FLT_MAX;
	VECTOR3 qnear;

	for(unsigned bside = 0; bside < 6; ++bside)
		if(rayV[BSI[bside].bnindex])
		{
			// find intersection plane-line
			const VECTOR3& p0 = bside < 3 ? Max : Min;
			float tt = (p0[BSI[bside].bnindex] - rayP0[BSI[bside].bnindex]) *
						BSI[bside].boxNormal / rayV[BSI[bside].bnindex] * BSI[bside].boxNormal;
			const VECTOR3 qq = rayP0 + tt * rayV;
			// check on accessory _BBox
			if(	qq[BSI[bside].windex] > Min[BSI[bside].windex] &&
				qq[BSI[bside].windex] < Max[BSI[bside].windex] && 
				qq[BSI[bside].hindex] > Min[BSI[bside].hindex] &&
				qq[BSI[bside].hindex] < Max[BSI[bside].hindex] &&
				tt > 0 && tt < tnear)
			{
				
				iNearSide = (eBoxSideInfo)bside;
				qnear = qq;
				tnear = tt;
			}
		}

	if(q) *q = qnear;
	if(t) *t = tnear;
	return iNearSide;
}



/*
=============================
ClipCross
=============================
*/
bool ClipCross(const POLYGONE2& poly, VECTOR2& A, VECTOR2& B)
{
	FLOAT t, u, tt[2];
	VECTOR2 V = B - A;
	size_t k = 0;

	for(size_t i = 0; i < poly.size(); ++i)
		if(CrossParm(A, B, poly[i], poly[(i+1)%poly.size()], &t, &u) != -1)
			if(u >= 0 && u < 1)
			{
				tt[k++] = t;
				if(k == 2)
					break;
			}

	if(k)
	{
		if(tt[0] < 0)
		{
			if(tt[1] < 0)
				B = A;
			else if(tt[1] >= 0 && tt[1] <= 1)
				B = A + V * tt[1];
		}
		else if(tt[0] > 1)
		{
			if(tt[1] >= 0 && tt[1] <= 1)
				A += V * tt[0];
			else if(tt[1] > 1)
				A = B;
		}
		else
		{
			if(tt[1] < 0)
				B = A + V * tt[0];
			else if(tt[1] > 1)
				A += V * tt[0];
			else
			{
				B = A + V * tt[1];
				A += V * tt[0];
			}
		}

		return (A - B).MagnitudeSq() != 0;
	}
	else return false;
}






/*
=============================
ClipConv
=============================
*/
int ClipConv(const POLYGONE2* poly, VECTOR2* A, VECTOR2* B,
			 VECTOR2* C, VECTOR2* D, bool internal )
{
	FLOAT t, L[4] = {0, 1};
	size_t k = 2;

	for(size_t i = 0, j = 1; i < poly->size() && k < 4; ++i, ++j)
		if(CrossParm(*A, *B, (*poly)[i], (*poly)[j != poly->size() ? j : 0], &t) == 1)
			L[k++] = t;

	VECTOR2 V = *B - *A;
	if(k > 2)
	{
		for(FLOAT *lo = &L[0], *hi = &L[k-1], *max = L, tmp; hi > lo; --hi, max = lo) {	// "shortsort"
			for(FLOAT *p = lo + 1; p <= hi; ++p)
				if(*p > *max)
					max = p;
			tmp = *max, *max = *hi, *hi = tmp;
		}

		if(internal)
		{
			VECTOR2 *s = A, *e = B, a = *A;
			size_t ret = 0;
			for(size_t m = 0, n = 1; m < k-1; ++m, ++n)
				if(OrientationConv(*poly, *A + V * 0.5F * (L[m] + L[n])) == 1)
				{
					*s = a + L[m]*V;
					*e = a + L[n]*V;
					s = C, e = D, ++ret;
				}

			return (signed)ret;
		}
		else
			for(size_t m = 0, n = 1; m < k-1; ++m, ++n)
				if(OrientationConv(*poly, *A + V * 0.5F * (L[m] + L[n])) == -1)
					return *B = *A + L[n]*V, *A += L[m]*V, 1;
	}

	return OrientationConv(*poly, *A + V * 0.5F) == (internal ? 1 : -1);
}




/*
=============================
Intersection
=============================
*/
int Intersection(const POLYHEDRON* PH, VECTOR3* A, VECTOR3* B,
				 VECTOR3* C, VECTOR3* D, bool strong, bool internal)
{
	float t, L[4] = {0, 1};
	size_t k = 2;

	for(size_t i = 0, j = 1; i < PH->G_.size() && k < 4; ++i, ++j)
		if(Intersection<OrientationConv>(PH->GetPoly(i), *A, *B, 0, &t, strong) == 1)
			L[k++] = t;

	VECTOR3 V = *B - *A;
	if(k > 2)
	{
		for(float *lo = &L[0], *hi = &L[k-1], *max = L, tmp; hi > lo; --hi, max = lo) {	// "shortsort"
			for(float *p = lo + 1; p <= hi; ++p)
				if(*p > *max)
					max = p;
			tmp = *max, *max = *hi, *hi = tmp;
		}

		if(internal)
		{
			VECTOR3 *s = A, *e = B, a = *A;
			size_t ret = 0;
			for(size_t m = 0, n = 1; m < k-1; ++m, ++n)
				if(OrientationConv(*PH, *A + V * 0.5F * (L[m] + L[n])) == 1)
				{
					*s = a + L[m]*V;
					*e = a + L[n]*V;
					s = C, e = D, ++ret;
				}

			return (signed)ret;
		}
		else
			for(size_t m = 0, n = 1; m < k-1; ++m, ++n)
				if(OrientationConv(*PH, *A + V * 0.5F * (L[m] + L[n])) == -1)
					return *B = *A + L[n]*V, *A += L[m]*V, 1;
	}

	return OrientationConv(*PH, *A + V * 0.5F) == (internal ? 1 : -1);
}




/*
=============================
Intersection
=============================
*/
bool Intersection(const VECTOR3& clipperN, const VECTOR3& clipperP0, const VECTOR3& clipperP1,
				  const POLYGONE3& clipped, const VECTOR3& clippedN, 
				  POLYGONE3& extr, POLYGONE3& intr, bool coincided)
{
	bool flag = false;
	for(POLYGONE3::const_iterator i1 = clipped.begin(), i2 = i1 + 1;
								  i1 != clipped.end();
								  ++i1, ++i2 != clipped.end() ? i2 : i2 = clipped.begin())
	{
		const float dot1 = (*i1 - clipperP0) | clipperN,
					dot2 = (*i2 - clipperP0) | clipperN;

		if(dot1 > 0)
			extr.push_back(*i1);
		else if(dot1 == 0)
			extr.push_back(*i1), intr.push_back(*i1);
		else
			intr.push_back(*i1);

		if((dot1 >/*=*/0 && dot2 < 0) || (dot2 >= 0 && dot1 < 0))
		{
			const VECTOR3 V(*i2 - *i1);
			const float t = ((clipperP0 - *i1) | clipperN) / (V | clipperN);
			if(t > 0 && t < 1)
			{
				if(coincided)
				{
				const VECTOR3 clippedBN(clippedN ^ -V);
				const float dot3 = (clipperP0 - *i1) | clippedBN,
							dot4 = (clipperP1 - *i1) | clippedBN;
				if(dot3 >= 0 && dot4 > 0)		//if(dot3 >= 0 && dot4 > 0)
					continue;
				}

				const VECTOR3 r(*i1 + V * t);
				extr.push_back(r), intr.push_back(r);
				flag = true;
			}
		}
	}
	return flag;
}



/*
=============================
Intersection
=============================
*/
bool Intersection(const POLYGONE3& clipper, const POLYGONELIST& clippedlist,
				  POLYGONELIST& dstlist)
{
	bool flag = false;
	POLYGONE3 extr(5), intr(5);
	const VECTOR3 clipperN(clipper[1] - clipper[0] ^ clipper[2] - clipper[1]/*clipper.GetNormal()*/),
				  clippedN(clippedlist[0][1] - clippedlist[0][0] ^ clippedlist[0][2] - clippedlist[0][1]/*clippedlist[0].GetNormal()*/);

	if((clipperN ^ clippedN).isZero())								// параллельны
	{
		if(((clippedlist[0][0] - clipper[0]) | clippedN) == 0)		// совпадают
		{
			for(POLYGONELIST::const_iterator i = clippedlist.begin();
											 i != clippedlist.end();
											 ++i)
			{
				POLYGONE3 tmp(*i);

				for(POLYGONE3::const_iterator p1 = clipper.begin(), p2 = p1 + 1;
								  p1 != clipper.end();
								  ++p1, ++p2 != clipper.end() ? p2 : p2 = clipper.begin())
				{
					Intersection(clipperN ^ (*p1 - *p2), *p1, *p2,
						tmp, clippedN, extr, intr, true);

					if(extr.size() > 2)			// есть отсечение
					{
						dstlist.push_back(extr);
						tmp = intr;
						flag = true;
					}

					if(intr.empty())			// отсекаемый вне
						break;

					extr.clear(), intr.clear();
				}
			}
		}
	}
	else
	{
		const BBOX3 clipperbox(&clipper[0], clipper.size());
		for(POLYGONELIST::const_iterator i = clippedlist.begin(); i != clippedlist.end(); ++i)
		{
			if(isIntersectGab(*i, clipperbox, true)/*&& isIntersectConv(clipper, *i) && isIntersectConv(*i, clipper)*/) // TODO: "isIntersectConv" is NEED this !!!
			{
				if(Intersection(clipperN, clipper[0], clipper[1], *i, clippedN, extr, intr))
					dstlist.push_back(extr),
					dstlist.push_back(intr),
					flag = true;
				else
					dstlist.push_back(*i);
				extr.clear(), intr.clear();
			}
			else
				dstlist.push_back(*i);
		}
	}

	return flag;
}