#ifndef WireHead_h__
#define WireHead_h__

#include "Wire.h"


class CWireChest;


class CWireHead
	: public CWire
{
private:
	explicit CWireHead(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CWireHead(void);
public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
public:
	virtual void TransformMatrix(void);
	virtual void Move();
public:
	static CWireHead* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	Engine::CTransform*		m_pHeliInfo;
	typedef list<CGameObject*>				OBJECTLIST;
	OBJECTLIST*	m_WireChestList;
	OBJECTLIST*	m_WireStomachList;
	OBJECTLIST*	m_WireTailList;

	CWireChest* m_WireChest; 
public:
	void SetHeliInfo(Engine::CTransform* pHeliInfo);
	void SetWireChestList(OBJECTLIST* WireChest){m_WireChestList = WireChest;}
	void SetWireStomachList(OBJECTLIST* WireStomach){m_WireStomachList = WireStomach;}
	void SetWireTailList(OBJECTLIST* WireTail){m_WireTailList = WireTail;}
	void CreateWireChest();

	void SetParentMat(D3DXMATRIX* pParentMat){m_pParentMat = pParentMat;}
	
	CWireChest* GetWireChest(){return m_WireChest;}
	OBJECTLIST* GetWireStomachList(){return m_WireStomachList;}
	OBJECTLIST* GetWireTailList(){return m_WireTailList;}

private:
	D3DXMATRIX* m_pParentMat;
};

#endif // WireHead_h__
