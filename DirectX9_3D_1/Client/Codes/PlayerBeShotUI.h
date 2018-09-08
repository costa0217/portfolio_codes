#ifndef PlayerBeShotUI_h__
#define PlayerBeShotUI_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
}

class CPlayerBeShotUI : public Engine::CGameObject
{
	DECLARE_SINGLETON(CPlayerBeShotUI)
private:
	explicit CPlayerBeShotUI(void);
	virtual ~CPlayerBeShotUI(void);

public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	void PlayerBeShotUI(void);

private:
	LPD3DXEFFECT			m_pEffect;
	_matrix					m_matOrtho;

	//피관련 텍스쳐가 총 15개이니 15개를 배열로 선언하여 한다.
	_float					m_fSizeX[15];
	_float					m_fSizeY[15];
	_float					m_fX[15];
	_float					m_fY[15];

	Engine::CTransform*		m_pTransCom;
	Engine::CVIBuffer*		m_pBufferCom[15];
	Engine::CTexture*		m_pTextureCom;
	Engine::VTXTEX*			m_pVTXInfo[15];

	//그리기 관련 선택
	_bool					m_bSelect[15];	//그릴지 안글리지
	_float					m_fAlpha[15];	//해당 텍스쳐 알파값
	
	_float					m_fMaxTime[15];
	_float					m_fTime[15];

	_float					m_fMaxSize[15];
	_float					m_fSize[15];

	_float					m_fSpeed[15];

public:
	virtual _ulong Release(void);

};
#endif 
