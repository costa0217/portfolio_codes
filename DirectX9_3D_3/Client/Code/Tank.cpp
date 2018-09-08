#include "stdafx.h"
#include "Tank.h"

#include "Export_Function.h"
#include "Include.h"

#include "Transform.h"
#include "CameraObserver.h"
#include "PlayerObserver.h"

#include "Pipeline.h"

#include "TerrainCol.h"
#include "CollisionMgr.h"
#include "Stage.h"
#include "Bullet.h"
#include "BasicBullet.h"
#include "ItemHp.h"
#include "ExplosionMon.h"

#define SHOT_ITVTIME 1.f
CTank::CTank(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourcesMgr())
, m_pTimeMgr(Engine::Get_TimeMgr())
, m_pInfoSubject(Engine::Get_InfoSubject())
, m_pManagement(Engine::Get_Management())
, m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_pTerrainCol(NULL)
, m_pCameraObserver(NULL)
, m_pPlayerObserver(NULL)
, m_pTerrainVertex(NULL)
, m_pPlayer(NULL)
, m_fShotTime(0.f)
, m_bLockOn(false)
, m_fSpeed(0)
, m_fMoveTime(4.f)
, m_bOnceItem(true)
, m_bDie(false)
{
	ZeroMemory(m_pBuffer, sizeof(Engine::CVIBuffer*) * TPART_END );
	ZeroMemory(m_pInfo, sizeof(Engine::CTransform*) * TPART_END );
	ZeroMemory(m_dwVtxCnt, sizeof(DWORD) * TPART_END );
	ZeroMemory(m_fAngle, sizeof(float) * Engine::ANGLE_END);
}

CTank::~CTank(void)
{
	Release();
}

HRESULT CTank::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	SetParts();
	m_pInfo[TPART_BODY]->m_vDir		= D3DXVECTOR3(0.f, 0.f, -1.f);
	m_pInfo[TPART_BODY]->m_fHP		= 15.f;
	m_pInfo[TPART_BODY]->m_eName	= Engine::OBJ_TANK;
	m_pInfo[TPART_BODY]->m_eState	= Engine::STATE_LIVE;

	m_pCameraObserver = CCameraObserver::Create();
	m_pPlayerObserver = CPlayerObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);
	m_pInfoSubject->Subscribe(m_pPlayerObserver);

	for(int iPART = 0; iPART < TPART_END; ++iPART)
	{
		m_dwVtxCnt[iPART]			= m_pBuffer[iPART]->GetVtxCnt();
		m_pVertex[iPART]			= new Engine::VTXCOL[m_dwVtxCnt[iPART]];
		m_pConvertVertex[iPART]		= new Engine::VTXCOL[m_dwVtxCnt[iPART]];

		if( iPART == TPART_BODY )
			m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_TankBody", m_pVertex[iPART]);
		else if( iPART == TPART_PAW )
			m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_TankPaw", m_pVertex[iPART]);
		else if( iPART == TPART_PAWSIN )
			m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_TankPawSin", m_pVertex[iPART]);
	}

	m_fSpeed = 5.f;
	

	return S_OK;
}

int CTank::Update(void)
{
	if(m_pInfo[TPART_BODY]->m_fHP <= 0.f)
	{
		m_pInfo[TPART_BODY]->m_eState = Engine::STATE_DIE;
		
		/////이펙트
		if(m_bDie == false)
		{

			m_bDie = true;
			CExplosionMon* m_pTemp = CExplosionMon::Create(m_pDevice);
			((CExplosionMon*)m_pTemp)->SetPos(m_pInfo[TPART_BODY]->m_vPos);

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
	m_pTerrainCol->SetColInfo(&m_pInfo[TPART_BODY]->m_vPos, m_pTerrainVertex, dwTerrainVtxCntX, dwTerrainVtxCntZ);


	Engine::CGameObject::Update();


	if(m_pInfo[TPART_BODY]->m_eState == Engine::STATE_DIE
		&& m_bOnceItem)
	{
		ItemCreate();
		m_bOnceItem = false;
	}



	//중력
	m_pInfo[TPART_BODY]->m_vPos.y -= 0.98f * m_pTimeMgr->GetTime() * 7;

	CombinePart();

	
	//총알 발사
	D3DXVECTOR3 vPlayerPos = (*m_pPlayerObserver->GetPos());
	D3DXVECTOR3	vDir = vPlayerPos - m_pInfo[TPART_BODY]->m_vPos;

	if(m_pPlayerObserver->GetState() == Engine::STATE_LIVE
		&& m_pInfo[TPART_BODY]->m_eState == Engine::STATE_LIVE )
	{
		if( D3DXVec3Length(&vDir) < 130.f )
		{
			D3DXVec3Normalize(&vDir, &vDir);
			m_fShotTime += m_pTimeMgr->GetInstance()->GetTime();

			if(m_fShotTime > SHOT_ITVTIME)
				BulletCreate(vDir);
		}
		else
			m_fShotTime = 0.f;
	}

	//움직임
	if(m_pInfo[TPART_BODY]->m_eState == Engine::STATE_LIVE)
	{
		SetPawRotation();
		SetDirMove();
	}


	TransformMatrix();

	return 0;
}

void CTank::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	for(int iPART = 0; iPART < TPART_END; ++iPART)
	{
		if( iPART == TPART_BODY )
			m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_TankBody", m_pConvertVertex[iPART]);
		else if( iPART == TPART_PAW )
			m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_TankPaw", m_pConvertVertex[iPART]);
		else if( iPART == TPART_PAWSIN )
			m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_TankPawSin", m_pConvertVertex[iPART]);

		m_pDevice->SetTexture(0, NULL);	

		m_pBuffer[iPART]->Render();


		if( iPART == TPART_BODY )
			m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_TankBody", m_pVertex[iPART]);
		else if( iPART == TPART_PAW )
			m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_TankPaw", m_pVertex[iPART]);
		else if( iPART == TPART_PAWSIN )
			m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_TankPawSin", m_pVertex[iPART]);
	}
}

CTank* CTank::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTank* pObj = new CTank(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}

HRESULT CTank::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	for(int i = 0; i < TPART_END; ++i)
	{
		pComponent = m_pInfo[i] = Engine::CTransform::Create(g_vLook);
		NULL_CHECK_RETURN(m_pInfo[i], E_FAIL);

		if(i != TPART_BODY)
			continue;
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));
	}
	

	//TerrainCol
	pComponent = m_pTerrainCol = CTerrainCol::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"TerrainCol", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_TankBody");
	m_pBuffer[TPART_BODY] = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer[TPART_BODY], E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_TankPaw");
	m_pBuffer[TPART_PAW] = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer[TPART_PAW], E_FAIL);

	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_TankPawSin");
	m_pBuffer[TPART_PAWSIN] = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer[TPART_PAWSIN], E_FAIL);

	return S_OK;
}

void CTank::Release(void)
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);
	m_pInfoSubject->UnSubscribe(m_pPlayerObserver);

	for(int i = 0; i < TPART_END; ++i)
	{
		Engine::Safe_Delete_Array(m_pVertex[i]);
		Engine::Safe_Delete_Array(m_pConvertVertex[i]);

		if(i != TPART_BODY)
		{
			Engine::Safe_Delete(m_pBuffer[i]);
			Engine::Safe_Delete(m_pInfo[i]);
		}
	}
	Engine::Safe_Delete(m_pCameraObserver);
	Engine::Safe_Delete(m_pPlayerObserver);
}

void CTank::SetPawRotation(void)
{
	D3DXMATRIX	matRot;
	D3DXVECTOR3* vpPlayer		= m_pPlayerObserver->GetPos();

	D3DXVECTOR3 vPlayerToMonDir	= (*vpPlayer) - m_pInfo[TPART_BODY]->m_vPos;
	vPlayerToMonDir	= D3DXVECTOR3(vPlayerToMonDir.x, 0.f, vPlayerToMonDir.z);
	D3DXVec3Normalize(&vPlayerToMonDir, &vPlayerToMonDir);

	//내적으로 각도를 구함
	float fAngle = acos(D3DXVec3Dot(&vPlayerToMonDir, &D3DXVECTOR3(0.f, 0.f, -1.f)));
	if(0.f < vPlayerToMonDir.x )		// 내적 예외처리
		fAngle = D3DX_PI * 2 - fAngle;

	m_pInfo[TPART_PAW]->m_fAngle[Engine::ANGLE_Y] = fAngle;

	//각도만큼 방향벡터 회전
	D3DXMatrixRotationY(&matRot, fAngle);
	D3DXVec3TransformNormal(&m_pInfo[TPART_PAW]->m_vDir, &D3DXVECTOR3(0.f, 0.f, -1.f), &matRot);
	D3DXVec3TransformNormal(&m_pInfo[TPART_PAWSIN]->m_vDir, &D3DXVECTOR3(0.f, 0.f, -1.f), &matRot);


	/* 포신 회전 */
	D3DXVECTOR3		vPToMDir = (*vpPlayer) - m_pInfo[TPART_BODY]->m_vPos;
	vPToMDir = D3DXVECTOR3(vPToMDir.x, vPToMDir.y, 0.f);
	D3DXVec3Normalize(&vPToMDir, &vPToMDir);

	float fPawAngle = acos(D3DXVec3Dot(&vPToMDir, &D3DXVECTOR3(1.f, 0.f, 0.f)));
	//if( 0.f < vPToMDir.y)		// 내적 예외처리
	//	fPawAngle = D3DX_PI * 2 - fPawAngle;
	if(fPawAngle > D3DXToRadian(90.f))
		fPawAngle = D3DXToRadian(180.f) - fPawAngle;

	m_pInfo[TPART_PAWSIN]->m_fAngle[Engine::ANGLE_X] = fPawAngle;

}
void CTank::TransformMatrix(void)
{
	const D3DXMATRIX* pmatView = m_pCameraObserver->GetView();
	NULL_CHECK(pmatView);
	const D3DXMATRIX* pmatProj = m_pCameraObserver->GetProj();
	NULL_CHECK(pmatProj);

	D3DXMATRIX matScale;
	D3DXMATRIX matRotx, matRoty, matRotz;

	D3DXMatrixRotationZ(&matRotx, m_fAngle[Engine::ANGLE_X]);
	D3DXMatrixRotationY(&matRoty, m_fAngle[Engine::ANGLE_Y]);
	D3DXMatrixRotationX(&matRotz, m_fAngle[Engine::ANGLE_Z]);

	m_pInfo[TPART_PAWSIN]->m_matWorldAng = matRotz * matRoty * matRotx * m_pInfo[TPART_PAWSIN]->m_matWorld;

	for(int iPART = 0; iPART < TPART_END; ++iPART )
	{
		D3DXMatrixScaling(&matScale, m_pInfo[iPART]->m_vScale.x, m_pInfo[iPART]->m_vScale.y, m_pInfo[iPART]->m_vScale.z);
		m_pInfo[iPART]->m_matWorld = matScale * m_pInfo[iPART]->m_matWorld;

		if(iPART == TPART_PAWSIN)
			m_pInfo[TPART_PAWSIN]->m_matWorldAng = matScale * m_pInfo[TPART_PAWSIN]->m_matWorldAng;

		for(size_t i = 0; i < m_dwVtxCnt[iPART]; ++i)
		{
			m_pConvertVertex[iPART][i] = m_pVertex[iPART][i];

			if(m_pInfo[TPART_BODY]->m_eState == Engine::STATE_DIE)
				m_pConvertVertex[iPART][i].dwColor = D3DCOLOR_ARGB(255, 0, 0, 0);

			if(iPART == TPART_PAWSIN)
				D3DXVec3TransformCoord(&m_pConvertVertex[iPART][i].vPos, &m_pConvertVertex[iPART][i].vPos, &m_pInfo[iPART]->m_matWorldAng);
			else
				D3DXVec3TransformCoord(&m_pConvertVertex[iPART][i].vPos, &m_pConvertVertex[iPART][i].vPos, &m_pInfo[iPART]->m_matWorld);
			D3DXVec3TransformCoord(&m_pConvertVertex[iPART][i].vPos, &m_pConvertVertex[iPART][i].vPos, pmatView);
			if(m_pConvertVertex[iPART][i].vPos.z < 1.f)
				m_pConvertVertex[iPART][i].vPos.z = 1.f;

			D3DXVec3TransformCoord(&m_pConvertVertex[iPART][i].vPos, &m_pConvertVertex[iPART][i].vPos, pmatProj);
		}
	}
}

void CTank::SetPos(const D3DXVECTOR3& rPos)
{
	m_pInfo[TPART_BODY]->m_vPos = rPos;
	RouteMake(m_pInfo[TPART_BODY]->m_vPos);
}


void CTank::SetHP(float rHP)
{
	m_pInfo[TPART_BODY]->m_fHP = rHP;
}


void CTank::SetParts(void)
{
	m_pInfo[TPART_BODY]->m_vPos			= D3DXVECTOR3(0.f, 0.f, 0.f)	;
	m_pInfo[TPART_BODY]->m_vScale		= D3DXVECTOR3(0.6f, 0.6f, 0.6f)	;
	m_pInfo[TPART_BODY]->m_vDir			= D3DXVECTOR3(0.f, 0.f, -1.f)	;

	m_pInfo[TPART_PAW]->m_vPos			= D3DXVECTOR3(0.f, 0.f, 0.f)	;
	m_pInfo[TPART_PAW]->m_vScale		= D3DXVECTOR3(0.6f, 0.6f, 0.6f)	;
	m_pInfo[TPART_PAW]->m_vDir			= D3DXVECTOR3(0.f, 0.f, -1.f)	;

	m_pInfo[TPART_PAWSIN]->m_vPos		= D3DXVECTOR3(0.f, 6.5f, -2.f)	;
	m_pInfo[TPART_PAWSIN]->m_vScale		= D3DXVECTOR3(0.5f, 0.5f, 0.5f)	;
	m_pInfo[TPART_PAWSIN]->m_vDir		= D3DXVECTOR3(0.f, 0.f, -1.f)	;

}

void CTank::CombinePart(void)
{
	D3DXVECTOR3 vDefaultScale = D3DXVECTOR3(1.f, 1.f, 1.f);
	D3DXMATRIX	matNoRotWorld, rRotX, rRotY, rRotZ;

	// 탱크바디
	Engine::CPipeline::MakeWorldMatrix(&m_pInfo[TPART_BODY]->m_matWorld, &vDefaultScale
		, m_pInfo[TPART_BODY]->m_fAngle, &m_pInfo[TPART_BODY]->m_vPos	);

	D3DXMatrixRotationX(&rRotX, -m_pInfo[TPART_BODY]->m_fAngle[Engine::ANGLE_X]);
	D3DXMatrixRotationY(&rRotY, -m_pInfo[TPART_BODY]->m_fAngle[Engine::ANGLE_Y]);
	D3DXMatrixRotationZ(&rRotZ, -m_pInfo[TPART_BODY]->m_fAngle[Engine::ANGLE_Z]);

	matNoRotWorld = rRotX * rRotY * rRotZ;

	/************************************************************************/
	// 탱크포

	Engine::CPipeline::MakeWorldMatrix(&m_pInfo[TPART_PAW]->m_matWorld, &vDefaultScale
		, m_pInfo[TPART_PAW]->m_fAngle, &m_pInfo[TPART_PAW]->m_vPos	);
	m_pInfo[TPART_PAW]->m_matWorld *= matNoRotWorld * m_pInfo[TPART_BODY]->m_matWorld;
	/************************************************************************/
	// 탱크포신

	Engine::CPipeline::MakeWorldMatrix(&m_pInfo[TPART_PAWSIN]->m_matWorld, &vDefaultScale
		, m_pInfo[TPART_PAWSIN]->m_fAngle, &m_pInfo[TPART_PAWSIN]->m_vPos	);
	m_pInfo[TPART_PAWSIN]->m_matWorld *= m_pInfo[TPART_PAW]->m_matWorld;
	/************************************************************************/
}
void CTank::BulletCreate(D3DXVECTOR3 _vDir)
{
	MAPOBJLIST::iterator	MonIter   = m_pMapObjlist->find(L"Player");

	Engine::CGameObject*	pBullet		= NULL;
	//생성
	pBullet		= CBasicBullet::Create(m_pDevice);
	((CBasicBullet*)pBullet)->SetList(&(MonIter->second));
	((CBasicBullet*)pBullet)->Initialize();
	((CBasicBullet*)pBullet)->SetPos(m_pInfo[TPART_BODY]->m_vPos);
	((CBasicBullet*)pBullet)->SetDir(_vDir);

	//푸쉬
	MAPOBJLIST::iterator	BulIter = m_pMapObjlist->find(L"Bullet");
	if(BulIter == m_pMapObjlist->end())
		(*m_pMapObjlist)[L"Bullet"] = OBJECTLIST();

	(*m_pMapObjlist)[L"Bullet"].push_back(pBullet);

	m_fShotTime = 0.f;
}
void CTank::ItemCreate()
{
	Engine::CGameObject*	pItem		= NULL;
	//생성
	pItem		= CItemHp::Create(m_pDevice);
	((CItemHp*)pItem)->SetPlayer(m_pPlayer);
	((CItemHp*)pItem)->SetPos(D3DXVECTOR3(m_pInfo[TPART_BODY]->m_vPos.x, m_pInfo[TPART_BODY]->m_vPos.y + 5.f, 118.f));

	//푸쉬
	MAPOBJLIST::iterator	ItemIter = m_pMapObjlist->find(L"Item");
	if(ItemIter == m_pMapObjlist->end())
		(*m_pMapObjlist)[L"Item"] = OBJECTLIST();

	(*m_pMapObjlist)[L"Item"].push_back(pItem);
}
void CTank::RouteMake(D3DXVECTOR3 _vPos)
{
	float	fInterval = 10.f;

	vPoint[0] = D3DXVECTOR3(_vPos.x - fInterval, 0.f, _vPos.z + fInterval);
	m_vecRoute.push_back(vPoint[0]);
	vPoint[1] = D3DXVECTOR3(_vPos.x - fInterval, 0.f, _vPos.z - fInterval);
	m_vecRoute.push_back(vPoint[1]);
	vPoint[2] = D3DXVECTOR3(_vPos.x + fInterval, 0.f, _vPos.z - fInterval);
	m_vecRoute.push_back(vPoint[2]);
	vPoint[3] = D3DXVECTOR3(_vPos.x + fInterval, 0.f, _vPos.z + fInterval);
	m_vecRoute.push_back(vPoint[3]);
	vPoint[4] = D3DXVECTOR3(_vPos.x , 0.f, _vPos.z + fInterval * 2.f);
	m_vecRoute.push_back(vPoint[0]);
	vPoint[5] = D3DXVECTOR3(_vPos.x - fInterval * 2.f, 0.f, _vPos.z - fInterval);
	m_vecRoute.push_back(vPoint[1]);
	vPoint[6] = D3DXVECTOR3(_vPos.x + fInterval * 2.f, 0.f, _vPos.z - fInterval);
	m_vecRoute.push_back(vPoint[2]);
	vPoint[7] = D3DXVECTOR3(_vPos.x , 0.f, _vPos.z - fInterval * 2.f);
	m_vecRoute.push_back(vPoint[3]);
}

void CTank::SetDirMove(void)
{
	if(m_fMoveTime > 3.f)
	{
		m_iGoPoint = rand()%8;
		m_fMoveTime	 = 0.f;
	}
	m_fMoveTime += m_pTimeMgr->GetTime();

	m_pInfo[TPART_BODY]->m_vDir = m_vecRoute[m_iGoPoint] - D3DXVECTOR3(m_pInfo[TPART_BODY]->m_vPos.x, 0.f, m_pInfo[TPART_BODY]->m_vPos.z);
	D3DXVec3Normalize(&m_pInfo[TPART_BODY]->m_vDir, &m_pInfo[TPART_BODY]->m_vDir);

	float fAngle = acos(D3DXVec3Dot(&m_pInfo[TPART_BODY]->m_vDir, &D3DXVECTOR3(0.f, 0.f, -1.f)));

	if( m_pInfo[TPART_BODY]->m_vDir.x > 0.f)
		fAngle = D3DX_PI * 2.f - fAngle;

	m_pInfo[TPART_BODY]->m_fAngle[Engine::ANGLE_Y]	= fAngle;

	if(D3DXVec3Length(&(m_vecRoute[m_iGoPoint] - D3DXVECTOR3(m_pInfo[TPART_BODY]->m_vPos.x, 0.f, m_pInfo[TPART_BODY]->m_vPos.z)) ) > 3.f)
		m_pInfo[TPART_BODY]->m_vPos += m_pInfo[TPART_BODY]->m_vDir * m_pTimeMgr->GetTime() * m_fSpeed;
}
