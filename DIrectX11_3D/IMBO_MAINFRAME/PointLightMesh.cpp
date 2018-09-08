#include "stdafx.h"
#include "PointLightMesh.h"

#define WIDTH 5
#define HEIGHT 5
#define DEPTH 5

bool CPointLightMesh::CreateVertexBuffer() {
	m_nVertices = 2;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST;

	m_ppd3dVertexBuffers = nullptr;
	//derectional light mesh�� ���� 0���� hlsl�ڵ忡�� �˾Ƽ� �����Ѵ�.
	
	float fx = WIDTH*0.5f, fy = HEIGHT*0.5f, fz = DEPTH*0.5f;

	//create space mesh aabb
	BoundingBox boundingBox;
	BoundingBox::CreateFromPoints(boundingBox, XMVectorSet(-fx, -fy, -fz, 0.f), XMVectorSet(fx, fy, fz, 0.f));
	m_AABB.SetBoundingBoxInfo(boundingBox);

	return true;
}
bool CPointLightMesh::CreateIndexBuffer() {

	return true;
}
CPointLightMesh::CPointLightMesh() : CMesh() {

}
CPointLightMesh::~CPointLightMesh() {

}