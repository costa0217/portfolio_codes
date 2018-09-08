#include "stdafx.h"
#include "Hook.h"

#include "Export_Function.h"
#include "Include.h"

#include "Pipeline.h"
#include "Transform.h"
#include "CameraObserver.h"

#include "TimeMgr.h"
#include "MathMgr.h"

CHook::CHook( LPDIRECT3DDEVICE9 pDevice )
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourcesMgr())
, m_pTimeMgr(Engine::Get_TimeMgr())
, m_pInfoSubject(Engine::Get_InfoSubject())
, m_pInfo(NULL)
, m_pTexture(NULL)
, m_pBuffer(NULL)
, m_pCameraObserver(NULL)
, m_fAngle(0)
, m_pParentInfo(NULL)
, m_pFreight_HookList(NULL)
, m_bHang(false)
{

}

CHook::~CHook( void )
{
	Release();
}

HRESULT CHook::Initialize( void )
{
	FAILED_CHECK(AddComponent());
	m_pInfo->m_vPos = D3DXVECTOR3(0.8f,-2.2f,0.f);
	m_pInfo->m_vScale = D3DXVECTOR3(0.5f, 0.5f, 0.5f);

	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_dwVtxCnt = m_pBuffer->GetVtxCnt();
	m_pVertex			= new Engine::VTXCOL[m_dwVtxCnt];
	m_pConvertVertex	= new Engine::VTXCOL[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_Hook", m_pVertex);

	return S_OK;
}

int CHook::Update( void )
{

	Engine::CGameObject::Update();
	
	if(m_pParentInfo != NULL)
	{
		m_pInfo->m_matWorld = m_pInfo->m_matWorld * m_pParentInfo->m_matWorldNoSca;
		m_pInfo->m_matWorldNoSca = m_pInfo->m_matWorldNoSca * m_pParentInfo->m_matWorldNoSca;
	}


	TransformMatrix();

	if(m_pFreight_HookList != NULL && m_pParentInfo != NULL)
	{
		D3DXVECTOR3 vPos;
		float fLength;
		//화물 훅을 검사하는 코드가들어가야함.
		OBJECTLIST::iterator iter = m_pFreight_HookList->begin();
		OBJECTLIST::iterator iterEnd = m_pFreight_HookList->end();

		for(;iter != iterEnd; ++iter)
		{	
			
			vPos = ((CHook*)(*iter))->GetPos() - this->GetParentPos();
			fLength = D3DXVec3Length(&vPos);
			if(fLength < 10.f)
			{
				if(Hang == false && ((CHook*)(*iter))->GetbHang() == false)
				{
					Hang = true;
					((CHook*)(*iter))->SetHang(true);
					((CHook*)(*iter))->SetParentInfo(this->m_pParentInfo);
					((CHook*)(*iter))->SetPos(D3DXVECTOR3(1.f,-10.f,0.f));
				}
			}
				
		}
	}

	return 0;
}

void CHook::Render( void )
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_Hook", m_pConvertVertex);
	m_pDevice->SetTexture(0, NULL);
	m_pBuffer->Render();

	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_Hook", m_pVertex);
}


HRESULT CHook::AddComponent( void )
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent =  Engine::CTransform::Create(g_vLook);
	//((Engine::CTransform*)pComponent)->SetScale(D3DXVECTOR3(0.3f,0.3f,0.3f));
	m_pInfo = (Engine::CTransform*)pComponent; 
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));


	//Buffer
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_Hook");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	return S_OK;
}

CHook* CHook::Create( LPDIRECT3DDEVICE9 pDevice )
{
	CHook* pObj = new CHook(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}

void CHook::TransformMatrix( void )
{
	const D3DXMATRIX* pmatView = m_pCameraObserver->GetView();
	NULL_CHECK(pmatView);
	const D3DXMATRIX* pmatProj = m_pCameraObserver->GetProj();
	NULL_CHECK(pmatProj);

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVertex[i] = m_pVertex[i];
		
		Engine::CMathMgr::MyRotationZ(&m_pConvertVertex[i].vPos,&m_pConvertVertex[i].vPos,m_fAngle);

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, &m_pInfo->m_matWorld);
		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatView);
		if(m_pConvertVertex[i].vPos.z < 1.f)
			m_pConvertVertex[i].vPos.z = 1.f;

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatProj);
	}
}

void CHook::Release( void )
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);

	Engine::Safe_Delete_Array(m_pVertex);
	Engine::Safe_Delete_Array(m_pConvertVertex);

	Engine::Safe_Delete(m_pCameraObserver);
}

void CHook::SetPos( D3DXVECTOR3 vPos )
{
	m_pInfo->m_vPos = vPos;
}

D3DXVECTOR3 CHook::GetParentPos()
{
	return m_pParentInfo->m_vPos;
}

D3DXVECTOR3 CHook::GetPos()
{
	return m_pInfo->m_vPos;
}
