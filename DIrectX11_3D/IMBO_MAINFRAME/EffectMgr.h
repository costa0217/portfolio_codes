#pragma once


#include "SingleTon.h"
#include "stdafx.h"
#include "EffectMgr.h"

class CMyEffect;
class CEffectMgr
	: public CSingleTonBase<CEffectMgr>
{

public:
	bool Begin();
	bool End();

private:
	map<const TCHAR*, vector<CMyEffect*>> m_mapProto;			// �����
	map<const TCHAR*, vector<CMyEffect*>> m_mapEffectList;		// ��Ƶδ¿�
	vector<CMyEffect*> m_vecPlayEffect;	//�÷��̿�
	

public:
	vector<CMyEffect*>* Find_ProtoEffectList(TCHAR* pKey);	
	vector<CMyEffect*>* Find_EffectList(TCHAR* pKey);		

	void Update_PlayEffectList(float fTimeDelta);

	void Load_EffectData(TCHAR* pName, TCHAR* pKey, int iCreatNum = 5);
	void Create_Effect(TCHAR* pKey);
	void Play_Effect(TCHAR* pKey, XMVECTOR xmvPos, XMVECTOR xmvRot, XMVECTOR xmvScl = XMVectorSet(1,1,1,1));
	void Play_Effect(TCHAR* pKey, CGameObject* pParent);

public:
	CEffectMgr();
	virtual ~CEffectMgr();
};