#include "TriColor.h"

Engine::CTriColor::CTriColor(LPDIRECT3DDEVICE9 pDevice)
: CVIBuffer(pDevice)
{

}

Engine::CTriColor::~CTriColor(void)
{

}

Engine::CResources* Engine::CTriColor::CloneResource(void)
{
	++(*m_pwRefCnt);

	return new CTriColor(*this);
}

HRESULT Engine::CTriColor::CreateBuffer(void)
{
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = 3;
	m_dwFVF = VTXCOL_FVF;
	m_dwTriCnt = 1;

	FAILED_CHECK(CVIBuffer::CreateBuffer());

	VTXCOL*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = D3DXVECTOR3(0.f, 1.f, 0.f);
	pVertex[0].dwColor = D3DCOLOR_ARGB(255,255,0,0);

	pVertex[1].vPos = D3DXVECTOR3(1.f, -1.f, 0.f);
	pVertex[1].dwColor = D3DCOLOR_ARGB(255,0,255,0);

	pVertex[2].vPos = D3DXVECTOR3(-1.f, -1.f, 0.f);
	pVertex[2].dwColor = D3DCOLOR_ARGB(255,125,125,0);

	m_pVB->Unlock();

	return S_OK;
}

Engine::CTriColor* Engine::CTriColor::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTriColor* pBuffer = new CTriColor(pDevice);
	if(FAILED(pBuffer->CreateBuffer()))
		Engine::Safe_Delete(pBuffer);

	return pBuffer;
}

void Engine::CTriColor::Render(void)
{
	m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VTXCOL));

	m_pDevice->SetFVF(VTXCOL_FVF);
	m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
}
