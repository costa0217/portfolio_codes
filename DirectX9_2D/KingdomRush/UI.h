#pragma once
#include "obj.h"

class CUI :
	public CObj
{
protected:
	UITYPE			m_eUIType;		// 유아이 ID
	SHEETSIZE		m_tShtSize;		// 싱글텍스처 시트 사이즈
	SHEETFRM		m_tShtFrame;
	bool			m_bClick;
	bool			m_bChoice;

	int				m_iMouseState;

public:
	void	SetMouseState(int _iMouseState){ m_iMouseState = _iMouseState; }
public:
	void	SetClick(bool	_bClick){ m_bClick = _bClick; }
	bool	GetClick(){ return m_bClick; }

	void	SetChoice(bool	_bChoice){ m_bChoice = _bChoice; }
	bool	GetChoice(){ return m_bChoice; }
public:
	UITYPE	GetUIType() { return m_eUIType; }
public:
	virtual HRESULT Initialize(void)PURE;
	virtual int Progress(void)		PURE;
	virtual void Render(void)		PURE;
	virtual void Release(void)		PURE;
public:
	CUI(void);
	virtual ~CUI(void);
};
