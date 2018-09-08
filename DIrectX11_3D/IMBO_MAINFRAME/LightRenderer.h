#pragma once
#include "DXObject.h"
#include "Camera.h"
#include "RenderContainer.h"
#include "Shadow.h"

class CLightRenderer : public DXObject {
public:
	bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState();

	//void Excute( CCamera* pCamera,  CCamera* pLightCam = nullptr, ID3D11ShaderResourceView* pShadowSRV = nullptr);
	void Excute( CCamera* pCamera, CShadow* m_pShadow = nullptr);
private:
	//rendercontainer map!
	mapRC m_mRenderContainer;

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

public:
	CLightRenderer();
	virtual ~CLightRenderer();
};