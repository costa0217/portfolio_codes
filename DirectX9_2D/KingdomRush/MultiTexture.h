#pragma once
#include "texture.h"

class CMultiTexture :
	public CTexture
{
private:
	map<wstring, vector<TEXINFO*>>		m_MapMultiTexture;

public:
	CMultiTexture(void);
	~CMultiTexture(void);

public:
	size_t GetImageCount(const wstring&  wstrStateKey);

public:
	virtual HRESULT InsertTexture(const wstring& wstrFilePath, 
		const wstring& wstrStateKey = L"", 
		const int& iCount = 0);
	
	virtual const TEXINFO* GetTexture(const wstring& wstrStateKey = L"",
		const int& iCount = 0);
	
	virtual void Release(void);

};
