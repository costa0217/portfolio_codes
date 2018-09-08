#include "StdAfx.h"
#include "SceneMgr.h"
#include "ObjMgr.h"

#include "Title.h"
#include "Field.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"


IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr(void)
:m_pScene(NULL)
{
}


CSceneMgr::~CSceneMgr(void)
{
	Release();
}

void CSceneMgr::Initialize(SCENEID _eScene)
{
	if(m_pScene != NULL)
	{
		CObjMgr::GetInstance()->Release();
		::Safe_Delete(m_pScene);
		
	}

	switch(_eScene)
	{
	case SC_TITLE:
		m_pScene = new CTitle;
		break;

	case SC_FIELD:
		m_pScene = new CField;
		break;

	case SC_STAGE1:
		m_pScene = new CStage1;
		break;

	case SC_STAGE2:
		m_pScene = new CStage2;
		break;

	case SC_STAGE3:
		m_pScene = new CStage3;
		break;

	}
	if(FAILED(CObjMgr::GetInstance()->Initialize()))
	{
		ERR_MSG(L"ObjMgr Init Failed");
		return;
	}	
	if(FAILED(m_pScene->Initialize()))
	{
		ERR_MSG(L"SceneMgr Init Failed");
		return;
	}


}

void CSceneMgr::Progress(void)
{
	int iResult = 0;

	if(m_pScene)
		iResult = m_pScene->Progress();
}

void CSceneMgr::Render(void)
{
	if(m_pScene)
		m_pScene->Render();
}

void CSceneMgr::Release(void)
{
	::Safe_Delete(m_pScene);
}
