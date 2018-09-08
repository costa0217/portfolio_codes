#include "stdafx.h"
#include "TrailMesh.h"


CTrailMesh::CTrailMesh()
{
}


CTrailMesh::~CTrailMesh()
{
}

bool CTrailMesh::CreateVertexBuffer()
{
	m_nVertices = 80;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	m_pVertices = new XMFLOAT3[m_nVertices];
	XMFLOAT2 pxmf2TexCoords[80];

	for (int i = 0; i < 80; ++i)
	{
		if (i < 40)
		{
			m_pVertices[i] = XMFLOAT3(0.f, 0.f, 0.f);
			pxmf2TexCoords[i] = XMFLOAT2(1 - (i  * 0.025f), 1.f);
		}
		else
		{
			m_pVertices[i] = XMFLOAT3(0.f, 0.f, 0.f);
			pxmf2TexCoords[i] = XMFLOAT2(1 - ((i - 40) * 0.025f), 0.f);
		}
	}

	m_pd3dPositionBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, m_pVertices, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dUVBuffer = CreateBuffer(sizeof(XMFLOAT2), m_nVertices, pxmf2TexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);


	XMVECTOR pxmvNormals[80];
	CalculateVertexNormal(pxmvNormals);
	XMFLOAT3 pxmf3Normals[80];
	for (int j = 0; j < 80; ++j) {
		pxmf3Normals[j] = XMFLOAT3(0, 1, 0);
	}
	m_pd3dNormalBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, pxmf3Normals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);


	ID3D11Buffer *pd3dBuffers[3] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dUVBuffer };
	UINT pnBufferStrides[3] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[3] = { 0, 0, 0 };
	AssembleToVertexBuffer(3, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	if (m_ppd3dVertexBuffers)	return true;

	return false;
}

bool CTrailMesh::CreateIndexBuffer()
{
	return true;
}
