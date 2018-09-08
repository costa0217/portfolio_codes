#include "stdafx.h"
#include "EffectMgr.h"
#include "Engine_Defines.h"
#include "Export_Function.h"
#include "MyEffect.h"

#include "Layer.h"

IMPLEMENT_SINGLETON(CEffectMgr)

CEffectMgr::CEffectMgr(void)
:m_pGraphicDev(NULL)
,m_pLayer(NULL)
{

}

CEffectMgr::~CEffectMgr(void)
{

}
void CEffectMgr::Initialize(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CLayer* pLayer)
{
	m_pGraphicDev	= pGraphicDev;
	m_pLayer			= pLayer;
}
void CEffectMgr::Play_Effect(TCHAR* pName, _vec3 vPos, _bool bReStart, _bool bHDR, _vec3 vSize, _vec3 vAngle, _float fRevolAngle, _bool bRefeat, _float fRefeatTime, _vec3* pvecParent /*= NULL*/, _bool bNoTesting, _float fColorDown, _float fStartTime, _bool bDistort)	// 이펙트 생성-> 플레이
{

	list<Engine::CGameObject*>* pEffectList = m_pLayer->GetpObjectList(pName);
	if(NULL != pEffectList)	// 이펙트 리스트가 있다.
	{
		vector<Engine::CGameObject*>* pVecEffect = Find_EffectList(pName);		// 원본을 가져온다.
		size_t iVecEffectSize = pVecEffect->size();

		list<Engine::CGameObject*>::iterator iter		= pEffectList->begin();
		list<Engine::CGameObject*>::iterator iter_end = pEffectList->end();

		_bool	bEnAbleEffect = true;

		while(iter != iter_end)
		{
			if(true == ((CMyEffect*)(*iter))->m_bReStart) break;		// 계속 반복되는 이펙트는 걸름

			if(true == ((CMyEffect*)(*iter))->m_bAbleEffect)			// 1이펙트가 돌고있다면
			{																			// 다음 이펙트 칸으로
				for(size_t i = 0; i < iVecEffectSize; ++i)
					++iter;
			}
			else																		// 쉬고있는 이펙트
			{
				for(size_t i = 0; i < iVecEffectSize; ++i)
				{
					((CMyEffect*)(*iter))->Setting_Init(vPos, vSize, vAngle, fColorDown, fRevolAngle, bRefeat, fRefeatTime, pvecParent);	//초기화 & 시작
					++iter;
				}
				bEnAbleEffect = false;
				break;
			}
		}
		if(true == bEnAbleEffect)		// 쉬고있는 이펙트가 없다면 생성
			Create_Effect(pName, vPos, bReStart, vSize, vAngle, fRevolAngle, bRefeat, fRefeatTime, pvecParent, bHDR, bNoTesting, fColorDown, fStartTime, bDistort);
	}
	else	// 리스트자체가 없으면 이펙트리스트와 이펙트 생성
	{
		Create_Effect(pName, vPos, bReStart, vSize, vAngle, fRevolAngle, bRefeat, fRefeatTime, pvecParent, bHDR, bNoTesting, fColorDown, fStartTime, bDistort);
	}	
}

void CEffectMgr::Stop_Effect(TCHAR* pName)
{
	list<Engine::CGameObject*>* pEffectList = m_pLayer->GetpObjectList(pName);
	if(NULL != pEffectList)	// 이펙트 리스트가 있다.
	{
		list<Engine::CGameObject*>::iterator iter		= pEffectList->begin();
		list<Engine::CGameObject*>::iterator iter_end = pEffectList->end();
		for( ; iter != iter_end; ++iter)
		{
			((CMyEffect*)(*iter))->m_bStop = true;
		}
	}
	else
	{

	}
}

void CEffectMgr::Move_Effect(TCHAR* pName, _vec3 vOriPos, _vec3 vMovePos)
{
	vector<Engine::CGameObject*>* pVecEffect = Find_EffectList(pName);		// 원본을 가져온다.
	if(pVecEffect == NULL) return;
	int iCnt = 0;
	size_t iVecSize = pVecEffect->size();

	list<Engine::CGameObject*>* pEffectList = m_pLayer->GetpObjectList(pName);
	if(NULL != pEffectList)	// 이펙트 리스트가 있다.
	{
		list<Engine::CGameObject*>::iterator iter			= pEffectList->begin();
		list<Engine::CGameObject*>::iterator iter_end	= pEffectList->end();

		for( ; iter != iter_end; ++iter)
		{
			((CMyEffect*)(*iter))->Set_Pos(vOriPos, vMovePos);
		}
	}
	else
	{

	}
}
_ulong CEffectMgr::Release(void)
{
	boost::unordered_map<const TCHAR*,vector<Engine::CGameObject*>>::iterator iter = m_mapEffect.begin();
	boost::unordered_map<const TCHAR*,vector<Engine::CGameObject*>>::iterator iter_end = m_mapEffect.end();

	for( ; iter != iter_end; ++iter)
	{
		for_each(iter->second.begin(), iter->second.end(), Engine::CRelease_Single());
	}
	m_mapEffect.clear();
	delete this;
	return 0;
}

void CEffectMgr::Load_EffectData(TCHAR* pName, TCHAR* pKey)
{
	//TCHAR	pKey[32] = L"";
	//Compute_Key_InStr(pKey, pName);

	HANDLE		hFile = CreateFile(pName, GENERIC_READ, 0, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD		dwByte;

	int iEffectCnt = 0;
	ReadFile(hFile, &iEffectCnt, sizeof(int), &dwByte, NULL);

	vector<Engine::CGameObject*>	VecNewObject;

	for(int i = 0; i < iEffectCnt; ++i)
	{
		Engine::EFFECTDATA  EffectData;
		ZeroMemory(&EffectData, sizeof(Engine::EFFECTDATA));

		ReadFile(hFile, &EffectData, sizeof(Engine::EFFECTDATA), &dwByte, NULL);

		Engine::CGameObject*	pEffect = CMyEffect::Create(Engine::Get_GraphicDev(), EffectData.wShaderIdx, EffectData.pObjName, EffectData.ptXY, EffectData.fFrameSpeed);
		((CMyEffect*)pEffect)->m_bBillBoard = EffectData.bBillBoard;

		int iMiriTexNum = 0;
		ReadFile(hFile, &iMiriTexNum, sizeof(int), &dwByte, NULL);
		for(int j = 0; j < EffectData.iListSize; ++j)
		{
			Engine::POINTLINEAR PtLinearData;
			ZeroMemory(&PtLinearData, sizeof(Engine::POINTLINEAR));
			ReadFile(hFile, &PtLinearData, sizeof(Engine::POINTLINEAR), &dwByte, NULL);

			Engine::POINTLINEAR* pData = new Engine::POINTLINEAR;

			pData->fTimePoint	= PtLinearData.fTimePoint/* * 10.f*/;
			pData->vPosPoint	= PtLinearData.vPosPoint/* * 3.f*/;
			pData->vAnglePoint	= PtLinearData.vAnglePoint;
			pData->vScalePoint	= PtLinearData.vScalePoint * 4.f;
			pData->vColorPoint	= PtLinearData.vColorPoint;

			((CMyEffect*)pEffect)->m_VecLinear.push_back(pData);
		}
		VecNewObject.push_back(pEffect);
	}	
	m_mapEffect.insert(boost::unordered_map<const TCHAR*, vector<Engine::CGameObject*>>::value_type(pKey, VecNewObject));
	CloseHandle(hFile);
}

void CEffectMgr::Compute_Key_InStr(TCHAR* pOut, TCHAR* pName)
{
	// 경로에서 맵에 넣을 키값 구하는 함수
	TCHAR	pFullPath[128] = L"";
	TCHAR	pKeyName[128] = L"";
	TCHAR	Dot[2] = L".";
	TCHAR	Slash[2] = L"/";

	lstrcpy(pFullPath, pName);
	int iStrSize = lstrlen(pFullPath);

	int iDotPoint	= 0;
	int iSlashPoint	= 0;

	for(int i = iStrSize; i > 0; --i)
	{
		if(0 == iDotPoint)
		{
			if(Dot[0] == pFullPath[i])		iDotPoint = i;
		}
		if(0 == iSlashPoint)
		{
			if(Slash[0] == pFullPath[i])		iSlashPoint = i;
		}
		if(0 != iDotPoint && 0 != iSlashPoint)	break;
	}
	int iCnt = 0;
	for(int i = 0; i < iStrSize; ++i)
	{
		if(i < iSlashPoint + 1)	continue;
		if(i == iDotPoint)			break;

		pKeyName[iCnt] = pName[i];		
		++iCnt;
	}
	lstrcpy(pOut, pKeyName);
}

vector<Engine::CGameObject*>* CEffectMgr::Find_EffectList(TCHAR* pName)
{
	boost::unordered_map<const TCHAR*, vector<Engine::CGameObject*>>::iterator iter = find_if(m_mapEffect.begin(), m_mapEffect.end(), Engine::CTagFinder(pName));
	if(iter == m_mapEffect.end())		return NULL; // iter가 end까지 도달하면 현재 맵 컨터이너에 없다는 뜻이다.

	return &(iter->second);
}

void CEffectMgr::Create_Effect(TCHAR* pName, _vec3 vPos, _bool bReStart, _vec3 vSize, _vec3 vAngle, _float fRevolAngle
										, _bool bRefeat, _float fRefeatTime, _vec3* pvecParent/*= NULL*/, bool bHDR, _bool bNoTesting, _float fColorDown, _float fStartTime, _bool bDistort)
{
	// 원본을 가져와서 생성후 원본값 대입해준다.
	vector<Engine::CGameObject*>* pVecEffect = Find_EffectList(pName);		// 원본을 가져온다.
	if(pVecEffect == NULL) return;

	size_t iVecEffectSize = pVecEffect->size();
	for(size_t  i = 0; i < iVecEffectSize; ++i)
	{
		Engine::CGameObject* pGameObj = NULL;
		pGameObj = CMyEffect::Create(m_pGraphicDev, ((CMyEffect*)(*pVecEffect)[i])->m_wIdx, ((CMyEffect*)(*pVecEffect)[i])->m_wstrTexName, ((CMyEffect*)(*pVecEffect)[i])->m_ptCntXY, ((CMyEffect*)(*pVecEffect)[i])->m_fFrameSpeed);

		if(((CMyEffect*)pGameObj)->Get_PosZ() < 0)
			((CMyEffect*)pGameObj)->m_bPost	 = false;
		else
			((CMyEffect*)pGameObj)->m_bPost	 = true;

		((CMyEffect*)pGameObj)->m_bBillBoard = ((CMyEffect*)(*pVecEffect)[i])->m_bBillBoard;
		((CMyEffect*)pGameObj)->Copy_Point(((CMyEffect*)(*pVecEffect)[i]));
		
		
		((CMyEffect*)pGameObj)->Set_Data(vPos, vAngle, vSize, fColorDown);

		((CMyEffect*)pGameObj)->Set_ReStart(bReStart);
		((CMyEffect*)pGameObj)->Set_MaxScale();
		((CMyEffect*)pGameObj)->Set_Name(pName);
		((CMyEffect*)pGameObj)->m_bAbleEffect = true;
		((CMyEffect*)pGameObj)->m_fRevolAngle = fRevolAngle;

		((CMyEffect*)pGameObj)->m_bRefeat		= bRefeat;
		((CMyEffect*)pGameObj)->m_fRefeatTime	= fRefeatTime;
		((CMyEffect*)pGameObj)->m_pvecParent	= pvecParent;
		((CMyEffect*)pGameObj)->m_bHDR			= bHDR;
		((CMyEffect*)pGameObj)->m_fStartTime	= fStartTime;
		((CMyEffect*)pGameObj)->m_bDistortion	= bDistort;

		if(true == bNoTesting) ((CMyEffect*)pGameObj)->Set_NoTesting();
		
		m_pLayer->Ready_Object(pName, pGameObj);
	}
}



