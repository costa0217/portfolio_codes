#include "stdafx.h"
#include "GatlingGun.h"

#include "Export_Function.h"
#include "Include.h"

#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "PlayerObserver.h"
#include "Pipeline.h"
#include "CollisionMgr.h"

CGatlingGun::CGatlingGun( LPDIRECT3DDEVICE9 pDevice )
:CBarrel(pDevice)
{

}

CGatlingGun::~CGatlingGun( void )
{
	Release();
}

HRESULT CGatlingGun::Initialize( void )
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vPos		= D3DXVECTOR3(0.f,-10.f,0.f);
	m_pInfo->m_vScale	= D3DXVECTOR3(1.2f, 1.2f, 1.2f);

	m_pCameraObserver	= CCameraObserver::Create();
	m_pPlayerObserver	= CPlayerObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);
	m_pInfoSubject->Subscribe(m_pPlayerObserver);

	m_dwVtxCnt = m_pBuffer->GetVtxCnt();
	m_pVertex = new Engine::VTXCOL[m_dwVtxCnt];
	m_pConvertVertex = new Engine::VTXCOL[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_GatlingGun", m_pVertex);

	m_fSpeed = 50.f;
	m_fAngleZ = 0.f;

	return S_OK;
}

int CGatlingGun::Update( void )
{
	Engine::CGameObject::Update();

	if(m_pTarget == NULL)
	{
		m_fAngleX = 0.f;
		m_fAngleY = 0.f;
		m_fAngleZ = 0.f;

		m_pInfo->m_matWorld = m_pInfo->m_matWorld * (*m_pParentMatrix);
	}
	else
	{
		Move();
		m_pInfo->m_matWorld = m_pInfo->m_matWorld * (*m_pNoRotParMatrix);
	}

	TransformMatrix();

	return 0;
}

void CGatlingGun::Render( void )
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_GatlingGun", m_pConvertVertex);
	m_pDevice->SetTexture(0, NULL);
	m_pBuffer->Render();

	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_GatlingGun", m_pVertex);
}

CGatlingGun* CGatlingGun::Create( LPDIRECT3DDEVICE9 pDevice )
{
	CGatlingGun* pObj = new CGatlingGun(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}

HRESULT CGatlingGun::AddComponent( void )
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	((Engine::CTransform*)pComponent)->SetScale(D3DXVECTOR3(0.5f,0.5f,0.5f));
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));


	//Buffer
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_GatlingGun");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	return S_OK;
}

void CGatlingGun::Move( void )
{
	D3DXVECTOR3		vTargetPos	= ((const Engine::CTransform*)m_pTarget->GetComponent(L"Transform"))->m_vPos ;

	D3DXVECTOR3	vTToGDirXY, vTToGDirXZ;
	D3DXVECTOR3 vTToGDir = vTargetPos - (*m_pPlayerObserver->GetPos());


	vTToGDirXY	= D3DXVECTOR3(vTToGDir.x, 0.f, vTToGDir.z);
	D3DXVec3Normalize(&vTToGDirXY, &vTToGDirXY);

	//내적으로 각도를 구함
	float fAngleXY = acos(D3DXVec3Dot(&vTToGDirXY, &D3DXVECTOR3(0.f, 0.f, -1.f)));
	if(0.f > vTToGDirXY.x )		// 내적 예외처리
		fAngleXY = D3DX_PI * 2 - fAngleXY;

	m_fAngleY = fAngleXY;

	vTToGDirXZ = D3DXVECTOR3(vTToGDir.x, vTToGDir.y, 0.f);
	D3DXVec3Normalize(&vTToGDirXZ, &vTToGDirXZ);

	float fAngleXZ = acos(D3DXVec3Dot(&vTToGDirXZ, &D3DXVECTOR3(1.f, 0.f, 0.f)));
	//if( 0.f < vTToGDirXZ.y)		// 내적 예외처리
	//	fAngleXZ = D3DX_PI * 2 - fAngleXZ;
	if(fAngleXZ > D3DXToRadian(90.f))
		fAngleXZ = D3DXToRadian(180.f) - fAngleXZ;

	m_fAngleX = fAngleXZ;
}
