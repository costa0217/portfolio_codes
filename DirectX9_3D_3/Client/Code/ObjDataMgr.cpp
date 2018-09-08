#include "stdafx.h"
#include "ObjDataMgr.h"

#include "Export_Function.h"
#include "Pipeline.h"
#include "Tree.h"

IMPLEMENT_SINGLETON(CObjDataMgr)


CObjDataMgr::CObjDataMgr(void)
{

}

CObjDataMgr::~CObjDataMgr(void)
{
	Release();
}

HRESULT CObjDataMgr::LoadObjectData(LPDIRECT3DDEVICE9 pDevice
									, wstring wstrDataPath
									, Engine::RESOURCETYPE eResourceType 
									, Engine::CVIBuffer::BUFFERTYPE eBufferType 
									, const wstring& wstrResourceKey)
{
	/************************************************************************/
	/* �����͸� �ҷ����� �κ�                                               */
	/************************************************************************/
	HANDLE	hFile = CreateFile(wstrDataPath.c_str(), GENERIC_READ, 0, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	NULL_CHECK_RETURN(hFile, E_FAIL);

	DWORD				dwByte;
	vector<Engine::MINSAVE*>			vecMinLoad;

	while(true)
	{
		Engine::MINSAVE*	LoadMinData	= new Engine::MINSAVE;
		ReadFile(hFile, LoadMinData, sizeof(Engine::MINSAVE), &dwByte, NULL);

		if(dwByte == 0)
		{
			Engine::Safe_Delete(LoadMinData);
			break;
		}
		vecMinLoad.push_back(LoadMinData);
	}

	/************************************************************************/
	/* �ҷ��� �����ͷ� ���۷� �� ���ؽ��� ����� �۾�                       */
	/************************************************************************/

	D3DXMATRIX				m_matWorld;						// ���ؽ��� �������
	D3DXVECTOR3				m_vPos;							// ��ġ
	D3DXVECTOR3				m_vScale;						// ������ 
	float					m_fAngle[Engine::ANGLE_END];	// ����
	int						m_iColor[Engine::COLOR_END];	// ����

	vector<Engine::VTXCOL> vecVTXBuffer;					// ������ǥ�� ���ؽ��� ������ǥ�� ��ȯ���� ���� �����̳�

	size_t vecSize = vecMinLoad.size();
	for(size_t i = 0; i < vecSize; ++i)
	{
		m_vPos		= vecMinLoad[i]->vRectPos;
		m_vScale	= vecMinLoad[i]->vScale;
		for(int j = 0; j < 3; ++j)
		{
			m_fAngle[j]	= vecMinLoad[i]->fAngle[j];
			m_iColor[j] = vecMinLoad[i]->iColor[j];
		}

		Engine::VTXCOL m_pVertex[4];	// �ӽ÷� RcCol ���ؽ��� ���� �迭

		Engine::Get_ResourcesMgr()->GetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcCol", m_pVertex);

		D3DXMatrixIdentity(&m_matWorld);
		Engine::CPipeline::MakeWorldMatrix(&m_matWorld, &m_vScale, m_fAngle, &m_vPos);

		for(size_t k = 0; k < 4; ++k)
		{
			Engine::VTXCOL m_pConvertVertex;	//�Ѱ��Ѱ� �־��ٷ��� ���� ����

			m_pConvertVertex = m_pVertex[k];

			m_pConvertVertex.dwColor = D3DCOLOR_ARGB(255, m_iColor[0], m_iColor[1], m_iColor[2]);					// ���ؽ� ���� ����
			D3DXVec3TransformCoord(&m_pConvertVertex.vPos, &m_pConvertVertex.vPos, &m_matWorld);		// ���ؽ� ������ǥ ����

			vecVTXBuffer.push_back(m_pConvertVertex);
		}
	}

	CloseHandle(hFile);

	// �ε嵥���� �����κ�
	vector<Engine::MINSAVE*>::iterator iter		= vecMinLoad.begin();
	vector<Engine::MINSAVE*>::iterator iter_end	= vecMinLoad.end();
	for( ; iter != iter_end; ++iter)
		Engine::Safe_Delete(*iter);


	// ������ �����ͷθ��� ���ؽ��� ���ҽ��Ŵ����� �������� ����
	Engine::Get_ResourcesMgr()->AddDataBuffer(pDevice, eResourceType, eBufferType, wstrResourceKey, vecVTXBuffer);

	return S_OK;
}

HRESULT CObjDataMgr::LoadObjectPositionFromFile(wstring wstrPath)
{
	HANDLE hFile = CreateFile(wstrPath.c_str(), GENERIC_READ, 0, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	NULL_CHECK_RETURN(hFile, E_FAIL);

	DWORD	dwByte = 0;

	Engine::STATICOBJECT* pObject = new Engine::STATICOBJECT;

	while(true)
	{
		ReadFile(hFile, pObject, sizeof(Engine::STATICOBJECT), &dwByte, NULL);

		if(dwByte == 0)
			break;

		MakeObject(wstring(pObject->szObjectName), pObject->vPos);
	}
	Engine::Safe_Delete(pObject);

	CloseHandle(hFile);

	return S_OK;
}

void CObjDataMgr::MakeObject(wstring wstrObjectName, D3DXVECTOR3 vPosition)
{
	//�ҷ��� �̸��� ��ġ�� ������Ʈ ����
	//�Ǵ� �����̳ʿ� ��Ƶα�
	//SmallTree_Min -> Buffer_SmallTree
	wstrObjectName = L"Buffer_" + wstrObjectName.substr(0, wstrObjectName.size() - 4);
	map<wstring, vector<D3DXVECTOR3>>::iterator iter = m_mapObjPos.find(wstrObjectName);
	
	if (iter == m_mapObjPos.end())
	{
		m_mapObjPos[wstrObjectName] = vector<D3DXVECTOR3>();
	}
		
	m_mapObjPos[wstrObjectName].push_back(vPosition);



	

}

void CObjDataMgr::Release(void)
{

}

map<wstring, vector<D3DXVECTOR3>>* CObjDataMgr::GetMapObPos(void)
{
	return &m_mapObjPos;
}
