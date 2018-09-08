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
	UNITSTATS				m_tStats;			//�̸�, ü��, �ּҰ�, �ִ��, �̵��ӵ�
	float					m_fMaxHP;

	//�������
	int						m_iGold;
	bool					m_bGoalLine;

	//�÷��̾� �ּ�
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

	float					m_fMonSize;			//�����Ҷ� ǲ�ǰ��� �Ÿ�
	float					m_fHPPostion;		//hp �� ��ġ�����ִ°�
	float					m_fMonWidth;

	//��
	float					m_fPoisonTime;
	float					m_fDePoisonTime;
	bool					m_bPoison;

	//���ʱ�
	bool					m_bIntengle;
	bool					m_bIntenImg;

	//���ο�
	bool					m_bSlow;

	//��Ʈ / ����
	vector<int>*			m_pVecRoute;		// ���� ��Ʈ
	vector<int>				m_MyVecRoute;		// ���Ͱ� �Ȱ��ִ� ��Ʈ

	bool					m_bTelpo;
	float					m_fTelpoTime;


	//jet
	bool					m_bBooster;

public:	//��� �ѱ��
	int				GetGold(){ return m_iGold; }
public: //����
	vector<int>*	GetOriginRoute(){ return m_pVecRoute; }
	vector<int>*	GetMyRoute(){ return &m_MyVecRoute; }

	void			SetMyRoute(vector<int> _MyVecRoute) { m_MyVecRoute.swap(_MyVecRoute); }

	bool			GetTelpo(){ return m_bTelpo; }
	void			SetTelpo(bool _bTelpo){ m_bTelpo = _bTelpo; }

public: //���ο�
	void	SetSpeed(float _fSpeed){ m_tStats.fSpeed = _fSpeed;}
	float	GetSpeed(){ return m_tStats.fSpeed; }

	void	SetSlow(bool _bSlow){ m_bSlow = _bSlow; }
	bool	GetSlow(){ return m_bSlow; }

public:	//���ʱ�
	void	SetIntengling(bool _bIntengle){ m_bIntengle = _bIntengle; }

public:	//��
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
