#include "StdAfx.h"
#include "TextureMgr.h"
#include "SingleTexture.h"
#include "MultiTexture.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr(void)
:m_iTextureNum(0)
{
}

CTextureMgr::~CTextureMgr(void)
{
	Release();
}

void CTextureMgr::Release(void)
{
	for_each(m_MapTexture.begin(), m_MapTexture.end(), DeleteMap());
	m_MapTexture.clear();
}

HRESULT CTextureMgr::InsertTexture(const wstring& wstrFilePath, /* 경로 */ 
								   const wstring& wstrObjKey, /* OBJKEY값 */ 
								   TEXTYPE _eType, /* s or m */ 
								   const wstring& wstrStateKey /*= L""*//* obj 상태 값 */,
								   const int& iCount /*= 0*/)
{
	map<wstring, CTexture*>::iterator	iter = m_MapTexture.find(wstrObjKey);

	++m_iTextureNum;

	if(iter == m_MapTexture.end())
	{
		CTexture*		pTexture = NULL;

		switch(_eType)
		{
		case TEX_SINGLE:
			pTexture = new CSingleTexture;
			break;

		case TEX_MULTI:
			pTexture = new CMultiTexture;
			break;
		}

		if(FAILED(pTexture->InsertTexture(wstrFilePath, wstrStateKey, iCount)))
		{
			ERR_MSG(wstrObjKey.c_str());
			return E_FAIL;
		}

		m_MapTexture.insert(make_pair(wstrObjKey, pTexture));
	}

	else
	{
		if(_eType == TEX_MULTI)
			iter->second->InsertTexture(wstrFilePath, wstrStateKey, iCount);
	}



	return S_OK;
}

const TEXINFO* CTextureMgr::GetTexture(const wstring& wstrObjKey, 
									   const wstring& wstrStateKey /*= L""*/,
									   const int& iCount /*= 0*/)
{
	map<wstring, CTexture*>::iterator		iter = m_MapTexture.find(wstrObjKey);

	if(iter == m_MapTexture.end())
		return NULL;


	return iter->second->GetTexture(wstrStateKey, iCount);
}

HRESULT CTextureMgr::ReadImage( wstring wstrImgPath )
{
	wifstream	LoadFile;

	LoadFile.open(wstrImgPath.c_str(), ios::in);

	TCHAR		szObjKey[MIN_STR]		= L"";
	TCHAR		szStateKey[MIN_STR]		= L"";
	TCHAR		szCount[MIN_STR]		= L"";
	TCHAR		szImgPath[MAX_PATH]		= L"";

	wstring		wstrCombine = L"";

	while(!LoadFile.eof())
	{
		LoadFile.getline(szObjKey, MIN_STR, '|');
		LoadFile.getline(szStateKey, MIN_STR, '|');
		LoadFile.getline(szCount, MIN_STR, '|');
		LoadFile.getline(szImgPath, MAX_PATH);

		int iCount = _ttoi(szCount);

		if(FAILED(InsertTexture(szImgPath, szObjKey, TEX_MULTI, szStateKey, iCount )))
		{
			ERR_MSG(L"TEXTUREMGR FILE LOAD FAILED");
			return E_FAIL;
		}
	
	}

	LoadFile.close();
	return S_OK;
}
size_t CTextureMgr::GetImageCount(const wstring& wstrObjKey, 
								  const wstring& wstrStateKey /*= L""*/)
{
	map<wstring, CTexture*>::iterator	iter = m_MapTexture.find(wstrObjKey);

	if(iter == m_MapTexture.end())
		return -1;

	return ((CMultiTexture*)iter->second)->GetImageCount(wstrStateKey);
}
int	CTextureMgr::GetTextureNum()
{
	return m_iTextureNum;
}