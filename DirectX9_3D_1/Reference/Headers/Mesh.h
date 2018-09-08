/*!
 * \file Mesh.h
 * \date 2015/11/06 22:48
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: CResources를 상속 받고 Dynamic, Static Mesh들의 부모 class.
 *
 * \note
*/

#ifndef Mesh_h__
#define Mesh_h__

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CMesh
	: public CResources
{
public:
	enum MESHTYPE {TYPE_STATIC, TYPE_DYNAMIC, TYPE_PLAYERDYNAMIC, YTPE_END};

protected:
	explicit CMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMesh(const CMesh& Instance);
	virtual ~CMesh(void);

public:
	void Get_MinMax(_vec3* pMin, _vec3* pMax) {
		*pMin = m_vMin; *pMax = m_vMax;}

	void Get_pMinMax(_vec3** pMin, _vec3** pMax) {
		*pMin = &m_vMin; *pMax = &m_vMax;}

public:
	virtual CResources* Clone_Resource(void) PURE;

public:
	virtual HRESULT Load_MeshFromFile(const TCHAR* pPath, const TCHAR*) PURE;
	virtual void Render_MeshForSDK(const D3DXMATRIX* pWorldMatrix) {}
protected:
	_vec3							m_vMin, m_vMax;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;

public:
	virtual _ulong Release(void);
};

END

#endif // Mesh_h__