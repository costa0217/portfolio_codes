#include "stdafx.h"
#include "EffectMgr.h"


bool CEffectMgr::Begin()
{
	return false;
}

bool CEffectMgr::End()
{
	
	for (auto vEffect : m_mapProto) {
		for (auto pEffect : vEffect.second) {
			pEffect->Release();
			delete pEffect;
		}
	}
	for (auto vEffect : m_mapEffectList) {
		for (auto pEffect : vEffect.second) {
			pEffect->Release();
			delete pEffect;
		}
	}
	return false;
}

void CEffectMgr::Load_EffectData(TCHAR * pName, TCHAR * pKey, int iCreatNum)
{
	HANDLE		hFile = CreateFile(pName, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD		dwByte;


	vector<CMyEffect*>	VecNewObject;

	size_t iEffectCnt = 0;
	ReadFile(hFile, &iEffectCnt, sizeof(size_t), &dwByte, NULL);

	bool bDecal;
	bool bFollow;

	ReadFile(hFile, &bDecal, sizeof(bool), &dwByte, NULL);
	ReadFile(hFile, &bFollow, sizeof(bool), &dwByte, NULL);
	float fEffectLastTime = 0.f;
	string sPath = "../../Assets/EffectTexture/";
	string sFile = ".tga";
	for (int i = 0; i < iEffectCnt; ++i)
	{
		EFFECTDATA  EffectData;
		ZeroMemory(&EffectData, sizeof(EFFECTDATA));

		ReadFile(hFile, &EffectData, sizeof(EFFECTDATA), &dwByte, NULL);

		CMyEffect* pObject = new CMyEffect;
		pObject->Initialize();
		string sFileName = TCHARToString(EffectData.szName);
		string sFullPath = sPath + sFileName + sFile;

		wchar_t* wide_string = new wchar_t[sFullPath.length() + 1];
		std::copy(sFullPath.begin(), sFullPath.end(), wide_string);
		wide_string[sFullPath.length()] = 0;

		//wstring wide_string = wstring(sFullPath.begin(), sFullPath.end());
		if (i != 0)
			RESOURCEMGR->CreateTexture(sFileName, wide_string, PS_TEXTURE, BIND_PS);
		pObject->SetTexName(EffectData.szName);
		pObject->m_tEffectInfo.m_iTexOpt = EffectData.iTexOpt;
		pObject->m_tEffectInfo.m_iU = EffectData.iU;
		pObject->m_tEffectInfo.m_iV = EffectData.iV;
		pObject->m_tEffectInfo.m_fFrameSpd = EffectData.fFrameSpd;
		pObject->m_bBillboard = EffectData.bBillboard;
		pObject->m_bDistortion = EffectData.bDistortion;
		pObject->m_bRepeat = EffectData.bRepeat;
		if (i != 0)
			pObject->m_pParent = VecNewObject[0];
		VecNewObject.push_back(pObject);

		pObject->Delete_PointAll();
		delete[] wide_string;

		for (int j = 0; j < EffectData.iPointSize; ++j)
		{
			POINTLERP PtLinearData;
			ZeroMemory(&PtLinearData, sizeof(POINTLERP));
			ReadFile(hFile, &PtLinearData, sizeof(POINTLERP), &dwByte, NULL);

			POINTLERP* PointData = new POINTLERP;
			PointData->fTimePoint = PtLinearData.fTimePoint;
			PointData->xmf3PosPoint = PtLinearData.xmf3PosPoint;
			PointData->xmf3AnglePoint = PtLinearData.xmf3AnglePoint;
			PointData->xmf3ScalePoint = PtLinearData.xmf3ScalePoint;
			PointData->xmf4ColorPoint = PtLinearData.xmf4ColorPoint;

			if (fEffectLastTime < PointData->fTimePoint)
				fEffectLastTime = PointData->fTimePoint;

			pObject->GetVectorLinear()->push_back(PointData);
		}
	}
	for (int i = 0; i < iEffectCnt; ++i)
	{
		VecNewObject[i]->m_fEffectEndTime = fEffectLastTime;
	}
	m_mapProto.insert(map<const TCHAR*, vector<CMyEffect*>>::value_type(pKey, VecNewObject));
	for (int i = 0; i < iCreatNum; ++i) {
		Create_Effect(pKey);
	}
	CloseHandle(hFile);
}

vector<CMyEffect*>* CEffectMgr::Find_ProtoEffectList(TCHAR * pKey)
{
	map<const TCHAR*, vector<CMyEffect*>>::iterator iter = find_if(m_mapProto.begin(), m_mapProto.end(), CTagFinder(pKey));
	if (iter == m_mapProto.end())		return nullptr; // iter가 end까지 도달하면 현재 맵 컨터이너에 없다는 뜻이다.

	return &(iter->second);
}

vector<CMyEffect*>* CEffectMgr::Find_EffectList(TCHAR * pKey)
{
	map<const TCHAR*, vector<CMyEffect*>>::iterator iter = find_if(m_mapEffectList.begin(), m_mapEffectList.end(), CTagFinder(pKey));
	if (iter == m_mapEffectList.end())		return nullptr; // iter가 end까지 도달하면 현재 맵 컨터이너에 없다는 뜻이다.

	return &(iter->second);
}

void CEffectMgr::Update_PlayEffectList(float fTimeDelta)
{
	size_t iPlayEffectSize = m_vecPlayEffect.size();
	DEBUGER->AddText(20.f, 100.f, 650.f, YT_Color(255.f, 255.f, 255.f), L"EffectCnt %d", iPlayEffectSize);

	vector<CMyEffect*>::iterator Iter = m_vecPlayEffect.begin();
	while (Iter != m_vecPlayEffect.end())
	{
		(*Iter)->Update(fTimeDelta);
		if ((*Iter)->m_bAlive == false) {
			Iter = m_vecPlayEffect.erase(Iter);
		}
		else
			++Iter;
	}


//	auto Iter = m_vecPlayEffect.begin();
//	auto Iter_end = m_vecPlayEffect.end();
//	//for (size_t i = 0; i < iPlayEffectSize; i++)
//	for( ; Iter != m_vecPlayEffect.end(); ){
//		(*Iter)->Update(fTimeDelta);
//		if ((*Iter)->m_bAlive == false){
//			m_vecPlayEffect.erase(Iter);
//			Iter_end = m_vecPlayEffect.end();
//		}
//		else
//			++Iter;
//	}
}

void CEffectMgr::Create_Effect(TCHAR* pKey)
{
	vector<CMyEffect*>* pOriVecEffect = Find_ProtoEffectList(pKey);		// 원본을 가져온다.
	vector<CMyEffect*>* pVecEffect = Find_EffectList(pKey);		// 담을곳을 가져온다.
	if (pOriVecEffect == nullptr) return;
	if (pVecEffect == nullptr)
	{
		vector<CMyEffect*>	VecObjectList;
		m_mapEffectList.insert(map<const TCHAR*, vector<CMyEffect*>>::value_type(pKey, VecObjectList));
		pVecEffect = Find_EffectList(pKey);		// 원본을 가져온다.
	}

	size_t iVecProtoEffectSize = pOriVecEffect->size();
	size_t iVecEffectSize = pVecEffect->size();
	for (size_t i = 0; i < iVecProtoEffectSize; ++i)
	{
		CMyEffect* pObject = new CMyEffect;
		pObject->Initialize();
		pObject->m_pOriEffect = (*pOriVecEffect)[i];
		pObject->SetTexName((*pOriVecEffect)[i]->m_szTexture);
		pObject->m_tEffectInfo.m_iTexOpt = (*pOriVecEffect)[i]->m_tEffectInfo.m_iTexOpt;
		pObject->m_tEffectInfo.m_iU = (*pOriVecEffect)[i]->m_tEffectInfo.m_iU;
		pObject->m_tEffectInfo.m_iV = (*pOriVecEffect)[i]->m_tEffectInfo.m_iV;
		pObject->m_tEffectInfo.m_fFrameSpd = (*pOriVecEffect)[i]->m_tEffectInfo.m_fFrameSpd;
		pObject->m_bBillboard = (*pOriVecEffect)[i]->m_bBillboard;
		pObject->m_bDistortion = (*pOriVecEffect)[i]->m_bDistortion;
		pObject->m_bRepeat = (*pOriVecEffect)[i]->m_bRepeat;
		pObject->m_fEffectEndTime = (*pOriVecEffect)[i]->m_fEffectEndTime;
		pObject->Copy_Point((*pOriVecEffect)[i]);
		if (i != 0)
			pObject->m_pParent = (*pVecEffect)[iVecEffectSize];
		pVecEffect->push_back(pObject);
	}
}

void CEffectMgr::Play_Effect(TCHAR * pKey, XMVECTOR xmvPos, XMVECTOR xmvRot, XMVECTOR xmvScl)
{
	vector<CMyEffect*>* pOriVecEffect = Find_ProtoEffectList(pKey);		// 원본을 가져온다.
	vector<CMyEffect*>* pVecEffect = Find_EffectList(pKey);		// 원본을 가져온다.
	if (pVecEffect == nullptr) return;
	size_t iVecEffectSize = pVecEffect->size();
	size_t iVecProtoSize = pOriVecEffect->size();
	for (size_t i = 0; i < iVecEffectSize; )
	{
		if ((*pVecEffect)[i]->m_bAlive == true)
		{
			i += iVecProtoSize;
			continue;
		}
		for (size_t j = 0; j < iVecProtoSize; ++j)
		{
			if (j == 0)
				(*pVecEffect)[i]->Modify_Point(xmvPos, xmvRot, xmvScl);
			(*pVecEffect)[j + i]->m_bAlive = true;
			(*pVecEffect)[j + i]->m_fAccTime = 0.f;
			m_vecPlayEffect.push_back((*pVecEffect)[j + i]);

		}
		return;
	}
}

void CEffectMgr::Play_Effect(TCHAR * pKey, CGameObject* pParent)
{
	vector<CMyEffect*>* pOriVecEffect = Find_ProtoEffectList(pKey);		// 원본을 가져온다.
	vector<CMyEffect*>* pVecEffect = Find_EffectList(pKey);		// 원본을 가져온다.
	if (pVecEffect == nullptr) return;
	size_t iVecEffectSize = pVecEffect->size();
	size_t iVecProtoSize = pOriVecEffect->size();
	for (size_t i = 0; i < iVecEffectSize; )
	{
		if ((*pVecEffect)[i]->m_bAlive == true)
		{
			i += iVecProtoSize;
			continue;
		}
		for (size_t j = 0; j < iVecProtoSize; ++j)
		{
			if (j == 0)
				(*pVecEffect)[i]->Modify_Point(pParent);
			(*pVecEffect)[j + i]->m_bAlive = true;
			(*pVecEffect)[j + i]->m_fAccTime = 0.f;
			m_vecPlayEffect.push_back((*pVecEffect)[j + i]);

		}
		return;
	}
}

CEffectMgr::CEffectMgr() : CSingleTonBase<CEffectMgr>("EffectMgrsingleton")
{
}


CEffectMgr::~CEffectMgr()
{
}
