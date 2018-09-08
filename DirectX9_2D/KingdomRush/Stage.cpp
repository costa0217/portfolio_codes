#include "StdAfx.h"
#include "Stage.h"

#include "ObjFactory.h"
#include "Obj.h"
#include "ObjMgr.h"
#include "Back.h"

#include "Stagebar.h"
#include "Activebar.h"
#include "StopButt.h"
#include "OptionButt.h"
#include "SkillButt.h"
#include "StartButt.h"

CStage::CStage(void)
{
}

CStage::~CStage(void)
{
}
HRESULT CStage::Initialize(void)
{
	/* 유아이창 */
	CObj*	pUI	  = CObjFactory<CStageBar>::CreateObj();
	CObjMgr::GetInstance()->AddObject(OBJ_UI, pUI);
	pUI	  = CObjFactory<CActiveBar>::CreateObj();
	CObjMgr::GetInstance()->AddObject(OBJ_UI, pUI);
	

	//메테오
	//pUI	  = CObjFactory<CSkillButt>::CreateObj(D3DXVECTOR3(73.f, 560.f, 0.f));
	CObj* pUIMeteo	= new CSkillButt;
	((CSkillButt*)pUIMeteo)->SetIndex(1);
	pUIMeteo->SetPos(D3DXVECTOR3(73.f, 560.f, 0.f));
	pUIMeteo->Initialize();
	
	CObjMgr::GetInstance()->AddObject(OBJ_UI, pUIMeteo);

	//밀리샤
	//pUI	  = CObjFactory<CSkillButt>::CreateObj(D3DXVECTOR3(138.f, 560.f, 0.f));
	
	CObj* pUIMill	= new CSkillButt;
	((CSkillButt*)pUIMill)->SetIndex(2);
	pUIMill->SetPos(D3DXVECTOR3(138.f, 560.f, 0.f));
	pUIMill->Initialize();

	CObjMgr::GetInstance()->AddObject(OBJ_UI, pUIMill);

	//스탑(스킬테크) / 옵션(나가기)
	pUI	  = CObjFactory<CStopButt>::CreateObj();
	((CStopButt*)pUI)->SetMeteoUI(pUIMeteo);	// 메테오창
	((CStopButt*)pUI)->SetMillUI(pUIMill);		// 밀리샤창
	CObjMgr::GetInstance()->AddObject(OBJ_UI, pUI);

	pUI	  = CObjFactory<COptionButt>::CreateObj();
	CObjMgr::GetInstance()->AddObject(OBJ_UI, pUI);


	pUI	  = CObjFactory<CStartButt>::CreateObj();
	CObjMgr::GetInstance()->AddObject(OBJ_UI, pUI);
	
	return S_OK;
}