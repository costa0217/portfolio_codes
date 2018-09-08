/*!
 * \file SelectDye.h
 * \date 2016/01/01 14:57
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

#ifndef SelectDye_h__
#define SelectDye_h__

#include "Defines.h"
#include "GameObject.h"
#include "SelectCloth.h"
#include "SelectPart.h"

namespace Engine
{
	class CTransform;
	class CMaterial;
	class CVIBuffer;
	class CTexture;
}

class CSelectDye	: public Engine::CGameObject
{
	enum DYEPART{ DYEPT_HEAD, DYEPT_UPPER, DYEPT_HAND, DYEPT_LOWER, DYEPT_FOOT, DYEPT_SWORD, DYEPT_END };
	enum DYERGB{ RGB_R, RGB_G, RGB_B, RGB_END };
private:
	explicit CSelectDye(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSelectDye(void);
public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CSelectDye* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static void		Set_Part(int iPart);
	static void		Set_RGB(int iRGB);

	static int		m_iPart;
	static int		m_iRGB;

private:
	_matrix		m_matOrtho;
	_bool			m_bPlay;
	_float			m_fClick;
	_float			m_fClickOverTime;

	RECT		m_rcUI[5][8];

	_vec3		m_vColor[5][8];

	_int		m_iX;
	_int		m_iY;

	_int		m_iSelectX;
	_int		m_iSelectY;

	_vec3		m_vSelectColor[RGB_END];

	_matrix		m_matColor[DYEPT_END];

	DYERGB		m_eSelectRGB;
	DYEPART		m_eSelectPart;

private:
	void	Set_ColorValue();
	void	Set_ColorDefault();

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
#endif	// SelectDye_h__
