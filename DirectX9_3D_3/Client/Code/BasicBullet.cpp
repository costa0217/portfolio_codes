
#include "stdafx.h"
#include "BasicBullet.h"

#include "Export_Function.h"
#include "Include.h"

#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "Pipeline.h"
#include "CollisionMgr.h"
#include "BulletCol.h"

CBasicBullet::CBasicBullet( LPDIRECT3DDEVICE9 pDevice )
: CBullet(pDevice)
, m_fDeleteTime(0.f)
{

}

CBasicBullet::~CBasicBullet( void )
{
	Release();
}

HRESULT CBasicBullet::Initialize( void )
{
	m_eBulletName	= BUL_BASIC;
	m_fDamage		= 3.f;

	FAILED_CHECK(AddComponent());

	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_dwVtxCnt			= m_pBuffer->GetVtxCnt();
	m_pVertex			= new Engine::VTXCOL[m_dwVtxCnt];
	m_pConvertVertex	= new Engine::VTXCOL[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_Bullet", m_pVertex);

	m_fSpeed	= 20.f;
	m_fAccel	= 0.f;

	m_fAngleZ	= 0.f;
	m_vDir		= D3DXVECTOR3(0.f, -1.f, 0.f);

	return S_OK;
}

int CBasicBullet::Update( void )
{
	m_fDeleteTime += m_pTimeMgr->GetInstance()->GetTime();

	if(m_fDeleteTime > 4.f)
		return 1;

	Engine::CGameObject::Update();

	TransformMatrix();
	Move();
	if(m_pBulletCol->GetbCollision())
		return 1;
	return 0;
}

void CBasicBullet::Render( void )
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_Bullet", m_pConvertVertex);

	m_pDevice->SetTexture(0, NULL);
	m_pBuffer->Render();

	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_Bullet", m_pVertex);
}

CBasicBullet* CBasicBullet::Create( LPDIRECT3DDEVICE9 pDevice)
{
	CBasicBullet* pObj = new CBasicBullet(pDevice);

	return pObj;
}	

HRESULT CBasicBullet::AddComponent( void )
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	((Engine::CTransform*)pComponent)->SetScale(D3DXVECTOR3(0.5f, 0.5f, 0.5f));
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));
	
	//BulletCol
	pComponent = m_pBulletCol = CBulletCol::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	((CBulletCol*)pComponent)->SetColInfo(&m_pInfo->m_vPos, m_fDamage, m_pMonsterList);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"BulletCol", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_Bullet");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	return S_OK;
}

void CBasicBullet::Move( void )
{
	m_fAccel += m_pTimeMgr->GetTime();
	m_pInfo->m_vPos += m_vDir * m_fSpeed * m_fAccel; 


	/* 총알 방향 전환 */
	D3DXVECTOR3 vRotZDir = D3DXVECTOR3(m_vDir.x, m_vDir.y, 0.f);
	D3DXVec3Normalize(&vRotZDir, &vRotZDir);

	float fAngleXY = acos(D3DXVec3Dot(&vRotZDir, &D3DXVECTOR3(0.f, 1.f, 0.f)));

	if(vRotZDir.x >= 0.f)
		fAngleXY = D3DX_PI * 2 - fAngleXY;
	
	m_fAngleZ	= fAngleXY;

	// 총알 근처에서 발사시 앞으로 발사되게하는 코드
	if( (m_fAngleZ  > 4.5f && m_fAngleZ < 4.9f) 
		|| (m_fAngleZ  > 1.4f && m_fAngleZ < 1.6f))
	{
		m_fAngleZ	= 0.f;
		m_fAngleX	= D3DXToRadian(90.f);
	}
}