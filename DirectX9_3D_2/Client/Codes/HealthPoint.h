#ifndef HealthPoint_h__
#define HealthPoint_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;

	class CHPoint;
}

class CHealthPoint : public Engine::CGameObject
{
private:
	explicit CHealthPoint(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHealthPoint(void);
public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);	
public:
	static CHealthPoint* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	LPD3DXEFFECT				m_pEffect;

private:
	Engine::CHPoint*			m_pHpPtCom;

private:
	_matrix						m_matOrtho;
	_float						m_fX, m_fY;
	_float						m_fSizeX, m_fSizeY;

	_float						m_fMaxHP;
private:
	Engine::CTransform*			m_pTransCom;
	Engine::CVIBuffer*			m_pHpBufferCom;
	Engine::CTexture*			m_pTextureCom;
private:
	void Set_ContantTable(void);
public:
	virtual _ulong Release(void);
};

#endif // HealthPoint_h__
