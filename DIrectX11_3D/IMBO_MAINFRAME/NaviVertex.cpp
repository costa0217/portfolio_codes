#include "stdafx.h"
#include "NaviVertex.h"


void CNaviVertex::Begin()
{
}

void CNaviVertex::End(){

}

CNaviVertex * CNaviVertex::CreateNaviVertex(XMFLOAT3 pos){
	CNaviVertex* pVertex = new CNaviVertex;
	pVertex->SetPosition(pos);

	return pVertex;
}

void CNaviVertex::RefreshPosition(){
	for (auto pNaviObject : m_vpNaviObject) {
		pNaviObject->RefreshPlane();
	}
}

void CNaviVertex::DeleteNaviObject(int index){
	auto Iter = m_vpNaviObject.begin();

	for (auto pNaviObject : m_vpNaviObject) {
		if (pNaviObject->GetNaviObjectID() == index) break;
		Iter++;
	}
	m_vpNaviObject.erase(Iter);
	//지웠는데 내가 가진 object가 없다면 난 허공에 뜬 점이 되니까 지워줌
	if (m_vpNaviObject.empty()) {
		CNaviObjectManager::DeleteVertex(m_index);
	}
	//여기서 다른 객체가 아직 이 정점을 들고있어서 문제가 될 수있는데 
	//그 경우에는 이 매커니즘에서 이미 오류가 난거기 때문에
	//이 로직에는 이상이 없는 것이다.
}
