#pragma once
#include "Light.h"

/*
float3 AmbientDown	: packoffset( c0 );
float3 AmbientRange	: packoffset( c1 );
float3 DirToLight		: packoffset( c2 );
float3 DirLightColor	: packoffset( c3 );
�ᱹ ����ϴ°� �� 4������. 
*/
struct DIRECTIONAL_AMBIENT_LIGHT {
	XMFLOAT4 m_DirToLight;
	XMFLOAT4 m_DirLightColor;
	XMFLOAT4 m_DirLightPower;

	XMFLOAT4 m_AmbientDown;
	XMFLOAT4 m_AmbientRange;
	XMFLOAT4 m_AmbientUp;
	XMFLOAT4 m_AmbientColor;
};

class CDirectionalLight : public CLight {
public:
	bool Begin(DIRECTIONAL_AMBIENT_LIGHT& light_info);
	virtual bool End();

	//instance buffer controll
	virtual void SetBufferInfo(void** ppMappedResources, int& nInstance,  CCamera* pCamera);

	//light info setter
	virtual void SetLength(float len);
	virtual void SetRange(float outer, float inner = 0.0f);
	virtual void SetColor(float r, float g, float b);//color
	virtual XMFLOAT3 GetColor();

	virtual bool IsVisible( CCamera* pCamera);
	void PickingProc();

	virtual void LoadInfo();

	//offset length set get
	void SetOffsetLength(float offsetLength) { m_fOffsetLength = -offsetLength; }
	float GetOffsetLength() { return -m_fOffsetLength; }
	//offset length set get
private:
	float m_fOffsetLength{ 200.f };
	//data
	DIRECTIONAL_AMBIENT_LIGHT m_Directional_AmbientData;
	//data

public:
	CDirectionalLight();
	virtual ~CDirectionalLight();
};