#pragma once
#include "player.h"

class CIsaac :
	public CPlayer
{
private:
	HDC			m_BlendDC;
	
	list<CObj*>*	m_pBulletlist;
	list<CObj*>*	m_pFamilyerlist;
	vector<CObj*>*	m_pVecHart;

	DWORD		m_dwKey;
	DWORD		m_dwTime;
	DWORD		m_dwState;
	int			m_iIsaacHead;
	int			m_iRoomNum;

	DIRECTION	m_eDir;

	DWORD		m_dwShot;

	bool		m_bHPUP;		//피통늘리기

	bool		m_bOnHit;		//맞았을때
	int			m_iHitTime;
	bool		m_bHPdown;		//피 한번만 깎이게
	
	int			m_iCoinNum;		//동전갯수
	int			m_iBombNum;		//폭탄갯수
	int			m_iKeyNum;		// 키 갯수

	int			m_iAttackSpeed;	// 공속

	int			m_iDamage;		//데미지

	bool		m_bFamFlyEvolve; //패밀리어_파리 생성
	bool		m_bFamBabyEvolve;//패밀리어_아기 생성

	ARROWTYPE	m_eArrowType;
	ACTIVETYPE	m_eActiveType;

	/*액티브 변수*/
	bool		m_bACT_hanging;
	bool		m_bACT_AllAttack;
	bool		m_bACT_Meteo;
	bool		m_bACT_Teleport;
	bool		m_bACT_Lemon;

private:
	void KeyInput(void);
	void Direction(void);
	void SetMotion(DWORD _dwState, int _iLast, int _iScene, DWORD _dwTime);

	float Total_AccelX(float _fSpeedX, DWORD _dwState);
	float Total_AccelY(float _fSpeedY, DWORD _dwState);

	void BoolReset();

public: /*아이템*/
	int GetCoinNum(){ return m_iCoinNum; }
	int GetBombNum(){ return m_iBombNum; }
	int GetKeyNum(){  return m_iKeyNum; }

	void SetCoinNum(int _iCoinNum) { m_iCoinNum = _iCoinNum; }
	void SetBombNum(int _iBombNum) { m_iBombNum = _iBombNum; }
	void SetKeyNum(int _iKeyNum) { m_iKeyNum = _iKeyNum; }

	//체력업
	void SetHPUp(bool _bHPUP) { m_bHPUP = _bHPUP; }

	//공속업
	int GetAttackSpeed(){  return m_iAttackSpeed; }
	void SetAttackSpeed(int _iAttackSpeed) { m_iAttackSpeed = _iAttackSpeed; }

	//사거리업
	void SetReach(int _iReach) { m_iReach = _iReach; }
	//데미지업
	void SetDamage(int _iDamage) { m_iDamage = _iDamage; }

	void SetFamFly(bool _bFamFlyEvolve) {m_bFamFlyEvolve = _bFamFlyEvolve;}
	void SetFamBaby(bool _bFamBabyEvolve) {m_bFamBabyEvolve = _bFamBabyEvolve;}

	//총알종류
	ARROWTYPE GetArrowType(){ return m_eArrowType; }
	void	SetArrowType(ARROWTYPE _eArrowType){ m_eArrowType = _eArrowType; }

	ACTIVETYPE GetActiveType(){ return m_eActiveType; }
	void	SetActiveType(ACTIVETYPE _eActiveType){ m_eActiveType = _eActiveType; }

	bool	GetHaning() { return m_bACT_hanging;}

	//치트
	void	SetAllAttack(bool _bACT_AllAttack){ m_bACT_AllAttack = _bACT_AllAttack;}
public:
	RECT GetCOlRect();

	void SetRoomNum(int _iRoomNum){ m_iRoomNum = _iRoomNum; }	// 현재 있는방 넘버
	int	 GetRoomNum(){ return m_iRoomNum; }

	DIRECTION GetIsaacDir(){ return m_eDir; }		// 플레이어의 방향


public:
	void SetOnHit(bool _bOnHit) { m_bOnHit = _bOnHit; }	// 플레이어 맞았을경우
	bool GetOnHit(){ return m_bOnHit; }

public:
	void SetBulletlist(list<CObj*>* _pBulletlist);
	void SetFamilyerlist(list<CObj*>* _pFamilyerlist);
	void SetVect(vector<CObj*>* _pVecHart);

public:
	virtual void Initialize(void);
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CIsaac(void);
	~CIsaac(void);
};
