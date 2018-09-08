#include "StdAfx.h"
#include "Scene.h"

CScene::CScene(void)
{
}

CScene::~CScene(void)
{
	for(map<string, CBitBmp*>::iterator iter = m_BitMap.begin();
		iter != m_BitMap.end(); ++iter)
	{
		::Safe_Delete(iter->second);
	}
	m_BitMap.clear();
}
