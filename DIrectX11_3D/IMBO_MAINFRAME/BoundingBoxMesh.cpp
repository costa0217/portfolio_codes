#include "stdafx.h"
#include "BoundingBoxMesh.h"

bool CBoundingBoxMesh::CreateVertexBuffer() {
	//�̳��� ������ ������.
	//vertex buffer nullptr
	//index buffer nullptr

	//test
	m_nVertices = 1;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	return true;

//	return false;
}
bool CBoundingBoxMesh::CreateIndexBuffer() {

	return true;

}

CBoundingBoxMesh::CBoundingBoxMesh() : CMesh() {

}
CBoundingBoxMesh::~CBoundingBoxMesh() {

}