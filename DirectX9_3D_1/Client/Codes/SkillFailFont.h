#ifndef SkillFailFont_h__
#define SkillFailFont_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
}

class CSkillFailFont : public Engine::CGameObject
{
	DECLARE_SINGLETON(CSkillFailFont)
private:
	explicit CSkillFailFont(void);
	virtual ~CSkillFailFont(void);

public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public://getter, setter
	void RenderEquipmentUI(int iNumber)
	{ 
		m_iTextureNumber	= iNumber; //0«Ô∏‰ 1∞©ø  2¿Â∞© 3πŸ¡ˆ 4Ω≈πﬂ
		m_bRender			= true;
		m_bAlphaCheck	= false;
		m_fAlpha		= 1.f;
	}

public:
	LPD3DXEFFECT			m_pEffect;
	_matrix					m_matOrtho;

	_float					m_fSizeX;
	_float					m_fSizeY;
	_float					m_fX;
	_float					m_fY;

	Engine::CTransform*		m_pTransCom;
	Engine::CVIBuffer*		m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;

	_bool					m_bRender;		//UI ¿¸√º∏¶ ∑£¥ı«“¡ˆ ∏ª¡ˆ ∞·¡§

	_bool					m_bAlphaCheck;
	_float					m_fAlpha;

	_int					m_iTextureNumber;

public:
	virtual _ulong Release(void);

};
#endif 
