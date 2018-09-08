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
	/* ������â */
	CObj*	pUI	  = CObjFactory<CStageBar>::CreateObj();
	CObjMgr::GetInstance()->AddObject(OBJ_UI, pUI);
	pUI	  = CObjFactory<CActiveBar>::CreateObj();
	CObjMgr::GetInstance()->AddObject(OBJ_UI, pUI);
	

	//���׿�
	//pUI	  = CObjFactory<CSkillButt>::CreateObj(D3DXVECTOR3(73.f, 560.f, 0.f));
	CObj* pUIMeteo	= new CSkillButt;
	((CSkillButt*)pUIMeteo)->SetIndex(1);
	pUIMeteo->SetPos(D3DXVECTOR3(73.f, 560.f, 0.f));
	pUIMeteo->Initialize();
	
	CObjMgr::GetInstance()->AddObject(OBJ_UI, pUIMeteo);

	//�и���
	//pUI	  = CObjFactory<CSkillButt>::CreateObj(D3DXVECTOR3(138.f, 560.f, 0.f));
	
	CObj* pUIMill	= new CSkillButt;
	((CSkillButt*)pUIMill)->SetIndex(2);
	pUIMill->SetPos(D3DXVECTOR3(138.f, 560.f, 0.f));
	pUIMill->Initialize();

	CObjMgr::GetInstance()->AddObject(OBJ_UI, pUIMill);

	//��ž(��ų��ũ) / �ɼ�(������)
	pUI	  = CObjFactory<CStopButt>::CreateObj();
	((CStopButt*)pUI)->SetMeteoUI(pUIMeteo);	// ���׿�â
	((CStopButt*)pUI)->SetMillUI(pUIMill);		// �и���â
	CObjMgr::GetInstance()->AddObject(OBJ_UI, pUI);

	pUI	  = CObjFactory<COptionButt>::CreateObj();
	CObjMgr::GetInstance()->AddObject(OBJ_UI, pUI);


	pUI	  = CObjFactory<CStartButt>::CreateObj();
	CObjMgr::GetInstance()->AddObject(OBJ_UI, pUI);
	
	return S_OK;
}