
#include "stdafx.h"
#include "Updater.h"
#include "EffectMgr.h"
bool CUpdater::Begin() {

	//SetCamera(pCamera);
	//m_pCamera =  CCamera* (CAMMGR->GetCamera(CAM_FREE));
	m_pCamera = CAMMGR->GetCamera(CAM_FREE);
	//m_pCamera = CAMMGR->GetCamera(CAM_FREE);
	SetSpaceContainer(CSpaceContainer::CreateSpaceContainer(512, 0));
	SetTerrainContainer(CTerrainContainer::CreateTerrainContainer(L"Temp", 256, 256, 0.5, UPDATER->GetSpaceContainer(), false));
	SetSkyBoxContainer(CSkyBoxContainer::CreateSkyBoxContainer(L"Temp", 0, UPDATER->GetSpaceContainer(), m_pCamera));

	//directional light
	m_pDirectionalLight = new CDirectionalLight;
	m_pDirectionalLight->Begin(DIRECTIONAL_AMBIENT_LIGHT{
		XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f),XMFLOAT4(0.2f, 0.2f, 0.2f, 0.0f) , XMFLOAT4(0.5f, 0.5f, 0.5f, 1.f),//dir
		XMFLOAT4(0.15f, 0.15f, 0.15f, 1.f), XMFLOAT4(0.1f, 0.1f, 0.1f, 1.f), XMFLOAT4(0.15f, 0.15f, 0.15f, 1.f), XMFLOAT4(0.5f, 0.5f, 0.5f, 1.f)//ambient
	});
	m_pDirectionalLight->SetPosition(XMVectorSet((float)m_pSpaceContainer->GetSize() * 0.5f, (float)m_pSpaceContainer->GetSize(), (float)m_pSpaceContainer->GetSize() * 0.5f, 0.f));
	m_pDirectionalLight->Rotate(45.f, 0.f, 0.f);
	//-------------------------------------space-------------------------------------
	return true;
}

bool CUpdater::End() {
	//terrain
	Safe_EndDelete(m_pTerrainContainer);
	Safe_EndDelete(m_pSkyBoxContainer);
	Safe_EndDelete(m_pSpaceContainer);
	Safe_EndDelete(m_pDirectionalLight);
	return true;
}

void CUpdater::PreUpdate(float fTimeElapsed){
	//
	
}

void CUpdater::Update(float fTimeElapsed) {
	//-----------------------------------space------------------------------
	m_pDirectionalLight->RegistToContainer();

	m_pSpaceContainer->Animate(fTimeElapsed);
	//m_pSpaceContainer->PrepareRender(m_pCamera);
	CEffectMgr::GetInstance()->Update_PlayEffectList(fTimeElapsed);

	m_pCamera->Update(fTimeElapsed);
	if (m_pTerrainContainer)m_pTerrainContainer->Update(m_pCamera);//이건 사실 할 필요가 없는것.
	if (m_pSkyBoxContainer)m_pSkyBoxContainer->Update(fTimeElapsed);
	//-----------------------------------space------------------------------
}

void CUpdater::PhisicsUpdate(float fTimeElapsed){
	m_pSpaceContainer->PhisicsUpdate(fTimeElapsed);
}

void CUpdater::ChangeSceneContainers() {
	if (m_pTerrainContainer) m_pTerrainContainer->DeleteAllTerrainObjects();
	if (m_pSpaceContainer)m_pSpaceContainer->ChangeSpaceData();
	if (m_pTerrainContainer) m_pTerrainContainer->ChangeSpaceData();

	//directional light
	m_pDirectionalLight->SetPosition(XMVectorSet(m_pSpaceContainer->GetSize() / 2.f, m_pSpaceContainer->GetSize(), m_pSpaceContainer->GetSize() / 2.f, 0.f));
}

void CUpdater::LoadSpaceInfo(){
	//clear all objects
	if (m_pTerrainContainer) m_pTerrainContainer->DeleteAllTerrainObjects();
	m_pSpaceContainer->ClearAllObjects();

	//space info
	float space_size = IMPORTER->ReadFloat();
	float space_lv = IMPORTER->ReadFloat();
	m_pSpaceContainer->SetSpaceSize((int)space_size);
	m_pSpaceContainer->SetSpaceLevel(space_lv);
	m_pSpaceContainer->ChangeSpaceData();

	//terrain onoff/ skybox onoff
	bool bTerrainContainer = IMPORTER->ReadBool();
	m_pTerrainContainer->SetActive(bTerrainContainer);
	m_pTerrainContainer->ChangeSpaceData();

	bool bSkyBoxContainer = IMPORTER->ReadBool();
	m_pSkyBoxContainer->SetActive(bSkyBoxContainer);

}
void CUpdater::LoadTerrainInfo(wstring wsOutputPath, wstring wsSceneName){
	if (m_pTerrainContainer->GetActive()) {//terrain container가 있다면..
		//추가 navi object
		CNaviObjectManager::LoadData();

		//추가
		float zn = IMPORTER->ReadFloat();
		m_pTerrainContainer->SetTessFacterZn(zn);
		float zf = IMPORTER->ReadFloat();
		m_pTerrainContainer->SetTessFacterZf(zf);
		float max_facter_value = IMPORTER->ReadFloat();
		m_pTerrainContainer->SetTessFacterMaxFV(max_facter_value);
		float min_facter_value = IMPORTER->ReadFloat();
		m_pTerrainContainer->SetTessFacterMinFV(min_facter_value);


		//m_pTerrainContainer->SetSceneName(wsSceneName);//이름은 나중에 바꾸도록 한다.
		//base texture path
		XMFLOAT3 xmf3Scale;
		//이렇게 되어야 함
		int width = IMPORTER->ReadInt();
		UPDATER->GetTerrainContainer()->SetTerrainWidth(width);
		int length = IMPORTER->ReadInt();
		UPDATER->GetTerrainContainer()->SetTerrainLength(length);
		float fHeightScale = IMPORTER->ReadFloat();
		UPDATER->GetTerrainContainer()->SetTerrainScale(XMFLOAT3(static_cast<float>(m_pSpaceContainer->GetSize() / (width)),
			fHeightScale, static_cast<float>(m_pSpaceContainer->GetSize() / (length))));
		UPDATER->GetTerrainContainer()->SetHeightScale(fHeightScale);

		wstring wsBaseTexturePath = IMPORTER->ReadWstring();
		UPDATER->GetTerrainContainer()->SetBaseTexture(wsBaseTexturePath);
		//height map texture name
		//터레인 추가분
		//height map data save
		int nData = IMPORTER->ReadInt();//전체 heigmtmap data의 수
		float* pData = new float[nData];//height map의 data
		vector<float> test;
		for (int i = 0; i < nData; ++i) {
			pData[i] = IMPORTER->ReadFloat();
			test.push_back(pData[i]);
		}
		UPDATER->GetTerrainContainer()->SetHeightData(pData);
		UPDATER->GetTerrainContainer()->ReadyHeightMap();
		//터레인 추가분

		//normal map texture name
		wstring wsNormalDataName = wsOutputPath + wsSceneName + L"NormalMap.bmp";
		UPDATER->GetTerrainContainer()->SetNormalData(IMPORTER->ReadBitmap24(wsNormalDataName.c_str()));//nomalmap
		UPDATER->GetTerrainContainer()->SetNormalMapTexture(CTexture::CreateTexture(wsNormalDataName.c_str(), PS_SLOT_NORMALMAP, BIND_PS));

		//create splatting info
		int nSplatting = IMPORTER->ReadInt();
		UPDATER->GetTerrainContainer()->GetSplattingInfoManager()->ClearSplattingInfo();
		for (int i = 0; i<nSplatting; ++i) {
			//splatting의 detail texture는 path로 받는다.
			wstring wsDetailTexturePath = IMPORTER->ReadWstring();

			//splatting의 blending info는 [scene name]이름을 토대로 가공한다.
			//[scene name] + [splatting blending info] + [index]
			WCHAR wcBlendinfoPath[256];
			wsprintf(wcBlendinfoPath, L"%s%sBlendInfo%d.bmp", wsOutputPath.c_str(), wsSceneName.c_str(), i);

			UPDATER->GetTerrainContainer()->GetSplattingInfoManager()->CreateSplattingInfo(wsDetailTexturePath.c_str(), wcBlendinfoPath);
		}
		//create splatting info
	}
}

void CUpdater::LoadObjectsInfo(){
	m_pDirectionalLight->LoadInfo();
	m_pSpaceContainer->LoadObjectInfos();
}

CGameObject * CUpdater::PickObject(XMVECTOR xmvWorldCameraStartPos, XMVECTOR xmvRayDir, float & distanse, UINT tag) {
	float fHitDistance = FLT_MAX;
	float fNearHitDistance = FLT_MAX;
	CGameObject* pNearObj = nullptr;

	pNearObj = m_pSpaceContainer->PickObject(xmvWorldCameraStartPos, xmvRayDir, fNearHitDistance, tag);

	if (m_pDirectionalLight->CheckPickObject(xmvWorldCameraStartPos, xmvRayDir, fHitDistance)) {
		if (fNearHitDistance > fHitDistance) {
			pNearObj = m_pDirectionalLight;
			fNearHitDistance = fHitDistance;
		}
	}
	distanse = fNearHitDistance;

	return pNearObj;
}

CUpdater::CUpdater() :CSingleTonBase<CUpdater>("updatersingleton") {

}

CUpdater::~CUpdater() {

}
