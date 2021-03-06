#include "stdafx.h"
#include "PlayButton.h"

#include "Export_Function.h"
#include "Include.h"
#include "SceneSelector.h"

#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "Pipeline.h"
#include "CollisionMgr.h"
#include "MouseCol.h"
#include "KeyMgr.h"

CPlayButton::CPlayButton(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourcesMgr())
, m_pTimeMgr(Engine::Get_TimeMgr())
, m_pInfoSubject(Engine::Get_InfoSubject())
, m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_pKeyMgr(Engine::Get_KeyMgr())
, m_pTexture(NULL)
, m_pBuffer(NULL)
, m_pCameraObserver(NULL)
, m_dwVtxCnt(0)
{

}

CPlayButton::~CPlayButton(void)
{
	Release();
}

HRESULT CPlayButton::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vPos = D3DXVECTOR3(0.790f, -0.768f, -0.5f);
	m_pInfo->m_vScale = D3DXVECTOR3(0.0930f, 0.247f, 0.f);

	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_dwVtxCnt = 4;
	m_pVertex = new Engine::VTXTEX[m_dwVtxCnt];
	m_pConvertVertex = new Engine::VTXTEX[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex2", m_pVertex);

	m_iButton = 0;
	m_bPlay = false;

	return S_OK;
}

int CPlayButton::Update(void)
{
	MouseOver();

	Engine::CGameObject::Update();

	TransformMatrix();
	

	return 0;
}

void CPlayButton::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex2", m_pConvertVertex);

	m_pTexture->Render(m_iButton);
	m_pBuffer->Render();

	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex2", m_pVertex);
}

CPlayButton* CPlayButton::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayButton* pObj = new CPlayButton(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}

HRESULT CPlayButton::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;	

	//Transform
	pComponent =  Engine::CTransform::Create(g_vLook);
	//((Engine::CTransform*)pComponent)->SetScale(D3DXVECTOR3(0.0930f, 0.247f, 0.f));
	m_pInfo = (Engine::CTransform*)pComponent; 
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Texture_Play");
	m_pTexture = static_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex2");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	return S_OK;
}

void CPlayButton::Release(void)
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);

	Engine::Safe_Delete_Array(m_pVertex);
	Engine::Safe_Delete_Array(m_pConvertVertex);

	Engine::Safe_Delete(m_pCameraObserver);
}

void CPlayButton::TransformMatrix(void)
{
	const D3DXMATRIX* pmatView	= m_pCameraObserver->GetView();
	NULL_CHECK(pmatView);

	const D3DXMATRIX* pmatOrtho	= m_pCameraObserver->GetOrtho();
	NULL_CHECK(pmatOrtho);

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVertex[i] = m_pVertex[i];

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, &m_pInfo->m_matWorld);
		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatView);
		if(m_pConvertVertex[i].vPos.z < 1.f)
			m_pConvertVertex[i].vPos.z = 1.f;

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatOrtho);
	}
}

void CPlayButton::MouseOver(void)
{
	Engine::Get_GraphicDev()->SoundPlay(SOUND_BUTTON); 

	POINT ptMouse = CMouseCol::GetMousePos();

	D3DXVECTOR3 vMousePos;
	vMousePos.x = (float)ptMouse.x;
	vMousePos.y = (float)ptMouse.y;
	vMousePos.z = 0.f;

	
	if(vMousePos.x >= 730 && vMousePos.x <=810 
		&&vMousePos.y >=500 && vMousePos.y <=580)
	{
		m_iButton = 1;
		m_pInfo->m_vPos = D3DXVECTOR3(0.790f, -0.783f, -0.5f);

		if(m_pKeyMgr->KeyPressed(KEY_LBUTTON, 1))
		{
			m_bPlay = true ; 
			//Engine::Get_Management()->SceneChange(CSceneSelector(SCENE_STAGE));
		}
	}
	else
	{
		m_iButton = 0;
		m_pInfo->m_vPos = D3DXVECTOR3(0.790f, -0.768f, -0.5f);
	}
}

bool CPlayButton::GetBool(void)
{
	return m_bPlay;
}