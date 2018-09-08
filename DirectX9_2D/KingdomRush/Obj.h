#pragma once

#include "Include.h"

class CObj
{
protected:
	static			D3DXVECTOR3		m_vScroll;

	INFO			m_tInfo;
	FRAME			m_tFrame;

	float			m_fSpeed;
	float			m_fTheta;
	//float

	wstring			m_wstrObjKey;
	wstring			m_wstrStateKey;


public:
	void	SetPos(D3DXVECTOR3 _vPos){ m_tInfo.vPos = _vPos; }
	void	SetDir(D3DXVECTOR3 _vDir){ m_tInfo.vDir = _vDir; }
	INFO	GetInfo(){ return m_tInfo; }
public:
	void			SetObjKey(wstring _wstrObjKey) { m_wstrObjKey = _wstrObjKey; }
	const wstring&  GetObjKey( void );
	void			SetFrame(const wstring& wstrStateKey/*, bool& _bStart*/);
	bool			FrameMove( float _fSpeed = 1.f );
public:
	virtual HRESULT Initialize(void)	PURE;
	virtual int		Progress(void)		PURE;
	virtual void	Render(void)		PURE;
	virtual void	Release(void)		PURE;
public:
	CObj(void);
	virtual ~CObj(void);
};
