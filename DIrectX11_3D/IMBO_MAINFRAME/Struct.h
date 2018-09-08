#pragma once
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

using namespace DirectX;
using namespace DirectX::PackedVector;


struct Pixel24 {
	byte b;
	byte g;
	byte r;
};

struct Pixel32 {
	byte a;
	byte b;
	byte g;
	byte r;
};


struct POINTLERP
{
	float		fTimePoint;
	XMFLOAT3	xmf3PosPoint;
	XMFLOAT3	xmf3AnglePoint;
	XMFLOAT3	xmf3ScalePoint;
	XMFLOAT4	xmf4ColorPoint;
};


struct EFFECTDATA
{
	int			iTexOpt;
	int			iU;
	int			iV;
	float		fFrameSpd;
	bool		bBillboard;
	bool		bDistortion;
	bool		bRepeat;
	TCHAR		szName[128];
	int			iPointSize;
};

