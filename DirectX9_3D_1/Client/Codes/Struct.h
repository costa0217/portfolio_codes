#ifndef Struct_h__
#define Struct_h__

typedef struct tagBoundingBox		// �Ľ̿�(���̸� ������ TCHAR ������ ��)
{
	int						iAniIdx;
	BOXTAG					eBoxTag;
	TCHAR					pFrameName[128];	// ������ �̸�
	LPDIRECT3DVERTEXBUFFER9	pVB;				// �ٿ���ڽ� ���ؽ�����
	LPDIRECT3DINDEXBUFFER9	pIB;				// �ٿ���ڽ� �ε�������
	D3DXVECTOR3				vMin;				// �ּҰ�
	D3DXVECTOR3				vMax;				// �ִ밪

	float					fStartTime;	// �ٿ���ڽ� �����ð�
	float					fEndTime;	// �ٿ���ڽ� �Ҹ�ð�

}BOXINFO;

typedef struct tagClientBoundingBox	// Ŭ���̾�Ʈ���� ���������� ����� �ڽ� ����ü
{
	int						iAniIdx;			//�ִϸ��̼� �ε���
	BOXTAG					eBoxTag;			//� ������ �ڽ�����
	char					pFrameName[128];	// ������ �̸�
	LPDIRECT3DVERTEXBUFFER9	pVB;				// �ٿ���ڽ� ���ؽ�����
	LPDIRECT3DINDEXBUFFER9	pIB;				// �ٿ���ڽ� �ε�������
	D3DXVECTOR3				vMin;				// �ּҰ�
	D3DXVECTOR3				vMax;				// �ִ밪

	float					fStartTime;	// �ٿ���ڽ� �����ð�
	float					fEndTime;	// �ٿ���ڽ� �Ҹ�ð�

}CLIENTBOXINFO;

typedef struct tagStaticObjCollisionBox	// Ŭ���̾�Ʈ���� ���������� ����� �ڽ� ����ü
{
	D3DXVECTOR3				vMin;				// �ּҰ�
	D3DXVECTOR3				vMax;				// �ִ밪

}STATICBOXINFO;
	
typedef struct tagBoundingSphere	// �ٿ�����Ǿ� ����ü
{
	D3DXVECTOR3				vPos;		// ��ġ(Y ���� ����)
	float					fRadius;	// ������

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
	float	fStartTime;	// �ٿ���ڽ� �����ð�
	float	fEndTime;	// �ٿ���ڽ� �Ҹ�ð�

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
