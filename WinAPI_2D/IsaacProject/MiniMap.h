#pragma once
#include "ui.h"

class CMiniMap :
	public CUI
{
private:
	int		m_iRoomNum;
	bool	m_bLeftMapOpen;
	bool	m_bRightMapOpen;
public:
	void SetRoom(int _iRoomNum){ m_iRoomNum = _iRoomNum; }	// 현재 있는방 넘버
	int	 GetRoom(){ return m_iRoomNum; }

	void SetLeftOpen(bool _bLeftMapOpen){ m_bLeftMapOpen = _bLeftMapOpen; }
	void SetRitghtOpen(bool _bRightMapOpen){ m_bRightMapOpen = _bRightMapOpen; }
	virtual RECT GetRect();
public:
	virtual void Initialize(void);
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CMiniMap(void);
	~CMiniMap(void);
};
