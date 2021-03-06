#ifndef StageScene_h__
#define StageScene_h__

#include "Defines.h"
#include "Scene.h"

class CStageScene : public Engine::CScene
{
private:
	explicit CStageScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStageScene(void);
public:
	virtual HRESULT Init_Scene(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
public:
	static CStageScene* Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	static vector<TCHAR*> vecObjName; // ������
private:
	HRESULT Ready_Lighting(void);
	HRESULT Ready_Environment(void);
	HRESULT Ready_GameLogic(void);
	HRESULT Ready_Effect(void);
	
private:
	void Load_EnvEffect();

public:
	virtual _ulong Release(void);
};


#endif // StageScene_h__
