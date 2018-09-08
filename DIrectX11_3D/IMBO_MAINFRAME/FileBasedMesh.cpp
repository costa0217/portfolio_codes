#include "stdafx.h"
#include "FileBasedMesh.h"

static bool gVerbose = true;

bool CFileBasedMesh::End() {
	if (m_pNormals) delete[] m_pNormals;
	if (m_pUVs) delete[] m_pUVs;
	if (m_pTs) delete[] m_pTs;
	if (m_pBs) delete[] m_pBs;
	//animation
	if (m_pxmf4BoneIndex) delete[] m_pxmf4BoneIndex;
	if (m_pxmf3Weight) delete[] m_pxmf3Weight;

	return CMesh::End();
}
 CFileBasedMesh* CFileBasedMesh::CreateMesh(wstring path, string name, UINT index, bool bHasAnimation){
	wstring extention{ PathFindExtension(path.c_str()) };
	if (L".FBX" == extention || L".fbx" == extention) {
		return CFileBasedMesh::CreateMeshFromFBXFile(name, index, bHasAnimation);
	}
	else if (L".GJM" == extention || L".gjm" == extention) {
		return CFileBasedMesh::CreateMeshFromGJMFile(name, index, bHasAnimation);
	}

	//return nullptr;
}

 CFileBasedMesh* CFileBasedMesh::CreateMeshFromFBXFile(string name, UINT index, bool bHasAnimation){
	// CFileBasedMesh* pFileBasedMesh;
	//pFileBasedMesh = new CFileBasedMesh();
	//pFileBasedMesh->SetName(name);
	////set mesh name, index
	//pFileBasedMesh->SetMeshIndex(index);
	//
	//pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULT"));
	//pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULTSPEC"));
	////set vertex buffer info
	//if (FBXIMPORTER->GetHasAnimation()) {
	//	//1. ��ü ������ ���� ���Ѵ�.
	//	UINT nVertices = FBXIMPORTER->GetMeshDatas()[index].GetVertexCnt();
	//	pFileBasedMesh->SetnVertices(nVertices);

	//	//2. �Ҵ�
	//	XMFLOAT3* pVertices = new XMFLOAT3[nVertices];
	//	XMFLOAT3* pNormals = new XMFLOAT3[nVertices];
	//	XMFLOAT2* pUVs = new XMFLOAT2[nVertices];
	//	XMFLOAT3* pxmf3Weight = new XMFLOAT3[nVertices];
	//	XMFLOAT4* pxmf4BoneIndex = new XMFLOAT4[nVertices];

	//	//3. ����
	//	int nVertex{ 0 };
	//	//mesh�� ��� ������ ���ؼ�
	//	for (UINT j = 0; j < FBXIMPORTER->GetMeshDatas()[index].GetVertexCnt(); ++j) {
	//		//mesh info
	//		pVertices[nVertex] = FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetPosition();
	//		pNormals[nVertex] = FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetNormal();
	//		pUVs[nVertex] = FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetUV();
	//		//animation info
	//		pxmf4BoneIndex[nVertex].x = static_cast<float>(FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetBlendWeightPairs()[0].GetIndex());
	//		pxmf4BoneIndex[nVertex].y = static_cast<float>(FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetBlendWeightPairs()[1].GetIndex());
	//		pxmf4BoneIndex[nVertex].z = static_cast<float>(FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetBlendWeightPairs()[2].GetIndex());
	//		pxmf4BoneIndex[nVertex].w = static_cast<float>(FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetBlendWeightPairs()[3].GetIndex());

	//		pxmf3Weight[nVertex].x = static_cast<float>(FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetBlendWeightPairs()[0].GetWeight());
	//		pxmf3Weight[nVertex].y = static_cast<float>(FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetBlendWeightPairs()[1].GetWeight());
	//		pxmf3Weight[nVertex++].z = static_cast<float>(FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetBlendWeightPairs()[2].GetWeight());
	//	}
	//	//4. set
	//	pFileBasedMesh->SetpVertices(pVertices);
	//	pFileBasedMesh->SetpNormals(pNormals);
	//	pFileBasedMesh->SetpUVs(pUVs);
	//	pFileBasedMesh->SetpWeights(pxmf3Weight);
	//	pFileBasedMesh->SetpBoneIndex(pxmf4BoneIndex);
	//}
	//else {
	//	//1. ��ü ������ ���Ѵ�.
	//	UINT nVertices = FBXIMPORTER->GetMeshDatas()[index].GetVertexCnt();
	//	pFileBasedMesh->SetnVertices(nVertices);

	//	//2. �Ҵ�
	//	XMFLOAT3* pVertices = new XMFLOAT3[nVertices];
	//	XMFLOAT3* pNormals = new XMFLOAT3[nVertices];
	//	XMFLOAT2* pUVs = new XMFLOAT2[nVertices];

	//	//3. ����
	//	int nVertex{ 0 };
	//	//mesh�� ��� ������ ���ؼ�
	//	for (UINT j = 0; j < FBXIMPORTER->GetMeshDatas()[index].GetVertexCnt(); ++j) {
	//		//���� ���� ���´�.
	//		pVertices[nVertex] = FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetPosition();
	//		pNormals[nVertex] = FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetNormal();
	//		pUVs[nVertex++] = FBXIMPORTER->GetMeshDatas()[index].GetVertexDatas()[j].GetUV();
	//	}

	//	//4. set
	//	pFileBasedMesh->SetpVertices(pVertices);
	//	pFileBasedMesh->SetpNormals(pNormals);
	//	pFileBasedMesh->SetpUVs(pUVs);
	//}


	////set index info
	////index
	////�߿��߿� offset�� �� ���� �ؾ��Ѵ�. 
	////ó�� mesh�� �ε����� �Ǿ��ִٸ� ��� mesh�� index�� �Ǿ��ִٰ� �����Ѵ�
	//if (!FBXIMPORTER->GetMeshDatas()[0].GetByControlPoint()) {
	//	//1. ��� �ε��� ���� ����
	//	UINT nIndices = FBXIMPORTER->GetMeshDatas()[index].GetIndexCnt();
	//	pFileBasedMesh->SetnIndices(nIndices);

	//	//2. �Ҵ�
	//	UINT* pnIndices = new UINT[nIndices];

	//	//3. offset�� ���ذ��� index������ �ϼ�
	//	//������ �ε��� 
	//	//		int nIndex{ 0 };
	//	int offset{ 0 };
	//	for (UINT j = 0; j < nIndices; ++j) {
	//		//mesh�� index�� 0�� �ƴ� ��� offset�� ���ذ���.
	//		//offset�� ���� index�� �����Ѵ�. offest�� 0��mesh�� 0/ 1�� mesh�� 0�� mesh�� ���� ����ŭ offset�� ������.
	//		//������ ������ �ʴ´�.
	//		pnIndices[j] = FBXIMPORTER->GetMeshDatas()[index].GetIndexs()[j] + offset;
	//	}

	//	pFileBasedMesh->SetpIndices(pnIndices);

	//}
	////index

	////set mesh info
	//pFileBasedMesh->Begin();
	//pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULT"));
	//return pFileBasedMesh;

	return nullptr;
}

 CFileBasedMesh* CFileBasedMesh::CreateMeshFromGJMFile(string name, UINT index, bool bHasAnimation) {
	/*
	test
	*/
	//WCHAR* p = IMPORTER->ReadWCHAR(0);

	 CFileBasedMesh* pFileBasedMesh;
	pFileBasedMesh = new CFileBasedMesh();
	pFileBasedMesh->SetName(name);
	//set mesh name, index
	pFileBasedMesh->SetMeshIndex(index);

	//set vertex buffer info
	if (bHasAnimation) {
		//mesh texture
		int MeshTextureCnt = IMPORTER->ReadInt();
		if (MeshTextureCnt <= 0) {//0�̸� set
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULT"));
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULTSPEC"));
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULTCP"));
		}
		for (int i = 0; i < MeshTextureCnt; ++i) {//n ��ŭ set
												  //char name[64];
												  //sprintf(name, "Test%d", index);
			string path;
			path = IMPORTER->Readstring();
			wstring wPath{ L"" };
			wPath.assign(path.cbegin(), path.cend());
			char cName[64];
			sprintf(cName, "%s_%d_%d", name.c_str(), index, i);
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->CreateTexture(cName, (WCHAR*)wPath.c_str(), i));
		}
		if (MeshTextureCnt == 1) {//���� 1���� spec map�� ���°� ����Ʈ�� set
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULTSPEC"));
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULTCP"));
		}
		else if (MeshTextureCnt == 2) {//���� 1���� spec map�� ���°� ����Ʈ�� set
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULTCP"));
		}
		//mesh texture

		//1. ��ü ������ ���� ���Ѵ�.
		UINT nVertices = IMPORTER->ReadUINT();
		pFileBasedMesh->SetnVertices(nVertices);

		//2. �Ҵ�
		XMFLOAT3* pVertices = new XMFLOAT3[nVertices];
		XMFLOAT3* pNormals = new XMFLOAT3[nVertices];
		XMFLOAT2* pUVs = new XMFLOAT2[nVertices];
		XMFLOAT3* pTs = new XMFLOAT3[nVertices];
		XMFLOAT3* pBs = new XMFLOAT3[nVertices];
		XMFLOAT3* pxmf3Weight = new XMFLOAT3[nVertices];
		XMFLOAT4* pxmf4BoneIndex = new XMFLOAT4[nVertices];

		//3. ����
		int nVertex{ 0 };
		//mesh�� ��� ������ ���ؼ�
		for (UINT j = 0; j < nVertices; ++j) {
			pVertices[nVertex].x = IMPORTER->ReadFloat();
			pVertices[nVertex].y = IMPORTER->ReadFloat();
			pVertices[nVertex++].z = IMPORTER->ReadFloat();
		}
		nVertex = 0;
		for (UINT j = 0; j < nVertices; ++j) {
			pNormals[nVertex].x = IMPORTER->ReadFloat();
			pNormals[nVertex].y = IMPORTER->ReadFloat();
			pNormals[nVertex++].z = IMPORTER->ReadFloat();
		}
		nVertex = 0;
		for (UINT j = 0; j < nVertices; ++j) {
			pUVs[nVertex].x = IMPORTER->ReadFloat();
			pUVs[nVertex++].y = IMPORTER->ReadFloat();
		}
		nVertex = 0;
		for (UINT j = 0; j < nVertices; ++j) {
			pTs[nVertex].x = IMPORTER->ReadFloat();
			pTs[nVertex].y = IMPORTER->ReadFloat();
			pTs[nVertex++].z = IMPORTER->ReadFloat();
		}
		nVertex = 0;
		for (UINT j = 0; j < nVertices; ++j) {
			pBs[nVertex].x = IMPORTER->ReadFloat();
			pBs[nVertex].y = IMPORTER->ReadFloat();
			pBs[nVertex++].z = IMPORTER->ReadFloat();
		}
		nVertex = 0;
		for (UINT j = 0; j < nVertices; ++j) {
			pxmf3Weight[nVertex].x = IMPORTER->ReadFloat();
			pxmf3Weight[nVertex].y = IMPORTER->ReadFloat();
			pxmf3Weight[nVertex++].z = IMPORTER->ReadFloat();
		}
		nVertex = 0;
		for (UINT j = 0; j < nVertices; ++j) {
			pxmf4BoneIndex[nVertex].x = IMPORTER->ReadFloat();
			pxmf4BoneIndex[nVertex].y = IMPORTER->ReadFloat();
			pxmf4BoneIndex[nVertex].z = IMPORTER->ReadFloat();
			pxmf4BoneIndex[nVertex++].w = IMPORTER->ReadFloat();
		}
		//4. set
		pFileBasedMesh->SetpVertices(pVertices);
		pFileBasedMesh->SetpNormals(pNormals);
		pFileBasedMesh->SetpUVs(pUVs);
		pFileBasedMesh->SetpTs(pTs);
		pFileBasedMesh->SetpBs(pBs);
		pFileBasedMesh->SetpWeights(pxmf3Weight);
		pFileBasedMesh->SetpBoneIndex(pxmf4BoneIndex);


	}
	else {
		//mesh texture
		int MeshTextureCnt = IMPORTER->ReadInt();

		if (MeshTextureCnt <= 0) {//0�̸� set
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULT"));
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULTSPEC"));
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULTCP"));
		}
		for (int i = 0; i < MeshTextureCnt; ++i) {//n ��ŭ set
												  //char name[64];
												  //sprintf(name, "Test%d", index);
			string path;
			path = IMPORTER->Readstring();
			wstring wPath{ L"" };
			wPath.assign(path.cbegin(), path.cend());
			char cName[64];
			sprintf(cName, "%s_%d_%d", name.c_str(), index, i);
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->CreateTexture(cName, (WCHAR*)wPath.c_str(), i));
		}
		if (MeshTextureCnt == 1) {//���� 1���� spec map�� ���°� ����Ʈ�� set
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULTSPEC"));
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULTCP"));
		}
		else if (MeshTextureCnt == 2) {//���� 1���� spec map�� ���°� ����Ʈ�� set
			pFileBasedMesh->AddMeshTexture(RESOURCEMGR->GetTexture("DEFAULTCP"));
		}
		
		//1. ��ü ������ ���Ѵ�.
		UINT nVertices = IMPORTER->ReadUINT();
		pFileBasedMesh->SetnVertices(nVertices);

		//2. �Ҵ�
		XMFLOAT3* pVertices = new XMFLOAT3[nVertices];
		XMFLOAT3* pNormals = new XMFLOAT3[nVertices];
		XMFLOAT2* pUVs = new XMFLOAT2[nVertices];
		XMFLOAT3* pTs = new XMFLOAT3[nVertices];
		XMFLOAT3* pBs = new XMFLOAT3[nVertices];

		//3. ����
		int nVertex{ 0 };
		//mesh�� ��� ������ ���ؼ�
		for (UINT j = 0; j < nVertices; ++j) {
			//���� ���� ���´�.
			pVertices[nVertex].x = IMPORTER->ReadFloat();
			pVertices[nVertex].y = IMPORTER->ReadFloat();
			pVertices[nVertex++].z = IMPORTER->ReadFloat();
		}
		nVertex = 0;
		for (UINT j = 0; j < nVertices; ++j) {
			pNormals[nVertex].x = IMPORTER->ReadFloat();
			pNormals[nVertex].y = IMPORTER->ReadFloat();
			pNormals[nVertex++].z = IMPORTER->ReadFloat();
		}
		nVertex = 0;
		for (UINT j = 0; j < nVertices; ++j) {
			pUVs[nVertex].x = IMPORTER->ReadFloat();
			pUVs[nVertex++].y = IMPORTER->ReadFloat();
		}
		nVertex = 0;
		for (UINT j = 0; j < nVertices; ++j) {
			pTs[nVertex].x = IMPORTER->ReadFloat();
			pTs[nVertex].y = IMPORTER->ReadFloat();
			pTs[nVertex++].z = IMPORTER->ReadFloat();
		}
		nVertex = 0;
		for (UINT j = 0; j < nVertices; ++j) {
			pBs[nVertex].x = IMPORTER->ReadFloat();
			pBs[nVertex].y = IMPORTER->ReadFloat();
			pBs[nVertex++].z = IMPORTER->ReadFloat();
		}
		//4. set
		pFileBasedMesh->SetpVertices(pVertices);
		pFileBasedMesh->SetpNormals(pNormals);
		pFileBasedMesh->SetpUVs(pUVs);
		pFileBasedMesh->SetpTs(pTs);
		pFileBasedMesh->SetpBs(pBs);
	}


	//set index info

	//1. ��� �ε��� ���� ����
	UINT nIndices = IMPORTER->ReadUINT();
	if (nIndices > 0) {
		pFileBasedMesh->SetnIndices(nIndices);

		//2. �Ҵ�
		UINT* pnIndices = new UINT[nIndices];

		//3. ����
		for (UINT j = 0; j < nIndices; ++j) {
			pnIndices[j] = IMPORTER->ReadUINT();
		}

		pFileBasedMesh->SetpIndices(pnIndices);
		//index
	}
	//�ٸ� tool�� �߰��Ǿ� �� �κ�
	if (false == bHasAnimation) {
		if (index == 0) {
			XMFLOAT3 xmf3Pos;
			xmf3Pos.x = IMPORTER->ReadFloat();
			xmf3Pos.y = IMPORTER->ReadFloat();
			xmf3Pos.z = IMPORTER->ReadFloat();

			XMFLOAT3 xmf3Scale;
			xmf3Scale.x = IMPORTER->ReadFloat();
			xmf3Scale.y = IMPORTER->ReadFloat();
			xmf3Scale.z = IMPORTER->ReadFloat();

			pFileBasedMesh->GetAABBObject().Begin(XMLoadFloat3(&xmf3Pos), XMVectorSet(xmf3Scale.x, xmf3Scale.y, xmf3Scale.z, 1.0f));

			int obbCnt = IMPORTER->ReadInt();

			//obb info
			//WriteWCHAR(L"ObbInfos"); WriteSpace();
			for (int i = 0; i < obbCnt; ++i) {
				XMFLOAT3 xmf3Pos;
				xmf3Pos.x = IMPORTER->ReadFloat();
				xmf3Pos.y = IMPORTER->ReadFloat();
				xmf3Pos.z = IMPORTER->ReadFloat();

				XMFLOAT3 xmf3Scale;
				xmf3Scale.x = IMPORTER->ReadFloat();
				xmf3Scale.y = IMPORTER->ReadFloat();
				xmf3Scale.z = IMPORTER->ReadFloat();

				XMFLOAT4 xmf4Quaternion;
				xmf4Quaternion.x = IMPORTER->ReadFloat();
				xmf4Quaternion.y = IMPORTER->ReadFloat();
				xmf4Quaternion.z = IMPORTER->ReadFloat();
				xmf4Quaternion.w = IMPORTER->ReadFloat();

				CBoundingBox obb;
				obb.Begin(XMLoadFloat3(&xmf3Pos), XMVectorSet(xmf3Scale.x, xmf3Scale.y, xmf3Scale.z, 1.0f), XMLoadFloat4(&xmf4Quaternion));
				obb.SetActive(true);
				pFileBasedMesh->GetvOBBObject().push_back(obb);
			}//end obb for 
		}
	}
	pFileBasedMesh->Begin();
	return pFileBasedMesh;

	return nullptr;
}

bool CFileBasedMesh::CreateVertexBuffer() {
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	UINT nVertexBuffer{ 0 };
	ID3D11Buffer *pd3dBuffers[1];
	UINT pnBufferStrides[1];
	UINT pnBufferOffsets[1];

	//position/ aabb
	if (m_pVertices) {
		m_pd3dPositionBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, m_pVertices, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
		pd3dBuffers[0] = m_pd3dPositionBuffer;
		pnBufferStrides[0] = sizeof(XMFLOAT3);
		pnBufferOffsets[0] = 0;
		AssembleToVertexBuffer(1, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	}
	//normal
	if (m_pNormals) {
		m_pd3dNormalBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, m_pNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

		pd3dBuffers[0] = m_pd3dNormalBuffer;
		pnBufferStrides[0] = sizeof(XMFLOAT3);
		pnBufferOffsets[0] = 0;
		AssembleToVertexBuffer(1, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	}
	//uv
	if (m_pUVs) {
		m_pd3dUVBuffer = CreateBuffer(sizeof(XMFLOAT2), m_nVertices, m_pUVs, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

		pd3dBuffers[0] = m_pd3dUVBuffer;
		pnBufferStrides[0] = sizeof(XMFLOAT2);
		pnBufferOffsets[0] = 0;
		AssembleToVertexBuffer(1, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	}
	//T
	if (m_pTs) {
		m_pd3dTBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, m_pTs, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

		pd3dBuffers[0] = m_pd3dTBuffer;
		pnBufferStrides[0] = sizeof(XMFLOAT3);
		pnBufferOffsets[0] = 0;
		AssembleToVertexBuffer(1, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	}
	//B
	if (m_pBs) {
		m_pd3dBBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, m_pBs, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

		pd3dBuffers[0] = m_pd3dBBuffer;
		pnBufferStrides[0] = sizeof(XMFLOAT3);
		pnBufferOffsets[0] = 0;
		AssembleToVertexBuffer(1, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	}
	//weight
	if (m_pxmf3Weight) {
		m_pd3dWeightBuffer = CreateBuffer(sizeof(XMFLOAT3), m_nVertices, m_pxmf3Weight, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

		pd3dBuffers[0] = m_pd3dWeightBuffer;
		pnBufferStrides[0] = sizeof(XMFLOAT3);
		pnBufferOffsets[0] = 0;
		AssembleToVertexBuffer(1, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	}
	//bone index
	if (m_pxmf4BoneIndex) {
		m_pd3dBoneIndexBuffer = CreateBuffer(sizeof(XMFLOAT4), m_nVertices, m_pxmf4BoneIndex, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

		pd3dBuffers[0] = m_pd3dBoneIndexBuffer;
		pnBufferStrides[0] = sizeof(XMFLOAT4);
		pnBufferOffsets[0] = 0;
		AssembleToVertexBuffer(1, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	}

	if (m_ppd3dVertexBuffers)   return true;

	return false;
}
bool CFileBasedMesh::CreateIndexBuffer() {
	if (m_pnIndices) {
		m_pd3dIndexBuffer = CreateBuffer(sizeof(UINT), m_nIndices, m_pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	}

	return true;
}

CFileBasedMesh::CFileBasedMesh() : CMesh() {

}
CFileBasedMesh::~CFileBasedMesh(){

}