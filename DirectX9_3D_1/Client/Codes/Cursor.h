/*!
 * \file Cursor.h
 * \date 2016/01/10 15:21
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: 모든 Scene에서 사용할 마우스커서
 *
 * \note
*/
#ifndef Cursor_h__
#define Cursor_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CMaterial;
	class CVIBuffer;
	class CTexture;
}

class CCursor : public Engine::CGameObject
{
	DECLARE_SINGLETON(CCursor)

private:
	explicit CCursor(void);
	virtual ~CCursor(void);

public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	void Set_Render(_bool bRender){ m_bRender = bRender; }

private:
	
	LPD3DXEFFECT			m_pEffect;

	_matrix					m_matOrtho;
	_float					m_fX, m_fY;
	_float					m_fSizeX, m_fSizeY;

	POINT					m_pt;

	Engine::CTransform*		m_pTransCom;
	Engine::CMaterial*		m_pMtrlCom;
	Engine::CVIBuffer*		m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;

	_bool	m_bRender; // 마우스 이미지를 그리는 변수

private:
	void Set_ContantTable(void);
		
public:
	virtual _ulong Release(void);

};

#endif // Cursor_h__