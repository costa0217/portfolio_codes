#pragma once



struct MyPoint
{
	float x; float y;

	MyPoint() {};
	MyPoint(float fX, float fY) :x(fX), y(fY) {};

	void operator = (MyPoint& a) {
		x = a.x;
		y = a.y;
	}
};

struct MyVector
{
	float x;	float y;	float z;
	MyVector() : x(0.f), y(0.f), z(0.f) {};
	MyVector(float fX, float fY, float fZ) :x(fX), y(fY), z(fZ) {};
	void operator = (MyVector& a) {
		x = a.x;	y = a.y;	z = a.z;
	}
	bool operator != (MyVector& a) {
		return !(x == a.x && y == a.y && z == a.z);
	}
	bool operator == (MyVector& a) {
		return (x == a.x && y == a.y && z == a.z);
	}
	MyVector operator - (MyVector& a) {
		return MyVector(x - a.x, y - a.y, z - a.z);
	}
	MyVector operator + (MyVector& a) {
		return MyVector(x + a.x, y + a.y, z + a.z);
	}
	MyVector operator * (MyVector& a) {
		return MyVector(x * a.x, y * a.y, z * a.z);
	}
	MyVector operator * (float d) {
		return MyVector(x * d, y * d, z * d);
	}
	MyVector operator / (float d) {
		return MyVector(x / d, y / d, z / d);
	}
};

struct MyQuaternion
{
	float x; float y; float z; float w;

	MyQuaternion() {};
	MyQuaternion(float fX, float fY, float fZ, float fW)
		:x(fX), y(fY), z(fZ), w(fW) {};

	void operator = (MyQuaternion& a) {
		x = a.x;
		y = a.y;
		z = a.z;
		w = a.w;
	}

	MyQuaternion operator * (MyQuaternion& a) {
		return MyQuaternion(x * a.x - y * a.y - z * a.z - w * a.w,
							x * a.y + y * a.x + z * a.w - w * a.z,
							x * a.z - y * a.w + z * a.x + w * a.y,
							x * a.w + y * a.z - z * a.y + w * a.x);
	}
};

#pragma once

typedef struct MATRIX4X4 {
	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;

		};
		float m[4][4];
	};
} MATRIXSTRUCT;


typedef struct MATRIX : public MATRIXSTRUCT
{
	MATRIX();
	MATRIX(CONST FLOAT *pf);
	MATRIX(CONST MATRIX& mat);
	MATRIX(FLOAT f11, FLOAT f12, FLOAT f13, FLOAT f14,
		FLOAT f21, FLOAT f22, FLOAT f23, FLOAT f24,
		FLOAT f31, FLOAT f32, FLOAT f33, FLOAT f34,
		FLOAT f41, FLOAT f42, FLOAT f43, FLOAT f44);

	const MATRIX* MyMultiplyMatrix(MATRIX *pOut, const MATRIX *pm1, const MATRIX *pm2);

	FLOAT& operator () (UINT row, UINT col);
	FLOAT operator () (UINT row, UINT col) const;

	operator FLOAT* ();
	operator CONST FLOAT* () const;

	MATRIX& operator *= (CONST MATRIX&);
	MATRIX& operator += (CONST MATRIX&);
	MATRIX& operator -= (CONST MATRIX&);
	MATRIX& operator *= (FLOAT);
	MATRIX& operator /= (FLOAT);

	MATRIX operator + () const;
	MATRIX operator - () const;

	MATRIX operator * (CONST MATRIX&) const;
	MATRIX operator + (CONST MATRIX&) const;
	MATRIX operator - (CONST MATRIX&) const;
	MATRIX operator * (FLOAT) const;
	MATRIX operator / (FLOAT) const;

	friend MATRIX operator * (FLOAT, CONST MATRIX&);

	BOOL operator == (CONST MATRIX&) const;
	BOOL operator != (CONST MATRIX&) const;
} MATRIX, *LPMATRIX;

inline MATRIX::MATRIX()
{
	_11 = 1.f; _12 = 0.f; _13 = 0.f; _14 = 0.f;
	_21 = 0.f; _22 = 1.f; _23 = 0.f; _24 = 0.f;
	_31 = 0.f; _32 = 0.f; _33 = 1.f; _34 = 0.f;
	_41 = 0.f; _42 = 0.f; _43 = 0.f; _44 = 1.f;
}

inline MATRIX::MATRIX(CONST FLOAT *pf)
{
	if (!pf) return;
	memcpy(&_11, pf, sizeof(MATRIX));
}

inline MATRIX::MATRIX(CONST MATRIX& mat)
{
	memcpy(&_11, &mat, sizeof(MATRIX));
}

inline MATRIX::MATRIX(FLOAT f11, FLOAT f12, FLOAT f13, FLOAT f14,
	FLOAT f21, FLOAT f22, FLOAT f23, FLOAT f24,
	FLOAT f31, FLOAT f32, FLOAT f33, FLOAT f34,
	FLOAT f41, FLOAT f42, FLOAT f43, FLOAT f44)
{
	_11 = f11; _12 = f12; _13 = f13; _14 = f14;
	_21 = f21; _22 = f22; _23 = f23; _24 = f24;
	_31 = f31; _32 = f32; _33 = f33; _34 = f34;
	_41 = f41; _42 = f42; _43 = f43; _44 = f44;
}

inline const MATRIX * MATRIX::MyMultiplyMatrix(MATRIX * pOut
	, const MATRIX * pm1, const MATRIX * pm2)
{
	int i, j;

	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 4; ++j)
		{
			pOut->m[i][j] =
				pm1->m[i][0] * pm2->m[0][j] +
				pm1->m[i][1] * pm2->m[1][j] +
				pm1->m[i][2] * pm2->m[2][j] +
				pm1->m[i][3] * pm2->m[3][j];
		}
	}

	return pOut;
}

inline FLOAT& MATRIX::operator () (UINT row, UINT col)
{
	return m[row][col];
}

inline FLOAT MATRIX::operator () (UINT row, UINT col) const
{
	return m[row][col];
}

inline MATRIX::operator FLOAT* ()
{
	return (FLOAT*)&_11;
}

inline MATRIX::operator CONST FLOAT* () const
{
	return (CONST FLOAT*)&_11;
}

inline MATRIX& MATRIX::operator *= (CONST MATRIX& mat)
{
	MATRIX::MyMultiplyMatrix(this, this, &mat);
	return *this;
}

inline MATRIX& MATRIX::operator += (CONST MATRIX& mat)
{
	_11 += mat._11; _12 += mat._12; _13 += mat._13; _14 += mat._14;
	_21 += mat._21; _22 += mat._22; _23 += mat._23; _24 += mat._24;
	_31 += mat._31; _32 += mat._32; _33 += mat._33; _34 += mat._34;
	_41 += mat._41; _42 += mat._42; _43 += mat._43; _44 += mat._44;
	return *this;
}

inline MATRIX& MATRIX::operator -= (CONST MATRIX& mat)
{
	_11 -= mat._11; _12 -= mat._12; _13 -= mat._13; _14 -= mat._14;
	_21 -= mat._21; _22 -= mat._22; _23 -= mat._23; _24 -= mat._24;
	_31 -= mat._31; _32 -= mat._32; _33 -= mat._33; _34 -= mat._34;
	_41 -= mat._41; _42 -= mat._42; _43 -= mat._43; _44 -= mat._44;
	return *this;
}

inline MATRIX& MATRIX::operator *= (FLOAT f)
{
	_11 *= f; _12 *= f; _13 *= f; _14 *= f;
	_21 *= f; _22 *= f; _23 *= f; _24 *= f;
	_31 *= f; _32 *= f; _33 *= f; _34 *= f;
	_41 *= f; _42 *= f; _43 *= f; _44 *= f;
	return *this;
}

inline MATRIX& MATRIX::operator /= (FLOAT f)
{
	FLOAT inv = 1.0f / f;
	_11 *= inv; _12 *= inv; _13 *= inv; _14 *= inv;
	_21 *= inv; _22 *= inv; _23 *= inv; _24 *= inv;
	_31 *= inv; _32 *= inv; _33 *= inv; _34 *= inv;
	_41 *= inv; _42 *= inv; _43 *= inv; _44 *= inv;
	return *this;
}

inline MATRIX MATRIX::operator + () const
{
	return *this;
}

inline MATRIX MATRIX::operator - () const
{
	return MATRIX(-_11, -_12, -_13, -_14,
		-_21, -_22, -_23, -_24,
		-_31, -_32, -_33, -_34,
		-_41, -_42, -_43, -_44);
}

inline MATRIX MATRIX::operator * (CONST MATRIX& mat) const
{
	MATRIX buf;
	int i, j;

	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 4; ++j)
		{
			buf.m[i][j] =
				this->m[i][0] * mat.m[0][j] +
				this->m[i][1] * mat.m[1][j] +
				this->m[i][2] * mat.m[2][j] +
				this->m[i][3] * mat.m[3][j];
		}
	}

	return buf;
}

inline MATRIX MATRIX::operator + (CONST MATRIX& mat) const
{
	return MATRIX(_11 + mat._11, _12 + mat._12, _13 + mat._13, _14 + mat._14,
		_21 + mat._21, _22 + mat._22, _23 + mat._23, _24 + mat._24,
		_31 + mat._31, _32 + mat._32, _33 + mat._33, _34 + mat._34,
		_41 + mat._41, _42 + mat._42, _43 + mat._43, _44 + mat._44);
}

inline MATRIX MATRIX::operator - (CONST MATRIX& mat) const
{
	return MATRIX(_11 - mat._11, _12 - mat._12, _13 - mat._13, _14 - mat._14,
		_21 - mat._21, _22 - mat._22, _23 - mat._23, _24 - mat._24,
		_31 - mat._31, _32 - mat._32, _33 - mat._33, _34 - mat._34,
		_41 - mat._41, _42 - mat._42, _43 - mat._43, _44 - mat._44);
}

inline MATRIX MATRIX::operator * (FLOAT f) const
{
	return MATRIX(_11 * f, _12 * f, _13 * f, _14 * f,
		_21 * f, _22 * f, _23 * f, _24 * f,
		_31 * f, _32 * f, _33 * f, _34 * f,
		_41 * f, _42 * f, _43 * f, _44 * f);
}

inline MATRIX MATRIX::operator / (FLOAT f) const
{
	FLOAT inv = 1.0f / f;
	return MATRIX(_11 * inv, _12 * inv, _13 * inv, _14 * inv,
		_21 * inv, _22 * inv, _23 * inv, _24 * inv,
		_31 * inv, _32 * inv, _33 * inv, _34 * inv,
		_41 * inv, _42 * inv, _43 * inv, _44 * inv);
}

inline MATRIX operator * (FLOAT f, CONST MATRIX& mat)
{
	return MATRIX(f * mat._11, f * mat._12, f * mat._13, f * mat._14,
		f * mat._21, f * mat._22, f * mat._23, f * mat._24,
		f * mat._31, f * mat._32, f * mat._33, f * mat._34,
		f * mat._41, f * mat._42, f * mat._43, f * mat._44);
}

inline BOOL MATRIX::operator == (CONST MATRIX& mat) const
{
	return (memcmp(this, &mat, sizeof(MATRIX)) == 0);
}

inline BOOL MATRIX::operator != (CONST MATRIX& mat) const
{
	return (memcmp(this, &mat, sizeof(MATRIX)) != 0);
}

typedef struct tagOBBInfo
{
	MyVector		vCenterPos;
	MyVector		vAxisDir[3];
}OBBINFO;

struct MyPlane
{
	MyVector		v0;
	MyVector		v1;
	MyVector		v2;
	MyVector		v3;

	MyPlane() {};
	MyPlane(MyVector a, MyVector b, MyVector c, MyVector d)
		:v0(a), v1(b), v2(c), v3(d) {};

	void operator = (MyPlane& a) {
		v0 = a.v0;	v1 = a.v1;	v2 = a.v2; v3 = a.v3;
	}
};