#pragma once

class CMesh;;

//�� mesh�� ���� �ε��� ���� �ƹ��͵� ����.
class CNaviMesh : public CMesh {
public:
	bool CreateVertexBuffer();
	bool CreateIndexBuffer();

private:

public:
	CNaviMesh();
	~CNaviMesh();

};