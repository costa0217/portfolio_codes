#pragma once
//그 position과 quaternion을 관리할 객체이다.
#include "PositionInfoData.h"

static wstring wroot_path = L"../../Assets/SceneResource/ActionPositionInfo/";
static string root_path = "../../Assets/SceneResource/ActionPositionInfo/";

static wstring wextent = L".poi";
static string extent = ".poi";//pos info/ poi


class CCamera;
struct stPositionInfo {
	vector<CPositionInfoData> m_vPositionInfoData;
	float m_fActionSpeed;
};
class CPositionInfoManager {
public:
	static void Begin();
	static void End();

	static void PushPositionInfoData(string name, CPositionInfoData PositionInfoData) { m_mvPositionInfoData[name].m_vPositionInfoData.push_back(PositionInfoData); }

	static void ClearPositionInfoData() { m_mvPositionInfoData.clear(); }
	static void LoadData(string name);//이름으로 정해진 디렉토리에서 찾아서 쓴다.

	static size_t GetInfoCnt(string name) { return m_mvPositionInfoData[name].m_vPositionInfoData.size(); }
	//static void ProcessInput(CCamera* pCamera);
	static void SetActoionSpeed(string name, float fSpeed) { m_mvPositionInfoData[name].m_fActionSpeed = fSpeed; }
	//render
	static void RenderPositionInfo(string name);
	//getter
	static map<string, stPositionInfo>& GetAllPositionInfo() { return m_mvPositionInfoData; };
	static float GetActionSpeed(string name) { return m_mvPositionInfoData[name].m_fActionSpeed; }
private:
	static map<string, stPositionInfo> m_mvPositionInfoData;

public:
	CPositionInfoManager() { m_mvPositionInfoData.clear(); };
	~CPositionInfoManager() {};

};