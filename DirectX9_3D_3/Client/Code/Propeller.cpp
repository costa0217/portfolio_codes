#include "stdafx.h"
#include "Propeller.h"


#include "Export_Function.h"
#include "Include.h"

#include "Pipeline.h"
#include "Transform.h"
#include "CameraObserver.h"

#include "TimeMgr.h"
#include "MathMgr.h"

CPropeller::CPropeller(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourcesMgr())
, m_pTimeMgr(Engine::Get_TimeMgr())
, m_pInfoSubject(Engine::Get_InfoSubject())
, m_pInfo(NULL)
, m_pTexture(NULL)
, m_pBuffer(NULL)
, m_pCameraObserver(NULL)
, m_fAngle(0)
, m_bHeliDie(false)
, m_fTime(0.f)
{

}

CPropeller::~CPropeller(void)
{
	Release();
}

HRESULT CPropeller::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vPos = D3DXVECTOR3(0.f,14.f,0.f);
	m_pInfo->m_vScale = D3DXVECTOR3(1.2f, 1.2f, 1.2f);

	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_dwVtxCnt = m_pBuffer->GetVtxCnt();
	m_pVertex			= new Engine::VTXCOL[m_dwVtxCnt];
	m_pConvertVertex	= new Engine::VTXCOL[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_Propeller", m_pVertex);

	m_vMoveDir = D3DXVECTOR3(30.f,30.f,30.f);
	Engine::CMathMgr::MyRotationY(&m_vMoveDir,&m_vMoveDir,float(rand()%200));

	return S_OK;
}

int CPropeller::Update(void)
{
	Move();
	Engine::CGameObject::Update();
	if(m_bHeliDie == false)
	{
		m_pInfo->m_matWorld = m_pInfo->m_matWorld * (*m_pParentMatrix);
	}
	else if(m_bHeliDie == true)
	{
		m_pInfo->m_vScale = D3DXVECTOR3(0.3f, 0.3f, 0.3f);
	}
	TransformMatrix();

	return 0;
}

void CPropeller::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_Propeller", m_pConvertVertex);
	m_pDevice->SetTexture(0, NULL);
	m_pBuffer->Render();

	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_Propeller", m_pVertex);
}

void CPropeller::Move(void)
{
	if(m_bHeliDie == false)
	{
		m_fAngle += 20.f * m_pTimeMgr->GetTime() * (*m_fHeliPosY)/2.f;
		m_pInfo->m_fAngle[Engine::ANGLE_Y] = m_fAngle;
	}
	else if(m_bHeliDie == true)
	{
		m_pInfo->m_vPos += m_vMoveDir * m_pTimeMgr->GetTime();
		//ม฿ทย
		m_pInfo->m_vPos.y -= 0.98f * m_fTime * m_pTimeMgr->GetTime() * 7;
		m_fTime += 5.f * m_pTimeMgr->GetTime();
	}
}

HRESULT CPropeller::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent =  Engine::CTransform::Create(g_vLook);
	//((Engine::CTransform*)pComponent)->SetScale(D3DXVECTOR3(0.7f,0.7f,0.7f));
	m_pInfo = (Engine::CTransform*)pComponent; 
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));


	//Buffer
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_Propeller");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	return S_OK;
}	

CPropeller* CPropeller::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPropeller* pObj = new CPropeller(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}

void CPropeller::TransformMatrix(void)
{
	const D3DXMATRIX* pmatView = m_pCameraObserver->GetView();
	NULL_CHECK(pmatView);
	const D3DXMATRIX* pmatProj = m_pCameraObserver->GetProj();
	NULL_CHECK(pmatProj);

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVertex[i] = m_pVertex[i];

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, &m_pInfo->m_matWorld);
		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatView);
		if(m_pConvertVertex[i].vPos.z < 1.f)
			m_pConvertVertex[i].vPos.z = 1.f;

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatProj);
	}
}

void CPropeller::Release(void)
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);

	Engine::Safe_Delete_Array(m_pVertex);
	Engine::Safe_Delete_Array(m_pConvertVertex);

	Engine::Safe_Delete(m_pCameraObserver);
}

void CPropeller::SetPos( D3DXVECTOR3 vPos )
{
	m_pInfo->m_vPos = vPos;
}
