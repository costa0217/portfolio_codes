#include "StdAfx.h"
#include "Fragment.h"

#include "Export_Function.h"
#include "Include.h"
#include "Transform.h"
#include "Texture.h"
#include "CameraObserver.h"
#include "MathMgr.h"
#include "TimeMgr.h"

CFragment::CFragment( LPDIRECT3DDEVICE9 pDevice )
:CEffect(pDevice)
, m_fTime(0.f)
{

}

CFragment::~CFragment( void )
{
	Engine::Safe_Delete_Array(m_pVertex_Frag);
	Engine::Safe_Delete_Array(m_pConvertVertex_Frag);
	Engine::Safe_Delete(m_pCameraObserver);
	//Release();
}

CFragment* CFragment::Create( LPDIRECT3DDEVICE9 pDevice )
{
	CFragment* pObj = new CFragment(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}

HRESULT CFragment::Initialize( void )
{
	FAILED_CHECK(AddComponent());
	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_dwVtxCnt = 8;
	m_pVertex_Frag = new Engine::VTXCUBE[m_dwVtxCnt];
	m_pConvertVertex_Frag = new Engine::VTXCUBE[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_CubeTex", m_pVertex_Frag);

	m_fSpeed = 30.f;

	m_vDir = D3DXVECTOR3(30.f,50.f,30.f);
	Engine::CMathMgr::MyRotationY(&m_vDir,&m_vDir,float(rand()%200));

	m_fAngleZ = 0.f;

	m_iFrameNum = rand()%14;

	return S_OK;
}

int CFragment::Update( void )
{
	Engine::CGameObject::Update();

	TransformFrag();
	Move();

	if(m_fDieTime >= 25.f)
	{
		m_pInfoSubject->UnSubscribe(m_pCameraObserver);
		return 1;
	}

	return 0;
}

void CFragment::Render( void )
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_CubeTex", m_pConvertVertex_Frag);

	m_pTexture->Render(m_iFrameNum);

	m_pBuffer->Render();

	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_CubeTex", m_pVertex_Frag);
}

void CFragment::Move( void )
{
	m_pInfo->m_vPos += m_vDir * m_pTimeMgr->GetTime();
	//ม฿ทย
	m_pInfo->m_vPos.y -= 0.98f * m_fTime * m_pTimeMgr->GetTime() * 7;
	m_fTime += 5.f * m_pTimeMgr->GetTime();

	m_fDieTime += m_pTimeMgr->GetTime() * 10.f;
	m_fAngleZ += m_pTimeMgr->GetTime() * 10.f;
}

HRESULT CFragment::AddComponent( void )
{
	Engine::CComponent*		pComponent = NULL;

	float fRand = rand()%20 * 0.1f;

	//Transform
	pComponent =  Engine::CTransform::Create(g_vLook);
	((Engine::CTransform*)pComponent)->SetScale(D3DXVECTOR3(fRand+0.2f,fRand+0.2f,fRand+0.2f));
	m_pInfo = (Engine::CTransform*)pComponent; 
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Texture_Fragment");
	m_pTexture = static_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_CubeTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	return S_OK;
}

void CFragment::TransformFrag()
{
	const D3DXMATRIX* pmatView = m_pCameraObserver->GetView();
	NULL_CHECK(pmatView);
	const D3DXMATRIX* pmatProj = m_pCameraObserver->GetProj();
	NULL_CHECK(pmatProj);

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVertex_Frag[i] = m_pVertex_Frag[i];

		Engine::CMathMgr::MyRotationZ(&m_pConvertVertex_Frag[i].vPos, &m_pConvertVertex_Frag[i].vPos, m_fAngleZ);
		Engine::CMathMgr::MyRotationY(&m_pConvertVertex_Frag[i].vPos, &m_pConvertVertex_Frag[i].vPos, m_fAngleZ);
		Engine::CMathMgr::MyRotationX(&m_pConvertVertex_Frag[i].vPos, &m_pConvertVertex_Frag[i].vPos, m_fAngleZ);

		D3DXVec3TransformCoord(&m_pConvertVertex_Frag[i].vPos, &m_pConvertVertex_Frag[i].vPos, &m_pInfo->m_matWorld);
		D3DXVec3TransformCoord(&m_pConvertVertex_Frag[i].vPos, &m_pConvertVertex_Frag[i].vPos, pmatView);
		if(m_pConvertVertex_Frag[i].vPos.z < 1.f)
			m_pConvertVertex_Frag[i].vPos.z = 1.f;

		D3DXVec3TransformCoord(&m_pConvertVertex_Frag[i].vPos, &m_pConvertVertex_Frag[i].vPos, pmatProj);
	}
}

