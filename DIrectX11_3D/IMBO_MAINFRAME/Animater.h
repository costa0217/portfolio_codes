#pragma once
#include "DXObject.h"
#include "AnimationInfo.h"
#include "SkeletonData.h"
#include "BoundingBox.h"

class CAnimater : public DXObject {
public:
	bool Begin();
	virtual bool End();
	virtual void SetShaderState();
	virtual void CleanShaderState();

	void Update(float fTimeElapsed);

	void AddAnimationInfo(CAnimationInfo* pAnimationInfo);
	//void CreateJointTree();
	//void ChangeAllAnimationInfoJointData();
	//void DeleteAnimationInfo(UINT AnimationIndex);

	bool SetCurAnimationIndex(UINT AnimationIndex);
	UINT GetCurAnimationIndex() { return m_CurAnimationIndex; }

	CAnimationInfo* GetCurAnimationInfo() { return m_vpAnimationInfos[m_CurAnimationIndex]; }
	CAnimationInfo* GetAnimationInfo(UINT AnimationIndex = 0) { return m_vpAnimationInfos[AnimationIndex]; }
	CSkeletonData* GetSkeletonData() { return m_pSkeletonData; }

	size_t GetAnimationCnt() { return m_vpAnimationInfos.size(); }

	CBoundingBox* GetMainAABB() { return m_pMainBoundingBox; }

	//util
	void ResetAnimationInfos();

	//helper func
	UINT GetAnimaterJointCnt();


	//create func
	static  CAnimater* CreateAnimaterFromFBXFile(bool bHasAnimation = true);
	static  CAnimater* CreateAnimaterFromGJMFile(bool bHasAnimation = true);

	vector<CBoundingBox*> GetActiveOBBs() { return m_vpAnimationInfos[m_CurAnimationIndex]->GetvActiveOBBs(); }

	void SetMeshOffsetMtx(XMFLOAT4X4& xmf4x4Mtx) { m_xmf4x4MeshOffsetMtx = xmf4x4Mtx; }
	void SetMeshOffsetMtx(XMMATRIX xmMtx) { XMStoreFloat4x4(&m_xmf4x4MeshOffsetMtx, xmMtx); }
	XMMATRIX GetMeshOffsetMtx() { return XMLoadFloat4x4(&m_xmf4x4MeshOffsetMtx); }
	void SetpAnimBuffer( CBuffer* pAnimBuffer) { m_pAnimBuffer = pAnimBuffer; }
	void SetpMainAABB(CBoundingBox* pMainBoundingBox) { m_pMainBoundingBox = pMainBoundingBox; }
	void SetpSkeletonData(CSkeletonData* pSkeletonData) { m_pSkeletonData = pSkeletonData; }

	void SetRHand(int nIndex) { m_nRHandIndex = nIndex; };
	void SetLHand(int nIndex) { m_nLHandIndex = nIndex; };
	void SetHead(int nIndex) { m_nHeadIndex = nIndex; }
	int GetRHand() { return m_nRHandIndex; };
	int GetLHand() { return m_nLHandIndex; };
	int GetHead() { return m_nHeadIndex; }

	void AddActiveOBB(CBoundingBox* pOBB) { m_lActiveBoundingBox.push_back(pOBB); };
	list<CBoundingBox*>& GetAnimaterActiveOBBs() { return m_lActiveBoundingBox; }
private:
	//animater�� ��ǥ obb list
	list<CBoundingBox*> m_lActiveBoundingBox;
	//animater�� ��ǥ obb list

	int m_nRHandIndex{ 0 };
	int m_nLHandIndex{ 0 };
	int m_nHeadIndex{ 0 };

	XMFLOAT4X4 m_xmf4x4MeshOffsetMtx;
	CBoundingBox* m_pMainBoundingBox{ nullptr };
	CSkeletonData* m_pSkeletonData{ nullptr };

	vector<CAnimationInfo*> m_vpAnimationInfos;
	//CAtlArray<CAnimationInfo*> m_vpAnimationInfos;
	UINT m_CurAnimationIndex{ 0 };

	//	//joint tree ����
	//	vector<string> m_vJointName;

	//buffer
	 CBuffer* m_pAnimBuffer{ nullptr };

public:
	CAnimater();
	~CAnimater();

};