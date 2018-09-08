#include "stdafx.h"
#include "Retona.h"

#include "Export_Function.h"
#include "Include.h"

#include "Transform.h"
#include "CameraObserver.h"

#include "TerrainCol.h"
#include "CollisionMgr.h"
#include "Stage.h"
#include "ExplosionMon.h"

CRetona::CRetona(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourcesMgr())
, m_pTimeMgr(Engine::Get_TimeMgr())
, m_pInfoSubject(Engine::Get_InfoSubject())
, m_pManagement(Engine::Get_Management())
, m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_pInfo(NULL)
, m_pTexture(NULL)
, m_pBuffer(NULL)
, m_pTerrainCol(NULL)
, m_pCameraObserver(NULL)
, m_pTerrainVertex(NULL)
, m_dwVtxCnt(0)
, m_fSpeed(0)
, m_fMoveTime(4.f)
, m_bDie(false)
{

}

CRetona::~CRetona(void)
{
	Release();
}

HRESULT CRetona::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vScale	= D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	m_pInfo->m_fHP		= 10.f;

	m_pInfo->m_eName	= Engine::OBJ_RETONA;
	m_pInfo->m_eState	= Engine::STATE_LIVE;
	m_pInfo->m_vDir		= D3DXVECTOR3(0.f, 0.f, -1.f);

	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_dwVtxCnt			= m_pBuffer->GetVtxCnt();
	m_pVertex			= new Engine::VTXCOL[m_dwVtxCnt];
	m_pConvertVertex	= new Engine::VTXCOL[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_Retona", m_pVertex);

	m_fSpeed = 10.f;

	return S_OK;
}

int CRetona::Update(void)
{
	if(m_pInfo->m_fHP <= 0.f)
	{
		m_pInfo->m_eState = Engine::STATE_DIE;
		/////이펙트
		if(m_bDie == false)
		{
			m_bDie = true;
			CExplosionMon* m_pTemp = CExplosionMon::Create(m_pDevice);
			((CExplosionMon*)m_pTemp)->SetPos(m_pInfo->m_vPos);

			MAPOBJLIST::iterator iter = m_pMapObjlist->find(L"Explosion");
			if(iter == m_pMapObjlist->end())
			{
				(*m_pMapObjlist)[L"Explosion"] = OBJECTLIST();
			}
			(*m_pMapObjlist)[L"Explosion"].push_back(m_pTemp);
		}
		////
	}

	DWORD dwTerrainVtxCntX, dwTerrainVtxCntZ;

	m_pResourceMgr->GetTerrainVtxCnt(Engine::RESOURCE_DYNAMIC, L"Buffer_TerrainTex", &dwTerrainVtxCntX, &dwTerrainVtxCntZ);
	m_pTerrainVertex = m_pManagement->GetTerrainVtx(Engine::LAYER_GAMELOGIC, L"Terrain");
	m_pTerrainCol->SetColInfo(&m_pInfo->m_vPos, m_pTerrainVertex, dwTerrainVtxCntX, dwTerrainVtxCntZ);

	Engine::CGameObject::Update();


	//중력
	m_pInfo->m_vPos.y -= 0.98f * m_pTimeMgr->GetTime() * 7;

	//이동
	if(m_pInfo->m_eState == Engine::STATE_LIVE)
	{
		SetDirMove();
	}


	TransformMatrix();

	return 0;
}

void CRetona::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_Retona", m_pConvertVertex);

	m_pDevice->SetTexture(0, NULL);

	m_pBuffer->Render();

	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_Retona", m_pVertex);
}

CRetona* CRetona::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CRetona* pObj = new CRetona(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}

HRESULT CRetona::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent =  Engine::CTransform::Create(g_vLook);
	m_pInfo = (Engine::CTransform*)pComponent; 
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//TerrainCol
	pComponent = m_pTerrainCol = CTerrainCol::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"TerrainCol", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_Retona");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	return S_OK;
}

void CRetona::Release(void)
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);

	Engine::Safe_Delete_Array(m_pVertex);
	Engine::Safe_Delete_Array(m_pConvertVertex);

	Engine::Safe_Delete(m_pCameraObserver);
}

void CRetona::TransformMatrix(void)
{
	const D3DXMATRIX* pmatView = m_pCameraObserver->GetView();
	NULL_CHECK(pmatView);
	const D3DXMATRIX* pmatProj = m_pCameraObserver->GetProj();
	NULL_CHECK(pmatProj);

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVertex[i] = m_pVertex[i];

		if(m_pInfo->m_eState == Engine::STATE_DIE)
			m_pConvertVertex[i].dwColor = D3DCOLOR_ARGB(255, 0, 0, 0);

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, &m_pInfo->m_matWorld);
		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatView);
		if(m_pConvertVertex[i].vPos.z < 1.f)
			m_pConvertVertex[i].vPos.z = 1.f;

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatProj);
	}
}

void CRetona::SetPos(const D3DXVECTOR3& rPos)
{
	m_pInfo->m_vPos = rPos;
	RouteMake(m_pInfo->m_vPos);
}

void CRetona::SetHP(float rHP)
{
	m_pInfo->m_fHP = rHP;
}

void CRetona::RouteMake(D3DXVECTOR3 _vPos)
{
	float	fInterval = 20.f;

	vPoint[0] = D3DXVECTOR3(_vPos.x - fInterval, 0.f, _vPos.z + fInterval);
	m_vecRoute.push_back(vPoint[0]);
	vPoint[1] = D3DXVECTOR3(_vPos.x - fInterval, 0.f, _vPos.z - fInterval);
	m_vecRoute.push_back(vPoint[1]);
	vPoint[2] = D3DXVECTOR3(_vPos.x + fInterval, 0.f, _vPos.z - fInterval);
	m_vecRoute.push_back(vPoint[2]);
	vPoint[3] = D3DXVECTOR3(_vPos.x + fInterval, 0.f, _vPos.z + fInterval);
	m_vecRoute.push_back(vPoint[3]);
}

void CRetona::SetDirMove(void)
{
	if(m_fMoveTime > 2.f)
	{
		m_iGoPoint = rand()%4;
		m_fMoveTime	 = 0.f;
	}
	m_fMoveTime += m_pTimeMgr->GetTime();

	m_pInfo->m_vDir = m_vecRoute[m_iGoPoint] - D3DXVECTOR3(m_pInfo->m_vPos.x, 0.f, m_pInfo->m_vPos.z);
	D3DXVec3Normalize(&m_pInfo->m_vDir, &m_pInfo->m_vDir);

	float fAngle = acos(D3DXVec3Dot(&m_pInfo->m_vDir, &D3DXVECTOR3(0.f, 0.f, -1.f)));

	if( m_pInfo->m_vDir.x > 0.f)
		fAngle = D3DX_PI * 2.f - fAngle;

	m_pInfo->m_fAngle[Engine::ANGLE_Y]	= fAngle;

	if(D3DXVec3Length(&(m_vecRoute[m_iGoPoint] - D3DXVECTOR3(m_pInfo->m_vPos.x, 0.f, m_pInfo->m_vPos.z)) ) > 3.f)
		m_pInfo->m_vPos += m_pInfo->m_vDir * m_pTimeMgr->GetTime() * m_fSpeed;
}
