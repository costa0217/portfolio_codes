#ifndef WireStomach_h__
#define WireStomach_h__

#include "Wire.h"


class CWireTail;

class CWireStomach
	: public CWire
{
private:
	explicit CWireStomach(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CWireStomach(void);
public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
public:
	virtual void TransformMatrix(void);
	virtual void Move();
public:
	static CWireStomach* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	D3DXVECTOR3 m_vIndex_0_Pos;
	D3DXVECTOR3 m_vIndex_1_Pos;

	typedef list<CGameObject*>				OBJECTLIST;
	OBJECTLIST*	m_WireTailList;

	CWireTail*  m_WireTail;
public:
	void SetIndex_0(D3DXVECTOR3 vIndex_0_Pos){m_vIndex_0_Pos = vIndex_0_Pos;}
	void SetIndex_1(D3DXVECTOR3 vIndex_1_Pos){m_vIndex_1_Pos = vIndex_1_Pos;}

	void SetWireTailList(OBJECTLIST* WireTail){m_WireTailList = WireTail;}
	void CreateWireTail();

	CWireTail*  GetWireTail(){return m_WireTail;}
};


#endif // WireStomach_h__
