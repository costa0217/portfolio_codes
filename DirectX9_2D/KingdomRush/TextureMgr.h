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


	HRESULT InsertTexture(const wstring& wstrFilePath,		// ���
						 const wstring& wstrObjKey,			// OBJKEY��
						 TEXTYPE _eType,					// s or m
						 const wstring& wstrStateKey = L"",	// obj ���� ��
						 const int& iCount = 0);			// �̹��� ����

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
