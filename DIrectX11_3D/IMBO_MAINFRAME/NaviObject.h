#pragma once

struct NaviMeshIndexData {
	UINT m_pIndices[3];
};
class CNaviVertex;

class CNaviObject {
public:
	bool Begin();
	bool End();

	vector<CNaviVertex*>& GetNaviVertices() { return m_vpNaviVertex; }
	vector<CNaviObject*>& GetBorderNaviObjects() { return m_vBorderNaviObject; }

	void AddNaviVertex(CNaviVertex* pVertex) { m_vpNaviVertex.push_back(pVertex); }
	void SetPlane(XMFLOAT4 xmf4Plane) { m_xmf4Plane = xmf4Plane; }
	void RefreshPlane();
	//void Setindieces(int* pIndieces) { m_pIndieces = pIndieces; }
	void AddBorderNaviObject(CNaviObject* pNaviObject) { m_vBorderNaviObject.push_back(pNaviObject); }

	static CNaviObject* CreateNaviObject(int idx0, int idx1, int idx2);
	static CNaviObject* CreateNaviObject(CNaviVertex* idx0, CNaviVertex* idx1, CNaviVertex* idx2);
	int GetNaviObjectID() { return m_NaviObjectID; }

	void DeleteVertexCallback(CNaviVertex* pVertex);
	void DeleteNaviObject(int index);

	float GetHeight(float x, float z);
	bool IsIntersection(float x, float z, XMVECTOR& edgeNormal);

	vector<XMVECTOR>& GetEdges() { return m_vEdges; }
	bool GetInterSectEdge(float x, float z, XMVECTOR& out);


	static int m_NaviObjectIDCount;
private:
	void AddEdge(XMVECTOR edge) { m_vEdges.push_back(edge); }

	int m_NaviObjectID{ 0 };

	vector<CNaviVertex*> m_vpNaviVertex;
	vector<CNaviObject*> m_vBorderNaviObject;
	vector<XMVECTOR> m_vEdges;
	XMFLOAT4 m_xmf4Plane;
public:
	CNaviObject();
	~CNaviObject();
};