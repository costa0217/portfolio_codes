#include "stdafx.h"
#include "Bomb.h"

#include "Export_Function.h"
#include "Include.h"

#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "Pipeline.h"
#include "CollisionMgr.h"
#include "Stage.h"

CBomb::CBomb( LPDIRECT3DDEVICE9 pDevice )
:CBullet(pDevice)
, m_bFragmentCreate(NULL)
{

}

CBomb::~CBomb( void )
{
	Release();
}

HRESULT CBomb::Initialize( void )
{
	m_eBulletName	= BUL_BOMB;
	m_fDamage		= 100.f;

	FAILED_CHECK(AddComponent());

	m_pInfo->m_vScale = D3DXVECTOR3(0.5f, 0.5f, 0.5f);

	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_dwVtxCnt			= m_pBuffer->GetVtxCnt();
	m_pVertex			= new Engine::VTXCOL[m_dwVtxCnt];
	m_pConvertVertex	= new Engine::VTXCOL[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_Bomb", m_pVertex);

	m_fSpeed = 30.f;
	m_fAngleZ = 0.f;
	m_vDir = D3DXVECTOR3(0.f,-1.f,0.f);

	return S_OK;
}

int CBomb::Update( void )
{
	Engine::CGameObject::Update();

	TransformMatrix();
	Move();

	m_pTerrainVertex = m_pManagement->GetTerrainVtx(Engine::LAYER_GAMELOGIC, L"Terrain");
	if(m_pInfo->m_vPos.y - m_pTerrainVertex->vPos.y < 3)
	{
		m_pInfoSubject->UnSubscribe(m_pCameraObserver);

		if(m_bFragmentCreate != NULL)
			*m_bFragmentCreate = true;
		return 1;
	}
	return 0;
}

void CBomb::Render( void )
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_Bomb", m_pConvertVertex);

	m_pDevice->SetTexture(0, NULL);
	m_pBuffer->Render();

	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_Bomb", m_pVertex);
}

CBomb* CBomb::Create( LPDIRECT3DDEVICE9 pDevice )
{
	CBomb* pObj = new CBomb(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}

HRESULT CBomb::AddComponent( void )
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));


	//Buffer
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_Bomb");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	return S_OK;
}

void CBomb::Move( void )
{
	m_pInfo->m_vPos += m_vDir * m_fSpeed * m_pTimeMgr->GetTime();
}
