#include "StdAfx.h"
#include "Stage1.h"
#include "Obj.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"

#include "KeyMgr.h"
#include "TimeMgr.h"

#include "Back.h"
#include "Forest1.h"
#include "Forest2.h"

#include "ObjFactory.h"
#include "MonFactory.h"
#include "Goblin.h"
#include "BigGoblin.h"
#include "TrollBuff.h"
#include "Gagoil.h"
#include "Shif.h"

#include "EmptyTower.h"

#include "UI.h"
#include "SkillButt.h"
#include "StartButt.h"

#include "Player.h"
#include "Device.h"

#include "TimeMgr.h"
#include "SDevice.h"

#include "Win.h"


#define	WAVE_TIME  50.f

CStage1::CStage1(void)
:m_fWaveTime(0.f)
,m_fNextTime(0.f)
,m_fMonsterTime(0.f)
,m_bWaveStart(false)
,m_bWaveEnd(false)
,m_bButtDown(false)
{
	ZeroMemory(m_bWave, sizeof(bool) * 3);
	ZeroMemory(m_bSummonWave, sizeof(bool) * 3);
}
CStage1::~CStage1(void)
{
	Release();
}

HRESULT CStage1::Initialize(void)
{
	LoadRoute(L"../data/Stage1Route.dat");
	LoadTile(L"../data/Stage1Tile.dat");
	CSDevice::GetInstance()->SoundPlay(SD_STOWAIT, 1);		// 스테이지1비지엠

	CStage::Initialize();

	for(list<CObj*>::iterator iter = CObjMgr::GetInstance()->GetObjList(OBJ_UI)->begin();
		iter != CObjMgr::GetInstance()->GetObjList(OBJ_UI)->end(); ++iter)
	{
		if(((CUI*)(*iter))->GetUIType() == UI_SKILLBUTT)
		{
			((CSkillButt*)(*iter))->SetMapRoute(&m_mapRoute);
			((CSkillButt*)(*iter))->SetVecTile(&m_VecTile);
		}
		else if( ((CUI*)(*iter))->GetUIType() == UI_STAGEBAR )
		{
			CObj* pPlayer = CObjFactory<CPlayer>::CreateObj(D3DXVECTOR3(WINCX/2.f, WINCY/2.f, 0.f));
			((CPlayer*)pPlayer)->SetStageBar(*iter);
			((CPlayer*)pPlayer)->SetMaxWave(3);
			CObjMgr::GetInstance()->AddObject(OBJ_PLAYER, pPlayer);
		}
		else if( ((CUI*)(*iter))->GetUIType() == UI_STARTBUTT )
		{
			((CStartButt*)(*iter))->SetWave(m_bWave);
			((CStartButt*)(*iter))->SetWaveStart(&m_bWaveStart);
			((CStartButt*)(*iter))->SetButtDown(&m_bButtDown);
			((CStartButt*)(*iter))->SetNextTime(&m_fNextTime);
		}
	}

	CCollisionMgr::GetInstance()->SetVecTile(&m_VecTile);
	
	/* 배경 */
	CObj*	pBack = CObjFactory<CBack>::CreateObj();
	((CBack*)pBack)->SetVecTile(&m_VecTile);
	pBack->SetObjKey(L"Stage1");
	CObjMgr::GetInstance()->AddObject(OBJ_BACK, pBack);

	pBack = CObjFactory<CForest2>::CreateObj();
	CObjMgr::GetInstance()->AddObject(OBJ_UI, pBack);

	pBack = CObjFactory<CForest1>::CreateObj();
	CObjMgr::GetInstance()->AddObject(OBJ_UI, pBack);

	unsigned int iVecSize = m_VecTile.size();
	for(unsigned int i = 0; i < iVecSize; ++i)
	{
		if(m_VecTile[i]->byDrawID == 2)
		{
			CObj* pTower = CObjFactory<CEmptyTower>::CreateObj(D3DXVECTOR3(m_VecTile[i]->vPos.x, m_VecTile[i]->vPos.y, 0.f));
			((CEmptyTower*)pTower)->SetStageNum(1);

			CObjMgr::GetInstance()->AddObject(OBJ_TOWER, pTower);
		}
	}

	m_pPlayerList	= CObjMgr::GetInstance()->GetObjList(OBJ_PLAYER);
	m_pMonsterList	= CObjMgr::GetInstance()->GetObjList(OBJ_MONSTER);
	m_pLastWndList	= CObjMgr::GetInstance()->GetObjList(OBJ_LASTWND);

	m_bCross		= true;

	//사운드
	m_bSoundBattlePlay	= false;
	m_bWaveIncoming		= false;
	m_bOne1				= false;


	return S_OK;
}

int CStage1::Progress(void)
{
	if(m_bWaveEnd && m_pMonsterList->empty() && !m_bOne1)
	{
		CObj* pOptWnd	 = new CWin;
		pOptWnd->Initialize();
		m_pLastWndList->push_back(pOptWnd);
		m_bOne1= true;
	}
	if(!m_bWaveEnd && m_bWave[((CPlayer*)m_pPlayerList->front())->GetWave()])
	{
		if(!m_bSoundBattlePlay)
		{
			CSDevice::GetInstance()->SoundStop(SD_STOWAIT);
			CSDevice::GetInstance()->SoundPlay(SD_STOBATTLE, 1);		// 필드비지엠
			m_bSoundBattlePlay	= true;
		}
		//int i = ((CPlayer*)m_pPlayerList->front())->GetWave();
		if(!m_bWaveStart)
		{
			m_fWaveTime = 0.f;
			m_fNextTime	= 0.f;
			m_bWaveStart = true;
		}
		//몬스터 소환 시작
		MonsterSummon(((CPlayer*)m_pPlayerList->front())->GetWave());
		m_fWaveTime += GET_TIME;
		m_fNextTime	= WAVE_TIME - m_fWaveTime;

		if(m_fNextTime <= 0.f)
		{
			if( /*!m_bButtDown && */((CPlayer*)m_pPlayerList->front())->GetWave() < 3)
			{
				((CPlayer*)m_pPlayerList->front())->SetWave(((CPlayer*)m_pPlayerList->front())->GetWave() + 1);
				m_bWave[((CPlayer*)m_pPlayerList->front())->GetWave()] = true;
			}
			m_fWaveTime		= 0.f;
			m_fNextTime		= 0.f;
			m_bWaveStart	= false;
			m_bButtDown		= false;
		}
	}
		
	KetInput();	

	int iResult = 0;
	iResult = CObjMgr::GetInstance()->Progress();

	return 0;
}
void CStage1::Render(void)
{
	CObjMgr::GetInstance()->Render();

	D3DXMATRIX  matTrans;

	if(!m_bWaveEnd)
	{
		TCHAR	m_szMouse[MIN_STR];
		D3DXMatrixTranslation(&matTrans, 565.f, 578.f, 0.f);
		wsprintf(m_szMouse, L"0: %d", (int)m_fNextTime);
		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDevice::GetInstance()->GetStageFont()->DrawTextW(CDevice::GetInstance()->GetSprite(), m_szMouse, lstrlen(m_szMouse), 
			NULL, NULL, D3DCOLOR_ARGB(255, 200, 200, 200));
	}
}
void CStage1::Release(void)
{
	//CObjMgr::GetInstance()->Release();

	CSDevice::GetInstance()->SoundStop(SD_STOWAIT);
	CSDevice::GetInstance()->SoundStop(SD_STOBATTLE);
	CSDevice::GetInstance()->SoundStop(SD_WAVEINCOMING);
}
void CStage1::LoadRoute(const wstring& wstrPath)
{
	HANDLE		hFile = CreateFile(wstrPath.c_str(), GENERIC_READ, 0, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	DWORD		dwByte;

	vector<int>		m_vecRoute;
	int				iKey;
	int				iSize;
	while(true)
	{
		int	iIndex;
		
		ReadFile(hFile, &iKey, sizeof(int), &dwByte, NULL);
		ReadFile(hFile, &iSize, sizeof(int), &dwByte, NULL);
			
		for(int i = 0; i < iSize; ++i)
		{
			ReadFile(hFile, &iIndex, sizeof(int), &dwByte, NULL);
			m_vecRoute.push_back(iIndex);
		}
		if(dwByte == 0)
		{
			break;
		}
		m_mapRoute[iKey] = m_vecRoute;
		m_vecRoute.clear();
	}
	CloseHandle(hFile);
}

void CStage1::LoadTile(const wstring& wstrPath)
{
	HANDLE		hFile = CreateFile(wstrPath.c_str(), GENERIC_READ, 0, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	DWORD		dwByte;

	while(true)
	{
		TILE*			pTile = new TILE;
		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);

		if(dwByte == 0)
		{
			::Safe_Delete(pTile);
			break;
		}
		m_VecTile.push_back(pTile);
	}
	CloseHandle(hFile);
}
void	CStage1::KetInput()
{
	if(CKeyMgr::GetInstance()->KeyPressed(KEY_1, KEYNUM_10))
	{
		CTimeMgr::GetInstance()->SetTimeControl3(1);
	}
	else if(CKeyMgr::GetInstance()->KeyPressed(KEY_2, KEYNUM_10))
	{
		CTimeMgr::GetInstance()->SetTimeControl3(5);
	}
	else if(CKeyMgr::GetInstance()->KeyPressed(KEY_3, KEYNUM_10))
	{
		CTimeMgr::GetInstance()->SetTimeControl3(3);
	}

	if(CKeyMgr::GetInstance()->KeyPressed(KEY_LBUTTON, KEYNUM_3))
	{
		//타워 UI클리어
		CCollisionMgr::GetInstance()->TowerWndClear();
		//타워 UI생성부분
		if(CCollisionMgr::GetInstance()->MouseToTowerCollision())
		{
			
		}
		CCollisionMgr::GetInstance()->MouseToUI();
		CCollisionMgr::GetInstance()->MouseToWnd();

	}
}
void	CStage1::MonsterSummon(int _iWaveNum)
{
	switch(_iWaveNum)
	{
	case 0:
		m_fWaveTime	= WAVE_TIME;
		break;
	case 1: //1Wave
		m_fMonsterTime	+= GET_TIME;
	
		if(m_fWaveTime > 16.2f)
		{
			m_fMonsterTime = 0.f;
			break;
		}
		else if(m_fWaveTime > 5.1f)
		{
			if(m_fMonsterTime > 1.5f)
			{
				m_bCross ^= true;

				if(m_bCross)
				{
					CObj* pMonster = CMonFactory<CGoblin>::CreateMonster(&m_VecTile, &m_mapRoute[3]);
					CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
				}
				else
				{
					CObj* pMonster = CMonFactory<CGoblin>::CreateMonster(&m_VecTile, &m_mapRoute[1]);
					CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
					pMonster = CMonFactory<CGoblin>::CreateMonster(&m_VecTile, &m_mapRoute[2]);
					CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
				}
				m_fMonsterTime = 0.f;
			}
		}
		else if(m_fWaveTime > 3.0f)
		{
			if(m_fMonsterTime > 1.f)
			{
				CObj* pMonster2 = CMonFactory<CBigGoblin>::CreateMonster(&m_VecTile, &m_mapRoute[1]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster2);

				pMonster2 = CMonFactory<CBigGoblin>::CreateMonster(&m_VecTile, &m_mapRoute[2]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster2);
				m_fMonsterTime = 0.f;
			}
		}
		else if(m_fWaveTime > 0.1f)
		{
			if(m_fMonsterTime > 1.2f)
			{
				CObj*	pMonster = CMonFactory<CGoblin>::CreateMonster(&m_VecTile, &m_mapRoute[1]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);

				pMonster = CMonFactory<CGoblin>::CreateMonster(&m_VecTile, &m_mapRoute[2]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
				m_fMonsterTime = 0.f;
			}
			if(!m_bWaveIncoming)
			{
				CSDevice::GetInstance()->SoundPlay(SD_WAVEINCOMING, 0);
				m_bWaveIncoming = true;
			}
		}

		break;
	case 2: //2Wave
		m_fMonsterTime	+= GET_TIME;
	
		if(m_fWaveTime > 15.2f)
		{
			m_fMonsterTime = 0.f;
			break;
		}
		else if(m_fWaveTime > 7.5f)
		{
			if(m_fMonsterTime > 1.0f)
			{
				m_bCross ^= true;

				if(m_bCross)
				{
					CObj* pMonster = CMonFactory<CShif>::CreateMonster(&m_VecTile, &m_mapRoute[1]);
					CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
					pMonster = CMonFactory<CShif>::CreateMonster(&m_VecTile, &m_mapRoute[2]);
					CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);

				}
				else
				{
					CObj* pMonster = CMonFactory<CShif>::CreateMonster(&m_VecTile, &m_mapRoute[5]);
					CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
					pMonster = CMonFactory<CShif>::CreateMonster(&m_VecTile, &m_mapRoute[4]);
					CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
				}
				m_fMonsterTime	= 0.f;
			}

		}
		else if(m_fWaveTime > 3.8f)
		{
			if(m_fMonsterTime > 1.2f)
			{
				CObj* pMonster4 = CMonFactory<CGagoil>::CreateMonster(&m_VecTile, &m_mapRoute[1]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster4);
				pMonster4 = CMonFactory<CGagoil>::CreateMonster(&m_VecTile, &m_mapRoute[2]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster4);	

				m_fMonsterTime	= 0.f;
			}
		}
		else if(m_fWaveTime > 0.1f)
		{
			if(m_fMonsterTime > 1.2f)
			{
				CObj* pMonster4 = CMonFactory<CGagoil>::CreateMonster(&m_VecTile, &m_mapRoute[4]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster4);
				pMonster4 = CMonFactory<CGagoil>::CreateMonster(&m_VecTile, &m_mapRoute[5]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster4);			

				m_fMonsterTime	= 0.f;
			}
			if(m_bWaveIncoming)
			{
				CSDevice::GetInstance()->SoundPlay(SD_WAVEINCOMING, 0);
				m_bWaveIncoming = false;
			}
		}
		else
		{
			m_fMonsterTime	= 0.f;
		}
		break;
	case 3:	//3Wave
		
		if(m_bWaveEnd)
		{
			
			break;
		}
		m_fMonsterTime	+= GET_TIME;		
	
		if(m_fWaveTime > 28.0f)
		{
			m_fMonsterTime = 0.f;
			if(!m_bWaveEnd)
				m_bWaveEnd = true;
			break;
		}
		else if(m_fWaveTime > 17.0f)
		{
			if(m_fMonsterTime > 2.0f)
			{
				CObj* pMonster2 = CMonFactory<CBigGoblin>::CreateMonster(&m_VecTile, &m_mapRoute[3]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster2);
				m_fMonsterTime = 0.f;
			}
		}
		else if(m_fWaveTime > 13.1f)
		{
			if(m_fMonsterTime > 1.4f)
			{
				m_fMonsterTime = 0.f;
			}
		}
		else if(m_fWaveTime > 8.f)
		{
			if(m_fMonsterTime > 1.4f)
			{
				m_bCross ^= true;

				if(m_bCross)
				{
					CObj*  pMonster = CMonFactory<CGoblin>::CreateMonster(&m_VecTile, &m_mapRoute[1]);
					CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);

				}
				else
				{
					CObj* pMonster = CMonFactory<CGoblin>::CreateMonster(&m_VecTile, &m_mapRoute[4]);
					CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
					
				}
				m_fMonsterTime = 0.f;
			}
		}
		else if(m_fWaveTime > 4.6f)
		{
			if(m_fMonsterTime > 0.1f)
			{
				m_fMonsterTime = 0.f;
			}
		}
		else if(m_fWaveTime > 4.5f)
		{
			if(m_fMonsterTime > 0.1f)
			{
				CObj* pMonster3 = CMonFactory<CTrollbuff>::CreateMonster(&m_VecTile, &m_mapRoute[1]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster3);
				pMonster3 = CMonFactory<CTrollbuff>::CreateMonster(&m_VecTile, &m_mapRoute[2]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster3);
				m_fMonsterTime = 0.f;
			}
		}
		else if(m_fWaveTime > 1.6f)
		{
			if(m_fMonsterTime > 0.7f)
			{
				CObj*	pMonster = CMonFactory<CGoblin>::CreateMonster(&m_VecTile, &m_mapRoute[1]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
				pMonster = CMonFactory<CGoblin>::CreateMonster(&m_VecTile, &m_mapRoute[2]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);

				m_fMonsterTime	= 0.f;
			}
		}
		else if(m_fWaveTime > 0.1f)
		{
			if(m_fMonsterTime > 0.7f)
			{
				CObj*	pMonster = CMonFactory<CGoblin>::CreateMonster(&m_VecTile, &m_mapRoute[3]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);

				m_fMonsterTime	= 0.f;
			}
			if(!m_bWaveIncoming)
			{
				CSDevice::GetInstance()->SoundPlay(SD_WAVEINCOMING, 0);
				m_bWaveIncoming = true;
			}
			
		}
		else
		{
			m_fMonsterTime	= 0.f;
		}
		break;
	}
}