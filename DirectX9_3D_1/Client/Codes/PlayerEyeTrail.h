#ifndef PlayerEyeTrail_h__
#define PlayerEyeTrail_h__


#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
}

class	CPlayer;
class CPlayerEyeTrail : public Engine::CGameObject
{
private:
	explicit CPlayerEyeTrail(LPDIRECT3DDEVICE9 pGraphicDev, _int iIndex);
	virtual ~CPlayerEyeTrail(void);

public:
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CPlayerEyeTrail* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iIndex);

private: //변수들
	LPD3DXEFFECT	m_pEffect;

	Engine::CTransform*			m_pTransCom;
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTexture*			m_pTextureCom;

	CPlayer*							m_pPlayer;					

	Engine::VTXTEX*				m_pVTXInfo;
	Engine::VTXTEX*				m_pOriVTXInfo;

	_matrix*						m_pmatWorldMonster;

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
	_float							m_fAccTime;


	const _matrix*				m_matParentBone;
private:
	virtual HRESULT Initialize(void);
	virtual HRESULT Add_Component(void);

	void Set_ContantTable(void);

	void Set_Point(_vec3 vStart, _vec3 vEnd);
	void Move(const _float& fTimeDelta);

public:
	virtual _ulong Release(void);
};

#endif // PlayerEyeTrail_h__
