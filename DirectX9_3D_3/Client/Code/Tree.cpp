#include "stdafx.h"
#include "Tree.h"

#include "Export_Function.h"
#include "Include.h"

#include "Pipeline.h"
#include "Transform.h"
#include "Texture.h"
#include "CameraObserver.h"

#include "TimeMgr.h"
#include "MathMgr.h"

CTree::CTree( LPDIRECT3DDEVICE9 pDevice )
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourcesMgr())
, m_pTimeMgr(Engine::Get_TimeMgr())
, m_pInfoSubject(Engine::Get_InfoSubject())
, m_pInfo(NULL)
, m_pTexture(NULL)
, m_pBuffer(NULL)
, m_pCameraObserver(NULL)
{

	D3DXMatrixIdentity(&m_matWorld);
	ZeroMemory(m_fAngle, sizeof(float) * Engine::ANGLE_END);
}

CTree::~CTree( void )
{
	Release();
}

HRESULT CTree::Initialize( void )
{
	FAILED_CHECK(AddComponent());
	
	//
	m_pInfo->m_vPos = D3DXVECTOR3(1300.f,5.f,118.f);
	//m_wstrBufferName = L"Buffer_TallTree";
	//

	m_pInfo->m_vScale = D3DXVECTOR3(1.2f, 1.2f, 1.2f);

	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_dwVtxCnt = m_pBuffer->GetVtxCnt();
	m_pVertex			= new Engine::VTXCOL[m_dwVtxCnt];
	m_pConvertVertex	= new Engine::VTXCOL[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, m_wstrBufferName, m_pVertex);

	return S_OK;
}

int CTree::Update( void )
{
	Engine::CGameObject::Update(); // 0812
	TransformMatrix(); // 0812

	return 0;
}	

void CTree::Render( void )
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, m_wstrBufferName, m_pConvertVertex);
	
	m_pDevice->SetTexture(0, NULL);
	m_pBuffer->Render();

	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, m_wstrBufferName, m_pVertex);
}

HRESULT CTree::AddComponent( void )
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent =  Engine::CTransform::Create(g_vLook);
	//((Engine::CTransform*)pComponent)->SetScale(D3DXVECTOR3(0.7f,0.7f,0.7f));
	m_pInfo = (Engine::CTransform*)pComponent; 
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));


	//Buffer
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, m_wstrBufferName);
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	return S_OK;
}

CTree* CTree::Create( LPDIRECT3DDEVICE9 pDevice, wstring wstrBufferName )
{
	CTree* pObj = new CTree(pDevice);
	pObj->SetBufferName(wstrBufferName);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}

void CTree::TransformMatrix( void )
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

void CTree::Release( void )
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);

	Engine::Safe_Delete_Array(m_pVertex);
	Engine::Safe_Delete_Array(m_pConvertVertex);

	Engine::Safe_Delete(m_pCameraObserver);
}

void CTree::SetPos( D3DXVECTOR3 vPos ) // 0812
{
	m_pInfo->m_vPos = vPos;
}

void CTree::SetScale(const D3DXVECTOR3& rScale)
{
	m_vScale	= rScale;
}

void CTree::SetAngle(float fAngle[3])
{
	for(int i = 0; i < Engine::ANGLE_END; ++i)
		m_fAngle[i] = fAngle[i];
}

void CTree::GetPos(D3DXVECTOR3* pPos)
{
	*pPos = m_vPos;
}

D3DXVECTOR3 CTree::GetPos(void)
{
	return m_pInfo->m_vPos;
}

void CTree::GetScale(D3DXVECTOR3* pScale)
{
	*pScale = m_vScale;
}

void CTree::GetAngle(float* fAngle)
{
	for(int i = 0; i < Engine::ANGLE_END; ++i)
		fAngle[i] = m_fAngle[i];
}

void CTree::SetWorldMatrix(const D3DXMATRIX& rMatrix)
{
	m_matWorld = rMatrix;
}

D3DXMATRIX CTree::GetWorldMatrix(void)
{
	return m_matWorld;
}
