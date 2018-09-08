#ifndef BLTrail_h__
#define BLTrail_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
}

class CBLTrail : public Engine::CGameObject
{
private:
	explicit CBLTrail(LPDIRECT3DDEVICE9 pGraphicDev, _int iIndex);
	virtual ~CBLTrail(void);

public:
	void Set_BloodLord(Engine::CGameObject* pMonster){ m_pMonster = pMonster; }

public:
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CBLTrail* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iIndex);

private: //변수들
	LPD3DXEFFECT	m_pEffect;

	Engine::CGameObject*		m_pMonster;

	Engine::CTransform*			m_pTransCom;
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTexture*			m_pTextureCom;

	Engine::VTXTEX*				m_pVTXInfo;
	Engine::VTXTEX*				m_pOriVTXInfo;

	const _matrix*				m_pmatWorldMonster;

	_vec3						m_vViewPos;	//알파 정리를 하기위해 선언

	const	_vec3*				m_pvecStartPos;
	const _vec3*				m_pvecEndPos;
private:
	_vec3						m_vSwordStartPoint;
	_vec3						m_vSwordEndPoint;

	_int							m_iImageNum;
	_vec4						m_vColor;

	_int							m_iIndex;
	_float							m_fTrailLength;

	_float							m_fTimeDelta;


	const _matrix*						m_matParentBone;
private:
	virtual HRESULT Initialize(void);
	virtual HRESULT Add_Component(void);

	void Set_ContantTable(void);

	void Set_Point(_vec3 vStart, _vec3 vEnd);
	void Move(const _float& fTimeDelta);

public:
	virtual _ulong Release(void);
};
#endif // BLTrail_h__
