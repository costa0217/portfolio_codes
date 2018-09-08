#pragma once
#include "DXObject.h"
#include "Camera.h"

struct stFinalPassCB {
	float fMiddleGrey;
	float fLumWhiteSqr;
	float fBloomScale;
	float pad;
};

class CPostProcessingFinalPass : public DXObject {
public:
	bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState();

	void Excute( CCamera* pCamera);
	void ChangeStaticData();
	void ResizeBuffer();
	void ReleaseBuffer();

	//	void SetBloomThreshold(float fBloomThreshold) { m_fBloomThreshold = fBloomThreshold; }
private:
	float m_fMeddleGrey{0.01f};
	float m_fWhite{1.0f};
	float m_fBloomScale{2.0f};
	//rendercontainer map!
	mapRC m_mRenderContainer;
	 CBuffer* m_pFinalPassCB{ nullptr };
public:
	CPostProcessingFinalPass();
	virtual ~CPostProcessingFinalPass();

	void SetBloomFinalPassValues(float fMeddleGrey, float fWhite, float fBloomScale);
	void SetMiddleGrey(float BLOOMMiddleGrey) { m_fMeddleGrey = BLOOMMiddleGrey; }
	float GetMiddleGrey() { return m_fMeddleGrey; }
	void SetWhite(float BLOOMWhith) { m_fWhite= BLOOMWhith; }
	float GetWhite() { return m_fWhite; }
	void SetBloomScale(float BLOOMScale) { m_fBloomScale = BLOOMScale; }
	float GetBloomScale() { return m_fBloomScale; }
};