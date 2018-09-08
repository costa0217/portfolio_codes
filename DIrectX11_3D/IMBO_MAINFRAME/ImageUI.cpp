#include "stdafx.h"
#include "ImageUI.h"


CImageUI::CImageUI()
{
}


CImageUI::~CImageUI()
{
	m_pUIRenderCont->ClearData();
}

CImageUI * CImageUI::Create(XMVECTOR xyPos, XMVECTOR xySize, string strTexName, float fRanderLayer)
{
	CImageUI*		pObject = new CImageUI();

	if (FAILED(pObject->Initialize()))
	{
		MSG_BOX("CImageUI Create Faild");
		//Safe_Delete(pObject);
		delete pObject;
		pObject = nullptr;
		return pObject;
	}
	pObject->SetInfo(xyPos, xySize, strTexName, fRanderLayer);

	return pObject;
}

void CImageUI::SetInfo(XMVECTOR xyPos, XMVECTOR xySize, string strTexName, float fRanderLayer)
{
	// memcpy(m_szTexture, pTexName, sizeof(TCHAR) * 64);
	StringToTCHAR(strTexName, m_szTexture);
	m_pTexture = RESOURCEMGR->GetTexture(strTexName);
	m_pUIRenderCont = RENDERER->GetUIRenderer();
	m_pMesh = RESOURCEMGR->GetMesh("UI");
	m_pCBuffer = CBuffer::CreateConstantBuffer(1, sizeof(tUImatVP), 11, BIND_VS, 0);	//직교뷰*투영변환행렬 / 알파
	m_fRenderLayer = fRanderLayer;

	XMStoreFloat2(&m_f2XYPos, xyPos);
	XMStoreFloat2(&m_f2XYSize, xySize);
}

HRESULT CImageUI::Initialize()
{
	//x y 왼쪽 상단이 0, 0 오른쪽 하단이 WINSIZEX, WINSIZEY 임
	XMStoreFloat4x4(&m_f4x4View, XMMatrixIdentity());
	XMStoreFloat4x4(&m_f4x4Proj, XMMatrixOrthographicLH(WINSIZEX, WINSIZEY, 0.f, 1.f));

	return S_OK;
}

int CImageUI::Update(float fTimeElapsed)
{
	if (false == m_bRender) return 0;
	if (m_pUIRenderCont) {
		if(m_fRenderLayer< 0)
			m_pUIRenderCont->SetRenderContainer(m_fRenderLayer, this, 1);
		else
			m_pUIRenderCont->SetRenderContainer(m_fRenderLayer, this);
	}
	return 0;
}

void CImageUI::Render()
{
	m_pTexture->UpdateShaderState();
	m_pTexture->SetShaderState();

	m_pCBuffer->UpdateShaderState();
	SetParameter();

	m_pMesh->Render(1);

	//해제
	m_pTexture->CleanShaderState();
	m_pCBuffer->CleanShaderState();
}

void CImageUI::Release()
{
	if (m_pCBuffer) {
		m_pCBuffer->End();
		delete m_pCBuffer;
	}
	m_pCBuffer = nullptr;
}

void CImageUI::SetParameter()
{
	m_f4x4View._11 = m_f2XYSize.x;
	m_f4x4View._22 = m_f2XYSize.y;
	m_f4x4View._33 = 1.f;

	m_f4x4View._41 = m_f2XYPos.x - WINSIZEX * 0.5f;
	m_f4x4View._42 = -m_f2XYPos.y + WINSIZEY * 0.5f;
	//if (m_fRenderLayer< 0)
	//	m_f4x4View._43 = 1000.f;

	tUImatVP* pdata = (tUImatVP*)m_pCBuffer->Map();

	XMStoreFloat4x4(&pdata->m_xmf4x4VP, XMMatrixTranspose(XMMatrixMultiply(XMLoadFloat4x4(&m_f4x4View), XMLoadFloat4x4(&m_f4x4Proj))));

	m_pCBuffer->Unmap();
	m_pCBuffer->SetShaderState();
}
