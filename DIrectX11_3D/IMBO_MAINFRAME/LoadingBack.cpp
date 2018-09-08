#include "stdafx.h"
#include "LoadingBack.h"

CLoadingBack::CLoadingBack()
{
}
CLoadingBack::~CLoadingBack()
{
	m_pUIRenderCont->ClearData();
}

CLoadingBack * CLoadingBack::Create(XMVECTOR xyPos, XMVECTOR xySize, string strTexName)
{
	CLoadingBack*		pObject = new CLoadingBack();

	if (FAILED(pObject->Initialize()))
	{
		MSG_BOX("CLoadingBack Create Faild");
		//Safe_Delete(pObject);
		delete pObject;
		pObject = nullptr;
		return pObject;
	}
	pObject->SetInfo(xyPos, xySize, strTexName);

	return pObject;
}

void CLoadingBack::SetInfo(XMVECTOR xyPos, XMVECTOR xySize, string strTexName)
{
	//memcpy(m_szTexture, pTexName, sizeof(TCHAR) * 64);
	StringToTCHAR(strTexName, m_szTexture);
	m_pTexture = RESOURCEMGR->GetTexture(strTexName);
	m_pUIRenderCont = RENDERER->GetUIRenderer();
	m_pMesh = RESOURCEMGR->GetMesh("UI");
	m_pCBuffer = CBuffer::CreateConstantBuffer(1, sizeof(tUImatVP), 11, BIND_VS , NULL);	//������*������ȯ��� / ����
	
	XMStoreFloat2(&m_f2XYPos, xyPos);
	XMStoreFloat2(&m_f2XYSize, xySize);
}

HRESULT CLoadingBack::Initialize()
{
	XMStoreFloat4x4(&m_f4x4View, XMMatrixIdentity());
	XMStoreFloat4x4(&m_f4x4Proj, XMMatrixOrthographicLH(WINSIZEX, WINSIZEY, 0.f, 1.f));

	m_iType = rand() % 3;
	return S_OK;
}

int CLoadingBack::Update(float fTimeElapsed)
{
	m_fAccTime += fTimeElapsed * 4.f;
	//m_f2XYPos.x += 2.f * cos(m_fAccTime);

	//x y ���� ����� 0, 0 ������ �ϴ��� WINSIZEX, WINSIZEY ��

	if (0 == m_iType)
	{
		m_f2XYPos.x = WINSIZEX * 0.5f - cos(m_fAccTime) * 200.f;
		m_f2XYPos.y = WINSIZEY * 0.5f - sin(m_fAccTime) * 200.f;
	}
	else if(1 == m_iType)
	{
		m_f2XYPos.x = WINSIZEX * 0.5f - cos(m_fAccTime) * 200.f;
		//m_f2XYPos.y = WINSIZEY * 0.5f - sin(m_fAccTime) * 200.f;
	}
	else if (2 == m_iType)
	{
		m_f2XYPos.x = WINSIZEX * 0.5f - cos(m_fAccTime) * 200.f;
		m_f2XYPos.y = WINSIZEY * 0.5f - cos(m_fAccTime) * 200.f;
	}

	if (m_pUIRenderCont) m_pUIRenderCont->SetRenderContainer(1.f, this);
	return 0;
}

void CLoadingBack::Render()
{
	m_pTexture->UpdateShaderState();
	m_pTexture->SetShaderState();

	m_pCBuffer->UpdateShaderState();
	SetParameter();

	m_pMesh->Render(1);

	//����
	m_pTexture->CleanShaderState();
	m_pCBuffer->CleanShaderState();
}
void CLoadingBack::Release(){
	if (m_pCBuffer) {
		m_pCBuffer->End();
		delete m_pCBuffer;
	}
	m_pCBuffer = nullptr;
}

void CLoadingBack::SetParameter()
{
	m_f4x4View._11 = m_f2XYSize.x;
	m_f4x4View._22 = m_f2XYSize.y;
	m_f4x4View._33 = 1.f;

	m_f4x4View._41 = m_f2XYPos.x - WINSIZEX * 0.5f;
	m_f4x4View._42 = -m_f2XYPos.y + WINSIZEY * 0.5f;

	tUImatVP* pdata = (tUImatVP*)m_pCBuffer->Map();

	XMStoreFloat4x4(&pdata->m_xmf4x4VP, XMMatrixTranspose(XMMatrixMultiply(XMLoadFloat4x4(&m_f4x4View), XMLoadFloat4x4(&m_f4x4Proj))));
//	pdata->m_fAlpha = 1.f /*�ӽ�*/;

	m_pCBuffer->Unmap();
	m_pCBuffer->SetShaderState();

}
