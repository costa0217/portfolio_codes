#pragma once
#include "Include.h"

class CBitBmp;
class CObj
{
protected:
	static map<string, CBitBmp*>*		m_pBitMap;
	

	
	INFO			m_tInfo;
	string			m_strKey;

	
	int				m_iDrawID;
	int				m_iIndex;

	int				m_iHP;
	float			m_fAngle;
	int				m_iStageNum;

	float			m_fSpeedX;
	float			m_fSpeedY;
	float			m_fMaxSpeedX;
	float			m_fMaxSpeedY;
	FRAME			m_tFrame;

	/*총알쏠때 플레이어의 위치*/
	int				m_iTargetDir;


	//아이템
	ITEMTYPE		m_eItemType;


	int				m_iReach;

public:
	static void SetBitMap(map<string, CBitBmp*>* pBitMap);
public:
	void SetHP(int _iHP);
	void SetPos(float _fX, float _fY);
	void SetKey(string _strKey);
	void SetAngle(float _fAngle);
	void SetDrawID(int _iDrawID);
	void SetIndex(int _iIndex);
	void SetStageNum(int _iRoomNum);


	float GetMaxSpeed(){ return	m_fMaxSpeedX;}
	void SetMaxSpeed(float _fMaxSpeed){ m_fMaxSpeedX = _fMaxSpeed; m_fMaxSpeedY = _fMaxSpeed;}
	

	void SetSpeedX(float _fSpeedX){m_fSpeedX = _fSpeedX;}
	void SetSpeedY(float _fSpeedY){m_fSpeedY = _fSpeedY;}
	void SetTargetDir(int _iTargetDir) {m_iTargetDir = _iTargetDir;}

	void SetReach(int _iReach) { m_iReach = _iReach; }
	
	int  GetHP();
	int	 GetDrawID();
	virtual RECT GetRect();
	INFO GetInfo();

	ITEMTYPE	GetItemType() { return m_eItemType; }

	void SetStrKey(string _strKey);				//아이템 키값
	string GetStrKey() { return m_strKey; }

public:
	POINT Rot(float _fX, float _fY, float _fOX, float _fOY, float _fAngle); //각 회전

public:
	virtual void Initialize(void)PURE;
	virtual int Progress(void)PURE;
	virtual void Render(HDC hdc)PURE;
	virtual void Release(void)PURE;
public:
	CObj(void);
	virtual ~CObj(void);
};
