#ifndef DyeIconColor_h__
#define DyeIconColor_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CMaterial;
	class CVIBuffer;
	class CTexture;
}

class CDyeIconColor	: public Engine::CGameObject
{
private:
	explicit CDyeIconColor(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDyeIconColor(void);
public:
	virtual HRESULT	Initialize(TCHAR* pTexName, int iPosX, int iPosY, int iSizeX, int iSizeY);
	virtual HRESULT Add_Component(TCHAR* pTexName, int iPosX, int iPosY, int iSizeX, int iSizeY);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CDyeIconColor* Create(LPDIRECT3DDEVICE9 pGraphicDev, TCHAR* pTexName, int iPosX, int iPosY
		, int iSizeX, int iSizeY);

	//static void		Set_TextureName(TCHAR* pTexName);
	static TCHAR*	m_pTexName;
	static int		m_iPosX;
	static int		m_iPosY;

public:
	void	Set_Select(_int iSelect) { m_iSelect = iSelect; }
	void	Set_Color(_matrix matColor) { memcpy( m_matColor,  matColor, sizeof(_matrix)); }

private:
	_matrix	m_matOrtho;
	_matrix	m_matColor;				
	//_bool	m_bSelect;
	_int	m_iSelect;

	int		m_iX;
	int		m_iY;
	int		m_iSizeX;
	int		m_iSizeY;


private:
	Engine::CTransform*			m_pTransCom;
	Engine::CMaterial*			m_pMtrlCom;
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTexture*			m_pTextureCom;
private:
	LPD3DXEFFECT				m_pEffect;
private:
	void Set_ContantTable(void);
public:
	virtual _ulong Release(void);
};
#endif	// DyeIconColor_h__
