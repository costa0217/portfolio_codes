#include "stdafx.h"
#include "Scene.h"
#include "DirectXFramework.h"
#include "UIObject.h"

bool CScene::Begin(string path){


	return true;
}
//m_vecUI delete가 없어서 추가함 나중에 문제되면 지움 다른데서 delete중일 수 있으나 없었음
bool CScene::End(){
	for (size_t i = 0; i < m_vecUI.size(); ++i) {
		m_vecUI[i]->Release();
		delete m_vecUI[i];
		m_vecUI[i] = nullptr;
	}
	m_vecUI.clear();

	CNaviObjectManager::ClearNaviObject();
	CNaviObjectManager::ClearNaviVertex();

	return true;
}

void CScene::Animate(float fTimeElapsed){

}


bool CScene::GetPickPositionByCursor(int xClient, int yClient, XMFLOAT3& output, UINT tag)
{
	XMFLOAT4X4 xmf4x4Projection;
	D3D11_VIEWPORT d3dViewport;
	XMFLOAT3 xmf3PickPosition;
	XMVECTOR xmvPickPosition;
	XMMATRIX xmMtxViewInverse;
	XMVECTOR xmvRayDir;
	CGameObject* pNearestObject = NULL;
	float fHitDistance = FLT_MAX;
	float fNearDistance = FLT_MAX;
	XMFLOAT3 pickingPosition;

	XMVECTOR xmvProjPickPosition;
	XMVECTOR xmvProjCameraPos;
	XMVECTOR xmvProjRayDir;

	//Get screen pos -> Camera pos
	XMStoreFloat4x4(&xmf4x4Projection, m_pCamera->GetProjectionMtx());
	d3dViewport = m_pCamera->GetViewport();

	//음 이건 화면을 찍은 점의 클립 공간 녀석이고
	xmf3PickPosition.x = (((2.0f * xClient) / d3dViewport.Width) - 1) / xmf4x4Projection._11;
	xmf3PickPosition.y = -(((2.0f * yClient) / d3dViewport.Height) - 1) / xmf4x4Projection._22;
	xmf3PickPosition.z = 1.0f;

	//view space picking pos
	xmvPickPosition = XMLoadFloat3(&xmf3PickPosition);//view space pick position
	xmMtxViewInverse = XMMatrixInverse(nullptr, m_pCamera->GetViewMtx());
	xmvRayDir = XMVector3Normalize(xmvPickPosition - XMVectorSet(0, 0, 0, 1));//view space pick ray

	pNearestObject = UPDATER->PickObject(xmvPickPosition, xmvRayDir, fHitDistance, tag);
	if (pNearestObject) {
		fNearDistance = fHitDistance;

		xmvPickPosition = XMVector3TransformCoord(xmvPickPosition, xmMtxViewInverse);
		xmvRayDir = XMVector3Normalize(xmvPickPosition - m_pCamera->GetPosition());
		//get picking position
		XMStoreFloat3(&pickingPosition, m_pCamera->GetPosition() + xmvRayDir * fNearDistance);
		if (pNearestObject->GetName() == "terrain") {
			//if terrain
			pickingPosition.y = UPDATER->GetTerrainContainer()->GetHeight(XMFLOAT2(pickingPosition.x, pickingPosition.z));
		}

		//proc picking NaviObjectManager
		output = pickingPosition;
		return true;
		//CNaviObjectManager::PickingProc(pickingPosition);
	}
	output = XMFLOAT3();
	return false;
}

CScene* CScene::CreateScene(string name, CDirectXFramework* pFramework, SCENE_ID eID){
	CScene* pScene = new CScene(eID);
	pScene->LoadScene(name);
	pScene->SetFrameWork(pFramework);
	return pScene;
	//return nullptr;
}

void CScene::LoadScene(string path){

	//load scene
	string name = GetFileName(path);
	char meshRoot[256] = { "../../Assets/SceneResource/" };
	wstring wsMeshRoot;
	strcat(meshRoot, name.c_str());
	string sMeshRoot{ meshRoot };

	wsMeshRoot.assign(sMeshRoot.cbegin(), sMeshRoot.cend());
	LoadResource(wsMeshRoot);
}

void CScene::LoadResource(wstring wsMeshRoot){
	//read resource root 

	//RESOURCEMGR->ReleaseAllResource();
	//RCSELLER->ClearStempRenderContainer();

	//해당 dir아래의 모든 gjm파일을 로드
	vector<wstring> vFile;
	DIRECTORYFINDER->GetFiles(vFile, wsMeshRoot, true, true, L".gjm");
	DIRECTORYFINDER->GetFiles(vFile, wsMeshRoot, true, true, L".GJM");

	string name;
	for (auto fileName : vFile) {
		string sPath{ "" };
		sPath.assign(fileName.begin(), fileName.end());
		/*여기서 file 이름을 가지고 name을 정하도록 한다.
		file 명에서 확장자를 제거하면 될 것이다.
		sprintf(name, "파 일 명", i);*/
		name = GetFileName(sPath);
		RESOURCEMGR->CreateMultiMesh(sPath, name);
		//sprintf(name, "StempMesh%d", i++);
	}
	RCSELLER->ClearTagRenderContainerObjects();
	RCSELLER->ClearStempRenderContainer();
	//RCSELLER->TestingRCAdd();
	RCSELLER->CreateStempRenderContainer();
}


bool CScene::FlagCollision(CGameObject * pDest) {
	if (false == m_bCollision) {
		XMVECTOR xmvPos = XMLoadFloat3(&m_xmf3CollisionOffset);

		BoundingOrientedBox obb;
		XMStoreFloat3(&obb.Center, xmvPos);
		obb.Extents = XMFLOAT3(m_fRadius, m_fRadius, m_fRadius);
		DEBUGER->RegistOBB(obb, UTAG_COLLISION);

		XMVECTOR xmvPlayerPos = pDest->GetPosition();

		XMFLOAT4 xmf4Result;
		XMStoreFloat4(&xmf4Result, XMVector3Length(xmvPlayerPos - xmvPos));
		if (xmf4Result.x < m_fRadius) {
			m_bCollision = true;//flag는 한번만 사용한다.
			return true;
		}
	}
	return false;
}