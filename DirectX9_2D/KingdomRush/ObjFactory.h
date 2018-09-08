#pragma once

class CObj;
template<typename T>
class CObjFactory
{
public:
	static CObj*	CreateObj(void)
	{
		CObj*	pObj = new T;
		pObj->Initialize();

		return pObj;
	}
	static CObj*	CreateObj(D3DXVECTOR3 _vPos)
	{
		CObj*	pObj = new T;
		pObj->SetPos(_vPos);
		pObj->Initialize();
		
		return pObj;
	}

public:
	CObjFactory(void){}
	~CObjFactory(void){}
};
