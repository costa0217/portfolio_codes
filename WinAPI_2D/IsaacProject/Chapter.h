#pragma once
#include "scene.h"

class CObj;
class CChapter :
	public CScene
{
protected:
	//list<CObj*>*		m_pObjList;
	vector<TILE*>		m_VecTile;

	CObj*				m_pPlayer;			//신매니져에서 받아옴
	CObj*				m_pUI;
	vector<CObj*>*		m_pVacHart;


	string				m_strArrow;
	string				m_strActive;

	int					m_iRoomNum;

	bool				bTileOn;

	bool				bOpenDoorSND;
public:
	/*void LoadPlayerData();
	void SavePlayerData();*/
	void LoadData(int _iRoomNum);
	void SetPlayer();
	void SetUI();
	void SetVacHart();
public:
	virtual void Initialize(void);
	virtual void Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void)PURE;
public:
	CChapter(void);
	virtual ~CChapter(void);
};
