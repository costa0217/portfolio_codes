#pragma once
#include "Object.h"
#include "Space.h"
#include "Camera.h"
#include "DirectionalLight.h"
//#include "SceneMain.h"
//#include "RenderContainerSeller.h"

class CSpaceContainer : public CObject {
public:
	void Begin();
	bool End();

	//space controller
	void Animate(float fTimeElapsed);
	void PhisicsUpdate(float fTimeElapsed);
	void PrepareRenderOneSpace( CCamera* pCamera, UINT renderFlag = TAG_TERRAIN | TAG_STATIC_OBJECT | TAG_DYNAMIC_OBJECT | TAG_LIGHT | TAG_BIGWATER, int render_space = -1);
	void PrepareRender( CCamera* pCamera, UINT renderFlag = TAG_TERRAIN | TAG_STATIC_OBJECT | TAG_DYNAMIC_OBJECT | TAG_LIGHT | TAG_REFLECTION | TAG_BIGWATER);
	//animate�ϴٰ� �ش� ������ ��� ��ü �ӽ� �����
	void AddBlockObjectList(CGameObject* pObject);
	void AddObject(CGameObject* pObject);
	void RemoveObject(CGameObject* pObject);
	void RemoveObject(string name);
	void ClearBlockObjectList();
	int SearchSpace(XMVECTOR xmvPos);
	void AddSpace(UINT index, CSpace* pSpace);
	void ChangeSpaceData();
	//��� lay �浹 �˻�� ��ü �� ���� ���� ����� ��ü
	CGameObject* PickObject(XMVECTOR xmvWorldCameraStartPos, XMVECTOR xmvRayDir, float& distanse, UINT renderFlag = TAG_TERRAIN | TAG_STATIC_OBJECT | TAG_DYNAMIC_OBJECT | TAG_LIGHT | TAG_REFLECTION | TAG_BIGWATER);
	//space controller

	static CSpaceContainer* CreateSpaceContainer(int size, int lv);

	void ClearAllObjects();
	void SaveObjectInfos();
	void LoadObjectInfos();
private:
	//space���� �����Ѵ�.
	CSpace** m_ppSpace{ nullptr };
	//���� Ʈ���� ��Ʈ��忡 �ش��ϴ� startSpace ����
	CSpace* m_pStartSpace{ nullptr };
	//block object list
	//list<CGameObject*> m_lpBlockObject;
	list <CGameObject*> m_lpBlockObject;

	//��ü ���� ũ��
	float m_size{ 0 };
	//������ ���� 
	float m_level{ 0 };
	//�� ���̵忡 �ִ� ������ ����
	int m_oneSideSpaceNum{ 0 };
	//���� �ϳ��� ũ��
	int m_oneSpaceSize{ 0 };
	//������ ����
	int m_nSpace{ 0 };

public:
	CSpaceContainer();
	~CSpaceContainer();

	//set get
	CSpace** GetAllSpace() { return m_ppSpace; }
	list <CGameObject*>& GetBlockObjectList() { return m_lpBlockObject; }
	CSpace* GetStartSpace() { return m_pStartSpace; }
	UINT GetSpaceNum() { return (UINT)m_nSpace; }
	UINT GetSize() { return (UINT)m_size; }
	UINT GetLevel() { return (UINT)m_level; }
	UINT GetOneSideSpaceNum() { return (UINT)m_oneSideSpaceNum; }
	UINT GetOneSpaceSize() { return (UINT)m_oneSpaceSize; }

	int GetSpaceSize() { return (int)m_size; }
	int GetSpaceLevel() { return (int)m_level; }
	void SetSpaceSize(int size) { m_size = (float)size; }
	void SetSpaceLevel(int lv) { m_level = (float)lv; }
	//set get
};