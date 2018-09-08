#pragma once
#include "GameObject.h"

enum light_id {
	//LIGHT_AMBIENT, ambient light ������ DIRECTIONAL mesh�ȿ� ����ִ�.
	LIGHT_DIRECTIONAL,
	LIGHT_POINT,
	LIGHT_SPOT,
	LIGHT_CAPSULE,
	LIGHT_END
};

class CLight : public CGameObject {
public:
	bool Begin();
	virtual bool End() { return CGameObject::End(); };

	//instance buffer controll
	virtual void SetBufferInfo(void** ppMappedResource, int& nInstance,  CCamera* pCamera);
	
	//light info setter
	virtual void SetLength(float len) = 0;
	virtual void SetRange(float outer, float inner = 0.0f) = 0;
	virtual void SetColor(float r, float g, float b) = 0;//color
	virtual XMFLOAT3 GetColor() = 0;
	light_id GetLightID() { return m_lightid; };

	void PickingProc() = 0;

protected:
	//light�� ���۸� ���� ������ �ʴ´�.
	//rendercontainer�� ���� ����
	light_id m_lightid{ light_id::LIGHT_END };

public:
	CLight(string name):CGameObject(name, tag::TAG_LIGHT) { m_lightid = light_id::LIGHT_END; }
	virtual ~CLight() {}
};
