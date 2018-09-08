#include "stdafx.h"
#include "Launcher.h"

#include "Export_Function.h"
#include "Include.h"

#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "PlayerObserver.h"
#include "Pipeline.h"
#include "CollisionMgr.h"

CLauncher::CLauncher( LPDIRECT3DDEVICE9 pDevice )
:CBarrel(pDevice)
{

}

CLauncher::~CLauncher( void )
{
	Release();
}

HRESULT CLauncher::Initialize( void )
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vPos		= D3DXVECTOR3(0.f,-10.f,0.f);
	m_pInfo->m_vScale	= D3DXVECTOR3(0.7f, 0.7f, 0.7f);

	m_pCameraObserver	= CCameraObserver::Create();
	m_pPlayerObserver	= CPlayerObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);
	m_pInfoSubject->Subscribe(m_pPlayerObserver);

	m_dwVtxCnt = m_pBuffer->GetVtxCnt();
	m_pVertex = new Engine::VTXCOL[m_dwVtxCnt];
	m_pConvertVertex = new Engine::VTXCOL[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_Launcher", m_pVertex);

	m_fSpeed	= 2.f;
	m_fAngleZ	= 0.f;

	return S_OK;
}

int CLauncher::Update( void )
{
	Engine::CGameObject::Update();

	if(m_pTarget == NULL)
	{
		m_fAngleX = 0.f;
		m_fAngleY = 0.f;
		m_fAngleZ = 0.f;

		m_pInfo->m_vPos = D3DXVECTOR3(0.f,-7.f,0.f);

		m_pInfo->m_matWorld = m_pInfo->m_matWorld * (*m_pParentMatrix);
	}
	else
	{
		Move();

		m_pInfo->m_vPos = D3DXVECTOR3(0.f,-10.f, -2.f);

		m_pInfo->m_matWorld = m_pInfo->m_matWorld * (*m_pNoRotParMatrix);		
	}


	TransformMatrix();

	return 0;
}

void CLauncher::Render( void )
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_Launcher", m_pConvertVertex);
	m_pDevice->SetTexture(0, NULL);
	m_pBuffer->Render();

	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_Launcher", m_pVertex);
}

CLauncher* CLauncher::Create( LPDIRECT3DDEVICE9 pDevice )
{
	CLauncher* pObj = new CLauncher(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}

HRESULT CLauncher::AddComponent( void )
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	((Engine::CTransform*)pComponent)->SetScale(D3DXVECTOR3(0.5f,0.5f,0.5f));
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));


	//Buffer
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_Launcher");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	return S_OK;
}

void CLauncher::Move( void )
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
	//if( 0.f < vXZ.y)		// 내적 예외처리
	//	fAngleXZ = D3DX_PI * 2 - fAngleXZ;
	if(fAngleXZ > D3DXToRadian(90.f))
		fAngleXZ = D3DXToRadian(180.f) - fAngleXZ;

	m_fAngleX = -fAngleXZ;
}
