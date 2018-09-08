#include "stdafx.h"
#include "StaticObject.h"

#include "Export_Function.h"
#include "Include.h"

#include "Pipeline.h"
#include "MathMgr.h"
#include "Nemo.h"


CStaticObject::CStaticObject(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourcesMgr())
//, m_pBuffer(NULL)
, m_pResource(NULL)
//, m_dwVtxCnt(0)
, m_vPos(D3DXVECTOR3(0.f, 0.f, 0.f))
, m_vScale(D3DXVECTOR3(1.f, 1.f, 1.f))
, m_wstrName(L"")
//, m_iRed(0)
//, m_iGreen(0)
//, m_iBlue(0)
{
	D3DXMatrixIdentity(&m_matWorld);
	ZeroMemory(m_fAngle, sizeof(float) * Engine::ANGLE_END);
}

CStaticObject::~CStaticObject(void)
{
	Release();
}



HRESULT CStaticObject::Initialize(void)
{
	//AddBuffer();

	//m_dwVtxCnt = 4;
	//m_pVertex = new Engine::VTXCOL[m_dwVtxCnt];
	//m_pConvertVertex = new Engine::VTXCOL[m_dwVtxCnt];

	//m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcCol", m_pVertex);

	return S_OK;
}
int CStaticObject::Update(void) // 0804
{
	//SetColor();
	return 0;
}
void CStaticObject::Render(void)
{
	//m_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

	//m_pBuffer->Render();

	D3DXMatrixIdentity(&m_matWorld);

	Engine::CPipeline::MakeWorldMatrix(&m_matWorld, &m_vScale
		, m_fAngle, &m_vPos);

	list<CNemo*>::iterator iter		= m_NemoList.begin();
	list<CNemo*>::iterator iter_end	= m_NemoList.end();

	for( ; iter != iter_end; ++iter)
	{
		(*iter)->Update();
		(*iter)->SetWorldMatrix((*iter)->GetWorldMatrix() * m_matWorld);
		(*iter)->Render();
	}
}	

CStaticObject* CStaticObject::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStaticObject* pObj = new CStaticObject(pDevice);
	if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);

	return pObj;	
}
void CStaticObject::Release(void)
{
	//Engine::Safe_Delete_Array(m_pVertex);
	//Engine::Safe_Delete_Array(m_pConvertVertex);

	//Engine::Safe_Delete(m_pBuffer);
	list<CNemo*>::iterator iter		= m_NemoList.begin();
	list<CNemo*>::iterator iter_end	= m_NemoList.end();

	for( ; iter != iter_end; ++iter)
	{
		Engine::Safe_Delete(*iter);
	}
}

//void CStaticObject::AddBuffer(void)
//{
//	m_pResource	= m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_RcCol");	
//	m_pBuffer	= dynamic_cast<Engine::CVIBuffer*>(m_pResource);
//}

//void CStaticObject::SetColor(void)
//{
//	for(size_t i = 0; i < m_dwVtxCnt; ++i)
//	{
//		m_pConvertVertex[i] = m_pVertex[i];
//		m_pConvertVertex[i].dwColor = D3DCOLOR_ARGB(255, m_iRed, m_iGreen, m_iBlue);
//	}	
//	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcCol", m_pConvertVertex);
//}


void CStaticObject::SetPos(const D3DXVECTOR3& rPos)
{
	m_vPos	= rPos;	
}

//void CStaticObject::SetRGB(int _iRed, int _iGreen, int _iBlue)
//{
//	m_iRed		= _iRed;
//	m_iGreen	= _iGreen;
//	m_iBlue		= _iBlue;
//}

void CStaticObject::SetScale(const D3DXVECTOR3& rScale)
{
	m_vScale	= rScale;
}

void CStaticObject::SetAngle(float fAngle[3])
{
	for(int i = 0; i < Engine::ANGLE_END; ++i)
		m_fAngle[i] = fAngle[i];
}

void CStaticObject::GetPos(D3DXVECTOR3* pPos)
{
	*pPos = m_vPos;
}

//void CStaticObject::GetRGB(int* _iRed, int* _iGreen, int* _iBlue)
//{
//	*_iRed		= m_iRed;
//	*_iGreen	= m_iGreen;
//	*_iBlue		= m_iBlue;
//}

void CStaticObject::GetScale(D3DXVECTOR3* pScale)
{
	*pScale = m_vScale;
}

void CStaticObject::GetAngle(float* fAngle)
{
	for(int i = 0; i < Engine::ANGLE_END; ++i)
		fAngle[i] = m_fAngle[i];
}

void CStaticObject::SetWorldMatrix(const D3DXMATRIX& rMatrix)
{
	m_matWorld = rMatrix;
}

D3DXMATRIX CStaticObject::GetWorldMatrix(void)
{
	return m_matWorld;
}

void CStaticObject::AddNemo(CNemo* pNemo)
{
	m_NemoList.push_back(pNemo);
}

list<CNemo*>* CStaticObject::GetNemoList(void)
{
	return &m_NemoList;
}

void CStaticObject::SetName(wstring wstrName)
{
	m_wstrName = wstrName;
}

std::wstring CStaticObject::GetName(void)
{
	return m_wstrName;
}
