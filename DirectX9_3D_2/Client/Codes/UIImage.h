#ifndef UIImage_h__
#define UIImage_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
	class CFrame;
}

class CUIImage : public Engine::CGameObject
{
private:
	explicit CUIImage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUIImage(void);
public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);	
public:
	static CUIImage* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	LPD3DXEFFECT				m_pEffect;
private:
	_matrix						m_matOrtho;
	_float						m_fX, m_fY;
	_float						m_fSizeX, m_fSizeY;

	_vec4						m_vColor;

private:
	Engine::CTransform*			m_pTransCom;
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTexture*			m_pTextureCom;
	Engine::CFrame*				m_pFrame;
private:
	void Set_ContantTable(void);
public:
	virtual _ulong Release(void);
};


#endif // UIImage_h__
