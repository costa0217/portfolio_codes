#pragma once

#include "GameObject.h"

class CCoordinateSys : public CGameObject {
public:
	void SetCoordinateSysInfo(XMMATRIX mtx);

	virtual bool IsVisible( CCamera* pCamera) { return true; };//���������� �༮���� �ٽ� ������� �ʿ䰡 ����
private:

public:
	CCoordinateSys();
	~CCoordinateSys();
};
