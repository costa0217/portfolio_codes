#pragma once

#include "Light.h"
#include "RotateComponent.h"

struct SPOT_LIGHT_DS_CB {
	XMMATRIX LightProjection;
	float SpotCosOuterCone;//�� ���� ����<OuterAngle>
	float SpotSinOuterCone;//<OuterAngle>
	XMFLOAT2 tempData;//float4�� ���߱� ���� ������
};

struct SPOT_LIGHT_PS_CB {
	XMFLOAT3 SpotLightPos;//����
	float SpotLightRangeRcp;//���� ����

	XMFLOAT3 SpotLightDir;
	float SpotCosOuterCone;//�� ���� ����<OuterAngle>

	XMFLOAT3 SpotLightColor;//�� ����
	float SpotCosConeAttRcp;//���� ���� ����<InnerAngle>
};

struct SPOT_LIGHT {
	float SpotLightRange;
	XMFLOAT3 SpotLightColor;
	float fOuterAngle;
	float fInnerAngle;
};
class CSpotLight : public CLight {
public:
	bool Begin();
	virtual bool End();

	//instance buffer controll
	virtual void SetBufferInfo(void** ppMappedResources, int& nInstance,  CCamera* pCamera);

	//light info setter
	void SetSpotLightData(SPOT_LIGHT data) { m_SpotData = data; }
	virtual void SetLength(float len);
	float GetLength() { return m_SpotData.SpotLightRange; }
	virtual void SetRange(float outer, float inner = 0.0f);
	float GetInner() { return m_SpotData.fInnerAngle; }
	void SetInner(float inner);
	float GetOuter() { return m_SpotData.fOuterAngle; }
	void SetOuter(float outer);
	virtual void SetColor(float r, float g, float b);//color
	virtual XMFLOAT3 GetColor();
	static CSpotLight* CreateSpotLight(float SpotLightRange, XMFLOAT3 SpotLightColor, float fOuterAngle, float fInnerAngle);

	void PickingProc();
	virtual void LoadInfo();
private:
	float m_fSpotLightRangeRcp{ 0.0 };
	float m_fSpotCosOuterCone{ 0.0 };
	float m_fSpotSinOuterCone{ 0.0 };
	float m_fSpotCosConeAttRcp{ 0.0 };

	//data
	SPOT_LIGHT m_SpotData{};
	//data

	XMMATRIX m_xmmtxScale;
public:
	CSpotLight();
	virtual ~CSpotLight();
};