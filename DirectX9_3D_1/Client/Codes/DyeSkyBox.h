#ifndef SkyBox_h__
#define SkyBox_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CMaterial;
	class CVIBuffer;
	class CTexture;
}

class CDyeSkyBox : public Engine::CGameObject
{
private:
	explicit CDyeSkyBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDyeSkyBox(void);

public:
	virtual HRESULT Initialize(_int iIndex);
	virtual HRESULT Add_Component();
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render();

public:
	static CDyeSkyBox* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iIndex);

public:
	void Set_TexIndex(_int iIndex);

private:
	Engine::CTransform*			m_pTransCom;
	Engine::CMaterial*			m_pMtrlCom;
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTexture*			m_pTextureCom;

private:
	Engine::CTransform*			m_pPlayerTransCom;

private:
	LPD3DXEFFECT				m_pEffect;

private:
	_int						m_iTexIndex;

private:
	void Set_ContantTable(void);
public:
	virtual _ulong Release(void);
};
#endif // SkyBox_h__