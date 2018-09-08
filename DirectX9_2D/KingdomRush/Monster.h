#pragma once
#include "obj.h"

class CMonster :
	public CObj
{
protected:
	vector<int>				m_vecRoute;
	wstring					m_szStateKey;
	
	MONSTERID				m_eMonsterID;
	MONTYPE					m_eMonType;
	UNITSTATS				m_tStats;			//이름, 체력, 최소공, 최대공, 이동속도
	float					m_fMaxHP;

	//소지골드
	int						m_iGold;
	bool					m_bGoalLine;

	//플레이어 주소
	list<CObj*>*			m_pPlayerList;
	list<CObj*>*			m_pEffectList;

	vector<TILE*>*			m_pVecTile;

	CObj*					m_pSoldier;

	bool					m_bMoveUp;
	bool					m_bMoveDown;
	bool					m_bMoveLR;
	bool					m_bAttack;

	bool					m_bBattle;
	bool					m_bDie;
	bool					m_bDeadImg;
	bool					m_bDamage;

	float					m_fMonSize;			//공격할때 풋맨과의 거리
	float					m_fHPPostion;		//hp 바 위치정해주는것
	float					m_fMonWidth;

	//독
	float					m_fPoisonTime;
	float					m_fDePoisonTime;
	bool					m_bPoison;

	//인탱글
	bool					m_bIntengle;
	bool					m_bIntenImg;

	//슬로우
	bool					m_bSlow;

	//루트 / 텔포
	vector<int>*			m_pVecRoute;		// 게임 루트
	vector<int>				m_MyVecRoute;		// 몬스터가 걷고있는 루트

	bool					m_bTelpo;
	float					m_fTelpoTime;


	//jet
	bool					m_bBooster;

public:	//골드 넘기기
	int				GetGold(){ return m_iGold; }
public: //텔포
	vector<int>*	GetOriginRoute(){ return m_pVecRoute; }
	vector<int>*	GetMyRoute(){ return &m_MyVecRoute; }

	void			SetMyRoute(vector<int> _MyVecRoute) { m_MyVecRoute.swap(_MyVecRoute); }

	bool			GetTelpo(){ return m_bTelpo; }
	void			SetTelpo(bool _bTelpo){ m_bTelpo = _bTelpo; }

public: //슬로우
	void	SetSpeed(float _fSpeed){ m_tStats.fSpeed = _fSpeed;}
	float	GetSpeed(){ return m_tStats.fSpeed; }

	void	SetSlow(bool _bSlow){ m_bSlow = _bSlow; }
	bool	GetSlow(){ return m_bSlow; }

public:	//인탱글
	void	SetIntengling(bool _bIntengle){ m_bIntengle = _bIntengle; }

public:	//독
	bool	GetPoisoning(){ return m_bPoison; }
	void	SetPoisoning(bool _bPoison){ m_bPoison = _bPoison; }
public:
	void	SetBattleSoldier(CObj* _pSoldier){ m_pSoldier = _pSoldier; }
	CObj*	GetBattleSoldier(){ return m_pSoldier; }

	void	SetMonsterHP(int _iHP){ m_tStats.iHP = _iHP; }
	int		GetMonsterHP(){ return m_tStats.iHP; }

	void	SetMonsterDie(bool _bDie){ m_bDie = _bDie; }
	bool	GetMonsterDie(){ return m_bDie; }

	UNITSTATS	GetStats(){return m_tStats;}

public:
	void	SetVecTile(vector<TILE*>* _pVecTile){ m_pVecTile = _pVecTile; }
	void	SetVecRoute(vector<int>* _pVecRoute){ m_pVecRoute = _pVecRoute; }

	MONSTERID	GetMonsterID(){ return m_eMonsterID; }
	MONTYPE		GetMonType(){ return m_eMonType; }

	bool	GetBattle(){ return m_bBattle; }
	void	SetBattle(bool _bBattle){ m_bBattle = _bBattle; }
	
	float	GetSize(){ return m_fMonSize; }

	void	DirectionMake();
public:
	virtual HRESULT Initialize(void)PURE;
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		PURE;
public:
	CMonster(void);
	virtual ~CMonster(void);
};
