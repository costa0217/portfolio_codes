#ifndef PlayerState_h__
#define PlayerState_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
}

class CPlayer;
class CPlayerState : public Engine::CGameObject
{
private:
	explicit CPlayerState(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayerState(void);

public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

private:
	enum PLAYERSTATE { HP_Bar2, HP_Bar, HP_Back, HP_Front, SP_Bar2, SP_Bar, SP_Back, SP_Front, Stamina_Bar2, Stamina_Bar, Stamina_Back, Stamina_Front, PSUIEND};

private:
	CPlayer*				m_pPlayer;

	LPD3DXEFFECT			m_pEffect;
	_matrix					m_matOrtho;

	_float					m_fSizeX[PSUIEND];
	_float					m_fSizeY[PSUIEND];
	_float					m_fX[PSUIEND];
	_float					m_fY[PSUIEND];

	Engine::CTransform*		m_pTransCom;
	Engine::CVIBuffer*		m_pBufferCom[PSUIEND];
	Engine::CTexture*		m_pTextureCom[PSUIEND];
	Engine::VTXTEX*			m_pVTXInfo[PSUIEND];

	_float					m_fHPFront;
	_float					m_fHPBack;
	_float					m_fSPFront;
	_float					m_fSPBack;
	_float					m_fStaminaFront;
	_float					m_fStaminaBack;

public:
	static CPlayerState* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual	_ulong		Release(void);
};

#endif // PlayerState_h__

