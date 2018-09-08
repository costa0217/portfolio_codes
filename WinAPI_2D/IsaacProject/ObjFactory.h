#pragma once
#include "Obj.h"

template<typename T>
class CObjFactory
{
public:
	static CObj* CreateObj(void)
	{
		CObj* pObj = new T;
		pObj->Initialize();

		return pObj;
	}
	static CObj* CreateObj(float _fX, float _fY)
	{
		CObj* pObj = new T;
		pObj->SetPos(_fX, _fY);
		pObj->Initialize();
		

		return pObj;
	}
	static CObj* CreateObj(float _fX, float _fY, string _strKey)
	{
		CObj* pObj = new T;
		pObj->SetPos(_fX, _fY);
		pObj->Initialize();		
		pObj->SetStrKey(_strKey);
		
		return pObj;
	}
	static CObj* CreateObj(float _fX, float _fY, float _fAngle)
	{
		CObj* pObj = new T;
		pObj->SetPos(_fX, _fY);
		pObj->Initialize();
		pObj->SetAngle(_fAngle);
		
		return pObj;
	}
	static CObj* CreateObj(float _fX, float _fY, float _fAngle, int _iReach, int _iHP)
	{
		CObj* pObj = new T;
		pObj->SetPos(_fX, _fY);
		pObj->Initialize();
		pObj->SetAngle(_fAngle);
		pObj->SetReach(_iReach);
		pObj->SetHP(_iHP);
		
		return pObj;
	}

	static CObj* CreateObj(float _fX, float _fY, float _fAngle, int _iTargetDir)
	{
		CObj* pObj = new T;
		pObj->SetPos(_fX, _fY);
		pObj->Initialize();
		pObj->SetTargetDir(_iTargetDir);
		pObj->SetAngle(_fAngle);
		
		return pObj;
	}
	static CObj* CreateObj(float _fX, float _fY, int _iDrawID)
	{
		CObj* pObj = new T;
		pObj->SetPos(_fX, _fY);
		pObj->SetDrawID(_iDrawID);
		pObj->Initialize();
		
		return pObj;
	}
	//문만들기
	static CObj* CreateObj(float _fX, float _fY, int _iDrawID, int _iIndex)
	{
		CObj* pObj = new T;
		pObj->SetPos(_fX, _fY);
		pObj->SetIndex(_iIndex);
		pObj->Initialize();
		pObj->SetDrawID(_iDrawID);
			
		return pObj;
	}
	static CObj* CreateObj(float _fX, float _fY, int _iDrawID, int _iIndex, int _iRoomNum)
	{
		CObj* pObj = new T;
		pObj->SetPos(_fX, _fY);
		pObj->SetIndex(_iIndex);
		pObj->Initialize();
		pObj->SetDrawID(_iDrawID);
		pObj->SetStageNum(_iRoomNum);
		
		return pObj;
	}
private:
	CObjFactory(void){}
	~CObjFactory(void){}
};
