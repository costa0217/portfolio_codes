#include "stdafx.h"
#include "ItemBullet.h"

#include "Export_Function.h"
#include "Include.h"

#include "Pipeline.h"
#include "MathMgr.h"
#include "CameraObserver.h"
#include "Texture.h"
#include "PlayerObserver.h"
#include "Player.h"

CItemBullet::CItemBullet(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourcesMgr())
, m_pBuffer(NULL)
, m_pTexture(NULL)
, m_pResource(NULL)
, m_dwVtxCnt(0)
, m_vPos(D3DXVECTOR3(1150.f, 10.f, 118.f))
, m_vScale(D3DXVECTOR3(5.f, 5.f, 5.f))
, m_pCameraObserver(NULL)
, m_pInfoSubject(Engine::Get_InfoSubject())
, m_iTextureNum(0)
, m_fPlayerBullet(0)
{
	D3DXMatrixIdentity(&m_matWorld);
	ZeroMemory(m_fAngle, sizeof(float) * Engine::ANGLE_END);
}

CItemBullet::~CItemBullet(void)
{
	Release();
}

HRESULT CItemBullet::Initialize(void)
{
	AddComponent();

	m_dwVtxCnt = 8;
	m_pVertex = new Engine::VTXCUBE[m_dwVtxCnt];
	m_pConvertVertex = new Engine::VTXCUBE[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_CubeTex", m_pVertex);

	m_pCameraObserver = CCameraObserver::Create();
	m_pPlayerObserver	= CPlayerObserver::Create();

	m_pInfoSubject->Subscribe(m_pCameraObserver);
	m_pInfoSubject->Subscribe(m_pPlayerObserver);

	//	m_iTextureNum = rand()%2; 

	return S_OK;
}
int CItemBullet::Update(void)
{
	D3DXMatrixIdentity(&m_matWorld);

	m_fAngle[Engine::ANGLE_Y] += D3DXToRadian(180.f) * Engine::Get_TimeMgr()->GetInstance()->GetTime();
	if(m_fAngle[Engine::ANGLE_Y] >= D3DXToRadian(360.f))
		m_fAngle[Engine::ANGLE_Y] = 0.f;

	Engine::CPipeline::MakeWorldMatrix(&m_matWorld, &m_vScale
		, m_fAngle, &m_vPos);

	m_fPlayerBullet = m_pPlayerObserver->GetBullet();
	m_vPlayerPos = (*m_pPlayerObserver->GetPos());

	D3DXVECTOR3 vDist = m_vPlayerPos - m_vPos;

	float m_fDistance = D3DXVec3Length(&vDist);

	if(m_fDistance <= m_vScale.x + 5.f)
	{
		((CPlayer*)m_pPlayer)->SetBulltCnt(m_fPlayerBullet + 50.f);

		m_pInfoSubject->UnSubscribe(m_pCameraObserver);
		return 1;
	}

	return 0;
}
void CItemBullet::Render(void)
{
	TransformMatrix();

	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_CubeTex", m_pConvertVertex);

	m_pTexture->Render(m_iTextureNum);
	m_pBuffer->Render();

	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_CubeTex", m_pVertex);
}	

void CItemBullet::TransformMatrix(void)
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

CItemBullet* CItemBullet::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CItemBullet* pObj = new CItemBullet(pDevice);

	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;	
}
void CItemBullet::Release(void)
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);
	m_pInfoSubject->UnSubscribe(m_pPlayerObserver);

	Engine::Safe_Delete(m_pCameraObserver);
	Engine::Safe_Delete(m_pPlayerObserver);

	Engine::Safe_Delete_Array(m_pVertex);
	Engine::Safe_Delete_Array(m_pConvertVertex);

	Engine::Safe_Delete(m_pBuffer);
	Engine::Safe_Delete(m_pTexture);
}

void CItemBullet::AddComponent(void)
{
	m_pResource	= m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Texture_ItemBullet");	
	m_pTexture	= dynamic_cast<Engine::CTexture*>(m_pResource);

	m_pResource	= m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_CubeTex");	
	m_pBuffer	= dynamic_cast<Engine::CVIBuffer*>(m_pResource);
}

void CItemBullet::SetPos(const D3DXVECTOR3& rPos)
{
	m_vPos	= rPos;	
}

void CItemBullet::SetScale(const D3DXVECTOR3& rScale)
{
	m_vScale	= rScale;
}

void CItemBullet::SetAngle(float fAngle[3])
{
	for(int i = 0; i < Engine::ANGLE_END; ++i)
		m_fAngle[i] = fAngle[i];
}

void CItemBullet::GetPos(D3DXVECTOR3* pPos)
{
	*pPos = m_vPos;
}

void CItemBullet::GetScale(D3DXVECTOR3* pScale)
{
	*pScale = m_vScale;
}

void CItemBullet::GetAngle(float* fAngle)
{
	for(int i = 0; i < Engine::ANGLE_END; ++i)
		fAngle[i] = m_fAngle[i];
}

void CItemBullet::SetWorldMatrix(const D3DXMATRIX& rMatrix)
{
	m_matWorld = rMatrix;
}

D3DXMATRIX CItemBullet::GetWorldMatrix(void)
{
	return m_matWorld;
}

void CItemBullet::SetPlayer(Engine::CGameObject* pPlayer)
{
	m_pPlayer = pPlayer;
}
