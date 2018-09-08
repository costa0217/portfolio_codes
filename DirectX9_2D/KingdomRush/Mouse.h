#pragma once
#include "Include.h"

class CMouse
{
public:
	static D3DXVECTOR3		GetPos(void)
	{
		POINT	pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		return D3DXVECTOR3(float(pt.x), float(pt.y), 0.f);
	}
public:
	CMouse(void){}
	~CMouse(void){}
};
