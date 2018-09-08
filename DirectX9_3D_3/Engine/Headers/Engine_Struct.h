/*!
 * \file Engine_Struct.h
 * \date 2015/07/02 13:47
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef struct tagVertexColor
	{
		D3DXVECTOR3		vPos;
		DWORD			dwColor;		
	}VTXCOL;
	const DWORD		VTXCOL_FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef struct tagVertexTexture
	{
		D3DXVECTOR3		vPos;
		D3DXVECTOR2		vTexUV;
	}VTXTEX;
	const DWORD		VTXTEX_FVF = D3DFVF_XYZ | D3DFVF_TEX1;

	typedef struct tagVertexCube
	{
		D3DXVECTOR3		vPos;
		D3DXVECTOR3		vTex;
	}VTXCUBE;
	const DWORD		VTXCUBE_FVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	typedef struct tagMinSaveData
	{
		D3DXVECTOR3		vRectPos;
		D3DXVECTOR3		vScale;
		float			fAngle[Engine::ANGLE_END];
		int				iColor[Engine::COLOR_END];
	}MINSAVE;

	typedef struct tagStaticObject 
	{
		TCHAR			szObjectName[128];
		D3DXVECTOR3		vPos;
	}STATICOBJECT;

	typedef struct tagIndex16
	{
		WORD		_1, _2, _3;
	}INDEX16;

	typedef struct tagIndex32
	{
		DWORD		_1, _2, _3;
	}INDEX32;
}

#endif // Engine_Struct_h__