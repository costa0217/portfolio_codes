#pragma once
#include "Mesh.h"
class CTrailMesh :
	public CMesh
{
public:
	CTrailMesh();
	virtual ~CTrailMesh();


public:
	//begin func
	virtual bool CreateVertexBuffer();
	virtual bool CreateIndexBuffer();

private:
	//----------------------vertex buffers---------------------------
	ID3D11Buffer* m_pd3dPositionBuffer{ nullptr };
	ID3D11Buffer* m_pd3dNormalBuffer{ nullptr };
	ID3D11Buffer* m_pd3dUVBuffer{ nullptr };
};

