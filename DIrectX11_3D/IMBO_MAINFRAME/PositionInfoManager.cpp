#include "stdafx.h"
#include "PositionInfoManager.h"

map<string, stPositionInfo> CPositionInfoManager::m_mvPositionInfoData;

void CPositionInfoManager::Begin() {
	LoadData("test1");
	LoadData("test2");

	LoadData("Firsttown_Fly1");
	LoadData("Firsttown_Fly2");
	LoadData("Firsttown_Fly3");
	LoadData("Firsttown_Boss1");
	LoadData("Firsttown_Boss2");

	LoadData("Aldenard_Fly1");
	LoadData("Aldenard_Fly2");
	LoadData("Aldenard_Fly3");

	LoadData("Sarasen_Boss1");
	LoadData("Sarasen_Boss2");
	LoadData("Sarasen_Boss3");
}

void CPositionInfoManager::End() {
	m_mvPositionInfoData.clear();
}

void CPositionInfoManager::LoadData(string name) {
	stPositionInfo data;
	IMPORTER->Begin(root_path + name);

	data.m_fActionSpeed = IMPORTER->ReadFloat();
	int nCnt = IMPORTER->ReadInt();
	for (int i = 0; i < nCnt; ++i) {
		XMFLOAT3 xmf3Pos;
		//pos
		xmf3Pos.x = IMPORTER->ReadFloat();
		xmf3Pos.y = IMPORTER->ReadFloat();
		xmf3Pos.z = IMPORTER->ReadFloat();

		XMFLOAT4 xmf4Qua;
		//qua
		xmf4Qua.x = IMPORTER->ReadFloat();
		xmf4Qua.y = IMPORTER->ReadFloat();
		xmf4Qua.z = IMPORTER->ReadFloat();
		xmf4Qua.w = IMPORTER->ReadFloat();

		data.m_vPositionInfoData.push_back(CPositionInfoData(xmf3Pos, xmf4Qua));
	}

	m_mvPositionInfoData.insert(pair<string, stPositionInfo>(name, data));
	IMPORTER->End();
}

//void CPositionInfoManager::ProcessInput(CCamera* pCamera) {
//	if (INPUTMGR->KeyDown(VK_R)) {
//		XMVECTOR xmvQua = XMQuaternionRotationMatrix(pCamera->GetWorldMtx());
//		CPositionInfoManager::PushPositionInfoData(CPositionInfoData(pCamera->GetPosition(), xmvQua));
//	}
//	else if (INPUTMGR->KeyDown(VK_T)) {
//		size_t i = CPositionInfoManager::GetInfoCnt();
//		if (i > 0) CPositionInfoManager::DeletePositionInfoData(i - 1);
//	}
//	else if (INPUTMGR->KeyDown(VK_Y)) {
//		CPositionInfoManager::ClearPositionInfoData();
//	}
//	else if (INPUTMGR->KeyDown(VK_F)) {
//		pCamera->ActionCamStart();
//	}
//}

void CPositionInfoManager::RenderPositionInfo(string name) {
	for (auto Info : m_mvPositionInfoData[name].m_vPositionInfoData) {
		XMMATRIX xmMtx = XMMatrixAffineTransformation(XMVectorSet(1, 1, 1, 1), XMQuaternionIdentity(), Info.GetQuaternion(), Info.GetPosition());
		DEBUGER->RegistCoordinateSys(xmMtx);
	}
}
