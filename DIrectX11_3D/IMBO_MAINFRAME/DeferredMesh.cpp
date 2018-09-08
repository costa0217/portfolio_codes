#include "stdafx.h"
#include "DeferredMesh.h"

/*정점의 색상을 무작위로(Random) 설정하기 위해 사용한다. 각 정점의 색상은 난수(Random Number)를 생성하여 지정한다.*/
#define RANDOM_COLOR XMFLOAT4(((rand() * 0xFFFFFF) / RAND_MAX),((rand() * 0xFFFFFF) / RAND_MAX),((rand() * 0xFFFFFF) / RAND_MAX),((rand() * 0xFFFFFF) / RAND_MAX))


#define WIDTH 10
#define HEIGHT 10
#define DEPTH 10

bool CDeferredMesh::CreateVertexBuffer() {
	m_nVertices = 4;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	////화면 좌표계 4개 준다
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