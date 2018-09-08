#include "stdafx.h"
#include "MCTrail.h"

#include "Export_Function.h"
#include "EffectMgr.h"
#include "Player.h"
#include "RenderTargetMgr.h"
#include "Moonchopper.h"

CMCTrail::CMCTrail(LPDIRECT3DDEVICE9 pGraphicDev, _int iIndex)
: CGameObject(pGraphicDev)
, m_vSwordStartPoint(0.f, 0.f, 0.f)
, m_vSwordEndPoint(0.f, 0.f, 0.f)
, m_iImageNum(0)
, m_vColor(0.f, 0.f, 0.f, 0.f)

, m_pVTXInfo(NULL)
, m_pOriVTXInfo(NULL)
, m_pMonster(NULL)
, m_pvecStartPos(NULL)
, m_pvecEndPos(NULL)
//, m_vecEndPos(0.f, 0.f, 0.f)
, m_pmatWorldMonster(NULL)
, m_iIndex(iIndex)
, m_fTrailLength(4.5f)
{
	//ZeroMemory(m_bTrailCheck, sizeof(_bool) * 2);
}

CMCTrail::~CMCTrail(void)
{

}

CMCTrail* CMCTrail::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iIndex)
{
	CMCTrail*		pTrail = new CMCTrail(pGraphicDev, iIndex);

	if(FAILED(pTrail->Initialize()))
	{
		MSG_BOX("pMCTrail Create Faild");
		::Safe_Release(pTrail);
	}
	return pTrail;
}

HRESULT CMCTrail::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_PlayerSwordTrail");
	if(NULL == m_pEffect)		return E_FAIL;

	if(FAILED(Add_Component()))		return E_FAIL;

	m_pVTXInfo		= new Engine::VTXTEX[60];
	m_pOriVTXInfo 	= new Engine::VTXTEX[60];
	m_pBufferCom->Get_VtxInfo(m_pVTXInfo);

	return S_OK;
}

HRESULT CMCTrail::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCom = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_MonTrail");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_PlayerTrail");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;
}

_int CMCTrail::Update(const _float& fTimeDelta)
{
	if(NULL == m_pmatWorldMonster)
	{
		m_pmatWorldMonster = &((Engine::CTransform*)Engine::Get_Component(L"Com_Transform", L"GameLogic", L"Moonchopper"))->m_matWorld;
		
		if(m_iIndex == 0)
		{
			m_pvecStartPos	= ((CMoonchopper*)m_pMonster)->Get_RightElbowPos();
			m_pvecEndPos		= ((CMoonchopper*)m_pMonster)->Get_RightHandPos();
		}
		else
		{
			m_pvecStartPos	= ((CMoonchopper*)m_pMonster)->Get_LeftElbowPos();
			m_pvecEndPos		= ((CMoonchopper*)m_pMonster)->Get_LeftHandPos();
		}		
	}
	//m_vecEndPos		=	*m_pvecStartPos +  _vec3(100.f, 0.f, 0.f);

	Engine::CGameObject::Update(fTimeDelta);

	m_vSwordStartPoint		= *m_pvecStartPos;
	m_vSwordEndPoint		= *m_pvecStartPos + (*m_pvecEndPos - *m_pvecStartPos) * m_fTrailLength;

	D3DXVec3TransformCoord(&m_vSwordEndPoint, &m_vSwordEndPoint, m_pmatWorldMonster);
	D3DXVec3TransformCoord(&m_vSwordStartPoint, &m_vSwordStartPoint, m_pmatWorldMonster);

	memcpy(m_pOriVTXInfo, m_pVTXInfo, sizeof(Engine::VTXTEX) * 60);

	Move(fTimeDelta);
	Set_Point(m_vSwordStartPoint, m_vSwordEndPoint);

	//printf("○ : %f   %f   %f\n", m_vSwordStartPoint.x, m_vSwordStartPoint.y, m_vSwordStartPoint.z);
	//printf("■ : %f   %f   %f\n", m_vSwordEndPoint.x, m_vSwordEndPoint.y, m_vSwordEndPoint.z);

	boost::unordered_map<int, vector<CLIENTBOXINFO*>>::iterator MapiterEnd	= ((CMonster*)m_pMonster)->Get_MonsterMapAttackBox()->end();
	boost::unordered_map<int, vector<CLIENTBOXINFO*>>::iterator MapiterFind	= ((CMonster*)m_pMonster)->Get_MonsterMapAttackBox()->find(((CMonster*)m_pMonster)->Get_MonsterAnimationIndex());

	if(MapiterFind == MapiterEnd)
	{
		return 0;
		/*vector<CLIENTBOXINFO*>::iterator iter		= MapiterFind->second.begin();
		vector<CLIENTBOXINFO*>::iterator iterEnd	= MapiterFind->second.end();

		for(; iter != iterEnd; ++iter)
		{
			if(((CMonster*)m_pMonster)->Get_MeshCom()->Check_EndPeriodMulTime((*iter)->fStartTime) == false)
				return 0;

			if(((CMonster*)m_pMonster)->Get_MeshCom()->Check_EndPeriodMulTime((*iter)->fEndTime) == true)
				return 0;
		}*/
	}

	_matrix			matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXVec3TransformCoord(&m_vViewPos, &m_vSwordEndPoint, &(*m_pmatWorldMonster * matView));

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ALPHA, this, m_vViewPos.z);

	return 0;
}

void CMCTrail::Render(void)
{
	if(NULL == m_pEffect)		return;
	
	m_pBufferCom->Set_VtxInfo(m_pOriVTXInfo);

	Set_ContantTable();
	LPDIRECT3DTEXTURE9 pDepthTexture = Engine::CRenderTargetMgr::GetInstance()->Get_TargetTexture(L"Target_Depth");
	m_pEffect->SetTexture("g_DepthTexture", pDepthTexture);

	// 고정기능렌더링파이프라인을 사용하지않고, 셰이더로 그리겠습니다.
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);

	m_pBufferCom->Render(NULL);

	m_pEffect->EndPass();
	m_pEffect->End();	

	m_pEffect->SetTexture("g_BaseTexture", NULL);
	m_pEffect->SetTexture("g_DepthTexture", NULL);

	m_pBufferCom->Set_VtxInfo(m_pVTXInfo);
}

void CMCTrail::Set_ContantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &matProj);

	m_pEffect->SetVector("g_vColor", &m_vColor);

	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture", 0);	
}

void CMCTrail::Set_Point(_vec3 vStart, _vec3 vEnd)
{
	for(int i = 19; i > -1; i -= 10)
	{
		m_pVTXInfo[i+10].vPos	= m_pVTXInfo[i].vPos;
		m_pVTXInfo[i+40].vPos = m_pVTXInfo[i+30].vPos;
	}

	m_pVTXInfo[9].vPos	= m_pVTXInfo[0].vPos;
	m_pVTXInfo[39].vPos = m_pVTXInfo[30].vPos;

	m_pVTXInfo[0].vPos		= vStart;
	m_pVTXInfo[30].vPos	= vEnd;

	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 9; ++j)
		{
			if( 0 == i )
			{
				if(0 == j )		continue;

				D3DXVec3CatmullRom(&m_pVTXInfo[i * 10 + j].vPos,
					&m_pVTXInfo[0].vPos,	&m_pVTXInfo[0].vPos,
					&m_pVTXInfo[9].vPos,	&m_pVTXInfo[19].vPos,
					(float)(j) * 0.125f);

				D3DXVec3CatmullRom(&m_pVTXInfo[(i * 10 + j + 30)].vPos,
					&m_pVTXInfo[30].vPos,	&m_pVTXInfo[30].vPos,
					&m_pVTXInfo[9 + 30].vPos,	&m_pVTXInfo[19 + 30].vPos,
					(float)(j) * 0.125f);
			}
			else if( 1== i )
			{
				D3DXVec3CatmullRom(&m_pVTXInfo[i * 10 + j].vPos,
					&m_pVTXInfo[0].vPos,	&m_pVTXInfo[9].vPos,
					&m_pVTXInfo[19].vPos,	&m_pVTXInfo[29].vPos,
					(float)(j) * 0.125f);

				D3DXVec3CatmullRom(&m_pVTXInfo[(i * 10 + j + 30)].vPos,
					&m_pVTXInfo[0 + 30].vPos,	&m_pVTXInfo[9 + 30].vPos,
					&m_pVTXInfo[19 + 30].vPos,	&m_pVTXInfo[29 + 30].vPos,
					(float)(j) * 0.125f);
			}	
			else if( 2 == i)
			{
				D3DXVec3CatmullRom(&m_pVTXInfo[i * 10 + j].vPos,
					&m_pVTXInfo[9].vPos,	&m_pVTXInfo[19].vPos,
					&m_pVTXInfo[29].vPos,	&m_pVTXInfo[29].vPos,
					(float)(j) * 0.125f);

				D3DXVec3CatmullRom(&m_pVTXInfo[(i * 10 + j + 30)].vPos,
					&m_pVTXInfo[9 + 30].vPos,	&m_pVTXInfo[19 + 30].vPos,
					&m_pVTXInfo[29 + 30].vPos,	&m_pVTXInfo[29 + 30].vPos,
					(float)(j) * 0.125f);
			}	
		}		
	}
}

void CMCTrail::Move(const _float& fTimeDelta)
{
	m_pBufferCom->Set_VtxInfo(m_pVTXInfo);
}

_ulong CMCTrail::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		::Safe_Delete_Array(m_pVTXInfo);
		::Safe_Delete_Array(m_pOriVTXInfo);
		delete this;
	}
	return dwRefCnt;
}