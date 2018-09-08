#pragma once
template<typename T>
static void Safe_Delete(T& Temp)
{
	if (Temp)
	{
		delete Temp;
		Temp = NULL;
	}
}

static float Vec3Dot(const MyVector Dest, const MyVector Sour)
{
	return (Dest.x * Sour.x + Dest.y * Sour.y + Dest.z * Sour.z);
}
static MyVector Vec3Cross(const MyVector Dest, const MyVector Sour)
{
	return MyVector(Dest.y * Sour.z - Dest.z * Sour.y,
		Dest.z * Sour.x - Dest.x * Sour.z,
		Dest.x * Sour.y - Dest.y * Sour.x);
}

static void MyRotationX(MyVector* pOut, MyVector* pIn, float fAngle)
{
	/*1	0	0	0
	0	c	s	0
	0	-s	c	0
	0	0	0	1*/

	MyVector	vTemp = *pIn;
	pOut->x = vTemp.x;
	pOut->y = vTemp.y * cosf(fAngle) - vTemp.z * sinf(fAngle);
	pOut->z = vTemp.y * sinf(fAngle) + vTemp.z * cosf(fAngle);
}

static void MyRotationY(MyVector* pOut, MyVector* pIn, float fAngle)
{
	/*c	0	-s	0
	0	1	0	0
	s	0	c	0
	0	0	0	1*/

	MyVector	vTemp = *pIn;
	pOut->x = vTemp.x * cosf(fAngle) + vTemp.z * sinf(fAngle);
	pOut->y = vTemp.y;
	pOut->z = vTemp.x * -sinf(fAngle) + vTemp.z * cosf(fAngle);
}


static void MyRotationZ(MyVector* pOut, MyVector* pIn, float fAngle)
{
	/*c	s	0	0
	-s	c	0	0
	0	0	1	0
	0	0	0	1*/

	MyVector		vTemp = *pIn;
	pOut->x = vTemp.x * cosf(fAngle) - vTemp.y * sinf(fAngle);
	pOut->y = vTemp.x * sinf(fAngle) + vTemp.y * cosf(fAngle);
	pOut->z = vTemp.z;
}
static void MyIdentityMatrix(MATRIX* pOut)
{
	pOut->m[0][1] = pOut->m[0][2] = pOut->m[0][3] =
		pOut->m[1][0] = pOut->m[1][2] = pOut->m[1][3] =
		pOut->m[2][0] = pOut->m[2][1] = pOut->m[2][3] =
		pOut->m[3][0] = pOut->m[3][1] = pOut->m[3][2] = 0.0f;

	pOut->m[0][0] = pOut->m[1][1] = pOut->m[2][2] = pOut->m[3][3] = 1.0f;
}

static void MyNormalize(MyVector* pOut, MyVector* pV)
{
	float fx = pV->x;
	float fy = pV->y;
	float fz = pV->z;

	float fLength = sqrtf((fx * fx) + (fy * fy) + (fz * fz));

	pOut->x = fx / fLength;
	pOut->y = fy / fLength;
	pOut->z = fz / fLength;
}

static void MyTransformCoord(MyVector* pOut,
	const MyVector* pV,
	const MATRIX* pM)
{
	// 이동 적용
	float x = (pV->x * pM->_11) + (pV->y * pM->_21) + (pV->z * pM->_31) + pM->_41;
	float y = (pV->x * pM->_12) + (pV->y * pM->_22) + (pV->z * pM->_32) + pM->_42;
	float z = (pV->x * pM->_13) + (pV->y * pM->_23) + (pV->z * pM->_33) + pM->_43;

	// 변환 가중치
	float w = (pV->x * pM->_14) + (pV->y * pM->_24) + (pV->z * pM->_34) + pM->_44;

	// 변환 가중치를 나누어 리턴시킴
	// 투영변환까지 생각하면 꼭 해줘야된다는군..
	pOut->x = x / w;
	pOut->y = y / w;
	pOut->z = z / w;
}


static void MyTransformNormal(MyVector* pOut,
	const MyVector* pV,
	const MATRIX* pM)
{

	// 이동 적용X
	float x = (pV->x * pM->_11) + (pV->y * pM->_21) + (pV->z * pM->_31);
	float y = (pV->x * pM->_12) + (pV->y * pM->_22) + (pV->z * pM->_32);
	float z = (pV->x * pM->_13) + (pV->y * pM->_23) + (pV->z * pM->_33);

	pOut->x = x;
	pOut->y = y;
	pOut->z = z;
}
static void MyTranslation(MATRIX *pOut, float x, float y, float z)
{
	pOut->_41 = x;
	pOut->_42 = y;
	pOut->_43 = z;
}

static void MyScaling(MATRIX *pOut, float cx, float cy, float cz)
{
	pOut->_11 = cx;
	pOut->_22 = cy;
	pOut->_33 = cz;
}

static void MyRotationXMatrix(MATRIX * pOut, float fAngle)
{
	pOut->_22 = cosf(fAngle);
	pOut->_23 = sinf(fAngle);
	pOut->_32 = -sinf(fAngle);
	pOut->_33 = cosf(fAngle);
}
static void MyRotationYMatrix(MATRIX * pOut, float fAngle)
{
	pOut->_11 = cosf(fAngle);
	pOut->_13 = -sinf(fAngle);
	pOut->_31 = sinf(fAngle);
	pOut->_33 = cosf(fAngle);
}
static void MyRotationZMatrix(MATRIX * pOut, float fAngle)
{
	pOut->_11 = cosf(fAngle);
	pOut->_12 = sinf(fAngle);
	pOut->_21 = -sinf(fAngle);
	pOut->_22 = cosf(fAngle);
}
static void MyQuternionToMat(MATRIX* pOut, MyQuaternion qua)
{
	pOut->_11 = 1.f - 2.f * pow(qua.y, 2) - 2.f * pow(qua.z, 2);
	//			1.f - 2y^2 - 2z^2
	pOut->_12 = 2.f * qua.x * qua.y - 2.f * qua.w * qua.z;
	//			2xy - 2wz;
	pOut->_13 = 2.f * qua.x * qua.z + 2.f * qua.w * qua.y;
	//			2xz + 2wy;
	pOut->_14 = 0.f;

	pOut->_21 = 2.f * qua.x * qua.y + 2.f * qua.w * qua.z;
	pOut->_22 = 1.f - 2.f * pow(qua.x, 2) - 2.f * pow(qua.z, 2);
	pOut->_23 = 2.f * qua.y * qua.z - 2.f * qua.w * qua.x;
	pOut->_24 = 0.f;

	pOut->_31 = 2.f * qua.x * qua.z - 2.f * qua.w * qua.y;
	pOut->_32 = 2.f * qua.y * qua.z + 2.f * qua.w * qua.x;
	pOut->_33 = 1.f - 2.f * pow(qua.x, 2) - 2.f * pow(qua.y, 2);
	pOut->_34 = 0.f;

	pOut->_41 = 0.f;
	pOut->_42 = 0.f;
	pOut->_43 = 0.f;
	pOut->_44 = 1.f;
}


static bool	IntersectTriangle(MyVector& pos, MyVector& dir,
	MyVector& v0, MyVector& v1, MyVector& v2, float* t, float* u, float* v)
{
	MyVector	edge1 = v1 - v0;
	MyVector	edge2 = v2 - v0;

	MyVector	pvec;
	pvec = Vec3Cross(dir, edge2);

	float det = Vec3Dot(edge1, pvec);

	MyVector	tvec;
	if (det > 0)
	{
		tvec = pos - v0;
	}
	else
	{
		tvec = v0 - pos;
		det = -det;
	}
	if (det < 0.0001f)
		return false;

	*u = Vec3Dot(tvec, pvec);
	if (*u < 0.0f || *u > det)
		return false;

	MyVector	qvec;
	qvec = Vec3Cross(tvec, edge1);
	*v = Vec3Dot(dir, qvec);
	if (*v < 0.0f || *u + *v > det)
	{
		return false;
	}

	*t = Vec3Dot(edge2, qvec);
	float fInvDet = 1.0f / det;
	*t *= fInvDet;
	*u *= fInvDet;
	*v *= fInvDet;

	return true;
}

static MyVector MakeBezier(MyVector p1, MyVector p2, MyVector p3, float t)
{
	// t 0 ~ 1
	MyVector vResult;
	vResult = p1 * pow(1 - t, 2.f)  +  p2 * 2 * t * (1 - t) + p3 * pow(t, 2.f);
	return vResult;
} 

static MyVector MakeCatmullRom(float t1, MyVector v0, MyVector v1, MyVector v2, MyVector v3)
{
	float t2 = t1 * t1;
	float t3 = t2 * t1;
	return (v0 * (-t3 + 2.0f * t2 - t1) +
			v1 * (3.0f * t3 - 5.0f * t2 + 2.0f) +
			v2 * (-3.0f * t3 + 4.0f * t2 + t1) +
			v3 * (t3 - t2)) / 2.0f;
}
static float GetVec3Dist(MyVector v0, MyVector v1)
{
	return sqrtf(pow(v1.x - v0.x, 2.f) + pow(v1.y - v0.y, 2.f) + pow(v1.z - v0.z, 2.f));
}

static MyVector CalculateLerp(float t, MyVector q0, MyVector q1)
{
	return q0 * (1 - t) + q1 * t;
}