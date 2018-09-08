#pragma once
#include "Camera.h"


class CFreeCamera :public CCamera{
public:
	//----------------------------dxobject-----------------------------
	virtual bool Begin();
	virtual bool End();

	virtual void UpdateShaderState();//CCamera::UpdateShaderStateȣ�� �� �ؾ��Ѵ�. 
	//----------------------------dxobject-----------------------------


	//---------------------------------------ī�޶� ����---------------------------------
	//ȸ��
	virtual void Rotate(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	//Ani ī�޶� �׼�

	//----------------------------------------���� �Ҹ� �Լ���------------------------
	//player����ٴϱ�
	virtual void Update(float fTimeElapsed);
	virtual void ProcessInput(float fTimeElapsed);
	//----------------------------------------���� �Ҹ� �Լ���------------------------
	//debug���� ī�޶� �׸��� 
	virtual void Render() {};
	//---------------------------------------ī�޶� ����---------------------------------

private:
	void CameraInputProcess(float fTimeDelta);
	void FixCamera();
	void CalcultateMouseMoveValue();

	virtual void CameraStartVibration(float fVibeYvalue, float fVibeSpeed) ;
	virtual void CameraVibration(float fTime);
	virtual void AttackZoomInOut(float fTime);


	POINT m_ptOldMousePos;

public:
	CFreeCamera();
	virtual ~CFreeCamera();

};