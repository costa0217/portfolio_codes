#ifndef TitleName_h__
#define TitleName_h__


#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CMaterial;
	class CVIBuffer;
	class CTexture;

	class CFrame;
}

class CTitleName	: public Engine::CGameObject
{
private:
	explicit CTitleName(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTitleName(void);
public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
public:
	static CTitleName* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	Engine::CTransform*			m_pTransCom;
	Engine::CMaterial*			m_pMtrlCom;
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTexture*			m_pTextureCom;

	Engine::CFrame*				m_pFrame;

private:
	LPD3DXEFFECT				m_pEffect;

private:
	_matrix						m_matOrtho;
	_float						m_fX, m_fY;
	_float						m_fSizeX, m_fSizeY;

	_float						m_fFrameCnt;

private:
	void Set_ContantTable(void);
public:
	virtual _ulong Release(void);


};
#endif // TitleName_h__
