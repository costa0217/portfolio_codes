
typedef struct tagLerp
{
	D3DXVECTOR3		vPos;
	float			fAngle[3];
	tagLerp()
	{
		ZeroMemory(fAngle, sizeof(float) * 3);
	}
}LERPSAVE;