#pragma once


struct SkillInfo {
	XMFLOAT4	m_xmf4Color;
	XMFLOAT4X4	m_xmatWorld;
};

class CSkillObj
{
public:
	CSkillObj();
	virtual ~CSkillObj();

public:
	void	SetRenderSwitch(bool bRender) { m_bRender = bRender; }
	void	SetName(CString name);
protected:
	bool		m_bRender{ false };
	SkillInfo	m_tSkillInfo;

	CBuffer*			m_pCBuffer;
	CEffectRenderCont*	m_pEffectRenderCont;
	TCHAR				m_szName[64];		//¿Ã∏ß
public:
	virtual HRESULT Initialize() PURE;
	virtual	int		Update(float fTimeElapsed) PURE;
	virtual	void	Render() PURE;
	virtual	void	Release() PURE;
};

