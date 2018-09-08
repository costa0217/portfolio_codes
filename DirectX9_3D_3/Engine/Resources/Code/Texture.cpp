#include "Texture.h"

Engine::CTexture::CTexture(LPDIRECT3DDEVICE9 pDevice)
: CResources(pDevice)
{

}

Engine::CTexture::~CTexture(void)
{
	Release();
}

Engine::CResources* Engine::CTexture::CloneResource(void)
{
	++(*m_pwRefCnt);

	return new CTexture(*this);
}

HRESULT Engine::CTexture::LoadTexture(TEXTURETYPE eTextureType, const wstring& wstrFilePath, const WORD& wCnt)
{
	if(wCnt == 0)
		return E_FAIL;

	m_vecTexture.reserve(wCnt);

	TCHAR		szFullPath[MAX_PATH] = L"";

	for(size_t i = 0; i < wCnt; ++i)
	{
		wsprintf(szFullPath, wstrFilePath.c_str(), i);

		IDirect3DBaseTexture9*		pTexture = NULL;
		HRESULT		hr = NULL;

		switch(eTextureType)
		{
		case TEXTURE_NORMAL:
			hr = D3DXCreateTextureFromFile(m_pDevice, szFullPath, (LPDIRECT3DTEXTURE9*)&pTexture);
			break;

		case TEXTURE_CUBE:
			hr = D3DXCreateCubeTextureFromFile(m_pDevice, szFullPath, (LPDIRECT3DCUBETEXTURE9*)&pTexture);
			break;
		}
		FAILED_CHECK_MSG(hr, szFullPath);

		m_vecTexture.push_back(pTexture);
	}

	m_dwContainerSize = m_vecTexture.size();
	return S_OK;
}

void Engine::CTexture::Render(const DWORD& iIndex)
{
	if(iIndex >= m_dwContainerSize)
		return;

	m_pDevice->SetTexture(0, m_vecTexture[iIndex]);
}

Engine::CTexture* Engine::CTexture::Create(LPDIRECT3DDEVICE9 pDevice 
										   , TEXTURETYPE eTextureType
										   , const wstring& wstrFilePath
										   , const WORD& wCnt)
{
	CTexture*	pTexutre = new CTexture(pDevice);
	if(FAILED(pTexutre->LoadTexture(eTextureType, wstrFilePath, wCnt)))
		Engine::Safe_Delete(pTexutre);

	return pTexutre;
}

void Engine::CTexture::Release(void)
{
	if( (*m_pwRefCnt) == 0 )
	{
		for(size_t i = 0; i < m_vecTexture.size(); ++i)
		{
			Engine::Safe_Release(m_vecTexture[i]);
		}
		m_vecTexture.clear();

		CResources::Release();
	}
	else
		--(*m_pwRefCnt);
}