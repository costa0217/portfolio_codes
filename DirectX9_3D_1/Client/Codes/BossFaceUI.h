#ifndef BossFaceUI_h__
#define BossFaceUI_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
}

class CBossFaceUI : public Engine::CGameObject
{
private:
	explicit CBossFaceUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBossFaceUI(void);

public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

	//0은 블러드 로드 1은 문차퍼와 글리터
	void SetFaceNumber(_int iNumber) { m_iFaceNumber = iNumber; }
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

	_int					m_iFaceNumber;

	_bool					m_bRender;		//UI 전체를 랜더할지 말지 결정


public:
	static CBossFaceUI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual _ulong Release(void);

};
#endif 
