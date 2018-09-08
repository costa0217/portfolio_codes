#pragma once

typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9		pTexture;
	D3DXIMAGE_INFO			tImgInfo;
}TEXINFO;


typedef struct tagTile
{
	D3DXVECTOR3			vPos;
	D3DXVECTOR3			vSize;
	BYTE				byOption;
	BYTE				byDrawID;

}TILE;

typedef struct tagUnitData
{
	wstring				wstrName;
	int					iHp;
	int					iAttack;
	int					iDef;
	BYTE				byJobIndex;
	BYTE				byDropItem;

}UNITDATA;

typedef	struct tagInfo
{
	D3DXVECTOR3			vPos;
	D3DXVECTOR3			vLook;
	D3DXVECTOR3			vDir;
	D3DXVECTOR3			vSize;
	D3DXMATRIX			matWorld;

}INFO;

typedef struct tagTexturePath
{
	wstring				wstrObjKey;
	wstring				wstrStateKey;
	wstring				wstrPath;
	int					iCount;

	tagTexturePath(void)
		: wstrObjKey(L""), wstrStateKey(L""), wstrPath(L""), iCount(0)
	{}

}IMGPATH;

typedef struct tagFrame
{
	float		fFrame;
	float		fCount;
	float		fMax;
	
	tagFrame(void){  }
	tagFrame(float _fFrame, float _fCount, float _fMax)
		: fFrame(_fFrame), fCount(_fCount), fMax(_fMax)
	{	}

}FRAME;

typedef struct tagSheetSize
{
	float		fCX;
	float		fCY;

	tagSheetSize(){}
	tagSheetSize(float _fCX, float _fCY)
		:fCX(_fCX), fCY(_fCY)
	{	}
}SHEETSIZE;

typedef struct tagSheetFrame
{
	int		iStart;
	int		iLast;
	int		iScene;
	DWORD	dwTime;

	tagSheetFrame(void){}
	tagSheetFrame(int _iStart, int _iLast, int _iScene, DWORD _dwTime)
		: iStart(_iStart), iLast(_iLast), iScene(_iScene), dwTime(_dwTime)
	{	}

}SHEETFRM;

typedef	struct	tagStats
{
	wstring		szName;
	int			iHP;
	int			iMinDam;
	int			iMaxDam;
	float		fSpeed;

	tagStats() {}
	tagStats(wstring _szName, int _iHP, int _iMinDam, int _iMaxDam, float _fSpeed)
		: szName(_szName), iHP(_iHP), iMinDam(_iMinDam), iMaxDam(_iMaxDam), fSpeed(_fSpeed)
	{	}
	tagStats(int _iHP)
		: iHP(_iHP)
	{	}
}UNITSTATS;


typedef	struct	tagTowerStats
{
	int			iMinDam;
	int			iMaxDam;
	float		fFireSpd;
	float		iRange;
	int			iNextPrice;

	tagTowerStats(){}
	tagTowerStats(int _iMinDam, int _iMaxDam, float _fFireSpd, float _iRange, int _iNextPrice)
		: iMinDam(_iMinDam), iMaxDam(_iMaxDam), fFireSpd(_fFireSpd)
		, iRange(_iRange),	iNextPrice(_iNextPrice)
	{	}

}TWRSTATS;