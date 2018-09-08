#pragma once
#include "stage.h"

class CObj;
class CStage2 :
	public CStage
{
private:
	map<int,vector<int>>	m_mapRoute;
	vector<TILE*>			m_VecTile;

	bool					m_bButtDown;
	bool					m_bWaveStart;
	bool					m_bWaveEnd;
	bool					m_bWave[5];
	bool					m_bSummonWave[5];
	list<CObj*>*			m_pPlayerList;
	float					m_fWaveTime;
	float					m_fNextTime;
	float					m_fMonsterTime;

	bool					m_bCross;
	bool					m_bBossSummon;
	//»ç¿îµå
	bool					m_bSoundPlay;
	bool					m_bWaveIncomming;

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
	CStage2(void);
	~CStage2(void);
};
