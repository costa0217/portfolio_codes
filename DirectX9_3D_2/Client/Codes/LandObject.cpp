#include "stdafx.h"
#include "LandObject.h"
#include "Export_Function.h"

CLandObject::CLandObject(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pNaviMesh(Engine::Get_NaviMesh())
, m_pBufferCom(NULL)
, m_pTransCom(NULL)
, m_pVertex(NULL)
//, m_eAniStat(CLandObject::STATE_IDLE)
, m_eObjType(TYPE_TERRAIN)
//, m_bJumpFall(false)
//, m_fJumpPos(0.f)
, m_dwNaviIndex(0)
{

}

CLandObject::~CLandObject(void)
{

}

HRESULT CLandObject::Initialize(void)
{
	//m_pVertex = new Engine::VTXTEX[VTXCNTX * VTXCNTZ];

	return S_OK;
}

_int CLandObject::Update(const _float& fTimeDelta)
{
	/*if(NULL == m_pBufferCom)
	{
		m_pBufferCom = (Engine::CVIBuffer*)Engine::Get_Component(L"Com_Buffer", L"GameLogic", L"Terrain");
		if(NULL == m_pBufferCom)
			return -1;
		m_pBufferCom->Get_VtxInfo(m_pVertex);
	}*/

	//if(m_eObjType == TYPE_NAVI)
	//{
	//	if(m_bJumpFall)
	//	{
	//		const _vec3 vCamparePos = m_pTransCom->m_vPosition;
	//		if(0 <= /*Compare_JumpingPoint()*/m_pNaviMesh->Compute_Height(vCamparePos, m_dwNaviIndex) - m_pTransCom->m_vPosition.y)
	//		{
	//			m_bJumpFall = false;
 //		 		m_eAniStat  = STATE_LAND;
	//			m_pTransCom->m_vPosition.y = m_pNaviMesh->Compute_Height(vCamparePos, m_dwNaviIndex);
	//		}
	//	}
	//	if(!m_bJumpFall && m_eAniStat != STATE_JUMP)
	//		m_pNaviMesh->Set_Jump(false);
	//		//Compute_Height();
	//}
	//else if(m_eObjType == TYPE_TERRAIN)
	//{
	//	//Compute_Height();
	//}
	//

	Engine::CGameObject::Update(fTimeDelta);


	return 0;
}

void CLandObject::Compute_Height(void)
{
	//_int iIndex = _int(m_pTransCom->m_vPosition.z) / VTXITV * VTXCNTX + _int(m_pTransCom->m_vPosition.x) / VTXITV;

	//_float	fRatioX = (m_pTransCom->m_vPosition.x - m_pVertex[iIndex + VTXCNTX].vPos.x) / VTXITV;
	//_float	fRatioZ = (m_pVertex[iIndex + VTXCNTX].vPos.z - m_pTransCom->m_vPosition.z) / VTXITV;

	//_float	fY[4] = {m_pVertex[iIndex + VTXCNTX].vPos.y
	//	, m_pVertex[iIndex + VTXCNTX + 1].vPos.y
	//	, m_pVertex[iIndex + 1].vPos.y
	//	, m_pVertex[iIndex].vPos.y};

	//if(fRatioX > fRatioZ) // 오른쪽위
	//{
	//	m_pTransCom->m_vPosition.y = fY[0] + (fY[1] - fY[0]) * fRatioX + (fY[2] - fY[1]) * fRatioZ;
	//}

	//else	// 왼쪽아래
	//{
	//	m_pTransCom->m_vPosition.y = fY[0] + (fY[3] - fY[0]) * fRatioZ + (fY[2] - fY[3]) * fRatioX;
	//}
	
}

_ulong CLandObject::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();


	if(0 == dwRefCnt)
	{
		//::Safe_Delete_Array(m_pVertex);
	}

	return dwRefCnt;
}

//_float CLandObject::Compare_JumpingPoint(void)
////{
////	_int iIndex = _int(m_pTransCom->m_vPosition.z) / VTXITV * VTXCNTX + _int(m_pTransCom->m_vPosition.x) / VTXITV;
////	_float fResult = 0.f;
////
////	_float	fRatioX = (m_pTransCom->m_vPosition.x - m_pVertex[iIndex + VTXCNTX].vPos.x) / VTXITV;
////	_float	fRatioZ = (m_pVertex[iIndex + VTXCNTX].vPos.z - m_pTransCom->m_vPosition.z) / VTXITV;
////
////	_float	fY[4] = {m_pVertex[iIndex + VTXCNTX].vPos.y
////		, m_pVertex[iIndex + VTXCNTX + 1].vPos.y
////		, m_pVertex[iIndex + 1].vPos.y
////		, m_pVertex[iIndex].vPos.y};
////
////	if(fRatioX > fRatioZ) // 오른쪽위
////	{
////		fResult = fY[0] + (fY[1] - fY[0]) * fRatioX + (fY[2] - fY[1]) * fRatioZ;
////	}
////
////	else	// 왼쪽아래
////	{
////		fResult = fY[0] + (fY[3] - fY[0]) * fRatioZ + (fY[2] - fY[3]) * fRatioX;
////	}
//
//	return 0/;
//}
//
