#pragma once
#include "DXObject.h"
#include "GameObject.h"

//dxobject
#include "Mesh.h"
#include "RenderShader.h"
#include "Texture.h"
#include "Buffer.h"
#include "Material.h"
#include "Animater.h"//animater!!
//dxobject

#include "Camera.h"

class CRenderContainer : public DXObject {
public:
	//---------------------------dxobject---------------------------------
	virtual bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState( CCamera* pCamera);
	//---------------------------dxobject---------------------------------

	//--------------------------container---------------------------------
	void ClearVolatileResources();
private:
	virtual void RenderExcute();
	void RenderExcuteWithOutObject();
public:
	void Render( CCamera* pCamera);
	//
	void SetShaderState_NoPS();
	void Render_NoPS(CCamera* pCamera);
	//
	void RenderWithOutObject( CCamera* pCamera);

	 CMesh* GetMesh(UINT index = 0) { return m_vpMesh[index]; }
	vector< CMesh*>& GetvMesh() { return m_vpMesh; }
	//set이 아니라이제 add다.
	void ClearMesh();
	void ClearBuffer();
	void ClearAnimater();
	void AddMesh( CMesh* pMesh);
	void SetMesh( CMesh* pMesh);
	void SetShader( CRenderShader* pShader);
	
	//void AddMesh( CMesh* pMesh);
	//void AddShader( CRenderShader* pShader);
	void AddTexture( CTexture* pTexture);
	//휘발성 texture를 add
	void AddVolatileTexture( CTexture* pTexture);
	void AddBuffer( CBuffer* pBuffers);
	//휘발성 buf를 추가합
	void AddVolatileBuffer( CBuffer* pBuffers);
	void AddInstanceBuffer( CBuffer* pBuffers);
	void AddMaterial( CMaterial* pMaterial);
	
	//animater!
	void SetAnimater( CAnimater* pAnimater) { m_pAnimater = pAnimater; }
	 CAnimater* GetAnimater() { return m_pAnimater; }
	//animater!

	void SetObejcts(int n, CGameObject** ppObjects);
	void AddObject(CGameObject* pObject);
	//void RemoveObject(CGameObject* pObject);
	void ClearObjectList() { m_lpObjects.clear(); }//그림 다 그리고 clear
	vector<CGameObject*>& GetObjectList() { return m_lpObjects; }

	//material
	vector< CMaterial*>& GetMaterialList() { return m_vpMaterial; }
	void ClearTextures() { m_vpTexture.clear(); }

	//pick
	bool CheckPickMesh(XMVECTOR xmvModelCameraStartPos, XMVECTOR xmvModelRayDir, float & distance);
	//--------------------------container---------------------------------
protected:
	//--------------------------관리 대상------------------------------
	//컨테이너의 객체들은 동적으로 변할 것이다. 그러므로 리스트로 관리하는 것이 맞다
//	int m_nInstance{ 0 };
	//vector<CGameObject*> m_lpObjects;
	vector<CGameObject*>	m_lpObjects;
	

	vector< CMesh*>	m_vpMesh;		//vector< CMesh>>		m_vpMesh;
	 CRenderShader*		m_pShader;
	vector< CTexture*>	m_vpTexture;	//vector< CTexture>>	m_vpTexture;
	//휘발성 texture 한번 랜더하면 사라진다. 
	vector< CTexture*>	m_vpVolatileTexture;	//vector< CTexture>>	m_vpVolatileTexture;
	vector< CBuffer*>	m_vpVolatileBuffer;		//vector< CBuffer>>	m_vpVolatileBuffer;

	vector< CBuffer*>	m_vpBuffer;			//vector< CBuffer>>		m_vpBuffer;
	vector< CMaterial*>	m_vpMaterial;	//vector< CMaterial>>	m_vpMaterial;
	 CAnimater*			m_pAnimater{ nullptr };//animater

	int m_nBuffer{ 0 };
	int m_nMaterial{ 0 };
	//--------------------------관리 대상------------------------------

	void** m_ppBufferData{ nullptr };

public:
	CBuffer*	m_pRimColorBuffer{ nullptr };
public:
	CRenderContainer();
	virtual ~CRenderContainer();
	//CAtlString a;
	//CString
};


typedef map<tag, map<string, CRenderContainer*>> mapTagRenderContainer;
typedef map<string, CRenderContainer*> mapRC;
//typedef map<string, CRenderContainer*> mapRenderContainer;
//typedef map<tag, mapRenderContainer> mapTagRenderContainer;
//typedef pair<string, CRenderContainer*> pairRenderContainer;
//typedef pair<tag, pairRenderContainer> pairTagRenderContainer;
