#include "stdafx.h"
#include "Man.h"

#include "Export_Function.h"

#include "Texture.h"
#include "CameraObserver.h"
#include "PlayerObserver.h"

#include "Pipeline.h"
#include "TerrainCol.h"
#include "CollisionMgr.h"
#include "Stage.h"
#include "Transform.h"
#include "Player.h"
#include "Flare.h"
#include "Effect.h"
#include "Mist.h"

#define DEF_FRAMESPEED	10.f

CMan::CMan(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourcesMgr())
, m_pTimeMgr(Engine::Get_TimeMgr())
, m_pInfoSubject(Engine::Get_InfoSubject())
, m_pManagement(Engine::Get_Management())
, m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_pCameraObserver(NULL)
, m_pPlayerObserver(NULL)
, m_dwVtxCnt(0)
, m_fTime(0.f)
, m_fAngle(0.f)
, m_bArrive(false)
, m_bBoard(false)
, m_bGoal(false)
, m_fCircleAngle(0.f)
, m_iRotDir(1)
, m_fSpeed(0.f)
, m_fDeadTime(0.f)
, m_pPlayer(NULL)
, m_iIndexNum(0)
, m_bFlareCreate(true)
, m_fFlareTime(0.f)

{
	ZeroMemory(m_pBuffer	, sizeof(Engine::CVIBuffer*) * PART_END );
	ZeroMemory(m_pTexture	, sizeof(Engine::CTexture*) * PART_END);
	ZeroMemory(m_iFrame		, sizeof(int) * PART_END);
	ZeroMemory(m_iFrameCnt	, sizeof(int) * PART_END);

}

CMan::~CMan(void)
{
	Release();
}

CMan* CMan::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMan* pObj = new CMan(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;
}

HRESULT CMan::Initialize(void)
{
	m_iIndexNum = rand()%5;

	switch(m_iIndexNum)
	{
	case 0:
		m_wstrBufferName = L"SoldierOne";
		break;
	case 1:
		m_wstrBufferName = L"SoldierTwo";
		break;
	case 2:
		m_wstrBufferName = L"SoldierThree";
		break;
	case 3:
		m_wstrBufferName = L"SoldierFour";
		break;
	case 4:
		m_wstrBufferName = L"SoldierFive";
		break;

	}
	FAILED_CHECK(AddComponent());

	m_pCameraObserver = CCameraObserver::Create();
	m_pPlayerObserver = CPlayerObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);
	m_pInfoSubject->Subscribe(m_pPlayerObserver);


	for(int iPART = 0; iPART < PART_END; ++iPART)
	{
		m_dwVtxCnt		= 8;
		m_pVertex[iPART]		= new Engine::VTXCUBE[m_dwVtxCnt];
		m_pConvertVertex[iPART]	= new Engine::VTXCUBE[m_dwVtxCnt];

		m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_CubeTex", m_pVertex[iPART]);
	}

	m_fSpeed	= rand()%15 + 5.f ;
	SetParts();	//
	SetAnimation();

	return S_OK;
}

int CMan::Update(void)
{	
	m_fTime += m_pTimeMgr->GetTime();

	DWORD dwTerrainVtxCntX, dwTerrainVtxCntZ;

	m_pResourceMgr->GetTerrainVtxCnt(Engine::RESOURCE_DYNAMIC, L"Buffer_TerrainTex", &dwTerrainVtxCntX, &dwTerrainVtxCntZ);
	m_pTerrainVertex = m_pManagement->GetTerrainVtx(Engine::LAYER_GAMELOGIC, L"Terrain");
	m_pTerrainCol->SetColInfo(&m_pInfo[PART_HART]->m_vPos, m_pTerrainVertex, dwTerrainVtxCntX, dwTerrainVtxCntZ);


	D3DXVECTOR3		vPlayerPos = (*m_pPlayerObserver->GetPos());

	if(!m_bBoard && !m_bGoal)
	{
		vPlayerPos = D3DXVECTOR3(vPlayerPos.x, 0.f, vPlayerPos.z);
		

		if( D3DXVec3Length( & ( vPlayerPos - m_pInfo[PART_HART]->m_vPos )) < 100.f )
		{
			m_fFlareTime -= 2.f * m_pTimeMgr->GetTime();
			if(m_fFlareTime < 0 && m_bFlareCreate)
			{
				m_fFlareTime = 2.f;
				MAPOBJLIST::iterator iter = m_pMapObjlist->find(L"Flare");
				if(iter == m_pMapObjlist->end())
				{
					(*m_pMapObjlist)[L"Flare"] = OBJECTLIST();
				}

				CGameObject* pFlare = CFlare::Create(m_pDevice);
				((CEffect*)pFlare)->SetPos(m_pInfo[PART_HART]->m_vPos);
				(*m_pMapObjlist)[L"Flare"].push_back(pFlare);

			}

			if(D3DXVec3Length( & ( vPlayerPos - m_pInfo[PART_HART]->m_vPos )) < 40.f)
				//중지하자.
				m_bFlareCreate = false;
				//
			SetDirMove();		// 방향 및 이동
			FrameMove();		// 애니메이션 
			if(D3DXVec3Length( & ( (*m_pPlayerObserver->GetPos()) - m_pInfo[PART_HART]->m_vPos )) < 10.f 
				&& abs(m_pPlayerObserver->GetPos()->y - m_pInfo[PART_HART]->m_vPos.y) < 5.f
				&& m_pPlayerObserver->GetState() == Engine::STATE_LIVE 
				&& ((CPlayer*)m_pPlayer)->GetBoardNum() < ((CPlayer*)m_pPlayer)->GetMAXBoardNum() )
			{
				m_bBoard = true;
				m_pInfo[PART_HART]->m_vPos		= D3DXVECTOR3(0.f, 0.f, 0.f);

				((CPlayer*)m_pPlayer)->SetBoardNum();
			}
		}
		else
		{
			ZeroMemory(m_pInfo[PART_HART]->m_fAngle, sizeof(float) * 3);
			m_pInfo[PART_HART]->m_vDir = D3DXVECTOR3(0.f, 0.f, -1.f);
			m_bBoard	= false;			
		}
	}
	m_pInfo[PART_HART]->m_vPos.y -= 0.98f * m_pTimeMgr->GetTime() * 7;

	CombineParts();

	Engine::CGameObject::Update();

	if(m_bBoard)
	{
		m_pInfo[PART_HART]->m_matWorld *= (*m_pPlayerObserver->GetWorld());
		//for(int i = 0; i < PART_END; ++i)
		//{
		//	//m_pInfo[i]->m_matWorld *= m_
		//}

		if((*m_pPlayerObserver->GetPos()).x >= 1275.f && (*m_pPlayerObserver->GetPos()).x <= 1305.f
			&& (*m_pPlayerObserver->GetPos()).y == 5.f)
		{
			m_bBoard = false;
			m_pInfo[PART_HART]->m_vPos = D3DXVECTOR3(float(rand()%7) + (*m_pPlayerObserver->GetPos()).x, 0.f, float(rand()%7) + 112.f);
			m_bGoal	 = true;
		}
	}

	if(m_bGoal)
	{
		FrameMove();
		m_pInfo[PART_HART]->m_fAngle[Engine::ANGLE_Y] = D3DXToRadian(-90.f);
		m_pInfo[PART_HART]->m_vPos += D3DXVECTOR3(1.f, 0.f, 0.f) * m_pTimeMgr->GetTime() * (m_fSpeed / 2.f);

		m_fDeadTime += m_pTimeMgr->GetTime();

		if(m_fDeadTime > 5.f)
			return 1;
	}
	TransformMatrix();

	return 0;
}

void CMan::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for(int iPART = 1; iPART < PART_END; ++iPART )
	{
		m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_CubeTex", m_pConvertVertex[iPART]);

		int iDraw = 2;
		if( iPART == PART_BODY)
			iDraw = 1;
		if( iPART == PART_LEFTARM || iPART == PART_RIGHTARM )
			iDraw = 0;
		if( iPART == PART_RIGHTLEG || iPART == PART_LEFTLEG || iPART == PART_LOWER)
			iDraw = 3;

		m_pTexture[iPART]->Render(iDraw);
		m_pBuffer[iPART]->Render();

		m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_CubeTex", m_pVertex[iPART]);
	}
}

void CMan::Release(void)
{

	m_pInfoSubject->UnSubscribe(m_pCameraObserver);
	m_pInfoSubject->UnSubscribe(m_pPlayerObserver);

	for(int i = 0; i < PART_END; ++i)
	{
		Engine::Safe_Delete_Array(m_pVertex[i]);
		Engine::Safe_Delete_Array(m_pConvertVertex[i]);

		if(i != PART_HART)
		{
			Engine::Safe_Delete(m_pBuffer[i]);
			Engine::Safe_Delete(m_pTexture[i]);
			Engine::Safe_Delete(m_pInfo[i]);
		}
		size_t vecSize = m_LerpVec[i].size();
		for(size_t k = 0; k < vecSize; ++k)
		{
			Engine::Safe_Delete(m_LerpVec[i][k]);
		}
	}	
	Engine::Safe_Delete(m_pCameraObserver);
	Engine::Safe_Delete(m_pPlayerObserver);
}

void CMan::SetDirMove(void)
{
	if(!m_bArrive)
	{
		D3DXVECTOR3	vToPlayerPos = (*m_pPlayerObserver->GetPos());

		if(D3DXVec3Length(&vToPlayerPos) == 0)
			return; 

		vToPlayerPos = D3DXVECTOR3(vToPlayerPos.x, 0.f, vToPlayerPos.z);

		D3DXVECTOR3		vToPlayerDir = vToPlayerPos - m_pInfo[PART_HART]->m_vPos;
		D3DXVec3Normalize(&vToPlayerDir, &vToPlayerDir);
		vToPlayerDir = D3DXVECTOR3(vToPlayerDir.x, 0.f, vToPlayerDir.z);

		m_pInfo[PART_HART]->m_vDir = vToPlayerDir;

		m_fAngle = acos(D3DXVec3Dot(&vToPlayerDir, &D3DXVECTOR3(0.f, 0.f, -1.f)));
		if(m_pInfo[PART_HART]->m_vPos.x < vToPlayerPos.x)
			m_fAngle = D3DX_PI * 2 - m_fAngle;

		m_pInfo[PART_HART]->m_fAngle[Engine::ANGLE_Y] = m_fAngle;
		m_pInfo[PART_HART]->m_vPos += m_pInfo[PART_HART]->m_vDir * m_pTimeMgr->GetTime() * m_fSpeed;


		D3DXVECTOR3		vNoYHartPos = D3DXVECTOR3(m_pInfo[PART_HART]->m_vPos.x, 0.f, m_pInfo[PART_HART]->m_vPos.z);
		if( D3DXVec3Length( & (vToPlayerPos - vNoYHartPos)) < 5.f)
		{
			m_pInfo[PART_HART]->m_vDir.x < 0.f ? m_iRotDir = 1 : m_iRotDir = -1;
			m_bArrive = true;
		}
	}
	else
	{
		D3DXMATRIX matRotY;
		if(m_fTime > 0.2f)
		{
			m_fCircleAngle	-= D3DXToRadian(10.f) * m_iRotDir;
			m_fTime	 = 0.f;
		}
		D3DXMatrixRotationY(&matRotY, m_fCircleAngle);		
		D3DXVec3TransformNormal(&m_pInfo[PART_HART]->m_vDir, &D3DXVECTOR3(0.f, 0.f, 1.f), &matRotY);
		D3DXVec3Normalize(&m_pInfo[PART_HART]->m_vDir, &m_pInfo[PART_HART]->m_vDir);

		m_fAngle = acos(D3DXVec3Dot(&m_pInfo[PART_HART]->m_vDir, &D3DXVECTOR3(0.f, 0.f, -1.f)));
		if( 0.f < m_pInfo[PART_HART]->m_vDir.x)
			m_fAngle = D3DX_PI * 2 - m_fAngle;

		m_pInfo[PART_HART]->m_fAngle[Engine::ANGLE_Y] =  m_fAngle;
		m_pInfo[PART_HART]->m_vPos += m_pInfo[PART_HART]->m_vDir * m_pTimeMgr->GetTime() * m_fSpeed;

		if(abs(m_fCircleAngle) >= D3DXToRadian(270.f))
		{
			m_bArrive = false;
			m_fCircleAngle = 0.f;
		}
	}
}
void CMan::FrameMove(void)
{
	for(int iPartNum = 1; iPartNum < PART_END; ++iPartNum)
	{
		if(m_LerpVec[iPartNum].empty())
			continue; 

		if( m_iFrame[iPartNum] + 1 != m_LerpVec[iPartNum].size())
		{
			m_pInfo[iPartNum]->m_vPos += (m_LerpVec[iPartNum][m_iFrame[iPartNum] + 1]->vPos - m_LerpVec[iPartNum][m_iFrame[iPartNum]]->vPos) / DEF_FRAMESPEED;

			for(int i = 0; i < 3; ++i )
				m_pInfo[iPartNum]->m_fAngle[i] += (m_LerpVec[iPartNum][m_iFrame[iPartNum] + 1]->fAngle[i] - m_LerpVec[iPartNum][m_iFrame[iPartNum]]->fAngle[i] )/ DEF_FRAMESPEED;


			if(DEF_FRAMESPEED == m_iFrameCnt[iPartNum])
			{
				++m_iFrame[iPartNum];
				m_iFrameCnt[iPartNum] = 0.f;
			}
			m_iFrameCnt[iPartNum] += 1.f;
		}
		else
			m_iFrame[iPartNum] = 0;

	}
}
HRESULT CMan::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	for(int i = 0; i < PART_END; ++i)
	{
		pComponent = m_pInfo[i] = Engine::CTransform::Create(g_vLook);
		NULL_CHECK_RETURN(m_pInfo[i], E_FAIL);

		if(i != PART_HART)
			continue;
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent)); 
	}

	//TerrainCol
	pComponent = m_pTerrainCol = CTerrainCol::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"TerrainCol", pComponent));

	//Texture
	for(int i = 0; i < PART_END; ++i)
	{
		pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, m_wstrBufferName);
		m_pTexture[i] = static_cast<Engine::CTexture*>(pComponent);
		NULL_CHECK_RETURN(m_pTexture[i], E_FAIL);

		if(i != PART_HART)
			continue;
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));
	}

	//Buffer
	for(int i = 0; i < PART_END; ++i)
	{
		pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_CubeTex");
		m_pBuffer[i] = dynamic_cast<Engine::CVIBuffer*>(pComponent);
		NULL_CHECK_RETURN(m_pBuffer[i], E_FAIL);

		if(i != PART_HART)
			continue;
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));
	}

	return S_OK;
}

void CMan::TransformMatrix(void)
{
	const D3DXMATRIX* pmatView = m_pCameraObserver->GetView();
	NULL_CHECK(pmatView);
	const D3DXMATRIX* pmatProj = m_pCameraObserver->GetProj();
	NULL_CHECK(pmatProj);

	D3DXMATRIX matScale, matParent;

	for(int iPART = 0; iPART < PART_END; ++iPART )
	{
		D3DXMatrixScaling(&matScale, m_pInfo[iPART]->m_vScale.x, m_pInfo[iPART]->m_vScale.y, m_pInfo[iPART]->m_vScale.z);
		m_pInfo[iPART]->m_matWorld = matScale * m_pInfo[iPART]->m_matWorld;

		for(size_t iVtxNum = 0; iVtxNum < m_dwVtxCnt; ++iVtxNum)
		{
			m_pConvertVertex[iPART][iVtxNum] = m_pVertex[iPART][iVtxNum];

			D3DXVec3TransformCoord(&m_pConvertVertex[iPART][iVtxNum].vPos, &m_pConvertVertex[iPART][iVtxNum].vPos, &m_pInfo[iPART]->m_matWorld);

			D3DXVec3TransformCoord(&m_pConvertVertex[iPART][iVtxNum].vPos, &m_pConvertVertex[iPART][iVtxNum].vPos, pmatView);
			if(m_pConvertVertex[iPART][iVtxNum].vPos.z < 1.f)
				m_pConvertVertex[iPART][iVtxNum].vPos.z = 1.f;

			D3DXVec3TransformCoord(&m_pConvertVertex[iPART][iVtxNum].vPos, &m_pConvertVertex[iPART][iVtxNum].vPos, pmatProj);
		}
	}
}

void CMan::CombineParts(void)
{

	D3DXMATRIX	matInvScale;
	D3DXVECTOR3 vDefaultScale = D3DXVECTOR3(1.f, 1.f, 1.f);

	// 심장
	Engine::CPipeline::MakeWorldMatrix(&m_pInfo[PART_HART]->m_matWorld, &vDefaultScale
		, m_pInfo[PART_HART]->m_fAngle, &m_pInfo[PART_HART]->m_vPos	);


	/************************************************************************/
	// 몸체

	Engine::CPipeline::MakeWorldMatrix(&m_pInfo[PART_BODY]->m_matWorld, &vDefaultScale
		, m_pInfo[PART_BODY]->m_fAngle, &m_pInfo[PART_BODY]->m_vPos	);
	m_pInfo[PART_BODY]->m_matWorld *= m_pInfo[PART_HART]->m_matWorld;


	/************************************************************************/
	// 머리

	Engine::CPipeline::MakeWorldMatrix(&m_pInfo[PART_HEAD]->m_matWorld, &vDefaultScale
		, m_pInfo[PART_HEAD]->m_fAngle, &m_pInfo[PART_HEAD]->m_vPos	);
	m_pInfo[PART_HEAD]->m_matWorld *=  m_pInfo[PART_BODY]->m_matWorld;
	/************************************************************************/
	// 왼쪽팔

	Engine::CPipeline::MakeWorldMatrix(&m_pInfo[PART_LEFTARM]->m_matWorld, &vDefaultScale
		, m_pInfo[PART_LEFTARM]->m_fAngle, &m_pInfo[PART_LEFTARM]->m_vPos	);
	m_pInfo[PART_LEFTARM]->m_matWorld *= m_pInfo[PART_BODY]->m_matWorld;
	/************************************************************************/
	// 오른쪽팔

	Engine::CPipeline::MakeWorldMatrix(&m_pInfo[PART_RIGHTARM]->m_matWorld, &vDefaultScale
		, m_pInfo[PART_RIGHTARM]->m_fAngle, &m_pInfo[PART_RIGHTARM]->m_vPos	);
	m_pInfo[PART_RIGHTARM]->m_matWorld *= m_pInfo[PART_BODY]->m_matWorld;
	/************************************************************************/
	// 하체

	Engine::CPipeline::MakeWorldMatrix(&m_pInfo[PART_LOWER]->m_matWorld, &vDefaultScale
		, m_pInfo[PART_LOWER]->m_fAngle, &m_pInfo[PART_LOWER]->m_vPos	);
	m_pInfo[PART_LOWER]->m_matWorld *= m_pInfo[PART_HART]->m_matWorld;
	/************************************************************************/
	// 왼쪽다리

	Engine::CPipeline::MakeWorldMatrix(&m_pInfo[PART_LEFTLEG]->m_matWorld, &vDefaultScale
		, m_pInfo[PART_LEFTLEG]->m_fAngle, &m_pInfo[PART_LEFTLEG]->m_vPos	);
	m_pInfo[PART_LEFTLEG]->m_matWorld *= m_pInfo[PART_LOWER]->m_matWorld;
	/************************************************************************/
	// 오른쪽다리

	Engine::CPipeline::MakeWorldMatrix(&m_pInfo[PART_RIGHTLEG]->m_matWorld, &vDefaultScale
		, m_pInfo[PART_RIGHTLEG]->m_fAngle, &m_pInfo[PART_RIGHTLEG]->m_vPos	);
	m_pInfo[PART_RIGHTLEG]->m_matWorld *= m_pInfo[PART_LOWER]->m_matWorld;
	/************************************************************************/
}

void CMan::SetParts(void)
{
	//D3DXVec3Lerp
	m_pInfo[PART_HART]->m_vPos			= D3DXVECTOR3(0.f, 0.f, 0.f)	;
	m_pInfo[PART_HART]->m_vScale		= D3DXVECTOR3(1.f, 1.f, 1.f)	;
	m_pInfo[PART_HART]->m_vDir			= D3DXVECTOR3(0.f, 0.f, -1.f);

	m_pInfo[PART_BODY]->m_vPos			= D3DXVECTOR3(0.f, 0.f, 0.f)	;
	m_pInfo[PART_BODY]->m_vScale		= D3DXVECTOR3(1.1f, 0.9f, 1.1f)	;

	m_pInfo[PART_HEAD]->m_vPos			= D3DXVECTOR3(0.f, 2.1f, 0.f)	;
	m_pInfo[PART_HEAD]->m_vScale		= D3DXVECTOR3(1.2f, 1.2f, 1.2f)	;

	m_pInfo[PART_LEFTARM]->m_vPos		= D3DXVECTOR3(-2.0f, 0.f, -1.f)	;
	m_pInfo[PART_LEFTARM]->m_vScale		= D3DXVECTOR3(1.f, 0.7f, 0.7f)	;
	m_pInfo[PART_LEFTARM]->m_fAngle[Engine::ANGLE_Y] = D3DXToRadian(-45.f);

	m_pInfo[PART_RIGHTARM]->m_vPos		= D3DXVECTOR3(2.0f, 0.f, -1.f)	;
	m_pInfo[PART_RIGHTARM]->m_vScale	= D3DXVECTOR3(1.f, 0.7f, 0.7f)	;
	m_pInfo[PART_RIGHTARM]->m_fAngle[Engine::ANGLE_Y] = D3DXToRadian(45.f);

	m_pInfo[PART_LOWER]->m_vPos			= D3DXVECTOR3(0.f, -1.5f, 0.f)	;
	m_pInfo[PART_LOWER]->m_vScale		= D3DXVECTOR3(0.8f, 0.5f, 0.8f)	;

	m_pInfo[PART_LEFTLEG]->m_vPos		= D3DXVECTOR3(-0.8f, -1.8f, 0.f);
	m_pInfo[PART_LEFTLEG]->m_vScale		= D3DXVECTOR3(0.5f, 0.8f, 0.7f)	;

	m_pInfo[PART_RIGHTLEG]->m_vPos		= D3DXVECTOR3(0.8f, -1.8f, 0.f)	;
	m_pInfo[PART_RIGHTLEG]->m_vScale	= D3DXVECTOR3(0.5f, 0.8f, 0.7f)	;
}

void CMan::SetAnimation(void)
{
	LERPSAVE*	pAnimation	= NULL;

	//오른팔
	pAnimation	= new LERPSAVE;
	pAnimation->vPos		= D3DXVECTOR3(2.0f, 0.f, -2.f)	;
	pAnimation->fAngle[Engine::ANGLE_Z]	= D3DXToRadian(0.f);
	//pAnimation->fAngle[Engine::ANGLE_Y]	= D3DXToRadian(0.f);
	m_LerpVec[PART_RIGHTARM].push_back(pAnimation);

	pAnimation	= new LERPSAVE;
	pAnimation->vPos		= D3DXVECTOR3(1.75f, 1.1f, -2.f)	;
	pAnimation->fAngle[Engine::ANGLE_Z]	= D3DXToRadian(80.f);
	//pAnimation->fAngle[Engine::ANGLE_Y]	= D3DXToRadian(40.f);
	m_LerpVec[PART_RIGHTARM].push_back(pAnimation);

	pAnimation	= new LERPSAVE;
	pAnimation->vPos		= D3DXVECTOR3(2.0f, 0.f, -2.f)	;
	pAnimation->fAngle[Engine::ANGLE_Z]	= D3DXToRadian(0.f);
	//pAnimation->fAngle[Engine::ANGLE_Y]	= D3DXToRadian(0.f);
	m_LerpVec[PART_RIGHTARM].push_back(pAnimation);

	pAnimation	= new LERPSAVE;
	pAnimation->vPos		= D3DXVECTOR3(1.75f, -1.1f, -2.f)	;
	pAnimation->fAngle[Engine::ANGLE_Z]	= D3DXToRadian(-50.f);
	//pAnimation->fAngle[Engine::ANGLE_Y]	= D3DXToRadian(-40.f);
	m_LerpVec[PART_RIGHTARM].push_back(pAnimation);

	pAnimation	= new LERPSAVE;
	pAnimation->vPos		= D3DXVECTOR3(2.0f, 0.f, -2.f)	;
	pAnimation->fAngle[Engine::ANGLE_Z]	= D3DXToRadian(0.f);
	//pAnimation->fAngle[Engine::ANGLE_Y]	= D3DXToRadian(0.f);
	m_LerpVec[PART_RIGHTARM].push_back(pAnimation);

	//왼팔
	pAnimation	= new LERPSAVE;
	pAnimation->vPos		= D3DXVECTOR3(-2.0f, 0.f, -2.f)	;
	pAnimation->fAngle[Engine::ANGLE_Z]	= D3DXToRadian(0.f);
	m_LerpVec[PART_LEFTARM].push_back(pAnimation);

	pAnimation	= new LERPSAVE;
	pAnimation->vPos		= D3DXVECTOR3(-1.75f, 1.1f, -2.f) ;
	pAnimation->fAngle[Engine::ANGLE_Z]	= D3DXToRadian(-80.f);
	m_LerpVec[PART_LEFTARM].push_back(pAnimation);

	pAnimation	= new LERPSAVE;
	pAnimation->vPos		= D3DXVECTOR3(-2.0f, 0.f, -2.f)	;
	pAnimation->fAngle[Engine::ANGLE_Z]	= D3DXToRadian(0.f);
	m_LerpVec[PART_LEFTARM].push_back(pAnimation);

	pAnimation	= new LERPSAVE;
	pAnimation->vPos		= D3DXVECTOR3(-1.75f, -1.1f, -2.f)	;
	pAnimation->fAngle[Engine::ANGLE_Z]	= D3DXToRadian(50.f);
	m_LerpVec[PART_LEFTARM].push_back(pAnimation);

	pAnimation	= new LERPSAVE;
	pAnimation->vPos		= D3DXVECTOR3(-2.0f, 0.f, -2.f)	;
	pAnimation->fAngle[Engine::ANGLE_Z]	= D3DXToRadian(0.f);
	m_LerpVec[PART_LEFTARM].push_back(pAnimation);

	//오른 다리
	pAnimation	= new LERPSAVE;
	pAnimation->vPos		= D3DXVECTOR3(-0.8f, -1.f, 0.f)	;
	pAnimation->fAngle[Engine::ANGLE_X]	= D3DXToRadian(0.f);
	m_LerpVec[PART_RIGHTLEG].push_back(pAnimation);

	pAnimation	= new LERPSAVE;
	pAnimation->vPos		= D3DXVECTOR3(-0.8f, -0.8f, -0.7f)	;
	pAnimation->fAngle[Engine::ANGLE_X]	= D3DXToRadian(45.f);
	m_LerpVec[PART_RIGHTLEG].push_back(pAnimation);

	pAnimation	= new LERPSAVE;
	pAnimation->vPos		= D3DXVECTOR3(-0.8f, -1.f, 0.f)	;
	pAnimation->fAngle[Engine::ANGLE_X]	= D3DXToRadian(0.f);
	m_LerpVec[PART_RIGHTLEG].push_back(pAnimation);

	pAnimation	= new LERPSAVE;
	pAnimation->vPos		= D3DXVECTOR3(-0.8f, -0.8f, 0.7f)	;
	pAnimation->fAngle[Engine::ANGLE_X]	= D3DXToRadian(-45.f);
	m_LerpVec[PART_RIGHTLEG].push_back(pAnimation);

	pAnimation	= new LERPSAVE;
	pAnimation->vPos		= D3DXVECTOR3(-0.8f, -1.f, 0.f);
	pAnimation->fAngle[Engine::ANGLE_X]	= D3DXToRadian(0.f);
	m_LerpVec[PART_RIGHTLEG].push_back(pAnimation);

	//왼 다리
	pAnimation	= new LERPSAVE;
	pAnimation->vPos		= D3DXVECTOR3(0.8f, -1.f, 0.f)	;
	pAnimation->fAngle[Engine::ANGLE_X]	= D3DXToRadian(0.f);
	m_LerpVec[PART_LEFTLEG].push_back(pAnimation);

	pAnimation	= new LERPSAVE;
	pAnimation->vPos		= D3DXVECTOR3(0.8f, -0.8f, 0.7f);
	pAnimation->fAngle[Engine::ANGLE_X]	= D3DXToRadian(-45.f);
	m_LerpVec[PART_LEFTLEG].push_back(pAnimation);

	pAnimation	= new LERPSAVE;
	pAnimation->vPos		= D3DXVECTOR3(0.8f, -1.f, 0.f)	;
	pAnimation->fAngle[Engine::ANGLE_X]	= D3DXToRadian(0.f);
	m_LerpVec[PART_LEFTLEG].push_back(pAnimation);

	pAnimation	= new LERPSAVE;
	pAnimation->vPos		= D3DXVECTOR3(0.8f, -0.8f, -0.7f)	;
	pAnimation->fAngle[Engine::ANGLE_X]	= D3DXToRadian(45.f);
	m_LerpVec[PART_LEFTLEG].push_back(pAnimation);

	pAnimation	= new LERPSAVE;
	pAnimation->vPos		= D3DXVECTOR3(0.8f, -1.f, 0.f)	;
	pAnimation->fAngle[Engine::ANGLE_X]	= D3DXToRadian(0.f);
	m_LerpVec[PART_LEFTLEG].push_back(pAnimation);

	//하체 
	pAnimation	= new LERPSAVE;
	pAnimation->vPos		=  D3DXVECTOR3(0.f, -1.5f, 0.f);
	pAnimation->fAngle[Engine::ANGLE_Y]	= D3DXToRadian(0.f);
	pAnimation->fAngle[Engine::ANGLE_Z]	= D3DXToRadian(0.f);
	m_LerpVec[PART_LOWER].push_back(pAnimation);

	pAnimation	= new LERPSAVE;
	pAnimation->vPos		=  D3DXVECTOR3(0.f, -1.5f, 0.f);
	pAnimation->fAngle[Engine::ANGLE_Y]	= D3DXToRadian(-10.f);
	pAnimation->fAngle[Engine::ANGLE_Z]	= D3DXToRadian(-10.f);
	m_LerpVec[PART_LOWER].push_back(pAnimation);

	pAnimation	= new LERPSAVE;
	pAnimation->vPos		=  D3DXVECTOR3(0.f, -1.5f, 0.f);
	pAnimation->fAngle[Engine::ANGLE_Y]	= D3DXToRadian(0.f);
	pAnimation->fAngle[Engine::ANGLE_Z]	= D3DXToRadian(0.f);
	m_LerpVec[PART_LOWER].push_back(pAnimation);

	pAnimation	= new LERPSAVE;
	pAnimation->vPos		=  D3DXVECTOR3(0.f, -1.5f, 0.f);
	pAnimation->fAngle[Engine::ANGLE_Y]	= D3DXToRadian(10.f);
	pAnimation->fAngle[Engine::ANGLE_Z]	= D3DXToRadian(10.f);
	m_LerpVec[PART_LOWER].push_back(pAnimation);

	pAnimation	= new LERPSAVE;
	pAnimation->vPos		=  D3DXVECTOR3(0.f, -1.5f, 0.f);
	pAnimation->fAngle[Engine::ANGLE_Y]	= D3DXToRadian(0.f);
	pAnimation->fAngle[Engine::ANGLE_Z]	= D3DXToRadian(0.f);
	m_LerpVec[PART_LOWER].push_back(pAnimation);

	//얼굴
	pAnimation	= new LERPSAVE;
	pAnimation->vPos		=  D3DXVECTOR3(0.f, 2.1f, 0.f);
	pAnimation->fAngle[Engine::ANGLE_Z]	= D3DXToRadian(0.f);
	m_LerpVec[PART_HEAD].push_back(pAnimation);

	pAnimation	= new LERPSAVE;
	pAnimation->vPos		=  D3DXVECTOR3(0.f, 2.1f, 0.f);
	pAnimation->fAngle[Engine::ANGLE_Z]	= D3DXToRadian(5.f);
	m_LerpVec[PART_HEAD].push_back(pAnimation);

	pAnimation	= new LERPSAVE;
	pAnimation->vPos		=  D3DXVECTOR3(0.f, 2.1f, 0.f);
	pAnimation->fAngle[Engine::ANGLE_Z]	= D3DXToRadian(0.f);
	m_LerpVec[PART_HEAD].push_back(pAnimation);

	pAnimation	= new LERPSAVE;
	pAnimation->vPos		=  D3DXVECTOR3(0.f, 2.1f, 0.f);
	pAnimation->fAngle[Engine::ANGLE_Z]	= D3DXToRadian(-5.f);
	m_LerpVec[PART_HEAD].push_back(pAnimation);

	pAnimation	= new LERPSAVE;
	pAnimation->vPos		=  D3DXVECTOR3(0.f, 2.1f, 0.f);
	pAnimation->fAngle[Engine::ANGLE_Z]	= D3DXToRadian(0.f);
	m_LerpVec[PART_HEAD].push_back(pAnimation);
}

void CMan::SetPos(D3DXVECTOR3& rPos)
{
	m_pInfo[PART_HART]->m_vPos = rPos;
	m_vMyPos = m_pInfo[PART_HART]->m_vPos;
}
