#include "stdafx.h"
#include "RenderContainerSeller.h"


bool CRenderContainerSeller::Begin(){
	string object_name;
	CString cs(object_name.c_str());
	tag t = tag::TAG_DYNAMIC_OBJECT;
	m_mStempRenderContainer.clear();
	m_mTagRenderContainer.clear();
	/*object_name = "fbx";
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->AddMesh(RESOURCEMGR->GetMesh("Rect1", 0));
#ifdef USE_ANIM
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("AnimationObject"));
#else
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("Core"));
#endif
	m_mTagRenderContainer[t][object_name]->AddBuffer(RESOURCEMGR->GetBuffer("FBX"));
	m_mTagRenderContainer[t][object_name]->AddMaterial(RESOURCEMGR->GetMaterial("FBX"));
	m_mTagRenderContainer[t][object_name]->Begin();
*/
	t = tag::TAG_LIGHT;
	object_name = "directionallight";

	m_mTagRenderContainer[t][(object_name.c_str())] = new CRenderContainer();
	m_mTagRenderContainer[t][(object_name.c_str())]->SetMesh(RESOURCEMGR->GetMesh("DirectionalLight", 0));//mesh set과 동시에 instancing buffer 생성 및 se
	m_mTagRenderContainer[t][(object_name.c_str())]->SetShader(RESOURCEMGR->GetRenderShader("DirectionalLight"));
	m_mTagRenderContainer[t][(object_name.c_str())]->AddBuffer(RESOURCEMGR->GetBuffer("DirectionalLightCB"));
	m_mTagRenderContainer[t][(object_name.c_str())]->Begin();
	
	object_name = "pointlight";
	//point light
	m_mTagRenderContainer[t][(object_name.c_str())] = new CRenderContainer();
	m_mTagRenderContainer[t][(object_name.c_str())]->SetMesh(RESOURCEMGR->GetMesh("PointLight", 0));
	m_mTagRenderContainer[t][(object_name.c_str())]->SetShader(RESOURCEMGR->GetRenderShader("PointLight"));
	m_mTagRenderContainer[t][(object_name.c_str())]->AddBuffer(RESOURCEMGR->GetBuffer("PointLightCB1"));
	m_mTagRenderContainer[t][(object_name.c_str())]->AddBuffer(RESOURCEMGR->GetBuffer("PointLightCB2"));
	m_mTagRenderContainer[t][(object_name.c_str())]->Begin();
		
	object_name = "spotlight";
	//spot light
	m_mTagRenderContainer[t][(object_name.c_str())] = new CRenderContainer();
	m_mTagRenderContainer[t][(object_name.c_str())]->SetMesh(RESOURCEMGR->GetMesh("SpotLight", 0));
	m_mTagRenderContainer[t][(object_name.c_str())]->SetShader(RESOURCEMGR->GetRenderShader("SpotLight"));
	m_mTagRenderContainer[t][(object_name.c_str())]->AddBuffer(RESOURCEMGR->GetBuffer("SpotLightCB1"));
	m_mTagRenderContainer[t][(object_name.c_str())]->AddBuffer(RESOURCEMGR->GetBuffer("SpotLightCB2"));
	m_mTagRenderContainer[t][(object_name.c_str())]->Begin();
	
	object_name = "capsulelight";
	//point light
	m_mTagRenderContainer[t][	 (object_name.c_str())] = new CRenderContainer();
	m_mTagRenderContainer[t][	 (object_name.c_str())]->SetMesh(RESOURCEMGR->GetMesh("CapsuleLight", 0));
	m_mTagRenderContainer[t][	 (object_name.c_str())]->SetShader(RESOURCEMGR->GetRenderShader("CapsuleLight"));
	m_mTagRenderContainer[t][	 (object_name.c_str())]->AddBuffer(RESOURCEMGR->GetBuffer("CapsuleLightCB1"));
	m_mTagRenderContainer[t][	 (object_name.c_str())]->AddBuffer(RESOURCEMGR->GetBuffer("CapsuleLightCB2"));
	m_mTagRenderContainer[t][	 (object_name.c_str())]->Begin();
	
	t = tag::TAG_POSTPROCESSING;
	object_name = "postprocessing";
	//자기 shader set
	m_mTagRenderContainer[t][	 (object_name.c_str())] = new CRenderContainer();
	m_mTagRenderContainer[t][	 (object_name.c_str())]->SetShader(RESOURCEMGR->GetRenderShader("PostProcessing"));
	m_mTagRenderContainer[t][	 (object_name.c_str())]->SetMesh(RESOURCEMGR->GetMesh("DirectionalLight", 0));
	m_mTagRenderContainer[t][	 (object_name.c_str())]->Begin();
	
	t = tag::TAG_SPACE;
	object_name = "skybox";
	m_mTagRenderContainer[t][	 (object_name.c_str())] = new CRenderContainer();
	m_mTagRenderContainer[t][	 (object_name.c_str())]->SetMesh(RESOURCEMGR->GetMesh("SkyBox", 0));
	m_mTagRenderContainer[t][	 (object_name.c_str())]->SetShader(RESOURCEMGR->GetRenderShader("SkyBox"));
	m_mTagRenderContainer[t][	 (object_name.c_str())]->AddMaterial(RESOURCEMGR->GetMaterial("SkyBox"));
	m_mTagRenderContainer[t][	 (object_name.c_str())]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("ONEIB"));
	m_mTagRenderContainer[t][	 (object_name.c_str())]->Begin();
	
	object_name = "space";
	m_mTagRenderContainer[t][	 (object_name.c_str())] = new CRenderContainer();
	m_mTagRenderContainer[t][	 (object_name.c_str())]->SetMesh(RESOURCEMGR->GetMesh("Space", 0));//mesh set과 동시에 instancing buffer 생성 및 set
	m_mTagRenderContainer[t][	 (object_name.c_str())]->SetShader(RESOURCEMGR->GetRenderShader("Core"));
	m_mTagRenderContainer[t][	 (object_name.c_str())]->AddMaterial(RESOURCEMGR->GetMaterial("DEFAULT"));
	m_mTagRenderContainer[t][	 (object_name.c_str())]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("SpaceIB"));
	m_mTagRenderContainer[t][	 (object_name.c_str())]->Begin();
		
	t = tag::TAG_TERRAIN;
	object_name = "terrain";
	m_mTagRenderContainer[t][	 (object_name.c_str())] = new CRenderContainer();
	m_mTagRenderContainer[t][	 (object_name.c_str())]->SetShader(RESOURCEMGR->GetRenderShader("Terrain"));
	m_mTagRenderContainer[t][	 (object_name.c_str())]->AddMaterial(RESOURCEMGR->GetMaterial("Terrain"));
	m_mTagRenderContainer[t][	 (object_name.c_str())]->Begin();
	
	t = tag::TAG_SSLR;
	object_name = "laytrace";
	m_mTagRenderContainer[t][ (object_name.c_str())] = new CRenderContainer();
	m_mTagRenderContainer[t][ (object_name.c_str())]->SetMesh(RESOURCEMGR->GetMesh("DirectionalLight", 0));//mesh set과 동시에 instancing buffer 생성 및 se
	m_mTagRenderContainer[t][ (object_name.c_str())]->SetShader(RESOURCEMGR->GetRenderShader("RayTrace"));
	m_mTagRenderContainer[t][ (object_name.c_str())]->Begin();
	
	object_name = "combine";
	m_mTagRenderContainer[t][ (object_name.c_str())] = new CRenderContainer();
	m_mTagRenderContainer[t][ (object_name.c_str())]->SetMesh(RESOURCEMGR->GetMesh("DirectionalLight", 0));//mesh set과 동시에 instancing buffer 생성 및 se
	m_mTagRenderContainer[t][ (object_name.c_str())]->SetShader(RESOURCEMGR->GetRenderShader("Combine"));
	m_mTagRenderContainer[t][ (object_name.c_str())]->Begin();

	t = tag::TAG_BIGWATER;
	object_name = "bigwater";
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->AddMesh(RESOURCEMGR->GetMesh("WaterT", 0));
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("WaterPlane"));
	m_mTagRenderContainer[t][object_name]->AddMaterial(RESOURCEMGR->GetMaterial("Terrain"));
	m_mTagRenderContainer[t][object_name]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("DEFAULTIB"));
	m_mTagRenderContainer[t][object_name]->Begin();

	object_name = "bigwatersurface";
	m_mTagRenderContainer[t][object_name] = new CRenderContainer();
	m_mTagRenderContainer[t][object_name]->AddMesh(RESOURCEMGR->GetMesh("WaterT", 0));
	m_mTagRenderContainer[t][object_name]->SetShader(RESOURCEMGR->GetRenderShader("WaterSurface"));
	m_mTagRenderContainer[t][object_name]->AddMaterial(RESOURCEMGR->GetMaterial("Terrain"));
	m_mTagRenderContainer[t][object_name]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("DEFAULTIB"));
	m_mTagRenderContainer[t][object_name]->Begin();

	t = tag::TAG_DEBUG;
	object_name = "aabb";
	m_mTagRenderContainer[t][ (object_name.c_str())] = new CDebugRenderContainer();
	m_mTagRenderContainer[t][ (object_name.c_str())]->SetMesh(RESOURCEMGR->GetMesh("BoundingBox", 0));
	m_mTagRenderContainer[t][ (object_name.c_str())]->SetShader(RESOURCEMGR->GetRenderShader("BoundingBox"));
	m_mTagRenderContainer[t][ (object_name.c_str())]->AddMaterial(RESOURCEMGR->GetMaterial("BoundingBox"));
	m_mTagRenderContainer[t][ (object_name.c_str())]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("BoundingBoxIB"));
	m_mTagRenderContainer[t][ (object_name.c_str())]->Begin();

	object_name = "coordinatesys";
	m_mTagRenderContainer[t][ (object_name.c_str())] = new CDebugRenderContainer();
	m_mTagRenderContainer[t][ (object_name.c_str())]->SetMesh(RESOURCEMGR->GetMesh("CoordinateSys", 0));
	m_mTagRenderContainer[t][ (object_name.c_str())]->SetShader(RESOURCEMGR->GetRenderShader("CoordinateSys"));
	m_mTagRenderContainer[t][ (object_name.c_str())]->AddMaterial(RESOURCEMGR->GetMaterial("AABB"));
	m_mTagRenderContainer[t][ (object_name.c_str())]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("CoordinateSysIB"));
	m_mTagRenderContainer[t][ (object_name.c_str())]->Begin();

	object_name = "debugpointlight";
	//point light
	m_mTagRenderContainer[t][ (object_name.c_str())] = new CDebugRenderContainer();
	m_mTagRenderContainer[t][ (object_name.c_str())]->SetMesh(RESOURCEMGR->GetMesh("PointLight", 0));
	m_mTagRenderContainer[t][ (object_name.c_str())]->SetShader(RESOURCEMGR->GetRenderShader("DebugPointLight"));
	m_mTagRenderContainer[t][ (object_name.c_str())]->AddBuffer(RESOURCEMGR->GetBuffer("PointLightCB1"));
	m_mTagRenderContainer[t][ (object_name.c_str())]->AddBuffer(RESOURCEMGR->GetBuffer("PointLightCB2"));
	m_mTagRenderContainer[t][ (object_name.c_str())]->Begin();

	object_name = "debugcapsulelight";
	//capsule light
	m_mTagRenderContainer[t][ (object_name.c_str())] = new CDebugRenderContainer();
	m_mTagRenderContainer[t][ (object_name.c_str())]->SetMesh(RESOURCEMGR->GetMesh("CapsuleLight", 0));
	m_mTagRenderContainer[t][ (object_name.c_str())]->SetShader(RESOURCEMGR->GetRenderShader("DebugCapsuleLight"));
	m_mTagRenderContainer[t][ (object_name.c_str())]->AddBuffer(RESOURCEMGR->GetBuffer("CapsuleLightCB1"));
	m_mTagRenderContainer[t][ (object_name.c_str())]->AddBuffer(RESOURCEMGR->GetBuffer("CapsuleLightCB2"));
	m_mTagRenderContainer[t][ (object_name.c_str())]->Begin();

	object_name = "debugspotlight";
	//spot light
	m_mTagRenderContainer[t][ (object_name.c_str())] = new CDebugRenderContainer();
	m_mTagRenderContainer[t][ (object_name.c_str())]->SetMesh(RESOURCEMGR->GetMesh("SpotLight", 0));
	m_mTagRenderContainer[t][ (object_name.c_str())]->SetShader(RESOURCEMGR->GetRenderShader("DebugSpotLight"));
	m_mTagRenderContainer[t][ (object_name.c_str())]->AddBuffer(RESOURCEMGR->GetBuffer("SpotLightCB1"));
	m_mTagRenderContainer[t][ (object_name.c_str())]->AddBuffer(RESOURCEMGR->GetBuffer("SpotLightCB2"));
	m_mTagRenderContainer[t][ (object_name.c_str())]->Begin();

	object_name = "debugtexture";
	m_mTagRenderContainer[t][ (object_name.c_str())] = new CDebugRenderContainer();
	m_mTagRenderContainer[t][ (object_name.c_str())]->SetMesh(RESOURCEMGR->GetMesh("DebugTexture", 0));
	m_mTagRenderContainer[t][ (object_name.c_str())]->SetShader(RESOURCEMGR->GetRenderShader("DebugTexture"));
	m_mTagRenderContainer[t][ (object_name.c_str())]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("DebugTextureIB"));
	m_mTagRenderContainer[t][ (object_name.c_str())]->Begin();

	object_name = "debugdepthtexture";
	m_mTagRenderContainer[t][ (object_name.c_str())] = new CDebugRenderContainer();
	m_mTagRenderContainer[t][ (object_name.c_str())]->SetMesh(RESOURCEMGR->GetMesh("DebugTexture", 0));
	m_mTagRenderContainer[t][ (object_name.c_str())]->SetShader(RESOURCEMGR->GetRenderShader("DebugDepthTexture"));
	m_mTagRenderContainer[t][ (object_name.c_str())]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("DebugTextureIB"));
	m_mTagRenderContainer[t][ (object_name.c_str())]->Begin();
	//자동 rendercontainer제작!

	t = tag::TAG_REFLECTION;
	object_name = "water";
	m_mTagRenderContainer[t][ (object_name.c_str())] = new CRenderContainer();
	m_mTagRenderContainer[t][ (object_name.c_str())]->AddMesh(RESOURCEMGR->GetMesh("WaterT", 0));
	m_mTagRenderContainer[t][ (object_name.c_str())]->SetShader(RESOURCEMGR->GetRenderShader("WaterT"));
	m_mTagRenderContainer[t][ (object_name.c_str())]->AddMaterial(RESOURCEMGR->GetMaterial("Terrain"));
	m_mTagRenderContainer[t][ (object_name.c_str())]->AddInstanceBuffer(RESOURCEMGR->GetBuffer("DEFAULTIB"));
	m_mTagRenderContainer[t][ (object_name.c_str())]->Begin();

	t = tag::TAG_SSRF;
	object_name = "SSRF";
	m_mTagRenderContainer[t][ (object_name.c_str())] = new CRenderContainer();
	m_mTagRenderContainer[t][ (object_name.c_str())]->SetMesh(RESOURCEMGR->GetMesh("DirectionalLight", 0));//mesh set과 동시에 instancing buffer 생성 및 se
	m_mTagRenderContainer[t][ (object_name.c_str())]->SetShader(RESOURCEMGR->GetRenderShader("SSRF"));
	m_mTagRenderContainer[t][ (object_name.c_str())]->Begin();
	

	t = tag::TAG_DYNAMIC_OBJECT;
	object_name = "Trail";
	m_mTagRenderContainer[t][(object_name.c_str())] = new CRenderContainer();
	m_mTagRenderContainer[t][(object_name.c_str())]->AddMesh(RESOURCEMGR->GetMesh("TrailMesh", 0));
	m_mTagRenderContainer[t][(object_name.c_str())]->SetShader(RESOURCEMGR->GetRenderShader("Trail"));
	m_mTagRenderContainer[t][(object_name.c_str())]->AddBuffer(RESOURCEMGR->GetBuffer("DEFAULTCB"));
	m_mTagRenderContainer[t][(object_name.c_str())]->Begin();

	//CreateStempRenderContainer();


	return true;
}

bool CRenderContainerSeller::End(){
	//render container delete
	for (auto pairTagRenderContainer : m_mTagRenderContainer) {
		for (auto pairRenderContainer : pairTagRenderContainer.second) {
			pairRenderContainer.second->End();
			delete pairRenderContainer.second;
		}
		pairTagRenderContainer.second.clear();
	}
	m_mTagRenderContainer.clear();
	//render container delete

	ClearStempRenderContainer();
	return true;
}

CRenderContainer* CRenderContainerSeller::GetRenderContainer(string name) {
	//있으면 바로 return
	for (auto pairTagRenderContainer : m_mTagRenderContainer) {
		if (pairTagRenderContainer.second.end() != pairTagRenderContainer.second.find(name))
			return pairTagRenderContainer.second[name];
	}
	for (auto pairTagRenderContainer : m_mStempRenderContainer) {
		if (pairTagRenderContainer.second.end() != pairTagRenderContainer.second.find(name))
			return pairTagRenderContainer.second[name];
	}
	return nullptr;
}

CRenderContainer * CRenderContainerSeller::GetRenderContainer(tag t, string name){
	//있으면 바로 return
	if (m_mStempRenderContainer.end() != m_mStempRenderContainer.find(t)) {
		if (m_mStempRenderContainer[t].end() != m_mStempRenderContainer[t].find(name))
			return m_mStempRenderContainer[t][name];
	}

	if (m_mTagRenderContainer.end() != m_mTagRenderContainer.find(t)) {
		if (m_mTagRenderContainer[t].end() != m_mTagRenderContainer[t].find(name))
			return m_mTagRenderContainer[t][name];
	}
	return nullptr;
}

void CRenderContainerSeller::ClearTagRenderContainerObjects(){
	for (auto pairTagRenderContainer : m_mTagRenderContainer) {
		for (auto pairRenderContainer : pairTagRenderContainer.second) {
			pairRenderContainer.second->ClearObjectList();
		}
	}
}

void CRenderContainerSeller::ClearStempRenderContainer()
{
	for (auto pairTagRenderContainer : m_mStempRenderContainer) {
		for (auto pairRenderContainer : pairTagRenderContainer.second) {
			pairRenderContainer.second->End();
			delete pairRenderContainer.second;
		}
		pairTagRenderContainer.second.clear();
	}
	m_mStempRenderContainer.clear();

}
void CRenderContainerSeller::CreateStempRenderContainer(){
	//tag는 mesh에서 얻어와야 한다. 
	//animation tool에서 새로운 정보로 save해줘야 한다.
	
	for (auto vStempMesh : RESOURCEMGR->GetAllStempMesh()) {
		string name = vStempMesh.second[0]->GetName();
		tag t = vStempMesh.second[0]->GetTag();
		m_mStempRenderContainer[t][name] = new CRenderContainer;
		 CAnimater* pAnimater = RESOURCEMGR->GetAnimater(name);
		//animation
		if (pAnimater) {
			m_mStempRenderContainer[t][name]->SetShader(RESOURCEMGR->GetRenderShader("AnimationObject"));
			m_mStempRenderContainer[t][name]->SetAnimater(RESOURCEMGR->GetAnimater(name));
		}
		else {
			m_mStempRenderContainer[t][name]->SetShader(RESOURCEMGR->GetRenderShader("DEFAULT"));
		}
		//animation
		//mesh
		for (auto pStempMesh : vStempMesh.second) {
			m_mStempRenderContainer[t][name]->AddMesh(pStempMesh);
		}//mesh가 1개 이상이면 mesh material, texture를 사용한다.
		if (vStempMesh.second.size() == 1) {
			//mesh가 1개라면 
			//mseh의 resource를 rc로 옮기고 
			size_t vecSize = vStempMesh.second[0]->GetvMeshTexture().size();
			for (size_t i = 0; i < vecSize; ++i)
			{
				m_mStempRenderContainer[t][name]->AddTexture(vStempMesh.second[0]->GetvMeshTexture()[i]);
			}
			/*for (auto pTexture : vStempMesh.second[0]->GetvMeshTexture()) {
				m_mStempRenderContainer[t][csName]->AddTexture(pTexture);
			}*/
			//자신이 가진 resource는 지워준다.
			vStempMesh.second[0]->ClearMeshResources();
		}
		//mesh
		m_mStempRenderContainer[t][name]->AddBuffer(RESOURCEMGR->GetBuffer("DEFAULTCB"));
	}
}

void CRenderContainerSeller::TestingRCAdd()
{
	string object_name = "Arrow";
	tag t = tag::TAG_DYNAMIC_OBJECT;
	
	vector< CMesh*> pMesh = RESOURCEMGR->GetvStempMesh("Weed01");
	m_mTagRenderContainer[t][ (object_name.c_str())] = new CRenderContainer();
	m_mTagRenderContainer[t][ (object_name.c_str())]->SetShader(RESOURCEMGR->GetRenderShader("DEFAULT"));
	m_mTagRenderContainer[t][ (object_name.c_str())]->AddMesh(pMesh[0]);
	m_mTagRenderContainer[t][ (object_name.c_str())]->AddTexture(pMesh[0]->GetvMeshTexture()[0]);
	m_mTagRenderContainer[t][ (object_name.c_str())]->Begin();
}
