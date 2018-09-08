#ifndef PlayerHeartBeatUI_h__
#define PlayerHeartBeatUI_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
}

class CPlayer;
class CPlayerHeartBeatUI : public Engine::CGameObject
{
	DECLARE_SINGLETON(CPlayerHeartBeatUI)
private:
	explicit CPlayerHeartBeatUI(void);
	virtual ~CPlayerHeartBeatUI(void);

public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public://getter, setter
	void SetRender(_bool SetBool) { m_bRender = SetBool; }

private:
	LPD3DXEFFECT			m_pEffect;
	_matrix					m_matOrtho;

	_float					m_fSizeX;
	_float					m_fSizeY;
	_float					m_fX;
	_float					m_fY;

	Engine::CTransform*		m_pTransCom;
	Engine::CVIBuffer*		m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;

	_bool					m_bRender;		//UI 전체를 랜더할지 말지 결정
	CPlayer*				m_pPlayer;
	_bool					m_bAlphaCheck;
	_float					m_fAlpha;
	_float					m_fPlayerMaxHp;
	_float					m_fPlayerHp;
public:
	virtual _ulong Release(void);

};
#endif 
