#include "stdafx.h"
#include "Space.h"
#include "SpaceContainer.h"
#include "RenderContainerSeller.h"



void CSpace::Begin(CSpaceContainer * pSpaceContainer, UINT size, int lv, XMVECTOR pos) {
	//object_id set 
	//	m_objectID = object_id::OBJECT_SPACE;
	CGameObject::Begin();
	//size�� �˾ƾ� ������ �𸣴ϱ� �ϴ� ����
	m_size = size;
	m_pSpaceContainer = pSpaceContainer;

	m_mlpObject.clear();
	m_mlpCollisionObject.clear();

	
	//�ڽ��� ��ġ�� ������ 
	//������ �߰��� �ƴ϶� ���� ó��/ ��ǥ�� ���� ��ǥ
	SetPosition(pos);

	//���� ���� --
	lv -= 1;

	//0���� ������ return ������ �ܰ��� ��
	/*
	ex) ó�� lootSpace�� Begin�� lv�� 1�� �־��ٸ�
	���ͼ� -1 �ϰ���? �׷� 0 �̾� 0���� ���� �ʾ�
	�׷� �ڽ��� 4�� ���鲨��. 1�� 1�ܰ�
	0�� �������� ����.
	*/
	if (lv < 0) {
		//�ڽ��� ������ ���� -> leaf space��
		//leaf space�� �ڽ��� index�� �˰� �־�� ��
		//�ڽ��� index�� ��� �� ����
		m_index = m_pSpaceContainer->SearchSpace(GetPosition());
		//space container�� �ڽ��� ���
		m_pSpaceContainer->AddSpace(m_index, this);

		float fx = static_cast<float>(m_pSpaceContainer->GetOneSpaceSize());
		float fy = static_cast<float>(m_pSpaceContainer->GetSpaceSize());
		float fz = static_cast<float>(m_pSpaceContainer->GetOneSpaceSize());

		BoundingBox::CreateFromPoints(m_OriBoundingBox, XMVectorSet(0.f, -fy, 0.f, 0.f), XMVectorSet(fx, fy, fz, 0.f));

		//SetRenderContainer(pSeller);//�׸��� �׸� ���� ������ RenderContainer set
		return;
	}

	//�ڽ� ���� �Ҵ�
	m_ppChildSpace = new CSpace*[4];
	//aabb
	//BoundingBox::CreateFromPoints(m_OriBoundingBox, XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(static_cast<float>(size), static_cast<float>(m_pSpaceContainer->GetSize()), static_cast<float>(size), 0.f));
	BoundingBox::CreateFromPoints(m_OriBoundingBox, XMVectorSet(0.f, -static_cast<float>(m_pSpaceContainer->GetSize()), 0.f, 0.f), XMVectorSet(static_cast<float>(size), static_cast<float>(m_pSpaceContainer->GetSize()), static_cast<float>(size), 0.f));


	int k = 0;
	for (int j = 0; j < 2; ++j) {
		for (int i = 0; i < 2; ++i) {
			UINT child_size = (size / 2);
			//x���� ����, ���� z����.
			/* �ڽ� ������ ����
			2 3
			0 1
			*/
			XMVECTOR xmvOffset = XMVectorSet(static_cast<float>(i*child_size), 0.f, static_cast<float>(j*child_size), 0.f);
			m_ppChildSpace[k] = new CSpace();
			m_ppChildSpace[k++]->Begin(pSpaceContainer, child_size, lv, GetPosition() + xmvOffset);
		}
	}
}

bool CSpace::End() {

	for (auto vpObject : m_mlpObject) {
		for (auto pObject : vpObject.second) {
			pObject->End();
			delete pObject;
		}
	}
	m_mlpObject.clear();
	m_mlpCollisionObject.clear();

	//m_mlpObject.RemoveAll();
	//m_mlpCollisionObject.RemoveAll();
	//for (auto mlp : m_mlpObject) {
	//	for (auto pObject : mlp.second) {
	//		pObject->End();
	//		delete pObject;
	//	}
	//	mlp.second.clear();
	//}
	//m_mlpObject.clear();

	if (m_ppChildSpace) {
		for (int i = 0; i < 4; ++i) {
			m_ppChildSpace[i]->End();
			m_ppChildSpace[i] = nullptr;
		}
	}
	delete m_ppChildSpace;
	m_ppChildSpace = nullptr;




	delete this;

	return true;
}

void CSpace::Animate(float fTimeElapsed) {
	if (m_ppChildSpace) {
		for (int i = 0; i < 4; ++i) {
			m_ppChildSpace[i]->Animate(fTimeElapsed);
		}
	}
	else {//leaf space�� animate
		int nObject = 0;

		list<CGameObject*>::iterator iter = m_mlpObject[tag::TAG_DYNAMIC_OBJECT].begin();
		for (; iter != m_mlpObject[tag::TAG_DYNAMIC_OBJECT].end(); )
		{
 			(*iter)->Animate(fTimeElapsed);
			int current_index = m_pSpaceContainer->SearchSpace((*iter)->GetPosition());
			if ((*iter)->GetSpaceIndex() != current_index)//���� ���� index�� ���� index�� �ٸ��ٸ�
			{
				utag ut = (*iter)->GetUTag();
				m_pSpaceContainer->AddBlockObjectList((*iter));//block Object list�� ���

				m_mlpCollisionObject[ut].remove_if([&iter](CGameObject* pObject) {
					return (pObject == (*iter));
				});

				iter = m_mlpObject[tag::TAG_DYNAMIC_OBJECT].erase(iter);

			}
			else
				++iter;
			nObject++;
		}

		iter = m_mlpObject[tag::TAG_STATIC_OBJECT].begin();
		for (; iter != m_mlpObject[tag::TAG_STATIC_OBJECT].end(); )
		{
			(*iter)->Animate(fTimeElapsed);
			int current_index = m_pSpaceContainer->SearchSpace((*iter)->GetPosition());
			if ((*iter)->GetSpaceIndex() != current_index)//���� ���� index�� ���� index�� �ٸ��ٸ�
			{
				utag ut = (*iter)->GetUTag();
				m_pSpaceContainer->AddBlockObjectList((*iter));//block Object list�� ���

				m_mlpCollisionObject[ut].remove_if([&iter](CGameObject* pObject) {
					return (pObject == (*iter));
				});

				iter = m_mlpObject[tag::TAG_STATIC_OBJECT].erase(iter);

			}
			else
				++iter;
		}

		if (INPUTMGR->GetDebugMode())
			DEBUGER->AddText(20.0f, 800.0f, static_cast<float>(m_index * 15.f), YT_Color(255, 255, 255), L"space %d object_num : %d", m_index, nObject);
		m_bRender = false;
	}
}

void CSpace::PhisicsUpdate(float fTimeElapsed){
	for (auto pPlayer : m_mlpCollisionObject[UTAG_PLAYER]) {
		pPlayer->PhisicsLogic(m_mlpCollisionObject, fTimeElapsed);
	}
	for (auto pBoss : m_mlpCollisionObject[UTAG_BOSS1]) {
		pBoss->PhisicsLogic(m_mlpCollisionObject, fTimeElapsed);
	}

}

void CSpace::OptimizePrepare(UINT renderFlag,  CCamera* pCamera)
{
	tag rt = (tag)renderFlag;
	for (auto pObject : m_mlpObject[rt]) {
		if(pObject->IsVisible(pCamera)) {
			pObject->RegistToContainer();
		}
	}
}
void CSpace::PrepareRender( CCamera* pCamera, UINT renderFlag) {

	if (IsVisible(pCamera))
	{											//���⿡	 space �������� �ø�
		if (nullptr == m_ppChildSpace) {		//�� �ڽ��� ������ ���� leaf node
			SetbRender(true);					//���� �׸��� space��.

			if (INPUTMGR->GetDebugMode())
				this->RegistToDebuger();

			for (auto vpObject : m_mlpObject) {
				if (vpObject.first == TAG_TERRAIN && (renderFlag & TAG_TERRAIN))
					OptimizePrepare(TAG_TERRAIN, pCamera);
				else if (vpObject.first == TAG_STATIC_OBJECT && (renderFlag & TAG_STATIC_OBJECT))
					OptimizePrepare(TAG_STATIC_OBJECT, pCamera);
				else if (vpObject.first == TAG_DYNAMIC_OBJECT && (renderFlag & TAG_DYNAMIC_OBJECT))
					OptimizePrepare(TAG_DYNAMIC_OBJECT, pCamera);
				else if (vpObject.first == TAG_LIGHT && (renderFlag & TAG_LIGHT))
					OptimizePrepare(TAG_LIGHT, pCamera);
				else if (vpObject.first == TAG_REFLECTION && (renderFlag & TAG_REFLECTION))
					OptimizePrepare(TAG_REFLECTION, pCamera);
				else if (vpObject.first == TAG_BIGWATER && (renderFlag & TAG_BIGWATER))
					OptimizePrepare(TAG_BIGWATER, pCamera);
				else if (vpObject.first == TAG_DYNAMIC_SKILL && (renderFlag & TAG_DYNAMIC_SKILL))
					OptimizePrepare(TAG_DYNAMIC_SKILL, pCamera);

			}
			//}//end for
		}//end if
		else {//leaf�� �ƴ϶��
			for (int i = 0; i < 4; ++i) {
				m_ppChildSpace[i]->PrepareRender(pCamera, renderFlag);//�� �ڽĵ� PrePareRender
			}
		}//end else
	}
	//ī�޶� ������ ������ 
	return;
}



void CSpace::SetObejcts(int n, CGameObject** ppObjects) {
	if (!ppObjects) return;

	for (int i = 0; i < n; ++i) {
		//��ü�� �ڽ� ��ȣ ���
		ppObjects[i]->SetSpaceIndex(m_index);

		m_mlpObject[ppObjects[i]->GetTag()].push_back(ppObjects[i]);
		m_mlpCollisionObject[ppObjects[i]->GetUTag()].push_back(ppObjects[i]);
	}
}

void CSpace::AddObject(CGameObject* pObject) {
	if (!pObject) return;
	//��ü�� �ڽ� ��ȣ ���
	pObject->SetSpaceIndex(m_index);

	m_mlpCollisionObject[pObject->GetUTag()].push_back(pObject);
	m_mlpObject[pObject->GetTag()].push_back(pObject);
}
void CSpace::RemoveObject(CGameObject* pObject) {
	if (!pObject) return;
	//if (0 == m_lpObjects.size()) return;

	m_mlpObject[pObject->GetTag()].remove_if([&pObject](CGameObject* pO) {
		return pObject == pO;
	});

}

void CSpace::RemoveObject(string name) {
	for (auto data : m_mlpObject) {
		for (auto pObject : data.second) {
			if (pObject->GetName() == name) {
				m_mlpObject[pObject->GetTag()].remove_if([&pObject](CGameObject* pO) {
					return pObject == pO;
				});
			}
		}
	}
}

CGameObject * CSpace::PickObject(XMVECTOR xmvWorldCameraStartPos, XMVECTOR xmvRayDir, float& distance, UINT tag) {
	float fHitDistance = FLT_MAX;
	distance = fHitDistance;
	float fNearHitDistance = FLT_MAX;
	CGameObject* pObj = nullptr;
	//�ڽ��� ��� ��ü�� ���ؼ� �˻�
	for (auto Objects : m_mlpObject) {
		if (Objects.first & tag) {
			for (auto pObject : Objects.second) {
				if (pObject->CheckPickObject(xmvWorldCameraStartPos, xmvRayDir, fHitDistance)) {//ray�� �浹�ߴٸ�
					if (fNearHitDistance > fHitDistance) {//������ ���� ����� �༮�� ��
						fNearHitDistance = fHitDistance;
						distance = fHitDistance;//�� ������ ���� ����� ��ü ����
						pObj = pObject;
					}
				}
			}
		}
	}
	return pObj;//�ش� ��ü return
}




CSpace::CSpace() : CGameObject("space", tag::TAG_SPACE) {

}

CSpace::~CSpace()
{
}
