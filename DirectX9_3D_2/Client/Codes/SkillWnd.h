#ifndef SkillWnd_h__
#define SkillWnd_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
}

class CSkillWnd : public Engine::CGameObject
{
private:
	explicit CSkillWnd(LPDIRECT3DDEVICE9 pGraphicDev, _int iIdx);
	virtual ~CSkillWnd(void);
public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);	
public:
	static CSkillWnd* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iIdx);
private:
	LPD3DXEFFECT				m_pEffect;
private:
	_matrix						m_matOrtho;
	_float						m_fX, m_fY;
	_float						m_fSizeX, m_fSizeY;

	_int						m_iIdx;

private:
	Engine::CTransform*			m_pTransCom;
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTexture*			m_pTextureCom;

private:
	void Set_ContantTable(void);
public:
	virtual _ulong Release(void);
};

#endif // SkillWnd_h__
