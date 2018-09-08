/*!
 * \file Engine_Struct.h
 * \date 2015/11/07 1:25
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief Engine_Struct ��� ����
 *
 * TODO: �������� ����� Struct�� ��� ���� ��� ����
 *
 * \note
*/

#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	/*
	typedef struct _D3DXFRAME {
	LPTSTR Name;						�������� �̸�
	D3DXMATRIX TransformationMatrix;	��ȯ���
	LPD3DXMESHCONTAINER pMeshContainer;	�޽� �����̳��� ������
	struct _D3DXFRAME *pFrameSibling;	���� �������� ������
	struct _D3DXFRAME *pFrameFirstChild;���� �������� ������
	} D3DXFRAME, *LPD3DXFRAME;
	*/
	typedef struct tagFrame_Derived : public _D3DXFRAME
	{
		//���� TransformationMatrix�� �θ��� TransformationMatrix�� ���� ���
		D3DXMATRIX		CombinedTransformationMatrix;
	}D3DXFRAME_DERIVED;

	typedef struct tagMeshContainer_Derived : public _D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9*		ppTextures;	//�ؽ��ĸ� �޾ƿ� ������
		LPDIRECT3DTEXTURE9*		m_ppNormalTexture;	//��ָ� �ؽ���
		LPD3DXMESH				pOriMesh;	//���� �޽�
		DWORD					dwNumBones;
		D3DXMATRIX*				pOffsetFrameMatrices; //ó�� ��Ʈ����?
		D3DXMATRIX**			ppCombinedTransformationMatrices; //�ִϸ��̼ǰ� ���յ� ��Ʈ����?
		D3DXMATRIX*				pRenderingMatrices;	//�������� ��Ʈ����?
	}D3DXMESHCONTAINER_DERIVED;

	typedef struct tagVertexScreen
	{
		D3DXVECTOR4				vPosition;	
		D3DXVECTOR2				vTexUV;
	}VTXSCREEN;
	const DWORD VTXFVF_VTXSCREEN = D3DFVF_XYZRHW | D3DFVF_TEX1;

	typedef struct tagVertexScreen2
	{
		D3DXVECTOR4				vPosition;	
		D3DXVECTOR2				vTexUV;
	}VTXSCREENXYZ;
	const DWORD VTXFVF_VTXSCREENXYZ = D3DFVF_XYZW | D3DFVF_TEX1;

	typedef struct tagVertexTexture
	{
		_vec3	vPos;		//��ġ
		_vec3	vNormal;	//��ֺ���(��������)
		_vec2	vTexUV;		//UV��ǥ��
	}VTXTEX;
	const _ulong VTXFVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagVertexCube
	{
		D3DXVECTOR3				vPos;				
		D3DXVECTOR3				vNormal;
		D3DXVECTOR3				vTexUV;
	}VTXCUBE;
	const DWORD VTXFVF_CUBE = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);


	typedef struct tagVertexColor
	{
		D3DXVECTOR3		vPos;
		DWORD			dwColor;
	}VTXCOLOR;
	const DWORD VTXFVF_COLOR = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef struct tagIndex16
	{
		_ushort	_1, _2, _3;	//�ε��� ��ȣ
	}INDEX16;

	typedef struct tagIndex32
	{
		_ulong	_1, _2, _3;	//�ε��� ��ȣ
	}INDEX32;

	typedef struct tagTexturePath
	{
		wstring				wstrObjKey;
		wstring				wstrStateKey;
		wstring				wstrPath;
		int					iCount;

		tagTexturePath(void)
			: wstrObjKey(L""), wstrStateKey(L""), wstrPath(L""), iCount(0)
		{}
	}IMGPATH;

	typedef struct tagUITexture
	{
		TCHAR				wstrOriginName[64];		// ���� �̹��� �̸� 
		TCHAR				wstrCuttingName[64];	// CuttingImage�� �̸� 
		_vec3				vPos;					// ��ġ
		_vec3				vSize;					// UI ������ 
		_vec2				vTexUV[4];				// UV��ǥ��

		tagUITexture(void)
			: vPos(0.f, 0.f, 0.f)
		{
			ZeroMemory(wstrOriginName, sizeof(TCHAR) * 64);
			ZeroMemory(wstrCuttingName, sizeof(TCHAR) * 64);
		}
	}UIINFO;

	typedef struct tagFontInfo
	{
		_vec2 vStartUV;
		_vec2 vLastUV;
		float fFontWidth;
		float fFontHeight;

		tagFontInfo(void)
			: vStartUV(0.f, 0.f), vLastUV(0.f, 0.f), fFontWidth(0.f), fFontHeight(0.f)
		{

		}
	}FONTINFO;

	typedef struct tagObjectInfo
	{
		TCHAR			pName[128];
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vScale;
		D3DXVECTOR3		vRotate;
		_int			iOption; // default, destroy
		_int			iPassIndex; // default, alphatest
	}OBJINFO;

	typedef struct tagAniSync
	{
		_float	fStartTime;
		_float	fEndTime;
		_float	fSpeed;
	}ANISYNC;

	typedef struct tagPointLinear
	{
		float		fTimePoint;
		_vec3		vPosPoint;
		_vec3		vAnglePoint;
		_vec3		vScalePoint;
		_vec4		vColorPoint;
	}POINTLINEAR;

	typedef struct tagEffectSaveData
	{
		WORD		wShaderIdx;
		TCHAR		pObjName[128];
		POINT		ptXY;
		float		fFrameSpeed;
		bool		bBillBoard;
		int			iListSize;
	}EFFECTDATA;
}

#endif // Engine_Struct_h__