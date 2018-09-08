#include "stdafx.h"
#include "Space.h"
#include "SpaceContainer.h"
#include "RenderContainerSeller.h"



void CSpace::Begin(CSpaceContainer * pSpaceContainer, UINT size, int lv, XMVECTOR pos) {
	//object_id set 
	//	m_objectID = object_id::OBJECT_SPACE;
	CGameObject::Begin();
	//size를 알아야 할지도 모르니까 일단 저장
	m_size = size;
	m_pSpaceContainer = pSpaceContainer;

	m_mlpObject.clear();
	m_mlpCollisionObject.clear();

	
	//자신의 위치를 정해줌 
	//공간의 중간이 아니라 제일 처음/ 좌표가 시작 좌표
	SetPosition(pos);

	//현재 래밸 --
	lv -= 1;

	//0보다 작으면 return 마지막 단계라는 것
	/*
	ex) 처음 lootSpace의 Begin의 lv에 1을 넣었다면
	들어와서 -1 하겟지? 그럼 0 이야 0보다 작지 않아
	그럼 자식을 4개 만들꺼야. 1은 1단계
	0은 공간분할 없음.
	*/
	if (lv < 0) {
		//자식을 가지지 않음 -> leaf space임
		//leaf space는 자신의 index를 알고 있어야 함
		//자신의 index를 계산 후 저장
		m_index = m_pSpaceContainer->SearchSpace(GetPosition());
		//space container에 자신을 등록
		m_pSpaceContainer->AddSpace(m_index, this);

		float fx = static_cast<float>(m_pSpaceContainer->GetOneSpaceSize());
		float fy = static_cast<float>(m_pSpaceContainer->GetSpaceSize());
		float fz = static_cast<float>(m_pSpaceContainer->GetOneSpaceSize());

		BoundingBox::CreateFromPoints(m_OriBoundingBox, XMVectorSet(0.f, -fy, 0.f, 0.f), XMVectorSet(fx, fy, fz, 0.f));

		//SetRenderContainer(pSeller);//그림을 그릴 수도 있으니 RenderContainer set
		return;
	}

	//자식 공간 할당
	m_ppChildSpace = new CSpace*[4];
	//aabb
	//BoundingBox::CreateFromPoints(m_OriBoundingBox, XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(static_cast<float>(size), static_cast<float>(m_pSpaceContainer->GetSize()), static_cast<float>(size), 0.f));
	BoundingBox::CreateFromPoints(m_OriBoundingBox, XMVectorSet(0.f, -static_cast<float>(m_pSpaceContainer->GetSize()), 0.f, 0.f), XMVectorSet(static_cast<float>(size), static_cast<float>(m_pSpaceContainer->GetSize()), static_cast<float>(size), 0.f));


	int k = 0;
	for (int j = 0; j < 2; ++j) {
		for (int i = 0; i < 2; ++i) {
			UINT child_size = (size / 2);
			//x먼저 증가, 이후 z증가.
			/* 자식 공간의 순서
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
	else {//leaf space만 animate
		int nObject = 0;

		list<CGameObject*>::iterator iter = m_mlpObject[tag::TAG_DYNAMIC_OBJECT].begin();
		for (; iter != m_mlpObject[tag::TAG_DYNAMIC_OBJECT].end(); )
		{
 			(*iter)->Animate(fTimeElapsed);
			int current_index = m_pSpaceContainer->SearchSpace((*iter)->GetPosition());
			if ((*iter)->GetSpaceIndex() != current_index)//이전 공간 index와 현재 index가 다르다면
			{
				utag ut = (*iter)->GetUTag();
				m_pSpaceContainer->AddBlockObjectList((*iter));//block Object list에 등록

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
			if ((*iter)->GetSpaceIndex() != current_index)//이전 공간 index와 현재 index가 다르다면
			{
				utag ut = (*iter)->GetUTag();
				m_pSpaceContainer->AddBlockObjectList((*iter));//block Object list에 등록

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
	{											//여기에	 space 프러스텀 컬링
		if (nullptr == m_ppChildSpace) {		//내 자식이 없으면 나는 leaf node
			SetbRender(true);					//나는 그리는 space다.

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
		else {//leaf가 아니라면
			for (int i = 0; i < 4; ++i) {
				m_ppChildSpace[i]->PrepareRender(pCamera, renderFlag);//내 자식들 PrePareRender
			}
		}//end else
	}
	//카메라에 보이지 않으면 
	return;
}



void CSpace::SetObejcts(int n, CGameObject** ppObjects) {
	if (!ppObjects) return;

	for (int i = 0; i < n; ++i) {
		//객체에 자신 번호 등록
		ppObjects[i]->SetSpaceIndex(m_index);

		m_mlpObject[ppObjects[i]->GetTag()].push_back(ppObjects[i]);
		m_mlpCollisionObject[ppObjects[i]->GetUTag()].push_back(ppObjects[i]);
	}
}

void CSpace::AddObject(CGameObject* pObject) {
	if (!pObject) return;
	//객체에 자신 번호 등록
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
	//자신의 모든 객체에 대해서 검사
	for (auto Objects : m_mlpObject) {
		if (Objects.first & tag) {
			for (auto pObject : Objects.second) {
				if (pObject->CheckPickObject(xmvWorldCameraStartPos, xmvRayDir, fHitDistance)) {//ray와 충돌했다면
					if (fNearHitDistance > fHitDistance) {//이전의 가장 가까운 녀석과 비교
						fNearHitDistance = fHitDistance;
						distance = fHitDistance;//더 가까우면 가장 가까운 객체 변경
						pObj = pObject;
					}
				}
			}
		}
	}
	return pObj;//해당 객체 return
}




CSpace::CSpace() : CGameObject("space", tag::TAG_SPACE) {

}

CSpace::~CSpace()
{
}
