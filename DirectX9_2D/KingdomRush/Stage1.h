#pragma once
#include "stage.h"

class CObj;
class CStage1 :
	public CStage
{
private:
	map<int,vector<int>>	m_mapRoute;
	vector<TILE*>			m_VecTile;

	bool					m_bButtDown;
	bool					m_bWaveStart;
	bool					m_bWaveEnd;
	bool					m_bWave[3];
	bool					m_bSummonWave[3];
	list<CObj*>*			m_pPlayerList;
	float					m_fWaveTime;
	float					m_fNextTime;
	float					m_fMonsterTime;

	bool					m_bCross;


	//»ç¿îµå
	bool					m_bSoundBattlePlay;
	bool					m_bWaveIncoming;

	list<CObj*>*			m_pMonsterList;
	list<CObj*>*			m_pLastWndList;

	bool					m_bOne1;

	
public:
	void	MonsterSummon(int _iWaveNum);
	

private:
	void	LoadRoute(const wstring& wstrPath);
	void	LoadTile(const wstring& wstrPath);

	void	KetInput();
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CStage1(void);
	~CStage1(void);
};
