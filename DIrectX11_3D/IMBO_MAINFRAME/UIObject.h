#pragma once
#include "UIRenderer.h"

struct tUImatVP {
	XMFLOAT4X4	m_xmf4x4VP;
	//float		m_fAlpha;
	//float		a, b, c;

};

class CUIObject
{
public:
	CUIObject();
	virtual ~CUIObject();

public:
	virtual HRESULT Initialize() { return S_OK; };
	virtual int		Update(float fTimeElapsed) { return 0; };
	virtual void	Render() {};
	virtual void	Release() { };

public:
	XMVECTOR	GetUIPos() { return XMLoadFloat2(&m_f2XYPos); }
	XMVECTOR	GetUISize() { return XMLoadFloat2(&m_f2XYSize); }
	//void		SetImageName(TCHAR* pTexName){ memcpy(m_szTexture, pTexName, sizeof(TCHAR) * 64); }
	void	SetUIPos(XMFLOAT2 xmPos) { m_f2XYPos.x = xmPos.x; m_f2XYPos.y = xmPos.y; };
	void SetImageName(string strTexName) {
		StringToTCHAR(strTexName, m_szTexture);
		//memcpy(m_szTexture, pTexName, sizeof(TCHAR) * 64);
		m_pTexture = RESOURCEMGR->GetTexture(strTexName);
	}

	void		SetRender(bool bRender) { m_bRender = bRender; }
	bool		GetRender() { return m_bRender; }

protected:
	XMFLOAT2	m_f2XYPos{ 0.f, 0.f };
	XMFLOAT2	m_f2XYSize{ 1.f, 1.f };
	
	XMFLOAT4X4	m_f4x4View;
	XMFLOAT4X4	m_f4x4Proj;

	CMesh*					m_pMesh;
	 CBuffer*		m_pCBuffer;
	 CTexture*	m_pTexture;

	CUIRenderer*		m_pUIRenderCont{ nullptr };

	bool		m_bRender{ true };

	float		m_fRenderLayer{ 0.f };
protected:
	TCHAR		m_szTexture[64];		//텍스처 이름
	float		m_fAccTime{ 0.f };
};

