#include "stdafx.h"
#include "AirGun.h"

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
#include "Missile.h"
#include "ExplosionMon.h"

#define SHOT_ITVTIME 1.f
CAirGun::CAirGun(LPDIRECT3DDEVICE9 pDevice)
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
, m_bDie(false)
{
	ZeroMemory(m_pBuffer, sizeof(Engine::CVIBuffer*) * APART_END );
	ZeroMemory(m_pInfo, sizeof(Engine::CTransform*) * APART_END );
	ZeroMemory(m_dwVtxCnt, sizeof(DWORD) * APART_END );
	ZeroMemory(m_fAngle, sizeof(float) * Engine::ANGLE_END);
}

CAirGun::~CAirGun(void)
{
	Release();
}

HRESULT CAirGun::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	SetParts();

	//m_pInfo->m_vScale	= D3DXVECTOR3(0.5f, 0.5f, 0.5f);

	m_pInfo[APART_BODY]->m_fHP		= 15.f;
	m_pInfo[APART_BODY]->m_eName	= Engine::OBJ_AIRGUN;
	m_pInfo[APART_BODY]->m_eState	= Engine::STATE_LIVE;

	m_pCameraObserver = CCameraObserver::Create();
	m_pPlayerObserver = CPlayerObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);
	m_pInfoSubject->Subscribe(m_pPlayerObserver);

	for(int iPART = 0; iPART < APART_END; ++iPART)
	{
		m_dwVtxCnt[iPART]			= m_pBuffer[iPART]->GetVtxCnt();
		m_pVertex[iPART]			= new Engine::VTXCOL[m_dwVtxCnt[iPART]];
		m_pConvertVertex[iPART]		= new Engine::VTXCOL[m_dwVtxCnt[iPART]];

		if( iPART == APART_BODY )
			m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_AirGunBody", m_pVertex[iPART]);
		else if( iPART == APART_HEAD )
			m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_AirGunHead", m_pVertex[iPART]);
		else if( iPART == APART_PAW )
			m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_AirGunPaw", m_pVertex[iPART]);
	}


	return S_OK;
}

int CAirGun::Update(void)
{
	if(m_pInfo[APART_BODY]->m_fHP <= 0.f)
	{
		m_pInfo[APART_BODY]->m_eState = Engine::STATE_DIE;

		/////이펙트
		if(m_bDie == false)
		{

			m_bDie = true;
			CExplosionMon* m_pTemp = CExplosionMon::Create(m_pDevice);
			((CExplosionMon*)m_pTemp)->SetPos(m_pInfo[APART_BODY]->m_vPos);

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
	m_pTerrainCol->SetColInfo(&m_pInfo[APART_BODY]->m_vPos, m_pTerrainVertex, dwTerrainVtxCntX, dwTerrainVtxCntZ);

	

	Engine::CGameObject::Update();

	CombinePart();

	//총알 발사
	D3DXVECTOR3 vPlayerPos = (*m_pPlayerObserver->GetPos());
	D3DXVECTOR3	vDir = vPlayerPos - m_pInfo[APART_BODY]->m_vPos;

	if(m_pPlayerObserver->GetState() == Engine::STATE_LIVE
		&& m_pInfo[APART_BODY]->m_eState == Engine::STATE_LIVE )
	{
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

	//이동
	if(m_pInfo[APART_BODY]->m_eState == Engine::STATE_LIVE)
	{
		SetPawRotation();
		m_pInfo[APART_BODY]->m_fAngle[Engine::ANGLE_Y]	+= D3DXToRadian(1.f);
	}

	TransformMatrix();

	return 0;
}

void CAirGun::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for(int iPART = 0; iPART < APART_END; ++iPART)
	{
		if( iPART == APART_BODY )
			m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_AirGunBody", m_pConvertVertex[iPART]);
		else if( iPART == APART_HEAD )
			m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_AirGunHead", m_pConvertVertex[iPART]);
		else if( iPART == APART_PAW )
			m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_AirGunPaw", m_pConvertVertex[iPART]);

		m_pDevice->SetTexture(0, NULL);	

		m_pBuffer[iPART]->Render();


		if( iPART == APART_BODY )
			m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_AirGunBody", m_pVertex[iPART]);
		else if( iPART == APART_HEAD )
			m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_AirGunHead", m_pVertex[iPART]);
		else if( iPART == APART_PAW )
			m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_AirGunPaw", m_pVertex[iPART]);
	}
}

CAirGun* CAirGun::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CAirGun* pObj = new CAirGun(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}

HRESULT CAirGun::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	for(int i = 0; i < APART_END; ++i)
	{
		pComponent = m_pInfo[i] = Engine::CTransform::Create(g_vLook);
		NULL_CHECK_RETURN(m_pInfo[i], E_FAIL);

		if(i != APART_BODY)
			continue;
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));
	}

	//TerrainCol
	pComponent = m_pTerrainCol = CTerrainCol::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"TerrainCol", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_AirGunBody");
	m_pBuffer[APART_BODY] = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer[APART_BODY], E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_AirGunHead");
	m_pBuffer[APART_HEAD] = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer[APART_HEAD], E_FAIL);

	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_AirGunPaw");
	m_pBuffer[APART_PAW] = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer[APART_PAW], E_FAIL);

	return S_OK;
}

void CAirGun::Release(void)
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);
	m_pInfoSubject->UnSubscribe(m_pPlayerObserver);

	for(int i = 0; i < APART_END; ++i)
	{
		Engine::Safe_Delete_Array(m_pVertex[i]);
		Engine::Safe_Delete_Array(m_pConvertVertex[i]);

		if(i != APART_BODY)
		{
			Engine::Safe_Delete(m_pBuffer[i]);
			Engine::Safe_Delete(m_pInfo[i]);
		}
	}
	Engine::Safe_Delete(m_pCameraObserver);
	Engine::Safe_Delete(m_pPlayerObserver);
}
void CAirGun::SetPawRotation(void)
{
	D3DXMATRIX	matRot;
	D3DXVECTOR3* vpPlayer		= m_pPlayerObserver->GetPos();

	D3DXVECTOR3 vPlayerToMonDir	= (*vpPlayer) - m_pInfo[APART_BODY]->m_vPos;
	vPlayerToMonDir	= D3DXVECTOR3(vPlayerToMonDir.x, 0.f, vPlayerToMonDir.z);
	D3DXVec3Normalize(&vPlayerToMonDir, &vPlayerToMonDir);

	//내적으로 각도를 구함
	float fAngle = acos(D3DXVec3Dot(&vPlayerToMonDir, &D3DXVECTOR3(0.f, 0.f, -1.f)));
	if(0.f < vPlayerToMonDir.x )		// 내적 예외처리
		fAngle = D3DX_PI * 2 - fAngle;

	m_pInfo[APART_HEAD]->m_fAngle[Engine::ANGLE_Y] = fAngle;

	//각도만큼 방향벡터 회전
	D3DXMatrixRotationY(&matRot, fAngle);
	D3DXVec3TransformNormal(&m_pInfo[APART_HEAD]->m_vDir, &D3DXVECTOR3(0.f, 0.f, -1.f), &matRot);
	D3DXVec3TransformNormal(&m_pInfo[APART_PAW]->m_vDir, &D3DXVECTOR3(0.f, 0.f, -1.f), &matRot);


	/* 포신 회전 */
	D3DXVECTOR3		vPToMDir = (*vpPlayer) - m_pInfo[APART_BODY]->m_vPos;
	vPToMDir = D3DXVECTOR3(vPToMDir.x, vPToMDir.y, 0.f);
	D3DXVec3Normalize(&vPToMDir, &vPToMDir);

	float fPawAngle = acos(D3DXVec3Dot(&vPToMDir, &D3DXVECTOR3(1.f, 0.f, 0.f)));
	//if( 0.f < vPToMDir.y)		// 내적 예외처리
	//	fPawAngle = D3DX_PI * 2 - fPawAngle;
	if(fPawAngle > D3DXToRadian(90.f))
		fPawAngle = D3DXToRadian(180.f) - fPawAngle;

	m_pInfo[APART_PAW]->m_fAngle[Engine::ANGLE_X] = fPawAngle;

}
void CAirGun::TransformMatrix(void)
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

	m_pInfo[APART_PAW]->m_matWorldAng = matRotz * matRoty * matRotx * m_pInfo[APART_PAW]->m_matWorld;

	for(int iPART = 0; iPART < APART_END; ++iPART )
	{
		D3DXMatrixScaling(&matScale, m_pInfo[iPART]->m_vScale.x, m_pInfo[iPART]->m_vScale.y, m_pInfo[iPART]->m_vScale.z);
		m_pInfo[iPART]->m_matWorld = matScale * m_pInfo[iPART]->m_matWorld;

		if(iPART == APART_PAW)
			m_pInfo[APART_PAW]->m_matWorldAng = matScale * m_pInfo[APART_PAW]->m_matWorldAng;

		for(size_t i = 0; i < m_dwVtxCnt[iPART]; ++i)
		{
			m_pConvertVertex[iPART][i] = m_pVertex[iPART][i];

			if(m_pInfo[APART_BODY]->m_eState == Engine::STATE_DIE)
				m_pConvertVertex[iPART][i].dwColor = D3DCOLOR_ARGB(255, 0, 0, 0);

			if(iPART == APART_PAW)
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


void CAirGun::SetParts(void)
{
	m_pInfo[APART_BODY]->m_vPos			= D3DXVECTOR3(0.f, 0.f, 0.f)	;
	m_pInfo[APART_BODY]->m_vScale		= D3DXVECTOR3(0.6f, 0.6f, 0.6f)	;
	m_pInfo[APART_BODY]->m_vDir			= D3DXVECTOR3(0.f, 0.f, -1.f)	;

	m_pInfo[APART_HEAD]->m_vPos			= D3DXVECTOR3(0.f, 0.f, 0.f)	;
	m_pInfo[APART_HEAD]->m_vScale		= D3DXVECTOR3(0.6f, 0.6f, 0.6f)	;
	m_pInfo[APART_HEAD]->m_vDir			= D3DXVECTOR3(0.f, 0.f, -1.f)	;

	m_pInfo[APART_PAW]->m_vPos			= D3DXVECTOR3(0.f, 0.f, 0.f)	;
	m_pInfo[APART_PAW]->m_vScale		= D3DXVECTOR3(0.5f, 0.5f, 0.5f)	;
	m_pInfo[APART_PAW]->m_vDir			= D3DXVECTOR3(0.f, 0.f, -1.f)	;

}

void CAirGun::CombinePart(void)
{
	D3DXVECTOR3 vDefaultScale = D3DXVECTOR3(1.f, 1.f, 1.f);
	D3DXMATRIX	matNoRotWorld, rRotX, rRotY, rRotZ;

	// 방공포 다리
	Engine::CPipeline::MakeWorldMatrix(&m_pInfo[APART_BODY]->m_matWorld, &vDefaultScale
		, m_pInfo[APART_BODY]->m_fAngle, &m_pInfo[APART_BODY]->m_vPos	);

	D3DXMatrixRotationX(&rRotX, -m_pInfo[APART_BODY]->m_fAngle[Engine::ANGLE_X]);
	D3DXMatrixRotationY(&rRotY, -m_pInfo[APART_BODY]->m_fAngle[Engine::ANGLE_Y]);
	D3DXMatrixRotationZ(&rRotZ, -m_pInfo[APART_BODY]->m_fAngle[Engine::ANGLE_Z]);

	matNoRotWorld = rRotX * rRotY * rRotZ;

	/************************************************************************/
	// 방공포대

	Engine::CPipeline::MakeWorldMatrix(&m_pInfo[APART_HEAD]->m_matWorld, &vDefaultScale
		, m_pInfo[APART_HEAD]->m_fAngle, &m_pInfo[APART_HEAD]->m_vPos	);
	m_pInfo[APART_HEAD]->m_matWorld *= matNoRotWorld * m_pInfo[APART_BODY]->m_matWorld;
	/************************************************************************/
	// 방공포

	Engine::CPipeline::MakeWorldMatrix(&m_pInfo[APART_PAW]->m_matWorld, &vDefaultScale
		, m_pInfo[APART_PAW]->m_fAngle, &m_pInfo[APART_PAW]->m_vPos	);
	m_pInfo[APART_PAW]->m_matWorld *= m_pInfo[APART_HEAD]->m_matWorld;
	/************************************************************************/
}

void CAirGun::BulletCreate(D3DXVECTOR3 _vDir)
{
	MAPOBJLIST::iterator	MonIter   = m_pMapObjlist->find(L"Player");

	Engine::CGameObject*	pBullet		= NULL;
	//생성
	pBullet		= CMissile::Create(m_pDevice);
	((CMissile*)pBullet)->SetList(&(MonIter->second));
	((CMissile*)pBullet)->Initialize();
	((CMissile*)pBullet)->SetPos(m_pInfo[APART_BODY]->m_vPos);
	((CMissile*)pBullet)->SetDir(_vDir);

	MAPOBJLIST::iterator iter = m_pMapObjlist->find(L"Mist");
	if(iter == m_pMapObjlist->end())
	{
		(*m_pMapObjlist)[L"Mist"] = OBJECTLIST();
	}
	((CMissile*)pBullet)->SetMistList(&(*m_pMapObjlist)[L"Mist"]);

	//푸쉬
	MAPOBJLIST::iterator	BulIter = m_pMapObjlist->find(L"Bullet");
	if(BulIter == m_pMapObjlist->end())
		(*m_pMapObjlist)[L"Bullet"] = OBJECTLIST();

	(*m_pMapObjlist)[L"Bullet"].push_back(pBullet);

}
void CAirGun::SetPos(const D3DXVECTOR3& rPos)
{
	m_pInfo[APART_BODY]->m_vPos = rPos;
}
void CAirGun::SetHP(float rHP)
{
	m_pInfo[APART_BODY]->m_fHP = rHP;
}
