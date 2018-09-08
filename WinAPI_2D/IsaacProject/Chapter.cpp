#include "StdAfx.h"
#include "Chapter.h"
#include "ObjFactory.h"
#include "BackGround.h"
#include "SceneMgr.h"
#include "RenderMgr.h"
#include "ObjMgr.h"
#include "Isaac.h"
#include "Device.h"

#include "Hart.h"

CChapter::CChapter(void)
:bTileOn(false)
{
}

CChapter::~CChapter(void)
{
}


void CChapter::Initialize(void)
{
	m_strArrow	= "Default";
	//m_strActive	= 
	//※기본 이미지

	//==타일
	m_BitMap["tile2"]		= (new CBitBmp)->LoadBmp(L"../Texture/tile2.bmp");
	m_BitMap["fire"]		= (new CBitBmp)->LoadBmp(L"../Texture/Tile/fire.bmp");
	m_BitMap["poop"]		= (new CBitBmp)->LoadBmp(L"../Texture/Tile/poop.bmp");
	m_BitMap["Stone"]		= (new CBitBmp)->LoadBmp(L"../Texture/Tile/Stone.bmp");
	m_BitMap["door"]		= (new CBitBmp)->LoadBmp(L"../Texture/Tile/door.bmp");
	m_BitMap["bossdoor"]	= (new CBitBmp)->LoadBmp(L"../Texture/Tile/bossdoor.bmp");
	m_BitMap["treasuredoor"]= (new CBitBmp)->LoadBmp(L"../Texture/Tile/treasuredoor.bmp");
	m_BitMap["poof1"]		= (new CBitBmp)->LoadBmp(L"../Texture/effect/poof1.bmp");
	m_BitMap["poof2"]		= (new CBitBmp)->LoadBmp(L"../Texture/effect/poof2.bmp");

	//==UI
	m_BitMap["alpha"]		= (new CBitBmp)->LoadBmp(L"../Texture/Buff/menu/alpha.bmp");

	m_BitMap["BackGround"]	= (new CBitBmp)->LoadBmp(L"../Texture/Buff/BackGround.bmp");
	m_BitMap["UI"]			= (new CBitBmp)->LoadBmp(L"../Texture/Buff/UI.bmp");
	m_BitMap["hart"]		= (new CBitBmp)->LoadBmp(L"../Texture/UI/hart.bmp");
	m_BitMap["minimap"]		= (new CBitBmp)->LoadBmp(L"../Texture/UI/minimap.bmp");
	m_BitMap["Heart"]		= (new CBitBmp)->LoadBmp(L"../Texture/UI/Heart.bmp");
	m_BitMap["minimap_cover"]		= (new CBitBmp)->LoadBmp(L"../Texture/UI/minimap_cover.bmp");
	m_BitMap["UI_count"]	= (new CBitBmp)->LoadBmp(L"../Texture/UI/UI_count.bmp");
	
	//==Player
	m_BitMap["isaac"]		= (new CBitBmp)->LoadBmp(L"../Texture/Character/isaac.bmp");

	//==Bullet
	m_BitMap["Bomb"]		= (new CBitBmp)->LoadBmp(L"../Texture/effect/Bomb.bmp");
	m_BitMap["explosion"]	= (new CBitBmp)->LoadBmp(L"../Texture/effect/explosion.bmp");
	m_BitMap["PlayerTear"]	= (new CBitBmp)->LoadBmp(L"../Texture/effect/PlayerTear.bmp");
	m_BitMap["shadow"]		= (new CBitBmp)->LoadBmp(L"../Texture/effect/shadow.bmp");
	m_BitMap["EnemyTear"]	= (new CBitBmp)->LoadBmp(L"../Texture/effect/EnemyTear.bmp");

	m_BitMap["bossSK"]		= (new CBitBmp)->LoadBmp(L"../Texture/Monster/bossSK.bmp");
	m_BitMap["target"]		= (new CBitBmp)->LoadBmp(L"../Texture/Monster/target.bmp");
	

	//==Monster
	m_BitMap["gaper"]		= (new CBitBmp)->LoadBmp(L"../Texture/Monster/gaper.bmp");
	m_BitMap["meat"]		= (new CBitBmp)->LoadBmp(L"../Texture/Monster/meat.bmp");
	m_BitMap["fly"]			= (new CBitBmp)->LoadBmp(L"../Texture/Monster/fly.bmp");
	m_BitMap["mulligan"]	= (new CBitBmp)->LoadBmp(L"../Texture/Monster/mulligan.bmp");
	m_BitMap["hopperleaper"]= (new CBitBmp)->LoadBmp(L"../Texture/Monster/hopperleaper.bmp");
	m_BitMap["miniboss"]	= (new CBitBmp)->LoadBmp(L"../Texture/Monster/miniboss.bmp");
	//==Boss
	m_BitMap["BossLeft"]		= (new CBitBmp)->LoadBmp(L"../Texture/Monster/BossLeft.bmp");
	m_BitMap["BossRight"]		= (new CBitBmp)->LoadBmp(L"../Texture/Monster/BossRight.bmp");

	m_BitMap["bosshealthbar"]	= (new CBitBmp)->LoadBmp(L"../Texture/UI/bosshealthbar.bmp");
	m_BitMap["MonsterDead"]	= (new CBitBmp)->LoadBmp(L"../Texture/effect/MonsterDead.bmp");

	m_BitMap["blood"]		= (new CBitBmp)->LoadBmp(L"../Texture/effect/blood.bmp");


	m_BitMap["ItemEffect"]		= (new CBitBmp)->LoadBmp(L"../Texture/Item/ItemEffect.bmp");
	//==Item_passive
	m_BitMap["AllUp"]			= (new CBitBmp)->LoadBmp(L"../Texture/Item/passive/01.AllUp.bmp");
	m_BitMap["AttackSpeedUp"]	= (new CBitBmp)->LoadBmp(L"../Texture/Item/passive/01.AttackSpeedUp.bmp");
	m_BitMap["AttackUp"]		= (new CBitBmp)->LoadBmp(L"../Texture/Item/passive/01.AttackUp.bmp");
	m_BitMap["Barrior"]			= (new CBitBmp)->LoadBmp(L"../Texture/Item/passive/01.Barrior.bmp");
	m_BitMap["Bomb 10EA"]		= (new CBitBmp)->LoadBmp(L"../Texture/Item/passive/01.Bomb 10EA.bmp");
	m_BitMap["Diner"]			= (new CBitBmp)->LoadBmp(L"../Texture/Item/passive/01.Diner.bmp");
	m_BitMap["Familyer"]		= (new CBitBmp)->LoadBmp(L"../Texture/Item/passive/01.Familyer.bmp");
	m_BitMap["Hart"]			= (new CBitBmp)->LoadBmp(L"../Texture/Item/passive/01.Hart.bmp");
	m_BitMap["HealthyDown"]		= (new CBitBmp)->LoadBmp(L"../Texture/Item/passive/01.HealthyDown.bmp");
	m_BitMap["Key"]				= (new CBitBmp)->LoadBmp(L"../Texture/Item/passive/01.Key.bmp");
	m_BitMap["ReachUp"]			= (new CBitBmp)->LoadBmp(L"../Texture/Item/passive/01.ReachUp.bmp");
	m_BitMap["SpeedUp"]			= (new CBitBmp)->LoadBmp(L"../Texture/Item/passive/01.SpeedUp.bmp");
	//m_BitMap["Coin"]			= (new CBitBmp)->LoadBmp(L"../Texture/Item/passive/01.Coin.bmp");

	//==Item_arrow
	m_BitMap["BigTear"]			= (new CBitBmp)->LoadBmp(L"../Texture/Item/arrow/02.BigTear.bmp");
	m_BitMap["BloodTear"]		= (new CBitBmp)->LoadBmp(L"../Texture/Item/arrow/02.BloodTear.bmp");
	m_BitMap["Knife"]			= (new CBitBmp)->LoadBmp(L"../Texture/Item/arrow/02.Knife.bmp");
	m_BitMap["Default"]			= (new CBitBmp)->LoadBmp(L"../Texture/Item/arrow/02.Default.bmp");
	m_BitMap["SnakeShot"]		= (new CBitBmp)->LoadBmp(L"../Texture/Item/arrow/02.SnakeShot.bmp");
	m_BitMap["Tripleshot"]		= (new CBitBmp)->LoadBmp(L"../Texture/Item/arrow/02.Tripleshot.bmp");
	
	//==Item_active
	m_BitMap["AllAttack"]		= (new CBitBmp)->LoadBmp(L"../Texture/Item/active/AllAttack.bmp");
	m_BitMap["BlackCandle"]		= (new CBitBmp)->LoadBmp(L"../Texture/Item/active/BlackCandle.bmp");
	m_BitMap["BlackHole"]		= (new CBitBmp)->LoadBmp(L"../Texture/Item/active/BlackHole.bmp");
	m_BitMap["Boomerang"]		= (new CBitBmp)->LoadBmp(L"../Texture/Item/active/Boomerang.bmp");
	m_BitMap["Guided Missile"]	= (new CBitBmp)->LoadBmp(L"../Texture/Item/active/Guided Missile.bmp");
	m_BitMap["Hanging"]			= (new CBitBmp)->LoadBmp(L"../Texture/Item/active/Hanging.bmp");
	m_BitMap["Kamikaze"]		= (new CBitBmp)->LoadBmp(L"../Texture/Item/active/Kamikaze.bmp");
	m_BitMap["Lemon"]			= (new CBitBmp)->LoadBmp(L"../Texture/Item/active/Lemon.bmp");
	m_BitMap["Meteo"]			= (new CBitBmp)->LoadBmp(L"../Texture/Item/active/Meteo.bmp");
	m_BitMap["Mine"]			= (new CBitBmp)->LoadBmp(L"../Texture/Item/active/Mine.bmp");
	m_BitMap["Sheet"]			= (new CBitBmp)->LoadBmp(L"../Texture/Item/active/Sheet.bmp");
	m_BitMap["Teleport"]		= (new CBitBmp)->LoadBmp(L"../Texture/Item/active/Teleport.bmp");

	//==Item_Image
	m_BitMap["BarriorImage"]	= (new CBitBmp)->LoadBmp(L"../Texture/Item/passive/01.BarriorImage.bmp");
	m_BitMap["FamilyerImage"]	= (new CBitBmp)->LoadBmp(L"../Texture/Item/passive/01.FamilyerImage.bmp");

	m_BitMap["KnifeImage"]		= (new CBitBmp)->LoadBmp(L"../Texture/Item/arrow/02.KnifeImage.bmp");
	m_BitMap["AllAttackImage"]	= (new CBitBmp)->LoadBmp(L"../Texture/Item/active/AllAttackImage.bmp");

	m_BitMap["MeteoEffect"]		= (new CBitBmp)->LoadBmp(L"../Texture/Item/active/MeteoEffect.bmp");
	m_BitMap["LemonImage"]		= (new CBitBmp)->LoadBmp(L"../Texture/Item/active/LemonImage.bmp");

	


	//CDevice::GetInstance()->LoadWave(L"../Sound/213_Isaac_Hurt_Grunt2.wav");  //3
	

	SetPlayer();
	SetUI();
	SetVacHart();

	((CIsaac*)m_pPlayer)->SetVect(m_pVacHart);

}
void CChapter::Progress(void)
{
	CObjMgr::GetInst()->PlayerTileCollision();
	CObjMgr::GetInst()->BulletBombCollision();
	CObjMgr::GetInst()->BulletTileCollision();
	CObjMgr::GetInst()->PlayerMonsterCollision();
	CObjMgr::GetInst()->TileMonsterCollision();
	CObjMgr::GetInst()->BulletMonsterCollision();
	CObjMgr::GetInst()->PlayerBulletCollision();
	CObjMgr::GetInst()->PlayerItemCollision();

	if( GetAsyncKeyState('P') & 0x0001)
	{
		if(!bTileOn)
			bTileOn = true;
		else
			bTileOn = false;
	}
	// 전체공격 치트
	if( GetAsyncKeyState('K') & 0x0001)
	{
		((CIsaac*)m_pPlayer)->SetAllAttack(true);
	}
	// 키 갯수 치트
	if( GetAsyncKeyState('C') & 0x0001)
	{
		((CIsaac*)m_pPlayer)->SetKeyNum(((CIsaac*)m_pPlayer)->GetKeyNum() + 1 );
	}
	// 폭탄갯수 치트
	if( GetAsyncKeyState('B') & 0x0001)
	{
		((CIsaac*)m_pPlayer)->SetBombNum(((CIsaac*)m_pPlayer)->GetBombNum() + 1 );
	}
	// 체력채우기 치트
	if( GetAsyncKeyState('L') & 0x0001)
	{
		for(vector<CObj*>::iterator iter = m_pVacHart->begin();
			iter != m_pVacHart->end(); )
		{
			if( ((CHart*)(*iter))->GetHP() < 2 )
			{
				((CHart*)(*iter))->SetHP(((CHart*)(*iter))->GetHP() + 1);
				((CIsaac*)m_pPlayer)->SetHP(((CIsaac*)m_pPlayer)->GetHP() + 1);
				break;
			}
			else/* if(((CHart*)(*iter))->GetHP() == 0 )*/
			{
				++iter;
			}
		}
	}

}
void CChapter::Render(HDC hdc)
{

	if(((CIsaac*)m_pPlayer)->GetArrowType() == ARROW_DEFAULT)
		m_strArrow = "Default";
	else if(((CIsaac*)m_pPlayer)->GetArrowType() == ARROW_BIG)
		m_strArrow = "BigTear";
	else if(((CIsaac*)m_pPlayer)->GetArrowType() == ARROW_SNAKE)
		m_strArrow = "SnakeShot";
	else if(((CIsaac*)m_pPlayer)->GetArrowType() == ARROW_TRIPLE)
		m_strArrow = "Tripleshot";
	else if(((CIsaac*)m_pPlayer)->GetArrowType() == ARROW_KNIFE)
		m_strArrow = "Knife";
	else if(((CIsaac*)m_pPlayer)->GetArrowType() == ARROW_BLOOD)
		m_strArrow = "BloodTear";

	TransparentBlt(hdc,		
		398, 10, 60, 60, m_BitMap[m_strArrow]->GetMemDC(),
		0,	0,
		32, 32,	RGB(63, 16, 123
		));

	if(!((CIsaac*)m_pPlayer)->GetActiveType() == ACTIVE_Default)//====작업중====
	{
		if(((CIsaac*)m_pPlayer)->GetActiveType() == ACTIVE_AllAttack)
			m_strActive = "AllAttack";
		else if(((CIsaac*)m_pPlayer)->GetActiveType() == ACTIVE_BlackCandle)
			m_strActive = "BlackCandle";
		else if(((CIsaac*)m_pPlayer)->GetActiveType() == ACTIVE_BlackHole)
			m_strActive = "BlackHole";
		else if(((CIsaac*)m_pPlayer)->GetActiveType() == ACTIVE_Boomerang)
			m_strActive = "Boomerang";
		else if(((CIsaac*)m_pPlayer)->GetActiveType() == ACTIVE_GuidedMissile)
			m_strActive = "Guided Missile";
		else if(((CIsaac*)m_pPlayer)->GetActiveType() == ACTIVE_Hanging)
			m_strActive = "Hanging";
		else if(((CIsaac*)m_pPlayer)->GetActiveType() == ACTIVE_kamikaze)
			m_strActive = "Kamikaze";
		else if(((CIsaac*)m_pPlayer)->GetActiveType() == ACTIVE_Lemon)
			m_strActive = "Lemon";
		else if(((CIsaac*)m_pPlayer)->GetActiveType() == ACTIVE_Meteo)
			m_strActive = "Meteo";
		else if(((CIsaac*)m_pPlayer)->GetActiveType() == ACTIVE_Mine)
			m_strActive = "Mine";
		else if(((CIsaac*)m_pPlayer)->GetActiveType() == ACTIVE_Sheet)
			m_strActive = "Sheet";
		else if(((CIsaac*)m_pPlayer)->GetActiveType() == ACTIVE_Teleport)
			m_strActive = "Teleport";

		TransparentBlt(hdc,		
			483, 12, 60, 60, m_BitMap[m_strActive]->GetMemDC(),
			0,	0,
			32, 32,	RGB(63, 16, 123
			));
	}



	TCHAR szBuf[128] = L"";
	if(bTileOn)
	{
		for(int i = 0; i < TILEY; ++i)
		{
			for(int j = 0; j < TILEX; ++j)
			{
				int INDEX = i * TILEX + j;

				if(INDEX < 0 || INDEX > TILEX * TILEY)
					break;

				TransparentBlt(m_BitMap["BackGround"]->GetMemDC(), 
					int(m_VecTile[INDEX]->fX - TILECX / 2.f),
					int(m_VecTile[INDEX]->fY - TILECY / 2.f),
					TILECX, TILECY,
					m_BitMap["tile2"]->GetMemDC(),
					int(TILECX*m_VecTile[INDEX]->iDrawID), 0,
					TILECX, TILECY,
					RGB(193, 193, 193));

				wsprintf(szBuf, L"%d", m_VecTile[INDEX]->iDrawID);
				TextOut(m_BitMap["BackGround"]->GetMemDC(), 
					int(m_VecTile[INDEX]->fX - TILECX / 2.f + 2),
					int(m_VecTile[INDEX]->fY - TILECY / 2.f + 2),
					szBuf, lstrlen(szBuf));
			}
		}

		TCHAR szMouse[32] = L"";
		wsprintf(szMouse, L"X : %d Y : %d \n HP: %d", ::GetMouse().x, ::GetMouse().y, ((CIsaac*)m_pPlayer)->GetHP());
		TextOut(m_BitMap["BackGround"]->GetMemDC(), 
			100,100,
			szMouse, lstrlen(szMouse));
	}

	/* 피 하트통 */
	for(vector<CObj*>::iterator iter = m_pVacHart->begin();
		iter != m_pVacHart->end(); ++iter)
	{
		(*iter)->Render(m_BitMap["BackGround"]->GetMemDC());
	}
	

	TransparentBlt(hdc,		// 코인 앞숫자
		354, 5,	12,	27,	m_BitMap["UI_count"]->GetMemDC(),
		0,	27 * int((((CIsaac*)m_pPlayer)->GetCoinNum() % 100 - ((CIsaac*)m_pPlayer)->GetCoinNum() % 10) / 10),
		12, 27,	RGB(0, 0, 255));

	TransparentBlt(hdc,		// 코인	뒷숫자
		366, 5,	12,	27, m_BitMap["UI_count"]->GetMemDC(),
		0,	27 * (((CIsaac*)m_pPlayer)->GetCoinNum() % 10),
		12, 27, RGB(0, 0, 255));

	TransparentBlt(hdc,		// 폭탄 앞숫자
		354, 31, 12, 27, m_BitMap["UI_count"]->GetMemDC(),
		0,	27 * int((((CIsaac*)m_pPlayer)->GetBombNum() % 100 - ((CIsaac*)m_pPlayer)->GetBombNum() % 10) / 10),
		12, 27, RGB(0, 0, 255));

	TransparentBlt(hdc,		// 폭탄	뒷숫자
		366, 31, 12, 27, m_BitMap["UI_count"]->GetMemDC(),
		0,	27 * (((CIsaac*)m_pPlayer)->GetBombNum() % 10),
		12, 27, RGB(0, 0, 255));

	TransparentBlt(hdc,		// 키 앞숫자
		354, 57, 12, 27, m_BitMap["UI_count"]->GetMemDC(),
		0,	27 * int((((CIsaac*)m_pPlayer)->GetKeyNum() % 100 - ((CIsaac*)m_pPlayer)->GetKeyNum() % 10) / 10),
		12, 27, RGB(0, 0, 255));

	TransparentBlt(hdc,		// 키 뒷숫자
		366, 57, 12, 27, m_BitMap["UI_count"]->GetMemDC(),
		0,	27 * (((CIsaac*)m_pPlayer)->GetKeyNum() % 10),
		12, 27, RGB(0, 0, 255));
}
void CChapter::SetVacHart()
{
	m_pVacHart = CSceneMgr::GetInst()->GetVacHart();
}
void CChapter::SetPlayer()
{
	m_pPlayer = CSceneMgr::GetInst()->GetPlayer();
}
void CChapter::SetUI()
{
	m_pUI = CSceneMgr::GetInst()->GetUI();
}

void CChapter::LoadData(int _iRoomNum)
{
	// 기존 타일 전부 지우기
	for(vector<TILE*>::iterator iter = m_VecTile.begin();
		iter != m_VecTile.end(); ++iter)
	{
		::Safe_Delete(*iter);
	}

	m_VecTile.clear();

	//저장한 타일 불러오기
	HANDLE	hFile	= NULL;
	DWORD	dwByte	= 0;

	if(_iRoomNum == 1)
		hFile = CreateFile(L"../Data/stage1.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	else if(_iRoomNum == 2)
		hFile = CreateFile(L"../Data/stage2.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	else if(_iRoomNum == 3)
		hFile = CreateFile(L"../Data/stage3.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	else if(_iRoomNum == 4)
		hFile = CreateFile(L"../Data/stage4.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	else if(_iRoomNum == 5)
		hFile = CreateFile(L"../Data/stage5.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	else if(_iRoomNum == 6)
		hFile = CreateFile(L"../Data/stage6.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	else if(_iRoomNum == 7)
		hFile = CreateFile(L"../Data/stage7.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	else if(_iRoomNum == 8)
		hFile = CreateFile(L"../Data/bossstage.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	while(true)
	{
		TILE*	pTile = new TILE;

		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);

		if(dwByte == 0)
		{
			Safe_Delete(pTile);
			break;
		}
		m_VecTile.push_back(pTile);
	}
	CloseHandle(hFile);
}
