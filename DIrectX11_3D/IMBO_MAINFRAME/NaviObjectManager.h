#pragma once

#include "NaviVertex.h"
#include "NaviObject.h"
#include "SingleTon.h"

class CBuffer;

//NaviObject�� �����Ѵ�. picking����� ó���ϸ� 
//gs�� ���� �����͸� �����Ѵ�.
//
class CNaviObjectManager{
public:
	static bool Begin();
	static bool End();

	//NaviObject�� ���� ���������� �ϴ� render��
	static void Render();
	static void PickingProc(XMFLOAT3 xmf3PickingPos);

	static void AddNaviObject(CNaviObject* pNaviObject) { m_vNaviObject.push_back(pNaviObject); }
	static void AddNaviVertex(CNaviVertex* pNaviVertex) { m_vpNaviVertex.push_back(pNaviVertex); }
	static void ClearNaviObject() { 
		for (auto pObject : m_vNaviObject) {
			pObject->End();
			delete pObject;
		}
		m_vNaviObject.clear(); 
	}
	static void ClearNaviVertex() {
		for (auto pVertex : m_vpNaviVertex) {
			pVertex->End();//�ƹ��ŵ� ���ϴ� ������ �׳�
			delete pVertex;
		}
		m_vpNaviVertex.clear(); 	
	}

	//navi vertex�� index�� �̳༮�� �ο��Ѵ�.
	static void ResetVertexIndex();
	static vector<CNaviVertex*>& GetNaviVertices() { return m_vpNaviVertex; }
	static void DeleteVertex(int index);
	static void DeleteNaviObject(int index);

	static float GetHeight(XMFLOAT2 pos, int index);
	static void CreateNaviObject();

	static bool IsValiableIndex(int index);
	static XMVECTOR GetNaviMeshPosition(int index);
	static int GetValiableIndex(XMVECTOR pos);
	static bool IsIntersection(float x, float z, int index, XMVECTOR& edgeNormal);
	//static bool GetIntersectEdge(float x, float z, int index, XMVECTOR& edge);
	static int GetIndex(float x, float z, int index);

	static void LoadData();
	//static bool Is
private:
	static bool m_bAutoCreateNaviObject;
	//navimesh�� ������ �� ����� control vertex �ִ� 3��
	//static int m_ControlVertexs[3];//index ��
	static CNaviVertex* m_ControlVertexs[3];//index ��
	static int m_ControlVertexCount;

	static vector<CNaviObject*> m_vNaviObject;

	static vector<CNaviVertex*> m_vpNaviVertex;
	static  CBuffer* m_pGSNaviMeshVertexBuffer;
	static  CBuffer* m_pNaviMeshInstancingBuffer;

	static  CMesh* m_pNaviMesh;
	static  CRenderShader* m_pNaviObjectShader;
public:
	CNaviObjectManager();
	~CNaviObjectManager();
};
