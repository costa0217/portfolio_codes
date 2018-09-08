/*!
 * \file SelectPart.h
 * \date 2016/01/02 1:01
 *
 * \author yoon.a.y
 * Contact: yoon_ay@naver.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef SelectPart_h__
#define SelectPart_h__

#include "Defines.h"
#include "GameObject.h"

#include "DyeIcon.h"

namespace Engine
{
	class CTransform;
	class CMaterial;
	class CVIBuffer;
	class CTexture;
}

class CSelectPart	: public Engine::CGameObject
{
private:
	explicit CSelectPart(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSelectPart(void);
public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CSelectPart* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static bool			m_bNext;

private:
	void	Create_DyeIcon();
	void	Update_DyeIcon(const _float& fTimeDelta);
	void	Render_DyeIcon();

private:
	RECT	m_rcStart;
	RECT	m_rcUI[6];
	RECT	m_rcPart[3];
	_bool			m_bPlay;
	_float			m_fClick;
	_float			m_fClickOverTime;
	_float			m_fTimeDelta;
	void	Initialize_DyeIcon();
	void	Select_DyeIcon();

private:
	CDyeIcon*				m_pStartIcon;

	CDyeIcon*				m_pDyeIcon[6];
	CDyeIcon*				m_pPartIcon[3];

private:
	_matrix					m_matOrtho;
	_int					m_iSelect;
	_int					m_iSelectPart;

private:
	Engine::CTransform*		m_pTransCom;
	Engine::CMaterial*		m_pMtrlCom;
	Engine::CVIBuffer*		m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;
private:
	LPD3DXEFFECT			m_pEffect;
private:
	void Set_ContantTable(void);
public:
	virtual _ulong Release(void);
};
#endif	// SelectPart_h__
