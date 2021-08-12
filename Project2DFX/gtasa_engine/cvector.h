#ifndef _CVECTOR
#define _CVECTOR

#include <RwV3D.h>
#include <cmath>

class CVector
{
public:
	float	x, y, z;

	CVector()
	{}

	CVector(float fX, float fY, float fZ=0.0f)
		: x(fX), y(fY), z(fZ)
	{}

	CVector(const RwV3d& rwVec)
		: x(rwVec.x), y(rwVec.y), z(rwVec.z)
	{}

	CVector&		operator+=(const CVector& vec)
			{ x += vec.x; y += vec.y; z += vec.z;
			return *this; }
	CVector&		operator+=(const RwV3d& vec)
			{ x += vec.x; y += vec.y; z += vec.z;
			return *this; }
	CVector&		operator-=(const CVector& vec)
			{ x -= vec.x; y -= vec.y; z -= vec.z;
			return *this; }
	CVector&		operator-=(const RwV3d& vec)
			{ x -= vec.x; y -= vec.y; z -= vec.z;
			return *this; }

	inline float	Magnitude()
		{ return sqrt(x * x + y * y + z * z); }
	inline float	MagnitudeSqr()
		{ return x * x + y * y + z * z; }
	inline CVector&	Normalise()
		{ float	fInvLen = 1.0f / Magnitude(); x *= fInvLen; y *= fInvLen; z *= fInvLen; return *this; }
	inline float	NormaliseAndMag()
		{ float fLen = Magnitude(); float fInvLen = 1.0f / fLen; x *= fInvLen; y *= fInvLen; z *= fInvLen; return fLen; }

	friend inline float DotProduct(const CVector& vec1, const CVector& vec2)
		{ return vec1.x * vec2.x + vec1.x * vec2.y + vec1.z * vec2.z; }
	friend inline CVector CrossProduct(const CVector& vec1, const CVector& vec2)
		{ return CVector(	vec1.y * vec2.z - vec1.z * vec2.y,
							vec1.z * vec2.x - vec1.x * vec2.z,
							vec1.x * vec2.y - vec1.y * vec2.x); }

	friend inline CVector operator*(const CVector& in, float fMul)
		{ return CVector(in.x * fMul, in.y * fMul, in.z * fMul); }
	friend inline CVector operator+(const CVector& vec1, const CVector& vec2)
		{ return CVector(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z); }
	friend inline CVector operator+(const CVector& vec1, const RwV3d& vec2)
		{ return CVector(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z); }
	friend inline CVector operator-(const CVector& vec1, const CVector& vec2)
		{ return CVector(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z); }
	friend inline CVector operator-(const CVector& vec1, const RwV3d& vec2)
		{ return CVector(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z); }
	friend inline CVector operator-(const CVector& vec)
		{ return CVector(-vec.x, -vec.y, -vec.z); }

	inline CVector&		FromMultiply(const class CMatrix& mat, const CVector& vec);
	inline CVector&		FromMultiply3X3(const class CMatrix& mat, const CVector& vec);
};

#endif // _CVECTOR