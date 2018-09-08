#include "stdafx.h"
#include "Bunker.h"

#include "Export_Function.h"
#include "Include.h"

#include "Transform.h"
#include "CameraObserver.h"
#include "PlayerObserver.h"

#include "TerrainCol.h"
#include "CollisionMgr.h"
#include "Stage.h"
#include "Bullet.h"
#include "BasicBullet.h"
#include "MultiBullet.h"
#include "ItemBullet.h"
#include "ExplosionMon.h"


#define SHOT_ITVTIME 0.5f
CBunker::CBunker(LPDIRECT3DDEVICE9 pDevice)
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
, m_pPlayer(NULL)
, m_fShotTime(0.f)
, m_bLockOn(false)
, m_bOnceItem(true)
, m_bDie(false)
{

}

CBunker::~CBunker(void)
{
	Release();
}

HRESULT CBunker::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vScale	= D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	m_pInfo->m_fHP		= 10.f;
	m_pInfo->m_eName	= Engine::OBJ_BUNKER;
	m_pInfo->m_eState	= Engine::STATE_LIVE;

	m_pCameraObserver = CCameraObserver::Create();
	m_pPlayerObserver = CPlayerObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);
	m_pInfoSubject->Subscribe(m_pPlayerObserver);

	m_dwVtxCnt			= m_pBuffer->GetVtxCnt();
	m_pVertex			= new Engine::VTXCOL[m_dwVtxCnt];
	m_pConvertVertex	= new Engine::VTXCOL[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_Bunker", m_pVertex);

	return S_OK;
}

int CBunker::Update(void)
{
	if(m_pInfo->m_fHP <= 0.f)
	{
		m_pInfo->m_eState = Engine::STATE_DIE;
		/////ÀÌÆåÆ®
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


	if(m_pInfo->m_eState == Engine::STATE_DIE
		&& m_bOnceItem)
	{
		ItemCreate();
		m_bOnceItem = false;
	}

	if(m_pPlayerObserver->GetState() == Engine::STATE_LIVE
		&& m_pInfo->m_eState == Engine::STATE_LIVE )
	{
		//ÃÑ¾Ë ¹ß»ç
		D3DXVECTOR3 vPlayerPos = (*m_pPlayerObserver->GetPos());
		D3DXVECTOR3	vDir = vPlayerPos - m_pInfo->m_vPos;

		if( D3DXVec3Length(&vDir) < 130.f )
		{
			D3DXVec3Normalize(&vDir, &vDir);
			m_fShotTime += m_pTimeMgr->GetInstance()->GetTime();

			if(m_fShotTime > SHOT_ITVTIME)
			{
				BulletCreate(vDir);
				m_fShotTime = 0.f;
			}
		}
		else
			m_fShotTime = 0.f;
	}

	//ÀÌµ¿
	if(m_pInfo->m_eState == Engine::STATE_LIVE)
	{

	}

	TransformMatrix();

	return 0;
}

void CBunker::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_Bunker", m_pConvertVertex);

	m_pDevice->SetTexture(0, NULL);

	m_pBuffer->Render();

	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_Bunker", m_pVertex);
}

CBunker* CBunker::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBunker* pObj = new CBunker(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}

HRESULT CBunker::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent =  Engine::CTransform::Create(g_vLook);
	((Engine::CTransform*)pComponent)->SetScale(D3DXVECTOR3(1.f,1.f,1.f));
	m_pInfo = (Engine::CTransform*)pComponent; 
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//TerrainCol
	pComponent = m_pTerrainCol = CTerrainCol::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"TerrainCol", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_Bunker");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	return S_OK;
}

void CBunker::Release(void)
{
	m_pInfoSubject->UnSubscribe(m_pPlayerObserver);
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);

	Engine::Safe_Delete_Array(m_pVertex);
	Engine::Safe_Delete_Array(m_pConvertVertex);

	Engine::Safe_Delete(m_pPlayerObserver);
	Engine::Safe_Delete(m_pCameraObserver);
}

void CBunker::TransformMatrix(void)
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

void CBunker::BulletCreate(D3DXVECTOR3 _vDir)
{
	MAPOBJLIST::iterator	MonIter   = m_pMapObjlist->find(L"Player");

	Engine::CGameObject*	pBullet		= NULL;
	//»ý¼º
	pBullet		= CMultiBullet::Create(m_pDevice);
	((CBullet*)pBullet)->SetList(&(MonIter->second));
	((CBullet*)pBullet)->Initialize();
	((CBullet*)pBullet)->SetPos(m_pInfo->m_vPos);
	((CBullet*)pBullet)->SetDir(_vDir);

	//Çª½¬
	MAPOBJLIST::iterator	BulIter = m_pMapObjlist->find(L"Bullet");
	if(BulIter == m_pMapObjlist->end())
		(*m_pMapObjlist)[L"Bullet"] = OBJECTLIST();

	(*m_pMapObjlist)[L"Bullet"].push_back(pBullet);

}

void CBunker::ItemCreate()
{
	Engine::CGameObject*	pItem		= NULL;
	//»ý¼º
	pItem		= CItemBullet::Create(m_pDevice);
	((CItemBullet*)pItem)->SetPlayer(m_pPlayer);
	((CItemBullet*)pItem)->SetPos(D3DXVECTOR3(m_pInfo->m_vPos.x, m_pInfo->m_vPos.y + 5.f, 118.f));

	//Çª½¬
	MAPOBJLIST::iterator	ItemIter = m_pMapObjlist->find(L"Item");
	if(ItemIter == m_pMapObjlist->end())
		(*m_pMapObjlist)[L"Item"] = OBJECTLIST();

	(*m_pMapObjlist)[L"Item"].push_back(pItem);
}


void CBunker::SetPos(const D3DXVECTOR3& rPos)
{
	m_pInfo->m_vPos = rPos;
}

void CBunker::SetHP(float rHP)
{
	m_pInfo->m_fHP = rHP;
}
