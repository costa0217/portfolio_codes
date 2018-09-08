#include "stdafx.h"
#include "TreeRect.h"

#include "Export_Function.h"
#include "Include.h"

#include "Pipeline.h"
#include "MathMgr.h"
#include "CameraObserver.h"
#include "Texture.h"


CTreeRect::CTreeRect(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourcesMgr())
, m_pBuffer(NULL)
, m_pTexture(NULL)
, m_pResource(NULL)
, m_dwVtxCnt(0)
, m_vPos(D3DXVECTOR3(0.f, 0.f, 0.f))
, m_vScale(D3DXVECTOR3(1.f, 1.f, 1.f))
, m_pCameraObserver(NULL)
, m_pInfoSubject(Engine::Get_InfoSubject())
, m_iTextureNum(0)
{
	D3DXMatrixIdentity(&m_matWorld);
	ZeroMemory(m_fAngle, sizeof(float) * Engine::ANGLE_END);
}

CTreeRect::~CTreeRect(void)
{
	Release();
}

HRESULT CTreeRect::Initialize(void)
{
	AddComponent();

	m_dwVtxCnt = 4;
	m_pVertex = new Engine::VTXTEX[m_dwVtxCnt];
	m_pConvertVertex = new Engine::VTXTEX[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex", m_pVertex);

	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

//	m_iTextureNum = rand()%2; 

	return S_OK;
}
int CTreeRect::Update(void)
{
	D3DXMatrixIdentity(&m_matWorld);

	Engine::CPipeline::MakeWorldMatrix(&m_matWorld, &m_vScale
		, m_fAngle, &m_vPos);

	return 0;
}
void CTreeRect::Render(void)
{
//	TransformMatrix();
	const D3DXMATRIX* pmatView = m_pCameraObserver->GetView();
	NULL_CHECK(pmatView);
	const D3DXMATRIX* pmatProj = m_pCameraObserver->GetProj();
	NULL_CHECK(pmatProj);

	m_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pDevice->SetTransform(D3DTS_VIEW, pmatView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, pmatProj);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex", m_pConvertVertex);

	m_pTexture->Render(m_iTextureNum);
	m_pBuffer->Render();

	//m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex", m_pVertex);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	D3DXMATRIX	matWorld;
	D3DXMatrixIdentity(&matWorld);

	m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
	m_pDevice->SetTransform(D3DTS_VIEW, &matWorld);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &matWorld);

}	

void CTreeRect::TransformMatrix(void)
{
	const D3DXMATRIX* pmatView = m_pCameraObserver->GetView();
	NULL_CHECK(pmatView);
	const D3DXMATRIX* pmatProj = m_pCameraObserver->GetProj();
	NULL_CHECK(pmatProj);

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVertex[i] = m_pVertex[i];

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, &m_matWorld);
		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatView);
		if(m_pConvertVertex[i].vPos.z < 1.f)
			m_pConvertVertex[i].vPos.z = 1.f;

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatProj);
	}
}

CTreeRect* CTreeRect::Create(LPDIRECT3DDEVICE9 pDevice, TERRAINTYPE _eTerrainType)
{
	CTreeRect* pObj = new CTreeRect(pDevice);
	pObj->SetTerrainType(_eTerrainType);

	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;	
}
void CTreeRect::Release(void)
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);

	Engine::Safe_Delete(m_pCameraObserver);

	Engine::Safe_Delete_Array(m_pVertex);
	Engine::Safe_Delete_Array(m_pConvertVertex);

	Engine::Safe_Delete(m_pBuffer);
	Engine::Safe_Delete(m_pTexture);
}

void CTreeRect::AddComponent(void)
{

	if (m_eTerrainType == TERRAIN_BACK)
	{
		m_pResource	= m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Texture_TreeFog");	
		m_pTexture	= dynamic_cast<Engine::CTexture*>(m_pResource);
	}
	else if (m_eTerrainType == TERRAIN_SKY)
	{
		m_pResource	= m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Texture_BigTree");	
		m_pTexture	= dynamic_cast<Engine::CTexture*>(m_pResource);
	}
	else if(m_eTerrainType == TERRAIN_FORESKY)
	{
		m_pResource	= m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Texture_SwapBigTree");	
		m_pTexture	= dynamic_cast<Engine::CTexture*>(m_pResource);
	}
	else
	{
		m_pResource	= m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Texture_Tree");	
		m_pTexture	= dynamic_cast<Engine::CTexture*>(m_pResource);
	}

	m_pResource	= m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex");	
	m_pBuffer	= dynamic_cast<Engine::CVIBuffer*>(m_pResource);
}

void CTreeRect::SetPos(const D3DXVECTOR3& rPos)
{
	m_vPos	= rPos;	
}

void CTreeRect::SetScale(const D3DXVECTOR3& rScale)
{
	m_vScale	= rScale;
}

void CTreeRect::SetAngle(float fAngle[3])
{
	for(int i = 0; i < Engine::ANGLE_END; ++i)
		m_fAngle[i] = fAngle[i];
}

void CTreeRect::GetPos(D3DXVECTOR3* pPos)
{
	*pPos = m_vPos;
}

void CTreeRect::GetScale(D3DXVECTOR3* pScale)
{
	*pScale = m_vScale;
}

void CTreeRect::GetAngle(float* fAngle)
{
	for(int i = 0; i < Engine::ANGLE_END; ++i)
		fAngle[i] = m_fAngle[i];
}

void CTreeRect::SetWorldMatrix(const D3DXMATRIX& rMatrix)
{
	m_matWorld = rMatrix;
}

D3DXMATRIX CTreeRect::GetWorldMatrix(void)
{
	return m_matWorld;
}

void CTreeRect::SetTerrainType(TERRAINTYPE _eTerrainType)
{
	m_eTerrainType = _eTerrainType;
}
