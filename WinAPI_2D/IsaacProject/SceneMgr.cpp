#include "StdAfx.h"
#include "SceneMgr.h"
#include "ObjFactory.h"
#include "Isaac.h"
#include "MiniMap.h"
#include "Hart.h"

#include "Logo.h"
#include "MyMenu.h"

#include "Chapter.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"
#include "Stage4.h"
#include "Stage5.h"
#include "Stage6.h"
#include "Stage7.h"
#include "BossStage.h"

CSceneMgr* CSceneMgr::m_pInst = NULL;
CSceneMgr::CSceneMgr(void)
:m_pPlayer(NULL)
,m_pUI(NULL)
{
	memset(m_pScene, 0, sizeof(m_pScene));
	m_eCurrentScene = SC_Stage1;
}

CSceneMgr::~CSceneMgr(void)
{
	Release();
}

void CSceneMgr::SetScene(SCENEID _eSCENEID)
{
	m_eCurrentScene = _eSCENEID;
	/*if(m_pScene != NULL)
	::Safe_Delete(m_pScene);*/
	if( m_eCurrentScene != SC_MENU)
	{
		if(!m_pPlayer)
			m_pPlayer = CObjFactory<CIsaac>::CreateObj(WINCX/2.f, WINCY /2.f);
		if(!m_pUI)
			m_pUI = CObjFactory<CMiniMap>::CreateObj(155.f, 45.f);
		if(m_VacHart.size() == 0)
		{
			for( int i = 0; i < ((CIsaac*)m_pPlayer)->GetHP() / 2; ++i)
			{
				m_VacHart.push_back(CObjFactory<CHart>::CreateObj(590.f + (33 * i), 40.f) );
			}
		}
	}

	switch(_eSCENEID)
	{
	case SC_MENU:
		if(m_pScene[SC_MENU] == NULL)
		{
			m_pScene[SC_MENU] = new CMyMenu;
			m_pScene[SC_MENU]->Initialize();
		}
		break;

	case SC_Stage1:
		if(m_pScene[SC_Stage1] == NULL)
		{
			m_pScene[SC_Stage1] = new CStage1;
			m_pScene[SC_Stage1]->Initialize();
		}
		break;
	case SC_Stage2:
		if(m_pScene[SC_Stage2] == NULL)
		{
			m_pScene[SC_Stage2] = new CStage2;
			m_pScene[SC_Stage2]->Initialize();
		}
		break;
	case SC_Stage3:
		if(m_pScene[SC_Stage3] == NULL)
		{
			m_pScene[SC_Stage3] = new CStage3;
			m_pScene[SC_Stage3]->Initialize();
		}
		break;
	case SC_Stage4:
		if(m_pScene[SC_Stage4] == NULL)
		{
			m_pScene[SC_Stage4] = new CStage4;
			m_pScene[SC_Stage4]->Initialize();
		}
		break;
	case SC_Stage5:
		if(m_pScene[SC_Stage5] == NULL)
		{
			m_pScene[SC_Stage5] = new CStage5;
			m_pScene[SC_Stage5]->Initialize();
		}
		break;
	case SC_Stage6:
		if(m_pScene[SC_Stage6] == NULL)
		{
			m_pScene[SC_Stage6] = new CStage6;
			m_pScene[SC_Stage6]->Initialize();
		}
		break;
	case SC_Stage7:
		if(m_pScene[SC_Stage7] == NULL)
		{
			m_pScene[SC_Stage7] = new CStage7;
			m_pScene[SC_Stage7]->Initialize();
		}
		break;
	case SC_BossStage:
		if(m_pScene[SC_BossStage] == NULL)
		{
			m_pScene[SC_BossStage] = new CBossStage;
			m_pScene[SC_BossStage]->Initialize();
		}
		break;
	}
	//((CChapter*)m_pScene)->SetObjList(m_ObjList);
	//m_pScene->Initialize();

		
}

void CSceneMgr::Progress(void)
{
	if(m_pScene[m_eCurrentScene])
		m_pScene[m_eCurrentScene]->Progress();
}

void CSceneMgr::Render(HDC hdc)
{
	if(m_pScene[m_eCurrentScene])
		m_pScene[m_eCurrentScene]->Render(hdc);
}

void CSceneMgr::Release(void)
{
	//for(int i = 0; i < OBJ_END; ++i)
	//{
	//	for(list<CObj*>::iterator iter= m_ObjList[i].begin();
	//		iter != m_ObjList[i].end(); ++iter)
	//	{
	//		::Safe_Delete(*iter);
	//	}
	//	m_ObjList[i].clear();
	//}

	for( int i = 0; i < SC_END; ++i)
	{
		::Safe_Delete(m_pScene[i]);
	}
	if( m_eCurrentScene >= 0)
	{
		SAFE_DELETE(m_pPlayer);
		SAFE_DELETE(m_pUI);



		for(vector<CObj*>::iterator iter = m_VacHart.begin();
			iter != m_VacHart.end(); ++iter)
		{
			::Safe_Delete(*iter);
		}
		m_VacHart.clear();
		vector<CObj*>().swap(m_VacHart);
	}
}


