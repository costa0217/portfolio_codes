#include "stdafx.h"
#include "DeferredMesh.h"

/*������ ������ ��������(Random) �����ϱ� ���� ����Ѵ�. �� ������ ������ ����(Random Number)�� �����Ͽ� �����Ѵ�.*/
#define RANDOM_COLOR XMFLOAT4(((rand() * 0xFFFFFF) / RAND_MAX),((rand() * 0xFFFFFF) / RAND_MAX),((rand() * 0xFFFFFF) / RAND_MAX),((rand() * 0xFFFFFF) / RAND_MAX))


#define WIDTH 10
#define HEIGHT 10
#define DEPTH 10

bool CDeferredMesh::CreateVertexBuffer() {
	m_nVertices = 4;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	////ȭ�� ��ǥ�� 4�� �ش�
	//VS_VB_SCREEN v[4];
	//v[0].xmf3Position = XMFLOAT4(-1, -1, 0.5, 1);
	//v[1].xmf3Position = XMFLOAT4(-1, 1, 0.5, 1);
	//v[2].xmf3Position = XMFLOAT4(1, -1, 0.5, 1);
	//v[3].xmf3Position = XMFLOAT4(1, 1, 0.5, 1);

	//v[0].xmf3TextCoord = XMFLOAT2(0, 1);
	//v[1].xmf3TextCoord = XMFLOAT2(0, 0);
	//v[2].xmf3TextCoord = XMFLOAT2(1, 1);
	//v[3].xmf3TextCoord = XMFLOAT2(1, 0);


	return true;

	return false;
}
bool CDeferredMesh::CreateIndexBuffer() {

	return true;

}
CDeferredMesh::CDeferredMesh() : CMesh() {

}
CDeferredMesh ::~CDeferredMesh() {

}