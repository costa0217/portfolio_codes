#include "stdafx.h"
#include "NaviMesh.h"

bool CNaviMesh::CreateVertexBuffer() {
	//�̳��� ������ ������.
	//vertex buffer nullptr
	//index buffer nullptr

	//test
	m_nVertices = 1;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	return true;

	//	return false;
}
bool CNaviMesh::CreateIndexBuffer() {

	return true;

}

CNaviMesh::CNaviMesh() : CMesh() {

}
CNaviMesh::~CNaviMesh() {

}