#include "stdafx.h"
#include "NaviObject.h"

int CNaviObject::m_NaviObjectIDCount = 0;

bool CNaviObject::Begin(){
	m_vBorderNaviObject.clear();
	m_vpNaviVertex.clear();

	return true;
}

bool CNaviObject::End(){

	//NaviObject�� NaviobjectManager���� �� �����.
	m_vBorderNaviObject.clear();
	m_vpNaviVertex.clear();
	
	return true;
}

void CNaviObject::RefreshPlane(){
	XMVECTOR xmvPos0 = XMLoadFloat3(&m_vpNaviVertex[0]->GetPosition());
	XMVECTOR xmvPos1 = XMLoadFloat3(&m_vpNaviVertex[1]->GetPosition());
	XMVECTOR xmvPos2 = XMLoadFloat3(&m_vpNaviVertex[2]->GetPosition());

	//01 02
	XMFLOAT4 xmf4Plane;
	XMStoreFloat4(&xmf4Plane, XMPlaneFromPoints(xmvPos0, xmvPos1, xmvPos2));
	SetPlane(xmf4Plane);
}

CNaviObject * CNaviObject::CreateNaviObject(int idx0, int idx1, int idx2){
	CNaviObject* pNaviObejct = new CNaviObject;

	pNaviObejct->AddNaviVertex(CNaviObjectManager::GetNaviVertices()[idx0]);
	pNaviObejct->AddNaviVertex(CNaviObjectManager::GetNaviVertices()[idx1]);
	pNaviObejct->AddNaviVertex(CNaviObjectManager::GetNaviVertices()[idx2]);

	XMVECTOR xmvPos0 = XMLoadFloat3(&CNaviObjectManager::GetNaviVertices()[idx0]->GetPosition());
	XMVECTOR xmvPos1 = XMLoadFloat3(&CNaviObjectManager::GetNaviVertices()[idx1]->GetPosition());
	XMVECTOR xmvPos2 = XMLoadFloat3(&CNaviObjectManager::GetNaviVertices()[idx2]->GetPosition());

	//01 02
	XMFLOAT4 xmf4Plane;
	XMStoreFloat4(&xmf4Plane, XMPlaneFromPoints(xmvPos0, xmvPos1, xmvPos2));
	pNaviObejct->SetPlane(xmf4Plane);

	return pNaviObejct;
}

CNaviObject * CNaviObject::CreateNaviObject(CNaviVertex* idx0, CNaviVertex* idx1, CNaviVertex* idx2) {
	CNaviObject* pNaviObejct = new CNaviObject;

	pNaviObejct->AddNaviVertex(idx0);
	pNaviObejct->AddNaviVertex(idx1);
	pNaviObejct->AddNaviVertex(idx2);

	XMVECTOR xmvPos0 = XMLoadFloat3(&idx0->GetPosition());
	XMVECTOR xmvPos1 = XMLoadFloat3(&idx1->GetPosition());
	XMVECTOR xmvPos2 = XMLoadFloat3(&idx2->GetPosition());

	//01 02
	XMFLOAT4 xmf4Plane;
	XMStoreFloat4(&xmf4Plane, XMPlaneFromPoints(xmvPos0, xmvPos1, xmvPos2));
	pNaviObejct->SetPlane(xmf4Plane);

	XMVECTOR vEdge;
	vEdge = xmvPos1 - xmvPos0;
	pNaviObejct->AddEdge(vEdge);
	vEdge = xmvPos2 - xmvPos1;
	pNaviObejct->AddEdge(vEdge);
	vEdge = xmvPos0 - xmvPos2;
	pNaviObejct->AddEdge(vEdge);
	
	return pNaviObejct;
}
void CNaviObject::DeleteVertexCallback(CNaviVertex* pDeleteVertex){
	//���� �����ϴ� �����߿� �ϳ��� �������. 
	//�׷� ���� �������Ѵ�. 
	//���� ����� ���� �� �ֺ��� ������ ���� ��������� �뺸�Ѵ�. 
	
	int test = 0;
	//for (int i = 0; i < 3; ++i) {
	//	if (index == m_vpNaviVertex[i]->GetIndex()) continue;
	//	//�ڽ��� id�� �����϶�� ����Ѵ�. 
	//	m_vpNaviVertex[i]->DeleteNaviObject(m_NaviObjectID);
	//	test++;
	//}
	for (auto pVertex : m_vpNaviVertex) {
		int index = pDeleteVertex->GetIndex();
		if (index == pVertex->GetIndex()) continue;
		//�ڽ��� id�� �����϶�� ����Ѵ�. 
		pVertex->DeleteNaviObject(m_NaviObjectID);
		test++;
	}
	m_vpNaviVertex.clear();

	if (test == 3)
		int a = 0;
	//�ֺ� ��ü����
	for (auto pNaviObject : m_vBorderNaviObject) {
		pNaviObject->DeleteNaviObject(m_NaviObjectID);
	}
	m_vBorderNaviObject.clear();
}

void CNaviObject::DeleteNaviObject(int index){
	auto Iter = m_vBorderNaviObject.begin();

	for (auto pNaviObject : m_vBorderNaviObject) {
		if (pNaviObject->GetNaviObjectID() == index) break;
		Iter++;
	}
	m_vBorderNaviObject.erase(Iter);
}

float CNaviObject::GetHeight(float x, float z){
	float a = m_xmf4Plane.x;
	float b = m_xmf4Plane.y;
	float c = m_xmf4Plane.z;
	float d = m_xmf4Plane.w;

	return (-a*x -c*z - d)/b;
}

bool CNaviObject::IsIntersection(float x, float z, XMVECTOR& edgeNormal){

	float y = GetHeight(x, z);
	XMVECTOR vPoint = XMVectorSet(x, y, z, 1.0f);

	XMVECTOR v0, v1, v2;
	v0 = XMLoadFloat3(&m_vpNaviVertex[0]->GetPosition());
	v1 = XMLoadFloat3(&m_vpNaviVertex[1]->GetPosition());
	v2 = XMLoadFloat3(&m_vpNaviVertex[2]->GetPosition());

	XMVECTOR n = XMLoadFloat4(&m_xmf4Plane);
	XMVector3Normalize(n);

	XMVECTOR vEdge, vEN, vDir;
	XMFLOAT4 vResult;

	vEdge = m_vEdges[0];
	vEN = XMVector3Cross(vEdge, n);
	vDir = v0 - vPoint;
	XMStoreFloat4(&vResult, XMVector3Dot(vDir, vEN));
	if (vResult.x < 0.f) {
		edgeNormal = XMVector3Normalize(vEN);
		return false;
	}

	vEdge = m_vEdges[1];
	vEN = XMVector3Cross(vEdge, n);
	vDir = v1 - vPoint;
	XMStoreFloat4(&vResult, XMVector3Dot(vDir, vEN));
	if (vResult.x < 0.f) {
		edgeNormal = XMVector3Normalize(vEN);
		return false;
	}

	vEdge = m_vEdges[2];
	vEN = XMVector3Cross(vEdge, n);
	vDir = v2 - vPoint;
	XMStoreFloat4(&vResult, XMVector3Dot(vDir, vEN));
	if (vResult.x < 0.f) {
		edgeNormal = XMVector3Normalize(vEN);
		return false;
	}

	return true;
}

bool CNaviObject::GetInterSectEdge(float x, float z, XMVECTOR& out){

	float y = GetHeight(x, z);
	XMVECTOR vPoint = XMVectorSet(x, y, z, 1.0f);

	XMVECTOR v0, v1, v2;
	v0 = XMLoadFloat3(&m_vpNaviVertex[0]->GetPosition());
	v1 = XMLoadFloat3(&m_vpNaviVertex[1]->GetPosition());
	v2 = XMLoadFloat3(&m_vpNaviVertex[2]->GetPosition());

	XMVECTOR n = XMLoadFloat4(&m_xmf4Plane);
	XMVector3Normalize(n);

	XMVECTOR vEdge, vEN, vDir;
	XMFLOAT4 vResult;

	vEdge = m_vEdges[0];
	vEN = XMVector3Cross(vEdge, n);
	vDir = v0 - vPoint;
	XMStoreFloat4(&vResult, XMVector3Dot(vDir, vEN));
	if (vResult.x < 0.f) return false;

	vEdge = m_vEdges[1];
	vEN = XMVector3Cross(vEdge, n);
	vDir = v1 - vPoint;
	XMStoreFloat4(&vResult, XMVector3Dot(vDir, vEN));
	if (vResult.x < 0.f)return false;

	vEdge = m_vEdges[2];
	vEN = XMVector3Cross(vEdge, n);
	vDir = v2 - vPoint;
	XMStoreFloat4(&vResult, XMVector3Dot(vDir, vEN));
	if (vResult.x < 0.f)return false;

	return true;
}

CNaviObject::CNaviObject(){
	//naviobject�� �����Ǹ� id�� �ο��Ѵ�.
	m_NaviObjectID = m_NaviObjectIDCount++;
}

CNaviObject::~CNaviObject(){

}
