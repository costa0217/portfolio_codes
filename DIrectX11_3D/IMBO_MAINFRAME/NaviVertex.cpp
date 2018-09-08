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
	//�����µ� ���� ���� object�� ���ٸ� �� ����� �� ���� �Ǵϱ� ������
	if (m_vpNaviObject.empty()) {
		CNaviObjectManager::DeleteVertex(m_index);
	}
	//���⼭ �ٸ� ��ü�� ���� �� ������ ����־ ������ �� ���ִµ� 
	//�� ��쿡�� �� ��Ŀ���򿡼� �̹� ������ ���ű� ������
	//�� �������� �̻��� ���� ���̴�.
}
