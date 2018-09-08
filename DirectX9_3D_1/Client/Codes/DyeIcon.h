/*!
 * \file DyeIcon.h
 * \date 2016/01/03 2:23
 *
 * \author yoon.a.y
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef DyeIcon_h__
#define DyeIcon_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CMaterial;
	class CVIBuffer;
	class CTexture;
}

class CDyeIcon	: public Engine::CGameObject
{
private:
	explicit CDyeIcon(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDyeIcon(void);
public:
	virtual HRESULT	Initialize(TCHAR* pTexName, int iPosX, int iPosY, int iSizeX, int iSizeY);
	virtual HRESULT Add_Component(TCHAR* pTexName, int iPosX, int iPosY, int iSizeX, int iSizeY);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CDyeIcon* Create(LPDIRECT3DDEVICE9 pGraphicDev, TCHAR* pTexName, int iPosX, int iPosY
		, int iSizeX, int iSizeY);

	//static void		Set_TextureName(TCHAR* pTexName);
	static TCHAR*	m_pTexName;
	static int		m_iPosX;
	static int		m_iPosY;

public:
	void	Set_Select(_int iSelect) { m_iSelect = iSelect; }

private:
	_matrix						m_matOrtho;
	//_bool						m_bSelect;
	_int						m_iSelect;

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
#endif	// SelectPart_h__
