#include "StdAfx.h"
#include "Title.h"

#include "TextureMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "Device.h"
#include "SDevice.h"

#define IMG_CNT 309.f
unsigned int __stdcall ImgLoadThread(void* pArg)
{
	CTitle*			pTitle = (CTitle*)pArg;

	EnterCriticalSection(&pTitle->GetCrt());

	if(FAILED(CTextureMgr::GetInstance()->ReadImage(L"../Data/ImgPath.txt")))
	{
		ERR_MSG(L"Texture Load Failed");
		return E_FAIL;
	}
	if(FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/Stage/1.png", 
		L"Stage1", TEX_SINGLE )))
	{
		ERR_MSG(L"Single Texture Load Failed");
		return E_FAIL;
	}
	if(FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Tile/%d.png", 
		L"BACK", TEX_MULTI, L"Tile", 4)))
	{
		ERR_MSG(L"tile 타일 텍스쳐 생성 실패");
		return E_FAIL;
	}
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/Stage/0.png", L"Field", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/Stage/2.png", L"Stage2", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/Stage/3.png", L"Stage3", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/StageUI/Stagebar.png", L"Stagebar", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/StageUI/Activebar.png", L"Activebar", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/UIButton/OptionButt.png", L"OptionButt", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/UIButton/StopButt.png", L"StopButt", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/StageUI/StartButt.png", L"StartButt", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/etc/TowerStructuer.png", L"TowerStructuer", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/etc/TowerStructuer2.png", L"TowerStructuer2", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/etc/TowerUp.png", L"TowerUp", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/etc/stage1G.png", L"stage1G", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/etc/Upgrade(Move).png", L"Upgrade(Move)", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/etc/Upgrade(Basic).png", L"Upgrade(Basic)", TEX_SINGLE );

	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/etc/Archor3.png", L"Archor3", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/etc/Knight3.png", L"Knight3", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/etc/Magic3.png", L"Magic3", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/etc/Bomb3.png", L"Bomb3", TEX_SINGLE );

	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/etc/Archor4-1.png", L"Archor4-1", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/etc/Archor4-2.png", L"Archor4-2", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/etc/Knight4-1.png", L"Knight4-1", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/etc/Knight4-2.png", L"Knight4-2", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/etc/Magic4-1.png", L"Magic4-1", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/etc/Magic4-2.png", L"Magic4-2", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/etc/Bomb4-1.png", L"Bomb4-1", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/etc/Bomb4-2.png", L"Bomb4-2", TEX_SINGLE );

	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/StageUI/RellyPoint.png", L"RellyPoint", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/StageUI/dontmove.png", L"Dontmove", TEX_SINGLE );

	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/StageUI/Forest1.png", L"Forest1", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/StageUI/Forest2.png", L"Forest2", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/StageUI/Snow.png", L"Snow", TEX_SINGLE );

	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/etc/2/HpBar3.png", L"HpBar", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/etc/2/HpBar4.png", L"HpBar2", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/etc/2/HpEmpty.png", L"HpEmpty", TEX_SINGLE );

	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/etc/2/bullet.png", L"bullet", TEX_SINGLE );

	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/Mill.png", L"Mill", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/Met.png", L"Met", TEX_SINGLE );

	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/window/ministop.png", L"MiniStop", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/window/skillwnd.png", L"SkillWnd", TEX_SINGLE );

	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/Field/StageFlag0.png", L"StageFlag", TEX_SINGLE );
	//CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/window/skillwnd.png", L"SkillWnd", TEX_SINGLE );

	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/window/Stage1Wnd.png", L"Stage1Wnd", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/window/Stage2Wnd.png", L"Stage2Wnd", TEX_SINGLE );

	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/font/PGold.png", L"PGold", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/font/GoldNumber.png", L"GoldNumber", TEX_SINGLE );

	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/window/StageMove0.png", L"StageMove0", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/window/StageMove1.png", L"StageMove1", TEX_SINGLE );

	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/window/UU.png", L"UU", TEX_SINGLE );
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/Win.png", L"Win", TEX_SINGLE );



	LeaveCriticalSection(&pTitle->GetCrt());
	//_endthreadex(0);

	return 0;
}

CTitle::CTitle(void)
{
}

CTitle::~CTitle(void)
{
	Release();
}
HRESULT CTitle::Initialize(void)
{
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/Title/shape2818.png", L"TitleBack", TEX_SINGLE);
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/Title/shape2858.png", L"TitleName", TEX_SINGLE);
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/Title/loadingbar.png", L"loadingbar", TEX_SINGLE);
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Single/UI/Title/loading.png", L"loading", TEX_SINGLE);

	CSDevice::GetInstance()->SoundPlay(SD_TITLE, 1);		// 타이틀 비지엠

	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, ImgLoadThread, this, 0, NULL);

	return S_OK;
}
int CTitle::Progress(void)		
{
	int iTextureNum = CTextureMgr::GetInstance()->GetTextureNum();

	if(CKeyMgr::GetInstance()->KeyDown(KEY_1) && iTextureNum == (int)IMG_CNT)
	{
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);

		DeleteCriticalSection(&m_Crt);

		CSceneMgr::GetInstance()->Initialize(SC_FIELD);

		return 0;
	}

	return 0;
}
void CTitle::Render(void)		
{
	const TEXINFO* pTexture = CTextureMgr::GetInstance()->GetTexture(L"TitleBack");

	if(pTexture == NULL)
		return;

	float fX = pTexture->tImgInfo.Width / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;

	D3DXMATRIX		matTrans;

	D3DXMatrixTranslation(&matTrans, WINCX / 2.f, WINCY /2.f, 0.f);

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, 
		NULL, &D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	/*D3DXMatrixTranslation(&matTrans, 50.f, 500.f, 0.f);


	wsprintf(m_szNum, L"NUM : %d", CTextureMgr::GetInstance()->GetTextureNum());
	D3DXMatrixTranslation(&matTrans, 50.f, 400.f, 0.f);
	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(), m_szNum, lstrlen(m_szNum), 
		NULL, NULL, D3DCOLOR_ARGB(255, 0, 255, 255));*/
	/* 타이틀 */
	pTexture = CTextureMgr::GetInstance()->GetTexture(L"TitleName");

	if(pTexture == NULL)
		return;

	fX = pTexture->tImgInfo.Width / 2.f;
	fY = pTexture->tImgInfo.Height / 2.f;

	D3DXMatrixTranslation(&matTrans, WINCX / 2.f, WINCY / 3.f, 0.f);

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, 
		NULL, &D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	/* 로고바 테두리 */
	pTexture = CTextureMgr::GetInstance()->GetTexture(L"loadingbar");

	if(pTexture == NULL)
		return;

	fX = pTexture->tImgInfo.Width / 2.f;
	fY = pTexture->tImgInfo.Height / 2.f;

	//D3DXMATRIX		matTrans;

	D3DXMatrixTranslation(&matTrans, WINCX / 2.f, WINCY /1.4f, 0.f);

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, 
		NULL, &D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	///* 로고바 차는거 */
	pTexture = CTextureMgr::GetInstance()->GetTexture(L"loading");

	if(pTexture == NULL)
		return;

	fX = pTexture->tImgInfo.Width / 2.f;
	fY = pTexture->tImgInfo.Height / 2.f;

	int iTextureNum = CTextureMgr::GetInstance()->GetTextureNum();
	RECT rc = {0, 0, LONG(pTexture->tImgInfo.Width * ( iTextureNum / IMG_CNT)), 18 };

	D3DXMatrixTranslation(&matTrans, WINCX / 2.f, WINCY /1.4f, 0.f);

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, 
		&rc, &D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
}
void CTitle::Release(void)		
{
	CSDevice::GetInstance()->SoundStop(SD_TITLE);		// 타이틀 비지엠
}
CRITICAL_SECTION CTitle::GetCrt(void)
{
	return m_Crt;
}


