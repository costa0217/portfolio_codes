#include "stdafx.h"
#include "StaticObject_Default.h"

#include "CullingMgr.h"
#include "CollisionMgr.h"

CStaticObject_Default::CStaticObject_Default(LPDIRECT3DDEVICE9 pGraphicDev)
: CStaticObject(pGraphicDev)
{

}

CStaticObject_Default::~CStaticObject_Default(void)
{

}

_int CStaticObject_Default::Update(const _float& fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;

	if(true == m_bActor)
	{
		//�ν����� ������Ʈ�϶�
		m_fDisappearTime -= m_fTimeDelta;

		if(m_fDisappearTime <= 0.f) // ���� �ð��� ������ ���ĺ���� ���ش�.
		{
			m_fDisappearTime = 0.f;
			m_iPassIndex = Shader_AlphaBlend;
		}
		if(m_fAlpha <= 0.f)
			return -1;
	}

	if(CCullingMgr::GetInstance()->CullingCamera(&m_vWorldCenter, m_fWorldRadius))
	{
		if(Shader_AlphaBlend == m_iPassIndex) //���ĺ������϶� ���İ��� �ٿ��ش�.
		{
			m_pEffect->SetFloat("g_fAlpha", m_fAlpha);

			m_fAlpha -= m_fTimeDelta;
			if(m_fAlpha <= 0.f)
				m_fAlpha = 0.f;
			
 			_matrix			matView;
 			m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
 			D3DXVec3TransformCoord(&m_vViewPos, &m_pTransCom->m_vPosition, &matView);
 			Engine::Add_RenderGroup(Engine::CRenderer::RENDER_OBJALPHABLEND, this, m_vViewPos.z);
 		}
 		else
		{
			Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);
		}
		
		Engine::CGameObject::Update(fTimeDelta);
	}
	return 0;
}

void CStaticObject_Default::Render(void)
{
	if(NULL == m_pEffect)
		return;

	Set_ConstantTable();

	
	if(NULL != m_pBoxActor)
	{
		//Actor�κ��� ����� �޾ƿ´�.
		NxMat34 worldMat;
		worldMat = m_pBoxActor->getGlobalPose();
		worldMat.getColumnMajor44( static_cast<NxF32*>(&m_pTransCom->m_matWorld._11) );
		//�޾ƿ� ����� �������� 1�� �Ǿ��־ �� ����������� �ٽ� ������.
		_matrix		matScale;
		D3DXMatrixScaling(&matScale, m_pTransCom->m_vScale.x, m_pTransCom->m_vScale.y, m_pTransCom->m_vScale.z);
		m_pTransCom->m_matWorld = matScale * m_pTransCom->m_matWorld;
		//shader�� ��ȯ�� ��������� �ٽ� �����ش�.
		m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	}

	m_pEffect->Begin(NULL, 0);

	m_pEffect->BeginPass(m_iPassIndex);
	
	m_pMeshCom->Render_MeshForShader(m_pEffect, true);

	m_pEffect->EndPass();

	m_pEffect->End();	

	m_pEffect->SetTexture("g_BaseTexture", NULL);
}

CStaticObject_Default* CStaticObject_Default::Create(LPDIRECT3DDEVICE9 pGraphicDev, TCHAR* pObjectName)
{
	CStaticObject_Default* pGameObject = new CStaticObject_Default(pGraphicDev);
	pGameObject->m_pObjectName = new TCHAR[128];
	lstrcpy(pGameObject->m_pObjectName, pObjectName);
	if(FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("CStaticObject_Default Create Faild");
		Engine::Safe_Release(pGameObject);
	}

	//ī�޶�� �浹 ó���� �ϱ� ���� ����
	CCollisionMgr::GetInstance()->Get_StaticObjectDefaultList()->push_back(pGameObject);

	return pGameObject;
}

HRESULT CStaticObject_Default::Initialize(void)
{
	if(FAILED(CStaticObject::Initialize()))
		return E_FAIL;

	m_pEffect = Engine::Get_EffectHandle(L"Shader_Default");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_fAlpha = 1.f;
	m_vViewPos = _vec3(0.f, 0.f, 0.f);

	return S_OK;
}

HRESULT CStaticObject_Default::Add_Component(void)
{
	return S_OK;
}

void CStaticObject_Default::Set_ConstantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &matProj);
}

_ulong CStaticObject_Default::Release(void)
{
	_ulong dwRefCnt = CStaticObject::Release();

	if(0 == dwRefCnt)
	{
		list<CStaticObject*>::iterator iter = CCollisionMgr::GetInstance()->Get_StaticObjectDefaultList()->begin();
		list<CStaticObject*>::iterator iterEnd = CCollisionMgr::GetInstance()->Get_StaticObjectDefaultList()->end();

		for(; iter != iterEnd; )
		{
			if((*iter) == this)
				iter = CCollisionMgr::GetInstance()->Get_StaticObjectDefaultList()->erase(iter);
			else
				++iter;
		}
		::Safe_Delete(m_pObjectName);
		delete this;
	}

	return dwRefCnt;
}
