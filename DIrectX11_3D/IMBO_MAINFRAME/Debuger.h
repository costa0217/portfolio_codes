#pragma once

#include "SingleTon.h"

#include "GameObject.h"
#include "BoundingBox.h"
#include "CoordinateSys.h"

#include "RenderContainer.h"
//#include "RenderContainerSeller.h"
#include "DirectXFramework.h"

//font
#include "../../Assets/import/FW1FontWrapper_1_1/FW1FontWrapper.h"
//debug data
#include "DebugFontData.h"
#include "DebugTextureData.h"
#include "DebugTexture.h"

class CDebuger : public CSingleTonBase<CDebuger>{

public:
	bool Begin();
	bool End();

	void RegistToDebugRenderContainer(CGameObject* pObject);
	void RegistCoordinateSys(FXMMATRIX mtx);
	void RegistAABB(BoundingBox& aabb, utag ut = UTAG_DEFAULT);
	void RegistOBB(BoundingOrientedBox& obb, utag ut = UTAG_DEFAULT);

	void DebugRender( CCamera* pCamera);
	
	void RenderAABB( CCamera* pCamera);
	void RenderCoordinateSys( CCamera* pCamera);
	void RenderLightVolume( CCamera* pCamera);
	void ClearDebuger();

	//utill func
	void AddText(float fontSize, float posX, float posY, UINT32 color, TCHAR * msg, ...);
	void RenderText();
	void AddGameText(float fontSize, float posX, float posY, UINT32 color, TCHAR * msg, ...);
	void RenderGameText();
	void AddTexture(XMFLOAT2 fLeftTop, XMFLOAT2 fRightBottom, ID3D11ShaderResourceView* pSRV);
	void AddDepthTexture(XMFLOAT2 fLeftTop, XMFLOAT2 fRightBottom, ID3D11ShaderResourceView* pSRV);

	void RenderTexture();

	//messagebox
	int DebugMessageBox(std::string _title, std::string _message);
	int DebugGMessageBox(TCHAR* title, TCHAR* message, ...);


	void start_Timemeasurement();
	void end_Timemeasurement(WCHAR* msg);
private:
	std::chrono::system_clock::time_point start;
	int Timemeasurement_count{ 0 };

	mapRC m_mDebugRenderContainer;
	
	ID3D11RasterizerState* m_pRasterizerState{ nullptr };

	//aabb ������ ���� ��ü.
	CBoundingBox** m_ppBoundingBox{ nullptr };
	UINT m_nAABB{ 0 };
	//CoordinateSys ������ ���� ��ü.
	CCoordinateSys** m_ppCoordinateSys{ nullptr };
	UINT m_nCoordinateSys{ 0 };

	//���� ����� ���� ���� state.
	ID3D11BlendState* m_pLightBlendState{ nullptr };
	ID3D11DepthStencilState* m_pLightDepthStencilState{ nullptr };
	ID3D11RasterizerState* m_pLightRasterizerState{ nullptr };
	//���� ���� ����
	ID3D11BlendState* m_pPreBlendState{ nullptr };
	float* m_pPreBlendFactor{ nullptr };
	UINT m_PreSampleMask{ 0 };
	ID3D11DepthStencilState* m_pPreDepthStencilState{ nullptr };
	UINT m_PreStencilRef{ 0 };
	ID3D11RasterizerState* m_pPreRasterizerState{ nullptr };

//font
	IFW1Factory*								 m_pFW1FontFactory{ nullptr };

	IFW1FontWrapper*							m_pFW1Font{ nullptr };
	IFW1TextRenderer*							 m_pFWTextRender{ nullptr };
	IFW1GlyphProvider*							 m_pFW1Glyphrovider{ nullptr };

	queue<CDebugFontData>						m_qDebugFontData;
	queue<CDebugFontData>						m_qGameFontData;
	UINT m_pivot{ FW1_TOP | FW1_LEFT };

//debug texture
	CDebugTexture* m_pDebugTextureObj{ nullptr };

	 CTexture* m_pDebugTexture{ nullptr };

	queue<CDebugTextureData>					m_qDebugTextureData;
	queue<CDebugTextureData>					m_qDebugDepthTextureData;
public:
	CDebuger();
	virtual ~CDebuger();


};
