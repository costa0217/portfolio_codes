#include "StdAfx.h"
#include "MultiTexture.h"
#include "Device.h"

CMultiTexture::CMultiTexture(void)
{
}

CMultiTexture::~CMultiTexture(void)
{
	Release();
}

HRESULT CMultiTexture::InsertTexture(const wstring& wstrFilePath, 
									 const wstring& wstrStateKey /*= L""*/, const int& iCount /*= 0*/)
{
	TCHAR					szPath[MAX_PATH] = L"";
	vector<TEXINFO*>		vecTexture;

	for(int i = 0; i < iCount; ++i)
	{
		wsprintf(szPath, wstrFilePath.c_str(), i);

		TEXINFO*	pTexInfo = new TEXINFO;
		ZeroMemory(pTexInfo, sizeof(TEXINFO));

		if(FAILED(D3DXGetImageInfoFromFile(szPath, &pTexInfo->tImgInfo)))
		{
			ERR_MSG(L"Multi Texture error");
			return E_FAIL;
		}

		if(FAILED(D3DXCreateTextureFromFileEx(CDevice::GetInstance()->GetDevice(),
			szPath, 
			pTexInfo->tImgInfo.Width, 
			pTexInfo->tImgInfo.Height,
			pTexInfo->tImgInfo.MipLevels,
			0, pTexInfo->tImgInfo.Format,
			D3DPOOL_MANAGED, 
			D3DX_DEFAULT, 
			D3DX_DEFAULT, NULL, 
			&pTexInfo->tImgInfo, NULL, &pTexInfo->pTexture)))
		{	
			ERR_MSG(L"multi Texture Image Load Failed");
			return E_FAIL;
		}

		vecTexture.push_back(pTexInfo);
		
	}

	m_MapMultiTexture.insert(make_pair(wstrStateKey, vecTexture));


	return S_OK;
}

void CMultiTexture::Release(void)
{
	for(map<wstring, vector<TEXINFO*>>::iterator iter = m_MapMultiTexture.begin();
		iter != m_MapMultiTexture.end(); ++iter)
	{
		for_each(iter->second.begin(), iter->second.end(), DeleteOBJ());
		iter->second.clear();
	}
	m_MapMultiTexture.clear();
}

const TEXINFO* CMultiTexture::GetTexture(const wstring& wstrStateKey /*= L""*/, 
										 const int& iCount /*= 0*/)
{
	map<wstring, vector<TEXINFO*>>::iterator	iter = m_MapMultiTexture.find(wstrStateKey);

	if(iter == m_MapMultiTexture.end())
		return NULL;


	return iter->second[iCount];
}
size_t CMultiTexture::GetImageCount(const wstring&  wstrStateKey)
{
	map<wstring, vector<TEXINFO*>>::iterator		iter = m_MapMultiTexture.find(wstrStateKey);

	if(iter == m_MapMultiTexture.end())
		return -1;

	return iter->second.size();
}
