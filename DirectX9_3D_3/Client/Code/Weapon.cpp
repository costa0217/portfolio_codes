#include "stdafx.h"
#include "Weapon.h"

#include "Export_Function.h"
#include "Include.h"

#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "PlayerObserver.h"
#include "Pipeline.h"
#include "CollisionMgr.h"
#include "MathMgr.h"
#include "Bullet.h"
#include "BasicBullet.h"
#include "Player.h"
#include "MultiBullet.h"
#include "Missile.h"

CWeapon::CWeapon(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourcesMgr())
, m_pTimeMgr(Engine::Get_TimeMgr())
, m_pInfoSubject(Engine::Get_InfoSubject())
, m_pManagement(Engine::Get_Management())
, m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_pInfo(NULL)
, m_pBuffer(NULL)
, m_pCameraObserver(NULL)
, m_pPlayerObserver(NULL)
, m_pTerrainVertex(NULL)
, m_fAngleX(0.f)
, m_fAngleZ(0.f)
, m_fAngleY(0.f)
, m_bDie(false)
, m_pTarget(NULL)
, m_pPlayer(NULL)
, m_fBulletTime(0.f)
, m_fFireSpeed(0.f)
{

}

CWeapon::~CWeapon(void)
{
	Release();
}

HRESULT CWeapon::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vPos		= D3DXVECTOR3(0.f,-10.f, 0.f);
	m_vMyPos			= D3DXVECTOR3(0.f,-10.f,-5.f);
	m_pInfo->m_vScale	= D3DXVECTOR3(0.7f, 0.7f, 0.7f);

	m_pCameraObserver	= CCameraObserver::Create();
	m_pPlayerObserver	= CPlayerObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);
	m_pInfoSubject->Subscribe(m_pPlayerObserver);

	m_dwVtxCnt			= m_pBuffer->GetVtxCnt();
	m_pVertex			= new Engine::VTXCOL[m_dwVtxCnt];
	m_pConvertVertex	= new Engine::VTXCOL[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_Gun", m_pVertex);

	//m_fSpeed	= 2.f;
	m_fAngleZ	= 0.f;

	return S_OK;
}

int CWeapon::Update(void)
{
	Engine::CGameObject::Update();

	D3DXVECTOR3	vDir = D3DXVECTOR3(0.f, 0.f, 0.f);	// 타겟의 방향
	FindTarget(vDir);

	if(m_eType == WEAPON_GUN)
		m_fFireSpeed = 0.25f;
	else if(m_eType == WEAPON_GATLELING)
		m_fFireSpeed = 0.175f;
	else if(m_eType == WEAPON_LAUNCHER)
		m_fFireSpeed = 0.5f;

	if(m_pPlayerObserver->GetState() == Engine::STATE_LIVE)
	{
		if(m_pTarget == NULL)
		{
			m_pInfo->m_matWorld = m_pInfo->m_matWorld * (*m_pParentMatrix);
			D3DXVec3TransformCoord(&m_vMyPos, &m_pInfo->m_vPos, m_pParentMatrix);

			m_fAngleX = 0.f;
			m_fAngleY = 0.f;
			m_fAngleZ = 0.f;

			m_pInfo->m_vPos = D3DXVECTOR3(0.f,-7.f,0.f);		
		}
		else
		{
			m_pInfo->m_matWorld = m_pInfo->m_matWorld * (*m_pNoRotParMatrix);		
			D3DXVec3TransformCoord(&m_vMyPos, &m_pInfo->m_vPos, m_pNoRotParMatrix);

			Move();

			//기본미사일생성
			m_fBulletTime -= m_pTimeMgr->GetTime() * 0.5f;
			if(m_fBulletTime < 0)
			{
				m_fBulletTime = 0.5f;

				if(m_eType == WEAPON_GUN)
					BasicBulletCreate(vDir);
				else if(m_eType == WEAPON_GATLELING)
					MultiBulletCreate(vDir);
				else if(m_eType == WEAPON_LAUNCHER)
					MissileCreate(vDir);
			}
			m_pInfo->m_vPos = D3DXVECTOR3(0.f,-10.f, -2.f);
		}
	}

	TransformMatrix();
	

	return 0;
}

void CWeapon::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, m_wstrBufferName, m_pConvertVertex);
	m_pDevice->SetTexture(0, NULL);
	m_pBuffer->Render();

	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, m_wstrBufferName, m_pVertex);
}

CWeapon* CWeapon::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CWeapon* pObj = new CWeapon(pDevice);

	return pObj;
}

HRESULT CWeapon::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_Gun");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	return S_OK;
}

void CWeapon::TransformMatrix(void)
{
	const D3DXMATRIX* pmatView = m_pCameraObserver->GetView();
	NULL_CHECK(pmatView);
	const D3DXMATRIX* pmatProj = m_pCameraObserver->GetProj();
	NULL_CHECK(pmatProj);

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVertex[i] = m_pVertex[i];

		Engine::CMathMgr::MyRotationZ(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, m_fAngleZ);
		Engine::CMathMgr::MyRotationX(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, m_fAngleX);
		Engine::CMathMgr::MyRotationY(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, m_fAngleY);

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, &m_pInfo->m_matWorld);
		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatView);
		if(m_pConvertVertex[i].vPos.z < 1.f)
			m_pConvertVertex[i].vPos.z = 1.f;

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatProj);
	}
}	

void CWeapon::Move(void)
{
	D3DXVECTOR3		vTargetPos	= ((const Engine::CTransform*)m_pTarget->GetComponent(L"Transform"))->m_vPos ;

	D3DXVECTOR3	vTToGDirXY, vXZ;
	D3DXVECTOR3 vTToGDir = vTargetPos - (*m_pPlayerObserver->GetPos());

	vTToGDirXY	= D3DXVECTOR3(vTToGDir.x, 0.f, vTToGDir.z);
	D3DXVec3Normalize(&vTToGDirXY, &vTToGDirXY);

	//내적으로 각도를 구함
	float fAngleXY = acos(D3DXVec3Dot(&vTToGDirXY, &D3DXVECTOR3(0.f, 0.f, -1.f)));
	if(0.f < vTToGDirXY.x )		// 내적 예외처리
		fAngleXY = D3DX_PI * 2 - fAngleXY;

	m_fAngleY = fAngleXY;

	vXZ = D3DXVECTOR3(vTToGDir.x, vTToGDir.y, 0.f);
	D3DXVec3Normalize(&vXZ, &vXZ);

	float fAngleXZ = acos(D3DXVec3Dot(&vXZ, &D3DXVECTOR3(1.f, 0.f, 0.f)));

	if(fAngleXZ > D3DXToRadian(90.f))
		fAngleXZ = D3DXToRadian(180.f) - fAngleXZ;

	m_fAngleX = -fAngleXZ;
}

void CWeapon::Release()
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);
	m_pInfoSubject->UnSubscribe(m_pPlayerObserver);

	Engine::Safe_Delete_Array(m_pVertex);
	Engine::Safe_Delete_Array(m_pConvertVertex);

	Engine::Safe_Delete(m_pCameraObserver);
	Engine::Safe_Delete(m_pPlayerObserver);
}

void CWeapon::SetPos( D3DXVECTOR3 vPos )
{
	m_pInfo->m_vPos = vPos;
}

void CWeapon::BasicBulletCreate(D3DXVECTOR3& vDir)
{

	Engine::Get_GraphicDev()->SoundPlay(SOUND_GUN); 

	MAPOBJLIST::iterator	MonIter   = m_pMapObjlist->find(L"Monster");	// MonIter->second => MonsterList

	//생성
	m_pBullet		= CBasicBullet::Create(m_pDevice);
	((CBullet*)m_pBullet)->SetList(&(MonIter->second));
	((CBullet*)m_pBullet)->Initialize();
	((CBullet*)m_pBullet)->SetPos(m_vMyPos);
	((CBullet*)m_pBullet)->SetDir(vDir);

	((CPlayer*)m_pPlayer)->SetBulltCnt( ((CPlayer*)m_pPlayer)->GetBulltCnt() - 2.f); // 보유총알 갯수 감소

	//푸쉬
	MAPOBJLIST::iterator	BulIter = m_pMapObjlist->find(L"Bullet");
	if(BulIter == m_pMapObjlist->end())
		(*m_pMapObjlist)[L"Bullet"] = OBJECTLIST();

	(*m_pMapObjlist)[L"Bullet"].push_back(m_pBullet);

}

void CWeapon::MultiBulletCreate(D3DXVECTOR3& vDir)
{
	MAPOBJLIST::iterator	MonIter   = m_pMapObjlist->find(L"Monster");	// MonIter->second => MonsterList

	//생성
	m_pBullet		= CMultiBullet::Create(m_pDevice);
	((CBullet*)m_pBullet)->SetList(&(MonIter->second));
	((CBullet*)m_pBullet)->Initialize();
	((CBullet*)m_pBullet)->SetPos(m_vMyPos);
	((CBullet*)m_pBullet)->SetDir(vDir);

	((CPlayer*)m_pPlayer)->SetBulltCnt( ((CPlayer*)m_pPlayer)->GetBulltCnt() - 2.f); // 보유총알 갯수 감소

	//푸쉬
	MAPOBJLIST::iterator	BulIter = m_pMapObjlist->find(L"Bullet");
	if(BulIter == m_pMapObjlist->end())
		(*m_pMapObjlist)[L"Bullet"] = OBJECTLIST();

	(*m_pMapObjlist)[L"Bullet"].push_back(m_pBullet);
}

void CWeapon::MissileCreate(D3DXVECTOR3& vDir)
{
	MAPOBJLIST::iterator	MonIter   = m_pMapObjlist->find(L"Monster");	// MonIter->second => MonsterList

	//생성
	m_pBullet		= CMissile::Create(m_pDevice);
	((CBullet*)m_pBullet)->SetList(&(MonIter->second));
	((CBullet*)m_pBullet)->Initialize();
	((CBullet*)m_pBullet)->SetPos(m_vMyPos);
	((CBullet*)m_pBullet)->SetDir(vDir);
	
	MAPOBJLIST::iterator iter = m_pMapObjlist->find(L"Mist");
	if(iter == m_pMapObjlist->end())
	{
		(*m_pMapObjlist)[L"Mist"] = OBJECTLIST();
	}
	((CMissile*)m_pBullet)->SetMistList(&(*m_pMapObjlist)[L"Mist"]);
	

	((CPlayer*)m_pPlayer)->SetBulltCnt( ((CPlayer*)m_pPlayer)->GetBulltCnt() - 2.f); // 보유총알 갯수 감소

	//푸쉬
	MAPOBJLIST::iterator	BulIter = m_pMapObjlist->find(L"Bullet");
	if(BulIter == m_pMapObjlist->end())
		(*m_pMapObjlist)[L"Bullet"] = OBJECTLIST();

	(*m_pMapObjlist)[L"Bullet"].push_back(m_pBullet);
}

void CWeapon::FindTarget(D3DXVECTOR3& vDir)
{
	MAPOBJLIST::iterator	iter = m_pMapObjlist->find(L"Monster");

	float	fMinDist = 150.f;						// 공격범위

	for(OBJECTLIST::iterator iterObj = iter->second.begin(); iterObj != iter->second.end(); ++iterObj)
	{
		D3DXVECTOR3 MonPos		= ((const Engine::CTransform*)(*iterObj)->GetComponent(L"Transform"))->m_vPos ;

		if( ((const Engine::CTransform*)(*iterObj)->GetComponent(L"Transform"))->m_eState == Engine::STATE_DIE)
			continue;

		D3DXVECTOR3 vMtoPDir	= MonPos - m_vMyPos;				// 방향
		float fLength			= D3DXVec3Length(&vMtoPDir);		// 거리

		D3DXVec3Normalize(&vMtoPDir, &vMtoPDir);

		if(fLength < fMinDist)	// 최단거리 구하기
		{
			fMinDist	= fLength;
			m_pTarget	= (*iterObj);
			vDir		= vMtoPDir;
		}
	}
	if(fMinDist == 150.f)
	{
		m_pTarget = NULL;
		return;
	}
}

void CWeapon::ChangeBuffer(wchar_t* BufferName)
{
	if(m_CurrentBuffer == BufferName )
		return ;

	m_CurrentBuffer = BufferName;

	MAPCOMPONENT::iterator iter = m_mapComponent.find(L"Buffer");
	Engine::Safe_Delete(iter->second);
	m_pBuffer = NULL;
	m_mapComponent.erase(iter);

	Engine::Safe_Delete_Array(m_pVertex);
	Engine::Safe_Delete_Array(m_pConvertVertex);
	//

	//새버퍼추가
	m_wstrBufferName = BufferName;

	Engine::CComponent*		pComponent = NULL;
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, m_wstrBufferName);
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	m_dwVtxCnt			= m_pBuffer->GetVtxCnt();
	m_pVertex			= new Engine::VTXCOL[m_dwVtxCnt];
	m_pConvertVertex	= new Engine::VTXCOL[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, m_wstrBufferName , m_pVertex);
}