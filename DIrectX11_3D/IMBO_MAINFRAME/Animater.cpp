#include "stdafx.h""
#include "Animater.h"

bool CAnimater::Begin() {
	m_vpAnimationInfos.clear();
//	m_pAnimBuffer = CBuffer::CreateConstantBuffer(256, sizeof(XMMATRIX), 0, BIND_VS, 0);

	return true;
}

bool CAnimater::End() {
	for (auto pOBB : m_lActiveBoundingBox){
		pOBB->End();
		delete pOBB;
	}
	m_lActiveBoundingBox.clear();

	size_t iVecSize = m_vpAnimationInfos.size();
	for (size_t i = 0; i < iVecSize; ++i) {
		m_vpAnimationInfos[i]->End();
		delete m_vpAnimationInfos[i];
	}
	/*for (auto data : m_vpAnimationInfos) {
		data->End();
		delete data;
	}*/
	m_vpAnimationInfos.clear();

	if (m_pMainBoundingBox) {
		m_pMainBoundingBox->End();
		delete m_pMainBoundingBox;
	}
	m_pMainBoundingBox = nullptr;

	if(m_pSkeletonData)m_pSkeletonData->End();
	delete m_pSkeletonData;
	m_pSkeletonData = nullptr;

	if (m_pAnimBuffer) {
		m_pAnimBuffer->End();
		delete m_pAnimBuffer;
	}
	m_pAnimBuffer = nullptr;

	return true;
}

void CAnimater::SetShaderState() {
	if (m_vpAnimationInfos.empty()) {
		m_pAnimBuffer->SetShaderState();
		return;
	}
	m_vpAnimationInfos[m_CurAnimationIndex]->SetShaderState();
}

void CAnimater::CleanShaderState() {
	if (m_vpAnimationInfos.empty()) {

		return;
	}
	m_vpAnimationInfos[m_CurAnimationIndex]->CleanShaderState();
}

void CAnimater::Update(float fTimeElapsed) {
	if (m_vpAnimationInfos.empty()) {
		void* pData = m_pAnimBuffer->Map();
		XMMATRIX* pAnimationData = static_cast<XMMATRIX*>(pData);

		for (int j = 0; j < 256; ++j) {
			pAnimationData[j] = XMMatrixIdentity();
		}
		m_pAnimBuffer->Unmap();
		return;
	}
	m_vpAnimationInfos[m_CurAnimationIndex]->Update(fTimeElapsed);
}

void CAnimater::AddAnimationInfo(CAnimationInfo * pAnimationInfo) {
	//1. �ְ�
	m_vpAnimationInfos.push_back(pAnimationInfo);

	//	//2. ��� anim info�� ������. ��ü���� Joint tree�� �����Ѵ�.
	//	CreateJointTree();
	//
	//	//3. ��� anim info���� ���ο� joint tree�� ������ �ڽ��� joint�� ������ �� ���� ��û
	//	ChangeAllAnimationInfoJointData();
}

//void CAnimater::CreateJointTree() {
//	//���? x ó���� ��
//	m_vJointName.clear();
//	int Lastindex = m_vpAnimationInfos.size() - 1;
//	vector<CFbxJointData>::iterator iter;
//	//������ �׻� ó�� �޽�
//	for (auto FJoint : m_vpAnimationInfos[0]->GetJoints()) {
//		m_vJointName.push_back(FJoint.GetJointName());
//	}
//	for (auto LJoint : m_vpAnimationInfos[Lastindex]->GetJoints()) {
//		iter = find_if(m_vpAnimationInfos[0]->GetJoints().begin(), m_vpAnimationInfos[0]->GetJoints().end(), [&LJoint](CFbxJointData& my) {
//			return (my.GetJointName() == LJoint.GetJointName());
//		});
//		if (iter == m_vpAnimationInfos[0]->GetJoints().end()) {
//			//������
//			m_vJointName.push_back(LJoint.GetJointName());
//		}
//	}
//}
//
//void CAnimater::ChangeAllAnimationInfoJointData(){
//	for (auto pAnimationInfo : m_vpAnimationInfos) {
//		pAnimationInfo->ChangeJointData(m_vJointName);
//	}
//}

//void CAnimater::DeleteAnimationInfo(UINT AnimationIndex) {
//	vector<CAnimationInfo*>::iterator iter;
//	for (iter = m_vpAnimationInfos.begin(); iter != m_vpAnimationInfos.end(); ++iter) {
//		if ((*iter)->GetAnimationIndex() == AnimationIndex) break;
//	}
//	if (iter != m_vpAnimationInfos.end()) {
//		delete (*iter);
//		m_vpAnimationInfos.erase(iter);
//		m_CurAnimationIndex = 0;
//		UINT index{ 0 };
//		//animation info ������
//		for (auto pAnimationInfo : m_vpAnimationInfos) {
//			pAnimationInfo->SetAnimationIndex(index++);
//		}
//	}
//}

bool CAnimater::SetCurAnimationIndex(UINT AnimationIndex) {
	if (m_CurAnimationIndex == AnimationIndex) return false;
	m_CurAnimationIndex = AnimationIndex;
	ResetAnimationInfos();
	return true;
}

void CAnimater::ResetAnimationInfos() {
	size_t iVecSize = m_vpAnimationInfos.size();
	for(size_t i = 0; i < iVecSize; ++i)
	{
	//for (auto pAnimationInfo : m_vpAnimationInfos) {
		m_vpAnimationInfos[i]->Reset();
		//pAnimationInfo->Reset();
	}
}

UINT CAnimater::GetAnimaterJointCnt() {
	UINT JointCnt{ 0 };

	if (false == m_vpAnimationInfos.empty()) {
		JointCnt = m_pSkeletonData->GetJointCnt();
	}

	return JointCnt;
}

 CAnimater* CAnimater::CreateAnimaterFromFBXFile(bool bHasAnimation) {
	//if (bHasAnimation) {
	//	 CAnimater* pAnimater = new CAnimater>();
	//	pAnimater->SetMeshOffsetMtx(XMMatrixIdentity());

	//	CBoundingBox* pMainBoundingBox = new CBoundingBox();
	//	pMainBoundingBox->Begin(XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(5.f, 5.f, 5.f, 1.f));
	//	pAnimater->SetpMainAABB(pMainBoundingBox);

	//	 CBuffer* pAnimBuffer = CBuffer::CreateConstantBuffer(256, sizeof(XMMATRIX), 10, BIND_VS);
	//	pAnimater->SetpAnimBuffer(pAnimBuffer);

	//	CSkeletonData* pSkeletonData = new CSkeletonData;
	//	*pSkeletonData = FBXIMPORTER->GetSkeletonData();
	//	pAnimater->SetpSkeletonData(pSkeletonData);

	//	pAnimater->Begin();
	//	return pAnimater;
	//}

	return nullptr;
}

 CAnimater* CAnimater::CreateAnimaterFromGJMFile(bool bHasAnimation) {

	if (bHasAnimation) {
		//animation data�� ������
		 CAnimater* pAnimater = new CAnimater();
		XMFLOAT4X4 xmf4x4MeshOffsetMtx = IMPORTER->ReadFloat4x4();
		pAnimater->SetMeshOffsetMtx(xmf4x4MeshOffsetMtx);
		
		pAnimater->SetHead(IMPORTER->ReadInt());
		pAnimater->SetRHand(IMPORTER->ReadInt());
		pAnimater->SetLHand(IMPORTER->ReadInt());

		XMFLOAT3 xmf3Position;
		xmf3Position.x = IMPORTER->ReadFloat();
		xmf3Position.y = IMPORTER->ReadFloat();
		xmf3Position.z = IMPORTER->ReadFloat();

		XMFLOAT4 xmf4Scale;
		xmf4Scale.x = IMPORTER->ReadFloat();
		xmf4Scale.y = IMPORTER->ReadFloat();
		xmf4Scale.z = IMPORTER->ReadFloat();
		xmf4Scale.w = 1.f;

		CBoundingBox* pMainBoundingBox = new CBoundingBox();
		pMainBoundingBox->Begin(XMLoadFloat3(&xmf3Position), XMLoadFloat4(&xmf4Scale));
		pAnimater->SetpMainAABB(pMainBoundingBox);
		//animater obb
		//obb info
		int obbCnt = IMPORTER->ReadInt();
		for (int j = 0; j < obbCnt; ++j) {
			CBoundingBox* pBoundingBox = new CBoundingBox();
			int myJointIndex = IMPORTER->ReadInt();

			XMFLOAT4 xmf4Pos;
			xmf4Pos.x = IMPORTER->ReadFloat();
			xmf4Pos.y = IMPORTER->ReadFloat();
			xmf4Pos.z = IMPORTER->ReadFloat();
			xmf4Pos.w = 1.0f;

			XMFLOAT4 xmf4Scale;
			xmf4Scale.x = IMPORTER->ReadFloat();
			xmf4Scale.y = IMPORTER->ReadFloat();
			xmf4Scale.z = IMPORTER->ReadFloat();
			xmf4Scale.w = 1.0f;

			XMFLOAT4 xmf4Quaternion;
			xmf4Quaternion.x = IMPORTER->ReadFloat();
			xmf4Quaternion.y = IMPORTER->ReadFloat();
			xmf4Quaternion.z = IMPORTER->ReadFloat();
			xmf4Quaternion.w = IMPORTER->ReadFloat();
			pBoundingBox->Begin(XMLoadFloat4(&xmf4Pos), XMLoadFloat4(&xmf4Scale), XMLoadFloat4(&xmf4Quaternion));
			//pBoundingBox->SetPosition(XMLoadFloat3(&xmf3Pos));

			pBoundingBox->SetMin(0);
			pBoundingBox->SetMax(999);
			pBoundingBox->SetMyJointIndex(myJointIndex);
			pAnimater->GetAnimaterActiveOBBs().push_back(pBoundingBox);
		}//obb for end
		 //animater obb
		 CBuffer* pAnimBuffer = CBuffer::CreateConstantBuffer(256, sizeof(XMMATRIX), 10, BIND_VS);
		pAnimater->SetpAnimBuffer(pAnimBuffer);

		CSkeletonData* pSkeletonData = new CSkeletonData;
		//set skeleton data
		UINT nJointCnt = IMPORTER->ReadUINT();
		for (UINT i = 0; i < nJointCnt; ++i) {
			CFbxJointData jointData;
			jointData.SetJointName(IMPORTER->Readstring());
			jointData.SetMyIndex(IMPORTER->ReadUINT());
			jointData.SetParentIndex(IMPORTER->ReadUINT());
			jointData.SetOffsetMtx(IMPORTER->ReadXMMatrix());
			//if (i == 0) continue;
			pSkeletonData->GetJointDatas().push_back(jointData);
		};
		//set skeleton data
		pAnimater->SetpSkeletonData(pSkeletonData);

		pAnimater->Begin();

		//CAnimationInfo* pAnimationInfo;
		//pAnimationInfo = new CAnimationInfo(pd3dDevice, pd3dDeviceContext);
		//pAnimationInfo->Begin(pAnimater);

		return pAnimater;
	}

	return nullptr;
	//return  CAnimater>();
}

CAnimater::CAnimater() : DXObject("animater") {
	XMStoreFloat4x4(&m_xmf4x4MeshOffsetMtx, XMMatrixIdentity());
}

CAnimater::~CAnimater() {

}