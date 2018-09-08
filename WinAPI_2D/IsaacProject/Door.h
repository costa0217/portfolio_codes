#pragma once
#include "block.h"

class CDoor :
	public CBlock
{
private:
	INFO	m_BossDoor;


	bool	m_bKeyOpenDoor;
	bool	m_bOpenDoor;
	bool	m_bKey;
	bool	m_bKeyInsert;
public:
	void	SetKeyOpenDoor(bool _bKeyOpenDoor);
	bool	GetKeyOpenDoor(){return m_bKeyOpenDoor;}

	void	SetOpenDoor(bool _bOpenDoor);
	bool	GetOpenDoor(){return m_bOpenDoor;}

	int		GetiStageNum(){return m_iStageNum;}

	bool	GetKeyInsert() { return m_bKeyInsert; }
	void	SetKeyInsert(bool _bKeyInsert) { m_bKeyInsert = _bKeyInsert; }
public:
	virtual void Initialize(void);
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CDoor(void);
	~CDoor(void);
};
