#ifndef Struct_h__
#define Struct_h__

typedef struct tagBoundingBox		// 파싱용(뼈이름 저장을 TCHAR 형으로 함)
{
	int						iAniIdx;
	BOXTAG					eBoxTag;
	TCHAR					pFrameName[128];	// 프레임 이름
	LPDIRECT3DVERTEXBUFFER9	pVB;				// 바운딩박스 버텍스버퍼
	LPDIRECT3DINDEXBUFFER9	pIB;				// 바운딩박스 인덱스버퍼
	D3DXVECTOR3				vMin;				// 최소값
	D3DXVECTOR3				vMax;				// 최대값

	float					fStartTime;	// 바운딩박스 생성시간
	float					fEndTime;	// 바운딩박스 소멸시간

}BOXINFO;

typedef struct tagClientBoundingBox	// 클라이언트에서 실질적으로 사용할 박스 구조체
{
	int						iAniIdx;			//애니메이션 인덱스
	BOXTAG					eBoxTag;			//어떤 종류의 박스인지
	char					pFrameName[128];	// 프레임 이름
	LPDIRECT3DVERTEXBUFFER9	pVB;				// 바운딩박스 버텍스버퍼
	LPDIRECT3DINDEXBUFFER9	pIB;				// 바운딩박스 인덱스버퍼
	D3DXVECTOR3				vMin;				// 최소값
	D3DXVECTOR3				vMax;				// 최대값

	float					fStartTime;	// 바운딩박스 생성시간
	float					fEndTime;	// 바운딩박스 소멸시간

}CLIENTBOXINFO;

typedef struct tagStaticObjCollisionBox	// 클라이언트에서 실질적으로 사용할 박스 구조체
{
	D3DXVECTOR3				vMin;				// 최소값
	D3DXVECTOR3				vMax;				// 최대값

}STATICBOXINFO;
	
typedef struct tagBoundingSphere	// 바운딩스피어 구조체
{
	D3DXVECTOR3				vPos;		// 위치(Y 값만 있음)
	float					fRadius;	// 반지름

}SPHEREINFO;

typedef struct ControlPointInfo
{
	_vec3 vAtPos;
	_vec3 vEyePos;
	_float fSpeed;
}CONTROLPOINTINFO;

typedef struct tagCharacterInfo
{
	_float		fMaxHP;
	_float		fHP;
	_float		fAttack;
	_float		fDownValue;
}CHARACTERINFO;

typedef struct tagFootAnimationSync
{
	float	fStartTime;	// 바운딩박스 생성시간
	float	fEndTime;	// 바운딩박스 소멸시간

	bool	bLeftFoot;
	bool	bFront;
	bool	bCompulsion;
}FOOTANISYNC;

typedef struct tagSwordInfo
{
	TCHAR	szMeshName[128];
	_matrix	matColor;
}SWORDINFO;

#endif // Struct_h__
