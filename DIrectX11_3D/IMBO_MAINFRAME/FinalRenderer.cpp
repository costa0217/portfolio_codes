#include "stdafx.h"
#include "FinalRenderer.h"


CFinalRenderer::CFinalRenderer()
{
}


CFinalRenderer::~CFinalRenderer()
{
	if (nullptr != m_pCBuffer)
	{
		m_pCBuffer->End();
		delete m_pCBuffer;
		m_pCBuffer = nullptr;
	}
}

HRESULT CFinalRenderer::Initialize()
{
	ResizeBuffer();
	m_pRenderShader = RESOURCEMGR->GetRenderShader("FinalPass");
	m_pMesh = RESOURCEMGR->GetMesh("DirectionalLight");

	m_pCBuffer = CBuffer::CreateConstantBuffer(1, sizeof(OptInfo), 5, BIND_PS, NULL);
	ZeroMemory(&m_tOptInfo, sizeof(OptInfo));
	return S_OK;
}

void CFinalRenderer::ResizeBuffer()
{

}

void CFinalRenderer::RenderFinalPass(ID3D11ShaderResourceView * pPostProcessSRV, ID3D11ShaderResourceView * pAlphaSRV,
	ID3D11ShaderResourceView * pSkyBoxSRV, ID3D11ShaderResourceView * pDistortion, bool bRadial, float fBlurStart, float fBlurWidth)
{
	m_pRenderShader->UpdateShaderState();
	m_pRenderShader->SetShaderState();

	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(1, 1, &pPostProcessSRV);
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(2, 1, &pAlphaSRV);
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(3, 1, &pSkyBoxSRV);
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(4, 1, &pDistortion);

	OptInfo* pdata = (OptInfo*)m_pCBuffer->Map();

	pdata->BlurStart = fBlurStart;
	pdata->BlurWidth = fBlurWidth;
	pdata->iOpt = bRadial == true ? 1 : 0;

	m_pCBuffer->Unmap();
	m_pCBuffer->SetShaderState();

	// screen mesh render
	m_pMesh->Render(1);
	m_pRenderShader->CleanShaderState();


	ID3D11ShaderResourceView* pSRVnull[1] = { nullptr };
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(1, 1, pSRVnull);
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(2, 1, pSRVnull);
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(3, 1, pSRVnull);
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(4, 1, pSRVnull);
	m_pCBuffer->CleanShaderState();
}
