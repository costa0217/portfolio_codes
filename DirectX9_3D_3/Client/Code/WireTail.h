#ifndef WireTail_h__
#define WireTail_h__

#include "Wire.h"

class CWireTail
	: public CWire
{
private:
	explicit CWireTail(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CWireTail(void);
public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
public:
	virtual void TransformMatrix(void);
	virtual void Move();
public:
	static CWireTail* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	D3DXVECTOR3 m_vIndex_0_Pos;
	D3DXVECTOR3 m_vIndex_1_Pos;
public:
	void SetIndex_0(D3DXVECTOR3 vIndex_0_Pos){m_vIndex_0_Pos = vIndex_0_Pos;}
	void SetIndex_1(D3DXVECTOR3 vIndex_1_Pos){m_vIndex_1_Pos = vIndex_1_Pos;}
};

#endif // WireTail_h__
