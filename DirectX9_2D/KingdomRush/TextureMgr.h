#pragma once
#include "Include.h"

class CTexture;
class CTextureMgr
{
DECLARE_SINGLETON(CTextureMgr)

private:
	map<wstring, CTexture*>		m_MapTexture;
	int							m_iTextureNum;
	
public:
	HRESULT	ReadImage(wstring wstrImgPath);


	HRESULT InsertTexture(const wstring& wstrFilePath,		// 경로
						 const wstring& wstrObjKey,			// OBJKEY값
						 TEXTYPE _eType,					// s or m
						 const wstring& wstrStateKey = L"",	// obj 상태 값
						 const int& iCount = 0);			// 이미지 개수

	const TEXINFO* GetTexture(const wstring& wstrObjKey, 
								const wstring& wstrStateKey = L"", 
								const int& iCount = 0);

	size_t GetImageCount(const wstring& wstrObjKey, const wstring& wstrStateKey = L"");
	int	GetTextureNum();

	void Release(void);

private:
	CTextureMgr(void);
	~CTextureMgr(void);
};
