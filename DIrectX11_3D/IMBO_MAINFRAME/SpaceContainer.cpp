#include "stdafx.h"
#include "SpaceContainer.h"


void CSpaceContainer::Begin(){

	//�� ���̵忡 �ִ� ������ ����
	m_oneSideSpaceNum = static_cast<int>(pow(2, m_level));
	//���� �ϳ��� ũ��
	m_oneSpaceSize = m_size / m_oneSideSpaceNum;
	//������ ���� ����Ʈ���ϱ� ����
	m_nSpace = static_cast<int>(pow(m_oneSideSpaceNum, 2));

	//�����Ǿ��� leaf spaces
	m_ppSpace = new CSpace*[m_nSpace];
	
	//���� �Ҵ�.
	m_pStartSpace = new CSpace();
	m_pStartSpace->Begin(this, m_size, m_level, XMVectorSet(0.f, 0.f, 0.f, 0.f));
}

bool CSpaceContainer::End(){
	
	for (auto pObject : m_lpBlockObject) {
		pObject->End();
		delete pObject;
		pObject = nullptr;
	}
	m_lpBlockObject.clear();

	//all space end
	//+ delete space pointer
	m_pStartSpace->End();
	m_pStartSpace = nullptr;

	delete m_ppSpace;
	m_ppSpace = nullptr;
	return false;
}

void CSpaceContainer::Animate(float fTimeElapsed){
	//all space animate
	m_pStartSpace->Animate(fTimeElapsed);
	
	//animate ���Ŀ� �и��� block object�� �����Ѵ�.
	if (false == m_lpBlockObject.empty()) {//block object list�� empty�� �ƴ϶��
		
		for (auto pObject : m_lpBlockObject) {//��ü �ٽ� ��ġ
			AddObject(pObject);
		}
		m_lpBlockObject.clear();
	}
}

void CSpaceContainer::PhisicsUpdate(float fTimeElapsed){

	for (int i = 0; i < m_nSpace; ++i) {
		m_ppSpace[i]->PhisicsUpdate(fTimeElapsed);
	}
}

void CSpaceContainer::PrepareRenderOneSpace( CCamera* pCamera, UINT renderFlag, int render_space){
	//if (render_space < 0) {
	//	PrepareRender(pCamera, renderFlag);
	//}else{
	//	for (int i = 0; i < m_nSpace; ++i) {
	//		if(render_space == i) m_ppSpace[i]->PrepareRender(renderFlag);
	//	}
	//}
}

void CSpaceContainer::PrepareRender( CCamera* pCamera, UINT renderFlag){
	DEBUGER->start_Timemeasurement();
	m_pStartSpace->PrepareRender(pCamera, renderFlag);
	DEBUGER->end_Timemeasurement(L"prepare_render");
}

void CSpaceContainer::AddBlockObjectList(CGameObject * pObject){
	m_lpBlockObject.emplace_back(pObject);
}

void CSpaceContainer::AddObject(CGameObject * pObject){
	//���� index ���ϱ�
	int current_index = SearchSpace(pObject->GetPosition());
	
	if (current_index < 0) {
		current_index = 0;
		pObject->SetPosition(XMVectorSet(0.f, 0.f, 0.f, 0.f));
	}

	//�ش� ������ ��ü �߰�
	m_ppSpace[current_index]->AddObject(pObject);
	//��ü�� ����index ���
	pObject->SetSpaceIndex(current_index);
}

void CSpaceContainer::RemoveObject(CGameObject * pObject){
	for (int i = 0; i < m_nSpace; ++i) {
		m_ppSpace[i]->RemoveObject(pObject);
	}
}

void CSpaceContainer::RemoveObject(string name){
	for (int i = 0; i < m_nSpace; ++i) {
		m_ppSpace[i]->RemoveObject(name);
	}
}

void CSpaceContainer::ClearBlockObjectList() {
	m_lpBlockObject.clear();
}

int CSpaceContainer::SearchSpace(XMVECTOR xmvPos){
	XMFLOAT3 xmf3Pos;
	XMStoreFloat3(&xmf3Pos, xmvPos);
	int x = static_cast<int>(xmf3Pos.x);
	int z = static_cast<int>(xmf3Pos.z);

	//�ڽ��� ��ġ�� �̿��� �ε����� ����
	int indexX = (int)x / m_oneSpaceSize;
	int indexZ = (int)z / m_oneSpaceSize;

	int index = indexX + (indexZ * m_oneSideSpaceNum);

	//���� ó�� ���� ��
	if (index < 0 || index >= m_nSpace) {
		return -1;
	}
	if (x < 0 || z < 0 ||
		x > m_size || z > m_size) {
		return -1;
	}

	return index;
}

void CSpaceContainer::AddSpace(UINT index, CSpace * pSpace){
	m_ppSpace[index] = pSpace;
}

void CSpaceContainer::ChangeSpaceData(){
	//1. space���� ��� ��ü �ӽ� ����
	vector<CGameObject*> m_vTempObjects;//�ӽ� ��ü ����
	for (int i = 0; i < m_nSpace; ++i) {//��� space�� ���ؼ�
		auto mlpObject = m_ppSpace[i]->GetmlpObject();
		for (auto lpObject : mlpObject) {
			for (auto pObject : lpObject.second) {
				m_vTempObjects.push_back(pObject);
			}
		}
	}
	
	//2. space delete/ not END()
	delete m_pStartSpace;
	m_pStartSpace = nullptr;
	if (m_nSpace != 1) {
		for (int i = 0; i < m_nSpace; ++i) {
			delete m_ppSpace[i];
		}
	}
	delete m_ppSpace;
	//3. ���ο� space����
	m_oneSideSpaceNum = static_cast<int>(pow(2, m_level));
	m_oneSpaceSize = m_size / m_oneSideSpaceNum;
	m_nSpace = static_cast<int>(pow(m_oneSideSpaceNum, 2));
	m_ppSpace = new CSpace*[m_nSpace];
	m_pStartSpace = new CSpace();
	m_pStartSpace->Begin(this, m_size, m_level, XMVectorSet(0.f, 0.f, 0.f, 0.f));

	//4. �ӽð�ü ���ġ
	for (auto pObject : m_vTempObjects) {
		AddObject(pObject);
	}
	m_vTempObjects.clear();
}

CGameObject * CSpaceContainer::PickObject(XMVECTOR xmvWorldCameraStartPos, XMVECTOR xmvRayDir, float& distanse, UINT renderFlag) {
	float fHitDistance = FLT_MAX;
	float fNearHitDistance = FLT_MAX;
	CGameObject* pHitObj = nullptr;
	CGameObject* pNearObj = nullptr;

	//�ڽ��� ��� leaf space�� ���ؼ� �˻�
	for (int i = 0; i < m_nSpace; ++i) {
		if (m_ppSpace[i]->GetbRender()) {//���� �׸��� �׸��� space���
			pHitObj = m_ppSpace[i]->PickObject(xmvWorldCameraStartPos, xmvRayDir, fHitDistance, renderFlag);
			if (fNearHitDistance > fHitDistance) {
				fNearHitDistance = fHitDistance;
				pNearObj = pHitObj;
			}
		}
	}
	distanse = fNearHitDistance;
	return pNearObj;
}

CSpaceContainer * CSpaceContainer::CreateSpaceContainer(int size, int lv){
	CSpaceContainer* pSpaceContainer = new CSpaceContainer();
	pSpaceContainer->SetSpaceSize(size);
	pSpaceContainer->SetSpaceLevel(lv);
	pSpaceContainer->Begin();
	return pSpaceContainer;
}

void CSpaceContainer::ClearAllObjects(){
	for (int i = 0; i < m_nSpace; ++i) {//��� ������
		for (auto v : m_ppSpace[i]->GetmlpObject()) {//��� map��
			for (auto pObject : v.second) {//��� vector
				if (pObject != nullptr)
				{
					pObject->End();
					delete pObject;
				}				
			}
			v.second.clear();
		}
		m_ppSpace[i]->GetmlpObject().clear();

		for (auto lpObject : m_ppSpace[i]->GetmlpCollisionObject()) {
			lpObject.second.clear();
		}
		m_ppSpace[i]->GetmlpCollisionObject().clear();
	}

	DEBUGER->ClearDebuger();
}

void CSpaceContainer::SaveObjectInfos(){
	//UINT nObject{ 0 };
	//for (int i = 0; i < m_nSpace; ++i) {//��� ������
	//	for (auto v : m_ppSpace[i]->GetmlpObject()) {//��� map��
	//		for (auto pObject : v.second) {//��� vector
	//			if (pObject->GetTag() == TAG_TERRAIN) continue;
	//			if (pObject->GetTag() == TAG_SPACE) continue;

	//			//�� ��� ��ü
	//			nObject++;
	//		}
	//	}
	//}
	//EXPORTER->WriteUINT(nObject);
	//EXPORTER->WriteEnter();
	////save
	//for (int i = 0; i < m_nSpace; ++i) {//��� ������
	//	for (auto v : m_ppSpace[i]->GetmlpObject()) {//��� map��
	//		for (auto pObject : v.second) {//��� vector
	//			if (pObject->GetTag() == TAG_TERRAIN) continue;
	//			if (pObject->GetTag() == TAG_SPACE) continue;
	//			//�� ��� ��ü
	//			EXPORTER->WriteEnter();
	//		}
	//	}
	//}
	//EXPORTER->WriteEnter();
}

void CSpaceContainer::LoadObjectInfos(){
	UINT nObject = IMPORTER->ReadUINT();
	for (int i = 0; i < nObject; ++i) {
		tag t = (tag)IMPORTER->ReadUINT();
		utag ut = (utag)IMPORTER->ReadUINT();

		string name = IMPORTER->Readstring();
		XMFLOAT4X4 xmf4x4Wolrd = IMPORTER->ReadFloat4x4();

		switch (t) {
		case TAG_LIGHT:
			if (name == "pointlight") {
				float fRange = IMPORTER->ReadFloat();
				float r = IMPORTER->ReadFloat();
				float g = IMPORTER->ReadFloat();
				float b = IMPORTER->ReadFloat();
				XMFLOAT3 xmf3Color = XMFLOAT3(r, g, b);

				CPointLight* pLight = CPointLight::CreatePointLight(fRange, xmf3Color);
				pLight->SetWorldMtx(XMLoadFloat4x4(&xmf4x4Wolrd));
				AddObject(pLight);
			}
			else if (name == "spotlight") {
				float inner = IMPORTER->ReadFloat();
				float outer = IMPORTER->ReadFloat();

				float r = IMPORTER->ReadFloat();
				float g = IMPORTER->ReadFloat();
				float b = IMPORTER->ReadFloat();
				XMFLOAT3 xmf3Color = XMFLOAT3(r, g, b);		
				float fRange = IMPORTER->ReadFloat();

				CSpotLight* pLight = CSpotLight::CreateSpotLight(fRange, xmf3Color, outer, inner);
				pLight->SetWorldMtx(XMLoadFloat4x4(&xmf4x4Wolrd));
				AddObject(pLight);
			}
			else if (name == "capsulelight") {
				float r = IMPORTER->ReadFloat();
				float g = IMPORTER->ReadFloat();
				float b = IMPORTER->ReadFloat();
				XMFLOAT3 xmf3Color = XMFLOAT3(r, g, b);

				float fLen = IMPORTER->ReadFloat();
				float fRange = IMPORTER->ReadFloat();

				CCapsuleLight* pLight = CCapsuleLight::CreateCapsuleLight(fLen, xmf3Color, fRange);
				pLight->SetWorldMtx(XMLoadFloat4x4(&xmf4x4Wolrd));
				AddObject(pLight);
			}
			break;
		default:
			CGameObject* pObject = CGameObject::CreateObject(name, t, XMLoadFloat4x4(&xmf4x4Wolrd));
			pObject->SetUTag(ut);
			AddObject(pObject);
		}
	}
}


CSpaceContainer::CSpaceContainer() : CObject("spacecontainer"){

}

CSpaceContainer::~CSpaceContainer(){

}

