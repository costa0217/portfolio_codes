#include "Loader.h"

USING(Engine)

CHierarchyLoader::CHierarchyLoader(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pPath)
: m_pGraphicDev(pGraphicDev)
, m_pPath(pPath)
{

}

CHierarchyLoader::~CHierarchyLoader(void)
{

}

// 뼈의 정보를 생성한다.(값을 채워주는 작업은 sdk가 알아서, 뼈의 공간을 생성, 최소의 정보만 복사)
STDMETHODIMP CHierarchyLoader::CreateFrame(THIS_ LPCSTR Name /*Input*/, LPD3DXFRAME *ppNewFrame /*OutPut*/)
{
	D3DXFRAME_DERIVED*			pNewFrame = new D3DXFRAME_DERIVED;

	ZeroMemory(pNewFrame, sizeof(D3DXFRAME_DERIVED));
	
	Allocate_Name(&pNewFrame->Name, Name);
		
	
	D3DXMatrixIdentity(&pNewFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pNewFrame->CombinedTransformationMatrix);

	*ppNewFrame = pNewFrame;

	return S_OK;
}

STDMETHODIMP CHierarchyLoader::CreateMeshContainer(THIS_ LPCSTR Name
												   , CONST D3DXMESHDATA *pMeshData
												   , CONST D3DXMATERIAL *pMaterials
												   , CONST D3DXEFFECTINSTANCE *pEffectInstances
												   , DWORD NumMaterials
												   , CONST DWORD *pAdjacency
												   , LPD3DXSKININFO pSkinInfo
												   , LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	D3DXMESHCONTAINER_DERIVED*		pNewMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	ZeroMemory(pNewMeshContainer, sizeof(D3DXMESHCONTAINER_DERIVED));

	Allocate_Name(&pNewMeshContainer->Name, Name);


	// 인접점에 대한 정보를 얻어오자.
	LPD3DXMESH				pMesh = pMeshData->pMesh;

	_ulong NumFaces = pMesh->GetNumFaces();

	pNewMeshContainer->pAdjacency = new _ulong[NumFaces * 3];

	memcpy(pNewMeshContainer->pAdjacency, pAdjacency, sizeof(_ulong) * NumFaces * 3);
	
	

	_ulong dwFVF = pMesh->GetFVF();

	pNewMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;	

	// 메시를 새롭게 복제해서 새롭게 대입해준다.
	if(dwFVF & D3DFVF_NORMAL)
	{
		if(FAILED(pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF
			, m_pGraphicDev, &pNewMeshContainer->MeshData.pMesh)))
		{
			MSG_BOX("복제실패");
		}
	}

	else
	{
		if(FAILED(pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF | D3DFVF_NORMAL
			, m_pGraphicDev, &pNewMeshContainer->MeshData.pMesh)))
		{
			MSG_BOX("복제실패");
		}

		D3DXComputeNormals(pNewMeshContainer->MeshData.pMesh, pNewMeshContainer->pAdjacency);
	}

	pMesh = pNewMeshContainer->MeshData.pMesh;

	// 재질정보를 셋팅해주낟.
	pNewMeshContainer->NumMaterials = max(NumMaterials, 1);

	pNewMeshContainer->pMaterials = new D3DXMATERIAL[pNewMeshContainer->NumMaterials];
	ZeroMemory(pNewMeshContainer->pMaterials, sizeof(D3DXMATERIAL) * pNewMeshContainer->NumMaterials);

	pNewMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pNewMeshContainer->NumMaterials];
	ZeroMemory(pNewMeshContainer->ppTextures, sizeof(LPDIRECT3DTEXTURE9) * pNewMeshContainer->NumMaterials);

	pNewMeshContainer->ppNormalTextures = new LPDIRECT3DTEXTURE9[pNewMeshContainer->NumMaterials];
	ZeroMemory(pNewMeshContainer->ppNormalTextures, sizeof(LPDIRECT3DTEXTURE9) * pNewMeshContainer->NumMaterials);

	pNewMeshContainer->ppSpecularTextures= new LPDIRECT3DTEXTURE9[pNewMeshContainer->NumMaterials];
	ZeroMemory(pNewMeshContainer->ppSpecularTextures, sizeof(LPDIRECT3DTEXTURE9) * pNewMeshContainer->NumMaterials);

	if(0 != NumMaterials)
	{
		memcpy(pNewMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * pNewMeshContainer->NumMaterials);
		for (_ulong i = 0; i < pNewMeshContainer->NumMaterials; ++i)
		{
			TCHAR				szFullPath[128] = L"";
			TCHAR				szFileName[128] = L"";

			MultiByteToWideChar(CP_ACP, 0, pNewMeshContainer->pMaterials[i].pTextureFilename, strlen(pNewMeshContainer->pMaterials[i].pTextureFilename)
				, szFileName, 128);

			lstrcpy(szFullPath, m_pPath);
			lstrcat(szFullPath, szFileName);

			if(FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &pNewMeshContainer->ppTextures[i])))
			{
				MSG_BOX("Mesh`s Texture Create Failed");
				return E_FAIL;
			}

			//노말맵
			TCHAR				szNormalFullPath[128]	= L"";
			TCHAR				szNormalFileName[128]	= L"";
			TCHAR				szNormalSave[32]		= L"";
			TCHAR				szBar[2] = L"_";

			int					istrLen = 0;

			lstrcpy(szNormalFileName, szFileName);
			istrLen = lstrlen(szNormalFileName) - 1;

			for(int k = istrLen; k > 0; --k)
			{
				if( szNormalFileName[k] != szBar[0] )
					continue;

				for(int j = 0; j < istrLen - k + 1; ++j)
				{
					szNormalSave[j] = szNormalFileName[k + j];
				}
				szNormalFileName[k-1] = NULL;
				break;
			}
			
			lstrcat(szNormalFileName, L"N");
			lstrcat(szNormalFileName, szNormalSave);
			lstrcpy(szNormalFullPath, m_pPath);
			lstrcat(szNormalFullPath, szNormalFileName);

			if(FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szNormalFullPath, &pNewMeshContainer->ppNormalTextures[i])))
			{
				lstrcpy(szNormalFullPath, L"../Bin/Resource/Meshes/Default.tga");
				if(FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szNormalFullPath, &pNewMeshContainer->ppNormalTextures[i])))
					MSG_BOX("Mesh`s Default Normal Texture Create Failed");

			}
			
			//스펙큘라맵
			TCHAR				szSpecFullPath[128]	= L"";
			TCHAR				szSpecFileName[128]	= L"";
			TCHAR				szSpecSave[32]		= L"";

			istrLen = 0;

			lstrcpy(szSpecFileName, szFileName);
			istrLen = lstrlen(szSpecFileName) - 1;

			for(int k = istrLen; k > 0; --k)
			{
				if( szSpecFileName[k] != szBar[0] )
					continue;

				for(int j = 0; j < istrLen - k + 1; ++j)
				{
					szSpecSave[j] = szSpecFileName[k + j];
				}
				szSpecFileName[k-1] = NULL;
				break;
			}

			lstrcat(szSpecFileName, L"SP");
			lstrcat(szSpecFileName, szSpecSave);
			lstrcpy(szSpecFullPath, m_pPath);
			lstrcat(szSpecFullPath, szSpecFileName);

			if(FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szSpecFullPath, &pNewMeshContainer->ppSpecularTextures[i])))
			{
				lstrcpy(szSpecFullPath, L"../Bin/Resource/Meshes/DefaultSP.tga");
				if(FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szSpecFullPath, &pNewMeshContainer->ppSpecularTextures[i])))
					MSG_BOX("Mesh`s Default Specular Texture Create Failed");

			}
		}				
	}

	else
	{
		pNewMeshContainer->pMaterials[0].MatD3D.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		pNewMeshContainer->pMaterials[0].MatD3D.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		pNewMeshContainer->pMaterials[0].MatD3D.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
		pNewMeshContainer->pMaterials[0].MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pNewMeshContainer->pMaterials[0].MatD3D.Power = 20.f;
		pNewMeshContainer->pMaterials[0].pTextureFilename = NULL;
		pNewMeshContainer->ppTextures[0] = NULL;
		pNewMeshContainer->ppNormalTextures[0] = NULL;
	}				
	
	if(NULL != pSkinInfo)
	{
		pNewMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		// 스키닝		
		pMesh->CloneMeshFVF(pMesh->GetOptions(), pMesh->GetFVF(), m_pGraphicDev, &pNewMeshContainer->pOriMesh);

		// 현재 메시에 영향을 미치는 뼈대들의 정보

		// 뼈의 갯수
		pNewMeshContainer->dwNumBones = pSkinInfo->GetNumBones();

		pNewMeshContainer->pOffsetFrameMatrices = new D3DXMATRIX[pNewMeshContainer->dwNumBones];
		pNewMeshContainer->ppCombinedTransformationMatrices = new LPD3DXMATRIX[pNewMeshContainer->dwNumBones];
		ZeroMemory(pNewMeshContainer->ppCombinedTransformationMatrices, sizeof(LPD3DXMATRIX) * pNewMeshContainer->dwNumBones);

		pNewMeshContainer->pRenderingMatrices = new D3DXMATRIX[pNewMeshContainer->dwNumBones];
		ZeroMemory(pNewMeshContainer->pRenderingMatrices, sizeof(D3DXMATRIX) * pNewMeshContainer->dwNumBones);

		// 뼈대들의 초기상태행렬들을 담는다.
		for (_ulong i = 0; i < pNewMeshContainer->dwNumBones; ++i)
		{
			pNewMeshContainer->pOffsetFrameMatrices[i] = *pSkinInfo->GetBoneOffsetMatrix(i);			
		}
	}

	*ppNewMeshContainer = pNewMeshContainer;

	return S_OK;
}

STDMETHODIMP CHierarchyLoader::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{	
	if(NULL != pFrameToFree)
	{
		Engine::Safe_Delete_Array(pFrameToFree->Name);

		if(NULL != pFrameToFree->pMeshContainer)
			DestroyMeshContainer(pFrameToFree->pMeshContainer);
	}

	if(NULL != pFrameToFree->pFrameSibling)
	{
		DestroyFrame(pFrameToFree->pFrameSibling);
	}

	if(NULL != pFrameToFree->pFrameFirstChild)
	{
		DestroyFrame(pFrameToFree->pFrameFirstChild);
	}

	Engine::Safe_Delete(pFrameToFree);	

	return S_OK;
}

STDMETHODIMP CHierarchyLoader::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	Engine::Safe_Delete_Array(pMeshContainerToFree->Name);
	Engine::Safe_Release(pMeshContainerToFree->MeshData.pMesh);
	Engine::Safe_Delete_Array(pMeshContainerToFree->pMaterials);
	Engine::Safe_Delete_Array(pMeshContainerToFree->pAdjacency);
	Engine::Safe_Release(pMeshContainerToFree->pSkinInfo);

	for (_ulong i = 0; i < pMeshContainerToFree->NumMaterials; ++i)
	{
		if(Engine::Safe_Release(((D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree)->ppTextures[i]))
			MSG_BOX("Texture Release Failed");
		if(Engine::Safe_Release(((D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree)->ppNormalTextures[i]))
			MSG_BOX("Texture Release Failed");
		if(Engine::Safe_Release(((D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree)->ppSpecularTextures[i]))
			MSG_BOX("Texture Release Failed");
	}

	Engine::Safe_Delete_Array(((D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree)->ppTextures);
	Engine::Safe_Delete_Array(((D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree)->ppNormalTextures);
	Engine::Safe_Delete_Array(((D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree)->ppSpecularTextures);
	Engine::Safe_Release(((D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree)->pOriMesh);
	Engine::Safe_Delete_Array(((D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree)->pOffsetFrameMatrices);
	Engine::Safe_Delete_Array(((D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree)->ppCombinedTransformationMatrices);
	Engine::Safe_Delete_Array(((D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree)->pRenderingMatrices);

	Engine::Safe_Delete(pMeshContainerToFree);

	return S_OK;
}

HRESULT CHierarchyLoader::Allocate_Name(char** ppDestName, const char* pSourName)
{	
	if(NULL == pSourName)
		return E_FAIL;

	_int iLength = strlen(pSourName);

	*ppDestName = new char[iLength + 1];

	ZeroMemory(*ppDestName, sizeof(char) * (iLength + 1));

	strcpy(*ppDestName, pSourName);

	return S_OK;
}
