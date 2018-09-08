#include "StdAfx.h"
#include "Stage2.h"

#include "Player.h"
#include "Device.h"

#include "Obj.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"

#include "KeyMgr.h"
#include "TimeMgr.h"

#include "Back.h"
#include "Snow.h"

#include "ObjFactory.h"
#include "MonFactory.h"

#include "EmptyTower.h"

#include "UI.h"
#include "SkillButt.h"
#include "StartButt.h"

#include "Nunu.h"
#include "Pang.h"
#include "Jetman.h"
#include "Spider.h"
#include "Snowman.h"
#include "Parker.h"

#include "TimeMgr.h"
#include "SDevice.h"


#define	WAVE_TIME  50.f

CStage2::CStage2(void)
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

CStage2::~CStage2(void)
{
	Release();
}
HRESULT CStage2::Initialize(void)
{
	LoadRoute(L"../data/Stage2Route.dat");
	LoadTile(L"../data/Stage2Tile.dat");
	CSDevice::GetInstance()->SoundPlay(SD_STTWAIT, 1);		// 스테이지2비지엠

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
			((CPlayer*)pPlayer)->SetMaxWave(5);
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
	pBack->SetObjKey(L"Stage2");
	CObjMgr::GetInstance()->AddObject(OBJ_BACK, pBack);

	pBack = CObjFactory<CSnow>::CreateObj();
	CObjMgr::GetInstance()->AddObject(OBJ_UI, pBack);

	unsigned int iVecSize = m_VecTile.size();
	for(unsigned int i = 0; i < iVecSize; ++i)
	{
		if(m_VecTile[i]->byDrawID == 2)
		{
			CObj* pTower = CObjFactory<CEmptyTower>::CreateObj(D3DXVECTOR3(m_VecTile[i]->vPos.x, m_VecTile[i]->vPos.y, 0.f));
			((CEmptyTower*)pTower)->SetStageNum(2);
			CObjMgr::GetInstance()->AddObject(OBJ_TOWER, pTower);
		}
	}
	
	m_pPlayerList	= CObjMgr::GetInstance()->GetObjList(OBJ_PLAYER);
	m_bCross		= false;

	m_bBossSummon	= false;

	//사운드
	m_bSoundPlay		= false;
	m_bWaveIncomming	= false;

	return S_OK;
}

int CStage2::Progress(void)
{

	if(!m_bWaveEnd && m_bWave[((CPlayer*)m_pPlayerList->front())->GetWave()])
	{
		if(!m_bSoundPlay)
		{
			CSDevice::GetInstance()->SoundStop(SD_STTWAIT);
			CSDevice::GetInstance()->SoundPlay(SD_STTBATTLE, 1);		// 스테이지2비지엠
			m_bSoundPlay = true;
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
			if( /*!m_bButtDown && */((CPlayer*)m_pPlayerList->front())->GetWave() < 5)
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
	CCollisionMgr::GetInstance()->BossTowerFreezing();

	int iResult = 0;
	iResult = CObjMgr::GetInstance()->Progress();


	return 0;
}
void CStage2::Render(void)
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
void CStage2::Release(void)
{
	CSDevice::GetInstance()->SoundStop(SD_STTWAIT);
	CSDevice::GetInstance()->SoundStop(SD_STTBATTLE);
	CSDevice::GetInstance()->SoundStop(SD_BOSSBATTLE);
	CSDevice::GetInstance()->SoundStop(SD_WAVEINCOMING);
}
void CStage2::LoadRoute(const wstring& wstrPath)
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

void CStage2::LoadTile(const wstring& wstrPath)
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
void	CStage2::KetInput()
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
void	CStage2::MonsterSummon(int _iWaveNum)
{
	switch(_iWaveNum)
	{
	case 0:
		m_fWaveTime	= WAVE_TIME;
		break;
	case 1: //1Wave
		m_fMonsterTime	+= GET_TIME;
	
		if(m_fWaveTime > 13.1f)
		{
			m_fMonsterTime = 0.f;
			break;
		}
		else if(m_fWaveTime > 9.0f)
		{
			if(m_fMonsterTime > 1.0f)
			{
				CObj*	pMonster = CMonFactory<CPang>::CreateMonster(&m_VecTile, &m_mapRoute[3]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
				pMonster = CMonFactory<CPang>::CreateMonster(&m_VecTile, &m_mapRoute[4]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
				pMonster = CMonFactory<CPang>::CreateMonster(&m_VecTile, &m_mapRoute[1]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
				pMonster = CMonFactory<CPang>::CreateMonster(&m_VecTile, &m_mapRoute[2]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);

				m_fMonsterTime = 0.f;
			}
		}
		else if(m_fWaveTime > 3.6f)
		{
			if(m_fMonsterTime > 1.0f)
			{
				m_fMonsterTime = 0.f;
			}
		}
		else if(m_fWaveTime > 0.1f)
		{
			if(m_fMonsterTime > 1.7f)
			{
				CObj*	pMonster = CMonFactory<CNunu>::CreateMonster(&m_VecTile, &m_mapRoute[1]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);

				pMonster = CMonFactory<CNunu>::CreateMonster(&m_VecTile, &m_mapRoute[2]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);

				pMonster = CMonFactory<CNunu>::CreateMonster(&m_VecTile, &m_mapRoute[3]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);

				pMonster = CMonFactory<CNunu>::CreateMonster(&m_VecTile, &m_mapRoute[4]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
				m_fMonsterTime = 0.f;
			}
			if(!m_bWaveIncomming)
			{
				CSDevice::GetInstance()->SoundPlay(SD_WAVEINCOMING, 0);
				m_bWaveIncomming = true;
			}
		}

		break;
	case 2: //2Wave
		m_fMonsterTime	+= GET_TIME;
	
		if(m_fWaveTime > 15.3f)
		{
			m_fMonsterTime = 0.f;
			break;
		}
		else if(m_fWaveTime > 14.1f)
		{
			if(m_fMonsterTime > 1.0f)
			{
				CObj*	pMonster = CMonFactory<CNunu>::CreateMonster(&m_VecTile, &m_mapRoute[2]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);

				pMonster = CMonFactory<CNunu>::CreateMonster(&m_VecTile, &m_mapRoute[3]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);

				m_fMonsterTime = 0.f;
			}
		}
		else if(m_fWaveTime > 10.0f)
		{
			if(m_fMonsterTime > 1.0f)
			{
				CObj*	pMonster = CMonFactory<CPang>::CreateMonster(&m_VecTile, &m_mapRoute[1]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
				pMonster = CMonFactory<CPang>::CreateMonster(&m_VecTile, &m_mapRoute[4]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
				

				m_fMonsterTime = 0.f;
			}
		}
		else if(m_fWaveTime > 6.2f)
		{
			if(m_fMonsterTime > 1.0f)
			{
				m_fMonsterTime = 0.f;
			}
		}
		else if(m_fWaveTime > 0.1f)
		{
			if(m_fMonsterTime > 2.0f)
			{
				CObj*	pMonster = CMonFactory<CJetman>::CreateMonster(&m_VecTile, &m_mapRoute[1]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);

				pMonster = CMonFactory<CJetman>::CreateMonster(&m_VecTile, &m_mapRoute[2]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);

				pMonster = CMonFactory<CJetman>::CreateMonster(&m_VecTile, &m_mapRoute[3]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);

				pMonster = CMonFactory<CJetman>::CreateMonster(&m_VecTile, &m_mapRoute[4]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
				m_fMonsterTime = 0.f;
			}
			if(m_bWaveIncomming)
			{
				CSDevice::GetInstance()->SoundPlay(SD_WAVEINCOMING, 0);
				m_bWaveIncomming = false;
			}
		}
		break;
	case 3:	//3Wave
		m_fMonsterTime	+= GET_TIME;
	
		if(m_fWaveTime > 18.1f)
		{
			m_fMonsterTime = 0.f;
			break;
		}
		else if(m_fWaveTime > 8.0f)
		{
			if(m_fMonsterTime > 2.0f)
			{
				m_bCross ^= true;

				if(m_bCross)
				{
					CObj*	pMonster = CMonFactory<CParker>::CreateMonster(&m_VecTile, &m_mapRoute[1]);
					CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
				}
				else
				{
					CObj*	pMonster = CMonFactory<CParker>::CreateMonster(&m_VecTile, &m_mapRoute[2]);
					CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
				}
				m_fMonsterTime = 0.f;
			}
		}		
		else if(m_fWaveTime > 2.2f)
		{
			if(m_fMonsterTime > 1.0f)
			{
				m_fMonsterTime = 0.f;
			}
		}
		else if(m_fWaveTime > 0.1f)
		{
			if(m_fMonsterTime > 2.0f)
			{
				CObj*	pMonster = CMonFactory<CSpider>::CreateMonster(&m_VecTile, &m_mapRoute[3]);
				CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
				m_fMonsterTime = 0.f;
			}

			if(!m_bWaveIncomming)
			{
				CSDevice::GetInstance()->SoundPlay(SD_WAVEINCOMING, 0);
				m_bWaveIncomming = true;
			}
		}
		break;

	case 4:	// 4Wave-------------------------------------
		m_fMonsterTime	+= GET_TIME;
	
		if(m_fWaveTime > 16.1f)
		{
			m_fMonsterTime = 0.f;
			break;
		}
		else if(m_fWaveTime > 10.0f)
		{
			if(m_fMonsterTime > 1.0f)
			{
				m_bCross ^= true;

				if(m_bCross)
				{
					CObj*	pMonster = CMonFactory<CParker>::CreateMonster(&m_VecTile, &m_mapRoute[3]);
					CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
					pMonster = CMonFactory<CNunu>::CreateMonster(&m_VecTile, &m_mapRoute[1]);
					CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
				}
				else
				{
					CObj*	pMonster = CMonFactory<CParker>::CreateMonster(&m_VecTile, &m_mapRoute[4]);
					CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
					pMonster = CMonFactory<CNunu>::CreateMonster(&m_VecTile, &m_mapRoute[2]);
					CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
				}
				m_fMonsterTime = 0.f;
			}
		}		
		else if(m_fWaveTime > 9.2f)
		{
			if(m_fMonsterTime > 1.0f)
			{
				m_fMonsterTime = 0.f;
			}
		}
		else if(m_fWaveTime > 0.1f)
		{
			if(m_fMonsterTime > 1.0f)
			{
				m_bCross ^= true;
				if(m_bCross)
				{
					CObj*	pMonster = CMonFactory<CPang>::CreateMonster(&m_VecTile, &m_mapRoute[3]);
					CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
					pMonster = CMonFactory<CPang>::CreateMonster(&m_VecTile, &m_mapRoute[4]);
					CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
				}
				else
				{
					CObj*	pMonster = CMonFactory<CPang>::CreateMonster(&m_VecTile, &m_mapRoute[1]);
					CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
					pMonster = CMonFactory<CPang>::CreateMonster(&m_VecTile, &m_mapRoute[2]);
					CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
				}
				
				m_fMonsterTime = 0.f;
			}
			if(m_bWaveIncomming)
			{
				CSDevice::GetInstance()->SoundPlay(SD_WAVEINCOMING, 0);
				m_bWaveIncomming = false;
			}
		}
		break;

	case 5:	// 5Wave-------------------------------------
		if(m_bWaveEnd)
			break;
		m_fMonsterTime	+= GET_TIME;
	
		if(m_fWaveTime > 21.2f)
		{
			m_fMonsterTime = 0.f;
			if(!m_bWaveEnd)
				m_bWaveEnd = true;
			break;
		}		
		else if(m_fWaveTime > 20.1f)
		{
			if(m_fMonsterTime > 1.0f)
			{
				if(!m_bBossSummon)
				{
					CObj*	pMonster = CMonFactory<CSnowman>::CreateMonster(&m_VecTile, &m_mapRoute[5]);
					CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
					m_bBossSummon	= true;
				}

				if(CSDevice::GetInstance()->SoundPlaying(SD_STTBATTLE))
				{
					CSDevice::GetInstance()->SoundStop(SD_STTBATTLE);
					CSDevice::GetInstance()->SoundPlay(SD_BOSSBATTLE, 1);
				}
				m_fMonsterTime = 0.f;				
			}
		}	
		else if(m_fWaveTime > 20.f)
		{
			if(m_fMonsterTime > 1.0f)
			{
				m_fMonsterTime = 0.f;				
			}
		}	
		else if(m_fWaveTime > 15.0f)
		{
			if(m_fMonsterTime > 1.0f)
			{
				m_bCross ^= true;

				if(m_bCross)
				{
					CObj*	pMonster = CMonFactory<CNunu>::CreateMonster(&m_VecTile, &m_mapRoute[3]);
					CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);

				}
				else
				{
					CObj*	pMonster = CMonFactory<CNunu>::CreateMonster(&m_VecTile, &m_mapRoute[4]);
					CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
				}
				m_fMonsterTime = 0.f;
			}
		}		
		else if(m_fWaveTime > 9.2f)
		{
			if(m_fMonsterTime > 1.0f)
			{
				m_fMonsterTime = 0.f;
			}
		}
		else if(m_fWaveTime > 0.1f)
		{
			if(m_fMonsterTime > 1.0f)
			{
				m_bCross ^= true;

				if(m_bCross)
				{
					CObj*	pMonster = CMonFactory<CPang>::CreateMonster(&m_VecTile, &m_mapRoute[3]);
					CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
					pMonster = CMonFactory<CPang>::CreateMonster(&m_VecTile, &m_mapRoute[4]);
					CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
				}
				else
				{
					CObj*	pMonster = CMonFactory<CPang>::CreateMonster(&m_VecTile, &m_mapRoute[1]);
					CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
					pMonster = CMonFactory<CPang>::CreateMonster(&m_VecTile, &m_mapRoute[2]);
					CObjMgr::GetInstance()->AddObject(OBJ_MONSTER, pMonster);
				}
				m_fMonsterTime = 0.f;
			}
			if(!m_bWaveIncomming)
			{
				CSDevice::GetInstance()->SoundPlay(SD_WAVEINCOMING, 0);
				m_bWaveIncomming = true;
			}			
		}
		break;
	}
}
