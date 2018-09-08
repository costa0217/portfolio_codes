#include "stdafx.h"
#include "Terrain.h"

#include "Include.h"
#include "Export_Function.h"
#include "Texture.h"
#include "CameraObserver.h"
#include "Transform.h"
#include "Pipeline.h"
#include "StaticObject.h"
#include "..\..\Engine\Resources\Code\TerrainTex.h"
#include "TreeRect.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourcesMgr())
, m_pInfoSubject(Engine::Get_InfoSubject())
, m_pVertex(NULL)
, m_pConvertVertex(NULL)
, m_dwVtxCnt(0)
, m_dwVtxCntX(0)
, m_dwVtxCntZ(0)
{

}

CTerrain::~CTerrain(void)
{
	Release();
}

const Engine::VTXTEX* CTerrain::GetTerrainVtx(void)
{
	return m_pVertex;
}

HRESULT CTerrain::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	((Engine::CTerrainTex*)m_pBuffer)->GetVtxCnt(&m_dwVtxCntX, &m_dwVtxCntZ);

	m_dwVtxCnt = m_dwVtxCntX * m_dwVtxCntZ;
	m_pVertex = new Engine::VTXTEX[m_dwVtxCnt];
	m_pConvertVertex = new Engine::VTXTEX[m_dwVtxCnt];

	m_pBuffer->GetVtxInfo(m_pVertex);

	if(m_eTerrainType == TERRAIN_FRONT)
	{
		m_pInfo->m_vPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	}
	else if(m_eTerrainType == TERRAIN_MID)
	{
		m_pInfo->m_vPos = D3DXVECTOR3(0.f, 0.f, float((m_dwVtxCntZ) * 8) - 8.f);
	
		for(int i = 0; i < 10; ++i)
		{
			CTreeRect* pTree = CTreeRect::Create(m_pDevice, TERRAIN_MID);
			pTree->SetPos(m_pInfo->m_vPos + D3DXVECTOR3(float(rand()%1790), 25.f, float(rand()%600)));
			pTree->SetScale(D3DXVECTOR3(430.f, 30.f, 10.f));
			m_vecTree.push_back(pTree);
		}

	}
	else if(m_eTerrainType == TERRAIN_BACK)
	{
		m_pInfo->m_vPos = D3DXVECTOR3(0.f, 0.f, float((m_dwVtxCntZ) * 50) - 100.f);

		for(int i = 0; i < 10; ++i)
		{
			CTreeRect* pTree = CTreeRect::Create(m_pDevice, TERRAIN_BACK);
			pTree->SetPos(m_pInfo->m_vPos + D3DXVECTOR3(float(rand()%1790), 25.f, float(rand()%600)));
			pTree->SetScale(D3DXVECTOR3(430.f, 30.f, 10.f));
			m_vecTree.push_back(pTree);
		}

	}
	else if(m_eTerrainType == TERRAIN_CLIFFLEFT) //절벽왼쪽
	{
		m_pInfo->m_vPos = D3DXVECTOR3(0.f,float((m_dwVtxCntX) * 32) - 32.f, 0.f/*float((m_dwVtxCntX) * 32)*/);
		m_pInfo->m_fAngle[Engine::ANGLE_Z] = -D3DX_PI/2.f;
	}
	else if(m_eTerrainType == TERRAIN_CLIFFRIGHT) //절벽오른쪽
	{
		m_pInfo->m_vPos = D3DXVECTOR3(1790.f, 0.f, 0.f);/*0.f, float(m_dwVtxCntX * 32));*/
		m_pInfo->m_fAngle[Engine::ANGLE_Z] = D3DX_PI/2.f;
	}
	else if(m_eTerrainType == TERRAIN_SKY) //배경
	{
		m_pInfo->m_vPos = D3DXVECTOR3(-700.f, 0.f, float(m_dwVtxCntZ * 700) - 150);
		m_pInfo->m_vScale = D3DXVECTOR3(1.8f, 1.8f, 1.f);
		m_pInfo->m_fAngle[Engine::ANGLE_X] = -D3DX_PI/2.f;		

		{
			CTreeRect* pTree = CTreeRect::Create(m_pDevice, TERRAIN_SKY);
			pTree->SetPos(m_pInfo->m_vPos - D3DXVECTOR3(-1600.f, -70.f, 80.f));
			pTree->SetScale(D3DXVECTOR3(850.f, 65.f, 10.f));
			m_vecTree.push_back(pTree);
		}
		{
			CTreeRect* pTree = CTreeRect::Create(m_pDevice, TERRAIN_FORESKY);
			pTree->SetPos(m_pInfo->m_vPos - D3DXVECTOR3(-1600.f, -50.f, 200.f));
			pTree->SetScale(D3DXVECTOR3(850.f, 65.f, 10.f));
			m_vecTree.push_back(pTree);
		}


	}

	m_pInfo->Update();

	return S_OK;
}

int CTerrain::Update(void)
{

	size_t s = m_vecTree.size(); 
	for(size_t i = 0; i < s; ++i)
		m_vecTree[i]->Update();

	TransformMatrix();

	return 0;
	
}

void CTerrain::Render(void)
{
	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
//	m_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pBuffer->SetVtxInfo(m_pConvertVertex);

	m_pTexture->Render(0);
	m_pBuffer->Render();

	size_t s = m_vecTree.size();
	for(size_t i = 0; i < s; ++i)
		m_vecTree[i]->Render();

	m_pBuffer->SetVtxInfo(m_pVertex);



}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pDevice, TERRAINTYPE eType)
{
	CTerrain* pObj = new CTerrain(pDevice);

	pObj->SetTerrainType(eType);

	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}

HRESULT CTerrain::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Texture

	if(m_eTerrainType == TERRAIN_SKY)
	{
		if(StageNum == 1)
		{
			pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Texture_Sky");
			m_pTexture = static_cast<Engine::CTexture*>(pComponent);
			NULL_CHECK_RETURN(m_pTexture, E_FAIL);
			m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));
		}
		else if(StageNum == 2)
		{
			pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Texture_Sky2");
			m_pTexture = static_cast<Engine::CTexture*>(pComponent);
			NULL_CHECK_RETURN(m_pTexture, E_FAIL);
			m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));
		}
	}
	else if(m_eTerrainType == TERRAIN_FRONT)
	{
		if(StageNum == 1)
		{
		pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Texture_TerrainFront");
		m_pTexture = static_cast<Engine::CTexture*>(pComponent);
		NULL_CHECK_RETURN(m_pTexture, E_FAIL);
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));
		}
		else if(StageNum == 2)
		{
			pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Texture_TerrainFront2");
			m_pTexture = static_cast<Engine::CTexture*>(pComponent);
			NULL_CHECK_RETURN(m_pTexture, E_FAIL);
			m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));
		}
	}
	else if(m_eTerrainType == TERRAIN_MID)
	{
		if(StageNum == 1)
		{
			pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Texture_TerrainMid");
			m_pTexture = static_cast<Engine::CTexture*>(pComponent);
			NULL_CHECK_RETURN(m_pTexture, E_FAIL);
			m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));
		}
		else if(StageNum == 2)
		{
			pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Texture_TerrainMid2");
			m_pTexture = static_cast<Engine::CTexture*>(pComponent);
			NULL_CHECK_RETURN(m_pTexture, E_FAIL);
			m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));
		}
	}
	else if(m_eTerrainType == TERRAIN_BACK)
	{
		if(StageNum == 1)
		{
			pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Texture_TerrainBack");
			m_pTexture = static_cast<Engine::CTexture*>(pComponent);
			NULL_CHECK_RETURN(m_pTexture, E_FAIL);
			m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));
		}
		else if(StageNum == 2)
		{
			pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Texture_TerrainBack2");
			m_pTexture = static_cast<Engine::CTexture*>(pComponent);
			NULL_CHECK_RETURN(m_pTexture, E_FAIL);
			m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));
		}
	}
	else if(m_eTerrainType == TERRAIN_CLIFFLEFT)
	{
		pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Texture_TerrainCliff_Left");
		m_pTexture = static_cast<Engine::CTexture*>(pComponent);
		NULL_CHECK_RETURN(m_pTexture, E_FAIL);
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));
	}
	else if(m_eTerrainType == TERRAIN_CLIFFRIGHT)
	{
		pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Texture_TerrainCliff_Right");
		m_pTexture = static_cast<Engine::CTexture*>(pComponent);
		NULL_CHECK_RETURN(m_pTexture, E_FAIL);
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));
	}

	//Buffer
	if(m_eTerrainType == TERRAIN_FRONT)
	{
		pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_TerrainTex");
		m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
		NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));
	}
	else if(m_eTerrainType == TERRAIN_MID)
	{
		pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_TerrainTex_Mid");
		m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
		NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));
	}
	else if(m_eTerrainType == TERRAIN_BACK)
	{
		pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_TerrainTex_Back");
		m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
		NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));
	}
	else if(m_eTerrainType == TERRAIN_CLIFFLEFT)
	{
		pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_TerrainTex_Cliff");
		m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
		NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));
	}
	else if(m_eTerrainType == TERRAIN_CLIFFRIGHT)
	{
		pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_TerrainTex_Cliff");
		m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
		NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));
	}
	else if(m_eTerrainType == TERRAIN_SKY)
	{
		pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_TerrainTex_Sky");
		m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
		NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));
	}


	return S_OK;
}

void CTerrain::Release(void)
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);

	Engine::Safe_Delete_Array(m_pVertex);
	Engine::Safe_Delete_Array(m_pConvertVertex);

	Engine::Safe_Delete(m_pCameraObserver);

	size_t s = m_vecTree.size();
	for(size_t i = 0; i < s; ++i)
		Engine::Safe_Delete(m_vecTree[i]);
	m_vecTree.clear();

}

void CTerrain::TransformMatrix(void)
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

void CTerrain::SetTerrainType(TERRAINTYPE _eTerrainType)
{
	m_eTerrainType = _eTerrainType;
}
