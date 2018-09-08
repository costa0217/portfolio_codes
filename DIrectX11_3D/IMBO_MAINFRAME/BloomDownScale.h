#pragma once
#include "DXObject.h"
#include "Camera.h"

struct stBloomStaticData {
	UINT nTotalPixels;//domain
	UINT nGroupSize;//groupsize
	float fBloomThreshold;//bloom �Ӱ谪
	UINT pad1;
};
struct stBloomDynamicData {
	float fAdaptation;//���� ��
	UINT pad[3];
};

class CBloomDownScale : public DXObject {
public:
	bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState();

	void Excute();
	void ResizeBuffer();
	void ReleaseBuffer();

	void SetAdaptation(float fAdaptation);

	void SwapAdaptationBuff();

private:
	void ChangeStaticData();

	UINT m_nWidth{ 0 };
	UINT m_nHeight{ 0 };
	UINT m_nDownScaleGroup{ 0 };
	float m_fAdaptation{ 0.f };
	float m_fBloomThreshold{ 0.f };

	//first pass
	ID3D11Buffer* m_pFirstPassBuffer{ nullptr };
	ID3D11UnorderedAccessView* m_pFirstPassAvgLumUAV{ nullptr };
	ID3D11ShaderResourceView* m_pFirstPassAvgLumSRV{ nullptr };
	 CBuffer* m_pBloomStaticCB{ nullptr };
	 CBuffer* m_pBloomDynamicCB{ nullptr };
	//ID3D11Buffer* m_pFirstPassCB{ nullptr };

	//1
	ID3D11Texture2D			 *m_pd3dtxtDownCasting{ nullptr };
	ID3D11ShaderResourceView *m_pd3dsrvDownCasting{ nullptr };
	ID3D11UnorderedAccessView*m_pd3duavDownCasting{ nullptr };
	CComputeShader* m_pFirstPassComputeShader{ nullptr };
	//first pass

	//Second Pass
	ID3D11Buffer* m_pSecondPassBuffer{ nullptr };
	ID3D11UnorderedAccessView* m_pSecondPassUAV{ nullptr };
	ID3D11ShaderResourceView* m_pSecondPassSRV{ nullptr };

	//���� ����ֵ� �� 
	ID3D11Buffer* m_pPrevAvgLumBuffer{ nullptr };
	ID3D11UnorderedAccessView* m_pPrevAvgLumUAV{ nullptr };
	ID3D11ShaderResourceView* m_pPrevAvgLumSRV{ nullptr };
	CComputeShader* m_pSecondPassComputeShader{ nullptr };
	//Second Pass
public:
	CBloomDownScale();
	virtual ~CBloomDownScale();

	void SetBloomThreshold(float fBloomThreshold);
	float GetBloomThreshold() { return m_fBloomThreshold; };
};