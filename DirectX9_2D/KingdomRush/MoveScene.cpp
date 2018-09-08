#include "StdAfx.h"
#include "MoveScene.h"

#include "Device.h"
#include "Back.h"
#include "ObjFactory.h"
#include "Obj.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "TextureMgr.h"
#include "SceneMgr.h"

#include "KeyMgr.h"
#include "TimeMgr.h"
#include "Mouse.h"

#include "SDevice.h"

CMoveScene::CMoveScene(void)
{
}

CMoveScene::~CMoveScene(void)
{
}
HRESULT CMoveScene::Initialize(void)
{
	return S_OK;
}
int CMoveScene::Progress(void)		
{
	return 0;
}
void CMoveScene::Render(void)		
{
}
void CMoveScene::Release(void)		
{
}
