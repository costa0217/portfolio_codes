#ifndef WireChest_h__
#define WireChest_h__

#include "Wire.h"


class CWireStomach;


class CWireChest
	: public CWire
{
private:
	explicit CWireChest(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CWireChest(void);
public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
public:
	virtual void TransformMatrix(void);
	virtual void Move();
public:
	static CWireChest* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	D3DXVECTOR3 m_vIndex_0_Pos;
	D3DXVECTOR3 m_vIndex_1_Pos;

	typedef list<CGameObject*>				OBJECTLIST;
	OBJECTLIST*	m_WireStomachList;

	CWireStomach*  m_WireStomach;
public:
	void SetIndex_0(D3DXVECTOR3 vIndex_0_Pos){m_vIndex_0_Pos = vIndex_0_Pos;}
	void SetIndex_1(D3DXVECTOR3 vIndex_1_Pos){m_vIndex_1_Pos = vIndex_1_Pos;}
	void SetWireStomachList(OBJECTLIST* WireStomach){m_WireStomachList = WireStomach;}
	void CreateWireStomach();

	CWireStomach*  Getm_WireStomach(){return m_WireStomach;}

};

#endif // WireChest_h__
