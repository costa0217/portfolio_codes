#pragma once
#include "Scene.h"
class CSCTitle :
	public CScene
{
public:
	CSCTitle(SCENE_ID eID, CDirectXFramework* pFrameWork);
	~CSCTitle();

public:
	//-----------------------------------scene--------------------------
	virtual bool Begin();
	virtual bool End();

	virtual void Animate(float fTimeElapsed);

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void ProcessInput(float fTimeElapsed);

private:
	CUIObject* m_pUI{ nullptr };

};

