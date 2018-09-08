/*!
 * \file StaticMesh.h
 * \date 2015/11/07 0:03
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: CMesh를 상속받고 애니메이션이 없는 CStaticMesh class.
 *
 * \note
*/

#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Mesh.h"

BEGIN(Engine)

class ENGINE_DLL CStaticMesh
	: public CMesh
{
private:
	explicit CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStaticMesh(void);

public:
	virtual CResources* Clone_Resource(void);
	virtual HRESULT	Load_MeshFromFile(const TCHAR* pPath, const TCHAR* pFileName);
	virtual void Render_MeshForSDK(const D3DXMATRIX* pWorldMatrix);
	virtual void Render_MeshForShader(LPD3DXEFFECT pEffect, _bool bColliderDraw = false, _matrix* matColor = NULL);
	virtual void Render_MeshForShaderVelocity(LPD3DXEFFECT pEffect, _bool bColliderDraw = false);
	void Render_CollisionBox(_bool bDraw, _bool bSolid = false);

public://getter setter
	LPD3DXMESH	GetMesh(void) { return m_pMesh; }
	LPD3DXMESH& GetRefereMesh(void) { return m_pMesh; }

public:
	static CStaticMesh* Create(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pPath, const TCHAR* pFileName);

private:
	LPDIRECT3DTEXTURE9*		m_ppTexture;
	LPDIRECT3DTEXTURE9*		m_ppNormalTexture;
	D3DMATERIAL9*			m_pMtrl;

private:
	LPD3DXMESH				m_pMesh;
	LPD3DXBUFFER			m_pAdjancey;	//인접 폴리곤에 대한 정보이다.
	LPD3DXBUFFER			m_pSubset;		//D3DXMATERLAL의 구조체와 관련이 있어보임
	_ulong					m_dwSubsetCnt;

public:
	virtual _ulong Release(void);

};

END

#endif // StaticMesh_h__