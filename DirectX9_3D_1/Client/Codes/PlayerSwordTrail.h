/*!
 * \file CPlayerSwordTrail.h
 * \date 2015/12/24 14:24
 *
 * \author USER
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: 플레이어 검에서 사용할 트레일
 *
 * \note
*/

#ifndef PlayerSwordTrail_h__
#define PlayerSwordTrail_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
}

class CPlayerSwordTrail : public Engine::CGameObject
{
private:
	explicit CPlayerSwordTrail(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayerSwordTrail(void);

public:
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CPlayerSwordTrail* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private: //변수들
	LPD3DXEFFECT	m_pEffect;

	Engine::CTransform*			m_pTransCom;
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTexture*			m_pTextureCom;

	Engine::VTXTEX*				m_pVTXInfo;
	Engine::VTXTEX*				m_pOriVTXInfo;

	const _matrix*				m_pmatSword;
	_vec3						m_vViewPos;	//알파 정리를 하기위해 선언
	_bool						m_bOnce;
	_float						m_fTimeDelta;

private:
	_vec3						m_vSwordStartPoint;
	_vec3						m_vSwordEndPoint;

	_int						m_iImageNum;
	_vec4						m_vColor;

	_float						m_fSkillTime;

	_bool						m_bTrailCheck[2];
private:
	virtual HRESULT Initialize(void);
	virtual HRESULT Add_Component(void);

	void Set_ContantTable(void);

	void Set_Point(_vec3 vStart, _vec3 vEnd);
	void Move(const _float& fTimeDelta);

public:
	virtual _ulong Release(void);
	

};
#endif // PlayerSwordTrail_h__