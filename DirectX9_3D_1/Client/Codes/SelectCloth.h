/*!
 * \file SelectCloth.h
 * \date 2016/01/01 14:53
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

#ifndef SelectCloth_h__
#define SelectCloth_h__

#include "Defines.h"
#include "GameObject.h"

#include "PlayerDynamicMesh.h"
#include "DyeIcon.h"
#include "DyeIconColor.h"
#include "Sword.h"

namespace Engine
{
	class CPlayerDynamicMesh;
	class CTransform;
	class CMaterial;
	class CVIBuffer;
	class CTexture;
}

class CSelectCloth	: public Engine::CGameObject
{
	enum CLOTH{C_HEAD, C_UPPER, C_HAND, C_LOWER, C_FOOT, C_SWORD, C_END};
private:
	explicit CSelectCloth(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSelectCloth(void);
public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

	_int	Get_SelectPart() { return m_iSelectPart; }
	void	Set_IconColor();

public:
	static CSelectCloth* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	CDyeIconColor*		m_pEqIcon[3][6];
	CDyeIcon*			m_pPoseIcon[3];
	CSword*				m_pSwordMesh[3];

	void			Create_Icon();
	void			Update_Icon(const _float& fTimeDelta);
	void			Render_Icon();
	void			Set_SwordInfo();

private:
	_matrix			m_matOrtho;
	_bool			m_bPlay;
	_float			m_fClick;
	_float			m_fClickOverTime;

	RECT			m_rcUI[C_END][3];
	RECT			m_rcPoseUI[3];


	_float m_fSizeX;
	_float m_fSizeY;

	_int	m_iSelectPart;

	_int	m_iIndex;
	_int	m_iSword;
	_int	m_iHair;
	_int	m_iUpper;
	_int	m_iLower;
	_int	m_iHand;
	_int	m_iFoot;

private:
	Engine::CPlayerDynamicMesh* m_pHairMesh[3];
	Engine::CPlayerDynamicMesh* m_pUpperMesh[3];
	Engine::CPlayerDynamicMesh* m_pLowerMesh[3];
	Engine::CPlayerDynamicMesh* m_pHandMesh[3];
	Engine::CPlayerDynamicMesh* m_pFootMesh[3];

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
#endif	// SelectCloth_h__
