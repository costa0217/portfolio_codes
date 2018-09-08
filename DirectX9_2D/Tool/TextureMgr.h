#pragma once
#include "Include.h"

class CTexture;
class CTextureMgr
{
DECLARE_SINGLETON(CTextureMgr)

private:
	map<wstring, CTexture*>		m_MapTexture;
	
public:
	HRESULT InsertTexture(const wstring& wstrFilePath,		
						 const wstring& wstrObjKey,			
						 TEXTYPE _eType,					
						 const wstring& wstrStateKey = L"",	
						 const int& iCount = 0);			

	const TEXINFO* GetTexture(const wstring& wstrObjKey, 
								const wstring& wstrStateKey = L"", 
								const int& iCount = 0);

	void Release(void);

private:
	CTextureMgr(void);
	~CTextureMgr(void);
};
