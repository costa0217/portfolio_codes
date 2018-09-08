#pragma once
#include "Mesh.h"

struct VS_VB_SCREEN {
	XMFLOAT4 xmf3Position;
	XMFLOAT2 xmf3TextCoord;
};


class CDeferredMesh : public CMesh {

public:
	//----------------------------dxobject-----------------------------
	//----------------------------dxobject-----------------------------

	//---------------------------mesh----------------------------------
	//begin func
	virtual bool CreateVertexBuffer();
	virtual bool CreateIndexBuffer();
	//begin func
	//---------------------------mesh----------------------------------
private:
	//----------------------vertex buffers---------------------------
	ID3D11Buffer* m_pd3dPositionBuffer{ nullptr };
	ID3D11Buffer* m_pd3dUVBuffer{ nullptr };
	//----------------------vertex buffers---------------------------

public:
	CDeferredMesh();
	virtual ~CDeferredMesh();
};