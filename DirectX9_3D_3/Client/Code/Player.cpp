#include "stdafx.h"
#include "Player.h"

#include "Export_Function.h"
#include "Include.h"

#include "Texture.h"
#include "Transform.h"
#include "CameraObserver.h"
#include "Pipeline.h"
#include "TerrainCol.h"
#include "CollisionMgr.h"
#include "Stage.h"
#include "Mousecol.h"
#include "Dust.h"
#include "Flare.h"
#include "Smoke.h"
#include "Effect.h"
#include "KeyMgr.h"
#include "MathMgr.h"
#include "BasicBullet.h"
#include "Bullet.h"
#include "StaticCamera.h"
#include "Bomb.h"

#include "WireHead.h"
#include "WireStomach.h"
#include "WireChest.h"

#include "Nemo.h"//0804
#include "Propeller.h"
#include "Component.h"
#include "MiniPropeller.h"
#include "Missile.h"
#include "MultiBullet.h"
#include "Gun.h"
#include "GatlingGun.h"
#include "Launcher.h"
#include "Fragment.h"
#include "Hook.h"
#include "WireTail.h"
#include "GameObject.h"
#include "Weapon.h"
#include "ButtonMgr.h"
#include "StartButton.h"
#include "Explosion.h"
#include "Tree.h"
#include "ExplosionHeli.h"
#include "Retona.h"
#include "Dodge.h"
#include "Bunker.h"
#include "AirGun.h"
#include "Tank.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourcesMgr())
, m_pTimeMgr(Engine::Get_TimeMgr())
, m_pInfoSubject(Engine::Get_InfoSubject())
, m_pManagement(Engine::Get_Management())
, m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_pInfo(NULL)
, m_pTexture(NULL)
, m_pBuffer(NULL)
, m_pTerrainCol(NULL)
, m_pMouseCol(NULL)
, m_pCameraObserver(NULL)
, m_pTerrainVertex(NULL)
, m_fSpeed(0.f)
, m_dwVtxCnt(0)
, m_fTime(0)
, m_fAngleZ(0.f)
, m_fAngleY(0.f)
, m_fAngleX(0.f)
//, m_fHp(100) ★★★ 재훈님 추가부분 ★★★
, m_pWeapon(NULL)
, m_fBullet(100)
, m_fSmokeTime(0.f)
, m_bJump(false)
, m_BulletTime(0.f)
, m_bBomb(false)
, m_bWireRightMove(true)
, m_bWireLeftMove(true)
, m_pWireHead(NULL)
, m_fRightSpeed(0.f)
, m_fLeftSpeed(0.f)
, m_vUpDir(0.f,1.f,0.f)
, m_bRightButtonUp(true)
, m_bLeftButtonUp(true)
, m_bLeftAngZ_Stabilizing(false)
, m_bRightAngZ_Stabilizing(false)
, m_fBefore_AngZ(0.f)
, m_bBefore_AngZ_Decrease(true)
, m_bAngZzero(true)
, m_fAngZTime(0.f)
, m_fAngleZ_Speed(0.f)
, m_fAngleZ_Stabilizing_Speed(0.f)
, m_fAngleY_Speed(0.f)
, m_fAngleX_Speed(0.f)
, m_fAngleZReturn_Speed(0.f)
, m_fX_Speed(0.f)
, m_fY_Speed(0.f)
, m_fWireX_Speed(0.f)
, m_fBeforeAngZ_Speed(0.f)
, m_fInertiaX_Speed(0.f)
, m_fAngZTime_Speed(0.f)
, m_fTime_Speed(0.f)
, m_fBefore_AngZ_Limit(0.f)
, m_fMaxPosY(0.f)
, m_fDustTime(0.f)
, m_fAngleRevolveZ(0.f)
, m_bHeliCol(false)
, m_pPropeller(NULL)
, m_pMiniPropeller(NULL)
, bPropellerSetPos(true)
, m_bGravity(true)
, m_fCollisionSpeed(0.f)
, m_bCollisionDamage(false)
, m_pTarget(NULL)
, m_bFragmentCreate(false)
, m_fFragmentCreateTime(10.f)
, m_bLeftKeyDown(false)
, m_bRightKeyDown(false)
, m_bStart(false)
, m_bAlreadyStart(false)
, m_iBoardNum(0)	
, m_iMAXBoardNum(0)	
, m_bOnce(true)
, m_bTreeCollision(false)
, m_fDieAngleZ(0.f)
{

}

CPlayer::~CPlayer(void)
{
	Release();
}

HRESULT CPlayer::Initialize(void)
{
	FAILED_CHECK(AddComponent());
	
	//임시 좌표
	m_pInfo->m_vPos = D3DXVECTOR3(1300.f,0.f, 118.f);
	m_pInfo->m_vDir = D3DXVECTOR3(0.f,0.f,-1.f);
	m_pInfo->m_eState = Engine::STATE_LIVE;

	m_pInfo->m_vScale = D3DXVECTOR3(0.5f, 0.5f, 0.5f);

	m_pInfo->m_fHP		= 100.f;	// 체력 ★★★ 재훈님 추가부분 ★★★
	m_pInfo->m_eName	= Engine::OBJ_PLAYER;

	m_iMAXBoardNum		= 4;	

	m_vLeftLegPos = D3DXVECTOR3(-10.f,-10.f, 0.f);
	m_vRightLegPos = D3DXVECTOR3(10.f,-10.f, 0.f);

	m_vOriginLeftLegPos = D3DXVECTOR3(-10.f,-10.f, 0.f);
	m_vOriginRightLegPos = D3DXVECTOR3(10.f,-10.f, 0.f);

	//
	m_wstrBufferName = L"Buffer_HeliCol";

	m_fSpeed = 10.f;

	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_dwVtxCnt = m_pBuffer->GetVtxCnt();
	m_pVertex			= new Engine::VTXCOL[m_dwVtxCnt];
	m_pConvertVertex	= new Engine::VTXCOL[m_dwVtxCnt];

	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, m_wstrBufferName, m_pVertex);

	m_pInfoSubject->AddData(PLAYER_POS, &m_pInfo->m_vPos);
	m_pInfoSubject->AddData(PLAYER_DIR, &m_pInfo->m_vDir);
	m_pInfoSubject->AddData(PLAYER_HP,	&m_pInfo->m_fHP);
	m_pInfoSubject->AddData(PLAYER_BULLET, &m_fBullet);
	m_pInfoSubject->AddData(PLAYER_STATE, &m_pInfo->m_eState);
	m_pInfoSubject->AddData(PLAYER_WORLD, &m_pInfo->m_matWorldNoSca);
	m_pInfoSubject->AddData(PLAYER_BOARDNUM, &m_iBoardNum);
	m_pInfoSubject->AddData(PLAYER_MAXBOARDNUM, &m_iMAXBoardNum);

	m_pTemp = CDust::Create(m_pDevice);
	m_Dust = m_pTemp;
	
	m_pTemp = CWireHead::Create(m_pDevice);
	m_pWire = m_pTemp;
	
	m_pTemp = CBasicBullet::Create(m_pDevice);
	m_pTemp->Initialize();
	m_pBasicBullet = m_pTemp;

	m_pTemp = CMissile::Create(m_pDevice);
	m_pTemp->Initialize();
	m_pMissileBullet = m_pTemp;

	m_pTemp = CMultiBullet::Create(m_pDevice);
	m_pTemp->Initialize();
	m_pMultiBullet = m_pTemp;	

	m_pTemp = CBomb::Create(m_pDevice);
	m_pBomb = m_pTemp;

	//
	m_fAngleZ_Speed = 2.5f;
	m_fAngleZ_Stabilizing_Speed = 1.25f;
	m_fAngleY_Speed = 1.25f;
	m_fAngleX_Speed = 1.75f;
	m_fAngleZReturn_Speed = 0.000125f;
	m_fX_Speed = 2500.f;
	m_fY_Speed = 5000.f;
	m_fWireX_Speed = 15000.f;
	m_fInertiaX_Speed = 6250.f;
	m_fTime_Speed = 5.f;
	m_fMaxPosY = 100.f;

	m_fDieAngleZ = 3.f;
	m_fDieAngleZ_Speed = 3.f;

	//FPS에따라 바꿔줘야하는 변수(Z값가속도받으며원래로돌리는변수)
	m_fBefore_AngZ_Limit = 0.2f;
	m_fBeforeAngZ_Speed = 10.f;
	m_fAngZTime_Speed = 10000.f;
	//
	

	//프로펠러생성
	PropellerCreate();
	
	//총구생성
	WeaponCreate(L"Buffer_Gun", WEAPON_GUN);

	//버튼리스트
	m_pButtonlist = CButtonMgr::GetInstance()->getlist();

	//
	m_vDieMove= D3DXVECTOR3(10.f,30.f,10.f);
	Engine::CMathMgr::MyRotationY(&m_vDieMove,&m_vDieMove,float(rand()%200));


	return S_OK;
}

int CPlayer::Update(void)
{
	TreeCollision();

	if( m_CurrentBuffer == L"Buffer_Chinook" )
		m_iMAXBoardNum	= 8;

	if(m_pInfo->m_fHP >= 100.f)
		m_pInfo->m_fHP = 100.f;

	if(m_fBullet >= 100.f)
		m_fBullet = 100.f;

	//처음카메라 돌려놓기
	if(m_bStart == false)
	{
		MAPOBJLIST::iterator	iterU = m_pMapObjlistUI->find(L"StaticCamera");
		OBJECTLIST::iterator iterUI  = iterU->second.begin();
		((CStaticCamera*)(*iterUI))->SetAngleUp(D3DXToRadian(40.f));
		((CStaticCamera*)(*iterUI))->SetTargetDistance(50.f);
	}

	if(m_bAlreadyStart == false)
	{
		BUTTONLIST::iterator	itera = m_pButtonlist->find(L"StartButton");
		if( ((CStartButton*)itera->second)->GetBool() )
		{
			m_bStart = true;
			m_bAlreadyStart = true;
			MAPOBJLIST::iterator	iterU = m_pMapObjlistUI->find(L"StaticCamera");
			OBJECTLIST::iterator iterUI  = iterU->second.begin();
			((CStaticCamera*)(*iterUI))->SetbGameStart(true);
		}
	}
		
	

	// "H" 헬기 이륙 착륙지점
	if(m_pInfo->m_vPos.x >= 1275.f && m_pInfo->m_vPos.x <= 1305.f && m_pInfo->m_vPos.y == 5.f)
	{
		if(m_pInfo->m_fHP < 99.f)
		{
			m_pInfo->m_fHP += 1.f;
		}

		else if(m_fBullet < 99.f)
		{
			m_fBullet += 1.f;
		}
		iCLEARNUM  += m_iBoardNum ;
		m_iBoardNum = 0;
	}

	if(m_pInfo->m_vPos.x >= 1705.f || m_pInfo->m_vPos.x < 22.f)
	{
		m_pInfo->m_fHP = 0.f;
		if(m_pInfo->m_vPos.x >= 1705.f)
		{
			m_vDieMove= D3DXVECTOR3(-m_fCollisionSpeed*0.5f,20.f,0.f);
		}
		else if(m_pInfo->m_vPos.x < 22.f)
		{
			m_vDieMove= D3DXVECTOR3(m_fCollisionSpeed*0.5f,20.f,0.f);
		}
			
	}
	//------------헬기가죽으면
	
	if(m_pInfo->m_fHP <= 0)
	{

		MAPOBJLIST::iterator	iterU = m_pMapObjlistUI->find(L"StaticCamera");
		OBJECTLIST::iterator iterUI  = iterU->second.begin();
		((CStaticCamera*)(*iterUI))->SetTargetDistance(300.f);
		//((CStaticCamera*)(*iterUI))->SetAngleUp(5.f);
		((CStaticCamera*)(*iterUI))->SetbEyeFix(true);
		((CStaticCamera*)(*iterUI))->SetbKeyCheck(false);
		

		m_pInfo->m_eState = Engine::STATE_DIE;
		m_pTerrainCol->SetbYfix(false);
		//총구제거
		//((CGun*)m_pGun)->SetbDie(true);
		//

		///프로펠러 날리기
		((CMiniPropeller*)m_pMiniPropeller)->SetbHeliDie(true);
		((CPropeller*)m_pPropeller)->SetbHeliDie(true);
		if(bPropellerSetPos)
		{
			//한번생성
			m_pTemp = CExplosionHeli::Create(m_pDevice);
			((CExplosion*)m_pTemp)->SetPos(m_pInfo->m_vPos);

			MAPOBJLIST::iterator iter = m_pMapObjlist->find(L"Explosion");
			if(iter == m_pMapObjlist->end())
			{
				(*m_pMapObjlist)[L"Explosion"] = OBJECTLIST();
			}
			(*m_pMapObjlist)[L"Explosion"].push_back(m_pTemp);
			//

			bPropellerSetPos = false;
			((CMiniPropeller*)m_pMiniPropeller)->SetPos(m_pInfo->m_vPos+D3DXVECTOR3(2.f,3.f,10.f));
			((CPropeller*)m_pPropeller)->SetPos(m_pInfo->m_vPos+D3DXVECTOR3(0.f,4.f,0.f));

			//
			m_fTime = 0.f;
			m_bHeliCol = false;
			m_fAngleZ = 0.f;
			
		}
		///
		m_fAngleZ += m_fDieAngleZ_Speed * m_pTimeMgr->GetTime();
		if(m_fAngleZ > m_fDieAngleZ)
			m_fAngleZ = m_fDieAngleZ;

		//
		
		if(m_pInfo->m_vPos.y <= m_pTerrainVertex[m_pTerrainCol->GetIndex()].vPos.y)
		{
			m_pInfo->m_vPos.y = m_pTerrainVertex[m_pTerrainCol->GetIndex()].vPos.y+3.f;
			m_bGravity = false;
		}
		else
		{
			if(m_bGravity)
				m_pInfo->m_vPos += m_vDieMove * m_pTimeMgr->GetTime();
		}
		

		
	}
	//------------------

	
	//카메라에게 앵글값을주기위해추가한부분 플레이어의 회전과는 무관하다.
	m_pInfo->m_fAngleNo[Engine::ANGLE_X] = m_fAngleX;
	m_pInfo->m_fAngleNo[Engine::ANGLE_Y] = m_fAngleY;
	m_pInfo->m_fAngleNo[Engine::ANGLE_Z] = m_fAngleZ;
	//

	//폭탄생성
	if(m_bBomb)
	{
		MAPOBJLIST::iterator	iterU = m_pMapObjlistUI->find(L"StaticCamera");
		OBJECTLIST::iterator iterUI  = iterU->second.begin();
		if(((CStaticCamera*)(*iterUI))->GetbBombCreate())
		{
			BombCreate();
			((CStaticCamera*)(*iterUI))->SetbBombCreate(false);
			m_bBomb = false;
		}
	}
	else
	{
		//중력
		if(m_bGravity)
			m_pInfo->m_vPos.y -= 0.98f * m_fTime * m_pTimeMgr->GetTime() * 7;

		MAPOBJLIST::iterator	iter = m_pMapObjlist->find(L"Bomb");
		if(iter != m_pMapObjlist->end())
		{

			list<CGameObject*>	BombList = iter->second;
			if(BombList.empty())
			{
				if(m_pInfo->m_eState == Engine::STATE_SPECIAL)
					m_pInfo->m_eState = Engine::STATE_LIVE;

				MAPOBJLIST::iterator	iterU = m_pMapObjlistUI->find(L"StaticCamera");
				OBJECTLIST::iterator iterUI  = iterU->second.begin();
				((CStaticCamera*)(*iterUI))->SetTargetInfo((const Engine::CTransform*)m_pInfo);

			}
			else
			{
				m_pInfo->m_vPos.y += 0.98f * m_fTime * m_pTimeMgr->GetTime() * 7;
			}
		}

		
	}

		
	if(m_fTime > 15.f)
	{
		m_fTime = 15.f;
	}
	else
	{
		m_fTime += m_fTime_Speed * m_pTimeMgr->GetTime();
	}

	//점프관련
	if(m_bJump)
	{

		if(m_fTime >= 0.06f)
		{
			m_bJump = false;
		}
	}

	DWORD dwTerrainVtxCntX, dwTerrainVtxCntZ;
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);
	m_pResourceMgr->GetTerrainVtxCnt(Engine::RESOURCE_DYNAMIC, L"Buffer_TerrainTex", &dwTerrainVtxCntX, &dwTerrainVtxCntZ);
	m_pTerrainVertex = m_pManagement->GetTerrainVtx(Engine::LAYER_GAMELOGIC, L"Terrain");


	//m_pTerrainCol->SetColInfo(&m_pInfo->m_vPos, m_pTerrainVertex, dwTerrainVtxCntX, dwTerrainVtxCntZ);
	if(m_pInfo->m_eState == Engine::STATE_LIVE)
		m_pTerrainCol->SetHeliColInfo(&m_vLeftLegPos,&m_vRightLegPos,&m_pInfo->m_vPos,m_pTerrainVertex, dwTerrainVtxCntX, dwTerrainVtxCntZ);
	else if(m_pInfo->m_eState == Engine::STATE_DIE)
	{
		m_pTerrainCol->SetHeliColInfo(NULL,NULL,NULL,m_pTerrainVertex, dwTerrainVtxCntX, dwTerrainVtxCntZ);
	}
	
	//m_pInfo->m_vPos.y = m_vLeftLegPos.y;

	BUTTONLIST::iterator	iter = m_pButtonlist->find(L"StartButton");

	if(m_pInfo->m_eState == Engine::STATE_LIVE && m_bBomb == false && 
		((CStartButton*)iter->second)->GetBool())
	{ 
		KeyCheck();
	}

	Engine::CGameObject::Update();
	

	TransformMatrix(); //0804

	//먼지생성
	if(m_pInfo->m_vPos.y - m_pTerrainVertex->vPos.y < 10 && m_pInfo->m_eState == Engine::STATE_LIVE )
	{
		m_pTerrainCol->SetbYfix(true);

		/*m_fDustTime -= m_pTimeMgr->GetTime() * 0.5f;
		if(m_fDustTime < 0)
		{
			m_fDustTime = 0.01f;*/
			DustCreate();
		//}
	}
	//연기생성
	if(m_pInfo->m_fHP <= 90 && m_pInfo->m_eState == Engine::STATE_LIVE )
	{
		m_fSmokeTime -= m_pTimeMgr->GetTime() * 0.5f;
		if(m_fSmokeTime < 0)
		{
			m_fSmokeTime = 0.1f;
			SmokeCreate();
		}
	}

	
	//속력계산.
	m_fCollisionSpeed = abs(m_pInfo->m_vDir.x*4000)+abs(m_pInfo->m_vDir.y*4000);
	
	//이동중속력에의한 데미지
	if(m_pInfo->m_vPos.y - m_pTerrainVertex[m_pTerrainCol->GetIndex()].vPos.y < 3 && m_fCollisionSpeed > 50)
		m_pInfo->m_fHP -= m_fCollisionSpeed;
	
	//-----중력에의한데미지
	if((m_bCollisionDamage && m_bHeliCol))
	{
		m_bCollisionDamage = false;
		if((m_fTime*7.f) > 30.f)
		{
			m_pInfo->m_fHP -= (m_fTime*7.f);
		}
	}
	//((CWeapon*)m_pWeapon)->SetTarget(m_pTarget);	// 총 타겟팅
	////



	//폭탄잔해 생성.
	if(m_bFragmentCreate)
	{
		if(m_fFragmentCreateTime > 0.f)
		{
			m_fFragmentCreateTime -= 5.f * m_pTimeMgr->GetTime();
			FragmentCreate();

			if(m_fFragmentCreateTime <= 0.f)
			{
				m_bFragmentCreate = false;
				m_fFragmentCreateTime = 10.f;

				MAPOBJLIST::iterator iter = m_pMapObjlist->find(L"Monster");

				OBJECTLIST::iterator Objiter		= (iter->second).begin();
				OBJECTLIST::iterator ObjiterEnd		= (iter->second).end();

				for(;Objiter != ObjiterEnd; ++Objiter)
				{

					Engine::OBJ_NAME eName = ((const Engine::CTransform*)(*Objiter)->GetComponent(L"Transform"))->m_eName;
					switch(eName)
					{
					case Engine::OBJ_RETONA:
						((CRetona*)(*Objiter))->SetHP(0);
						break;
					case Engine::OBJ_DODGE:
						((CDodge*)(*Objiter))->SetHP(0);
						break;
					case Engine::OBJ_BUNKER:
						((CBunker*)(*Objiter))->SetHP(0);
						break;
					case Engine::OBJ_AIRGUN:
						((CAirGun*)(*Objiter))->SetHP(0);
						break;
					case Engine::OBJ_TANK:
						((CTank*)(*Objiter))->SetHP(0);
						break;
					}
				}
			}
		}
	}
	//

	m_pInfoSubject->Notify(PLAYER_POS);
	m_pInfoSubject->Notify(PLAYER_DIR);
	m_pInfoSubject->Notify(PLAYER_HP);
	m_pInfoSubject->Notify(PLAYER_BULLET);
	m_pInfoSubject->Notify(PLAYER_STATE);
	m_pInfoSubject->Notify(PLAYER_WORLD);
	m_pInfoSubject->Notify(PLAYER_BOARDNUM);
	m_pInfoSubject->Notify(PLAYER_MAXBOARDNUM);

	
	return 0;
}

void CPlayer::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);
	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, m_wstrBufferName, m_pConvertVertex);

	m_pDevice->SetTexture(0, NULL);
	if(m_pBuffer != NULL)
		m_pBuffer->Render();

	m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, m_wstrBufferName, m_pVertex);


	/*RECT		rc = {200, 200, 800, 600};
	wsprintf(m_szFps, L"속력 : %d",int(m_fCollisionSpeed));
	Engine::Get_Management()->GetRenderer()->GetFont()->DrawText(NULL, m_szFps, lstrlen(m_szFps), &rc, DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 255, 0));
	
	RECT		rc2 = {624, 100, 725, 144};
	wsprintf(m_szFps, L"X포즈값: %d",int(m_pInfo->m_vPos.x));
	Engine::Get_Management()->GetRenderer()->GetFont()->DrawText(NULL, m_szFps, lstrlen(m_szFps), &rc2, DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 255, 0));

	RECT		rc3 = {624, 160, 730, 199};
	wsprintf(m_szFps, L"Y포즈값: %d",int(m_pInfo->m_vPos.y));
	Engine::Get_Management()->GetRenderer()->GetFont()->DrawText(NULL, m_szFps, lstrlen(m_szFps), &rc3, DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 255, 0));*/
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayer* pObj = new CPlayer(pDevice);
	/*if(FAILED(pObj->Initialize()))
		Engine::Safe_Delete(pObj);*/

	return pObj;
}

HRESULT CPlayer::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	((Engine::CTransform*)pComponent)->SetScale(D3DXVECTOR3(0.5f,0.5f,0.5f));
	NULL_CHECK_RETURN(m_pInfo, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//TerrainCol
	pComponent = m_pTerrainCol = CTerrainCol::Create();
	((CTerrainCol*)pComponent)->SetbHeliCol(&m_bHeliCol);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"TerrainCol", pComponent));

	//MouseCol
	pComponent = m_pMouseCol = CMouseCol::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"MouseCol", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, L"Buffer_HeliCol");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	return S_OK;
}

void CPlayer::KeyCheck(void)
{
	float		fGetTime = m_pTimeMgr->GetTime();


	if(GetAsyncKeyState('1'))
		m_pInfo->m_fHP -= 1;
	if(GetAsyncKeyState('0')& 0x0001)
	{
		m_pTemp = CExplosion::Create(m_pDevice);
		((CExplosion*)m_pTemp)->SetPos(m_pInfo->m_vPos);

		MAPOBJLIST::iterator iter = m_pMapObjlist->find(L"Explosion");
		if(iter == m_pMapObjlist->end())
		{
			(*m_pMapObjlist)[L"Explosion"] = OBJECTLIST();
		}
		(*m_pMapObjlist)[L"Explosion"].push_back(m_pTemp);
	}	


	if(GetAsyncKeyState(VK_LEFT) && GetAsyncKeyState(VK_RIGHT))
	{
		m_pTerrainCol->SetbYfix(false);
		m_pInfo->m_vPos.y += 50.f * fGetTime;
		m_fTime = 0.f;
		m_bJump = true;
		if(m_pInfo->m_vPos.y >= m_fMaxPosY)
			m_pInfo->m_vPos.y = m_fMaxPosY;

		D3DXVec3Normalize(&m_pInfo->m_vDir,&m_pInfo->m_vDir);

	}
	else
	{

		if(GetAsyncKeyState(VK_LEFT))	
		{
			m_bCollisionDamage = true;
			m_bHeliCol = false;
			m_pTerrainCol->SetbYfix(false);
			m_fTime = 0.f;
			m_bJump = true;
			
			//AngZ 움직임.
			if(m_bLeftAngZ_Stabilizing == false && m_fAngleZ < 1.7f)
			{
				m_fAngleZ += m_fAngleZ_Speed * fGetTime;
				if(m_fAngleZ > 1.7f)
					m_bLeftAngZ_Stabilizing = true;
			}
			else if(m_bLeftAngZ_Stabilizing)
			{	
				m_fAngleZ -= m_fAngleZ_Stabilizing_Speed * fGetTime;
				if(m_fAngleZ < 0.3f)
					m_fAngleZ = 0.3f;
			}
			m_fBefore_AngZ = m_fAngleZ;
			m_bBefore_AngZ_Decrease = true;
			m_bAngZzero	 = false;
			//

			m_fAngleY += m_fAngleY_Speed * fGetTime;
			if(m_fAngleY < 0)
				m_fAngleX -= m_fAngleX_Speed  * fGetTime;
			else
				m_fAngleX += m_fAngleX_Speed  * fGetTime;

			if(m_fAngleY > 1.3f)
				m_fAngleY = 1.3f;
			if(m_fAngleX > 0.8f)
				m_fAngleX = 0.8f;
			

			Engine::CMathMgr::MyRotationY(&m_pInfo->m_vDir, &m_pInfo->m_vDir, D3DXToRadian(m_fAngleY));
			Engine::CMathMgr::MyRotationX(&m_pInfo->m_vDir, &m_pInfo->m_vDir, D3DXToRadian(m_fAngleX));
			Engine::CMathMgr::MyRotationZ(&m_pInfo->m_vDir, &m_pInfo->m_vDir, D3DXToRadian(m_fAngleZ));
		
			D3DXVec3Normalize(&m_pInfo->m_vDir,&m_pInfo->m_vDir);

			
			m_pInfo->m_vPos.x -= abs(m_pInfo->m_vDir.x * m_fX_Speed  * fGetTime);//0.2f*abs(m_fAngleY);
			m_pInfo->m_vPos.y -= m_pInfo->m_vDir.y * m_fY_Speed  * fGetTime;///8.f;
			//
			m_vLeftLegPos.x -= abs(m_pInfo->m_vDir.x * m_fX_Speed  * fGetTime);//0.2f*abs(m_fAngleY);
			m_vLeftLegPos.y -= m_pInfo->m_vDir.y * m_fY_Speed  * fGetTime;///8.f;
			//

			if(m_pInfo->m_vPos.y >= m_fMaxPosY)
				m_pInfo->m_vPos.y = m_fMaxPosY;

			//wire 움직임관련
			CWire::fMoveValue += 10.f * fGetTime;
			if(CWire::fMoveValue > 6.f)
				CWire::fMoveValue = 6.f;
			
			
			if(Hang)
				m_fLeftSpeed = -1* abs(m_pInfo->m_vDir.x * m_fWireX_Speed * fGetTime)*0.5f;
			else	
				m_fLeftSpeed = -1* abs(m_pInfo->m_vDir.x * m_fWireX_Speed * fGetTime)*0.2f;


			m_bWireRightMove = true;
			if(m_bWireLeftMove && m_pWireHead != NULL)
			{
				m_bWireLeftMove = false;
				m_pWireHead->Set_T_and_V(0.f,-m_fRightSpeed);
				m_pWireHead->SetbWireMove(true);
				m_pWireHead->SetHeliSpeed(-m_fRightSpeed);
			}
			//
		
			//
			m_bRightButtonUp = false;
			m_bLeftButtonUp  = true;
			//

			//
			m_bLeftKeyDown = true;


		}
		else
		{
			//wire 움직임관련(키를땟을때)

			if(m_bLeftKeyDown && m_pWireHead != NULL)
			{
				m_bLeftKeyDown = false;

				m_bWireLeftMove = false;
				m_pWireHead->Set_T_and_V(0.f,-m_fLeftSpeed);
				m_pWireHead->SetbWireMove(true);
				m_pWireHead->SetHeliSpeed(-m_fLeftSpeed);
			}
			//

			if(m_bJump == false && m_bLeftButtonUp)
			{
				//Wire 돌아오는값
				if(CWire::fMoveValue > 0.f)
					CWire::fMoveValue -= 10.f * fGetTime;
				else if(CWire::fMoveValue < 0.f)
					CWire::fMoveValue += 10.f * fGetTime;

				if(abs(CWire::fMoveValue) < 0.5f)
					CWire::fMoveValue = 0.f;
				//

				if(m_fAngleY > 0)
				{
					m_fAngleY -= m_fAngleY_Speed * fGetTime;
					if(m_fAngleY <= 0)
						m_fAngleY = 0.f;
				}
				if(m_fAngleY < 0)
				{
					m_fAngleY += m_fAngleY_Speed * fGetTime;
					if(m_fAngleY >= 0)
						m_fAngleY = 0.f;
				}
				if(m_fAngleX > 0)
				{
					m_fAngleX -= m_fAngleX_Speed * fGetTime;
					if(m_fAngleX <= 0)
						m_fAngleX = 0.f;
				}
				else if(m_fAngleX < 0)
				{
					m_fAngleX += m_fAngleX_Speed * fGetTime;
					if(m_fAngleX >= 0)
						m_fAngleX = 0.f;
				}

				//앵글Z값 원상복구되는 부분 LEFT	
				if(m_bAngZzero == false && m_bHeliCol == false)
				{
					//앵글z값을 0으로해주고 이부분 더이상안들어오게만드는곳.
					if(abs(m_fBefore_AngZ) < m_fBefore_AngZ_Limit)
					{
						m_fAngZTime = 0.f;
						m_fAngleZ = 0.f;
						m_bAngZzero = true;
						m_bBefore_AngZ_Decrease = false;
					}
					//

					if(m_bBefore_AngZ_Decrease)
					{
						m_fAngZTime = 0.f;

						if(m_fBefore_AngZ > 0)
							m_fBefore_AngZ -= m_fBeforeAngZ_Speed * fGetTime;
						else if(m_fBefore_AngZ < 0)
							m_fBefore_AngZ += m_fBeforeAngZ_Speed * fGetTime;

						m_fBefore_AngZ *= -1.f;
					}


					if(m_fBefore_AngZ > 0)
					{
						if(m_fAngleZ <= 0)
							m_fAngZTime += m_fAngZTime_Speed * fGetTime;
						else if(m_fAngleZ > 0)
							m_fAngZTime -= m_fAngZTime_Speed * fGetTime;

						m_fAngleZ += m_fAngleZReturn_Speed * fGetTime * m_fAngZTime;

						if(m_fAngleZ >= m_fBefore_AngZ)
							m_bBefore_AngZ_Decrease = true;
						else
							m_bBefore_AngZ_Decrease = false;

					}
					else if(m_fBefore_AngZ < 0)
					{
						if(m_fAngleZ >= 0)
							m_fAngZTime += m_fAngZTime_Speed * fGetTime;
						else if(m_fAngleZ < 0)
							m_fAngZTime -= m_fAngZTime_Speed * fGetTime;

						m_fAngleZ -= m_fAngleZReturn_Speed * fGetTime * m_fAngZTime;
						if(m_fAngleZ <= m_fBefore_AngZ)
							m_bBefore_AngZ_Decrease = true;
						else
							m_bBefore_AngZ_Decrease = false;
						
					}
				}
				
			}
			//왼쪽만땟을때 이것이 동작하게해야함. bool 변수 2개를 추가해서
			
			if(m_bLeftButtonUp)
			{
				
				Engine::CMathMgr::MyRotationY(&m_pInfo->m_vDir, &m_pInfo->m_vDir, D3DXToRadian(m_fAngleY));
				Engine::CMathMgr::MyRotationX(&m_pInfo->m_vDir, &m_pInfo->m_vDir, D3DXToRadian(m_fAngleX));
				Engine::CMathMgr::MyRotationZ(&m_pInfo->m_vDir, &m_pInfo->m_vDir, D3DXToRadian(m_fAngleZ));

				D3DXVec3Normalize(&m_pInfo->m_vDir,&m_pInfo->m_vDir);

			
				if(m_pInfo->m_vDir.x < 0)
					m_pInfo->m_vPos.x += abs(m_pInfo->m_vDir.x * m_fInertiaX_Speed * fGetTime);//0.2f*abs(m_fAngleY);
				else
					m_pInfo->m_vPos.x -= abs(m_pInfo->m_vDir.x * m_fInertiaX_Speed * fGetTime);

			}
			//
			m_bLeftAngZ_Stabilizing = false;
			

		}
		if(GetAsyncKeyState(VK_RIGHT))	
		{
			m_bCollisionDamage = true;
			m_bHeliCol = false;
			m_pTerrainCol->SetbYfix(false);
			m_fTime = 0.f;
			m_bJump = true;
			
			//AngZ 움직임.
			if(m_bRightAngZ_Stabilizing == false && m_fAngleZ > -1.7f)
			{ 
				m_fAngleZ -= m_fAngleZ_Speed * fGetTime;
				if(m_fAngleZ < -1.7f)
					m_bRightAngZ_Stabilizing = true;
			}
			else if(m_bRightAngZ_Stabilizing)
			{	
				m_fAngleZ += m_fAngleZ_Stabilizing_Speed * fGetTime;
				if(m_fAngleZ > -0.3f)
					m_fAngleZ = -0.3f;
			}
			m_fBefore_AngZ = m_fAngleZ;
			m_bBefore_AngZ_Decrease = true;
			m_bAngZzero	 = false;
			//

			m_fAngleY -= m_fAngleY_Speed * fGetTime;
			if(m_fAngleY > 0)
				m_fAngleX -= m_fAngleX_Speed * fGetTime;
			else
				m_fAngleX += m_fAngleX_Speed * fGetTime;

			if(m_fAngleY < -1.3f)
				m_fAngleY = -1.3f;
			if(m_fAngleX > 0.8f)
				m_fAngleX = 0.8f;

			Engine::CMathMgr::MyRotationY(&m_pInfo->m_vDir, &m_pInfo->m_vDir, D3DXToRadian(m_fAngleY));
			Engine::CMathMgr::MyRotationX(&m_pInfo->m_vDir, &m_pInfo->m_vDir, D3DXToRadian(m_fAngleX));
			Engine::CMathMgr::MyRotationZ(&m_pInfo->m_vDir, &m_pInfo->m_vDir, D3DXToRadian(m_fAngleZ));

			D3DXVec3Normalize(&m_pInfo->m_vDir,&m_pInfo->m_vDir);

			m_pInfo->m_vPos.x -= -1 *abs(m_pInfo->m_vDir.x * m_fX_Speed * fGetTime);//0.2f*abs(m_fAngleY);
			m_pInfo->m_vPos.y -= m_pInfo->m_vDir.y * m_fY_Speed * fGetTime;///8.f;
			
			//
			m_vLeftLegPos.y-= m_pInfo->m_vDir.y * m_fY_Speed * fGetTime;///8.f;
			m_vLeftLegPos.x -= -1 *abs(m_pInfo->m_vDir.x * m_fX_Speed * fGetTime);//0.2f*abs(m_fAngleY);
			//

			if(m_pInfo->m_vPos.y >= m_fMaxPosY)
				m_pInfo->m_vPos.y = m_fMaxPosY;

			//wire 움직임관련
			CWire::fMoveValue -= 10.f * fGetTime;
			if(CWire::fMoveValue < -6.f)
				CWire::fMoveValue = -6.f;

			if(Hang)
				m_fRightSpeed = abs(m_pInfo->m_vDir.x * m_fWireX_Speed * fGetTime)*0.5f;
			else	
				m_fRightSpeed = abs(m_pInfo->m_vDir.x * m_fWireX_Speed * fGetTime)*0.2f;


			m_bWireLeftMove = true;
			if(m_bWireRightMove && m_pWireHead != NULL)
			{
				m_bWireRightMove = false;
				m_pWireHead->Set_T_and_V(0.f,-m_fLeftSpeed);
				m_pWireHead->SetbWireMove(true);
				m_pWireHead->SetHeliSpeed(-m_fLeftSpeed);
			}
			//

			//
			m_bRightButtonUp = true;
			m_bLeftButtonUp  = false;
			//

			m_bRightKeyDown = true;
		}
		else
		{

			//wire 움직임관련(키를땟을때)

			if(m_bRightKeyDown && m_pWireHead != NULL)
			{
				m_bRightKeyDown = false;

				m_bWireLeftMove = false;
				m_pWireHead->Set_T_and_V(0.f,-m_fRightSpeed);
				m_pWireHead->SetbWireMove(true);
				m_pWireHead->SetHeliSpeed(-m_fRightSpeed);
			}
			//
			if(m_bJump == false && m_bRightButtonUp)
			{
				//Wire 돌아오는값
				if(CWire::fMoveValue > 0.f)
					CWire::fMoveValue -= 10.f * fGetTime;
				else if(CWire::fMoveValue < 0.f)
					CWire::fMoveValue += 10.f * fGetTime;

				if(abs(CWire::fMoveValue) < 0.5f)
					CWire::fMoveValue = 0.f;
				//

				if(m_fAngleY < 0)
				{
					m_fAngleY += m_fAngleY_Speed * fGetTime;
					if(m_fAngleY >= 0)
						m_fAngleY = 0.f;
				}
				if(m_fAngleY > 0)
				{
					m_fAngleY -= m_fAngleY_Speed * fGetTime;
					if(m_fAngleY <= 0)
						m_fAngleY = 0.f;
				}
				if(m_fAngleX > 0)
				{
					m_fAngleX -= m_fAngleX_Speed * fGetTime;
					if(m_fAngleX <= 0)
						m_fAngleX = 0.f;
				}
				else if(m_fAngleX < 0)
				{
					m_fAngleX += m_fAngleX_Speed * fGetTime;
					if(m_fAngleX >= 0)
						m_fAngleX = 0.f;
				}

				//앵글Z값 원상복구되는 부분 Right	
				if(m_bAngZzero == false && m_bHeliCol == false)
				{
					//앵글z값을 0으로해주고 이부분 더이상안들어오게만드는곳.
					if(abs(m_fBefore_AngZ) < m_fBefore_AngZ_Limit)
					{
						m_fAngZTime = 0.f;
						m_fAngleZ = 0.f;
						m_bAngZzero = true;
						m_bBefore_AngZ_Decrease = false;
					}
					//

					if(m_bBefore_AngZ_Decrease)
					{
						m_fAngZTime = 0.f;

						if(m_fBefore_AngZ > 0)
							m_fBefore_AngZ -= m_fBeforeAngZ_Speed * fGetTime;
						else if(m_fBefore_AngZ < 0)
							m_fBefore_AngZ += m_fBeforeAngZ_Speed * fGetTime;

						m_fBefore_AngZ *= -1.f;
					}


					if(m_fBefore_AngZ > 0)
					{
						if(m_fAngleZ <= 0)
							m_fAngZTime += m_fAngZTime_Speed * fGetTime;
						else if(m_fAngleZ > 0)
							m_fAngZTime -= m_fAngZTime_Speed * fGetTime;

						m_fAngleZ += m_fAngleZReturn_Speed * fGetTime * m_fAngZTime;

						if(m_fAngleZ >= m_fBefore_AngZ)
							m_bBefore_AngZ_Decrease = true;
						else
							m_bBefore_AngZ_Decrease = false;

					}
					else if(m_fBefore_AngZ < 0)
					{
						if(m_fAngleZ >= 0)
							m_fAngZTime += m_fAngZTime_Speed * fGetTime;
						else if(m_fAngleZ < 0)
							m_fAngZTime -= m_fAngZTime_Speed * fGetTime;

						m_fAngleZ -= m_fAngleZReturn_Speed * fGetTime * m_fAngZTime;
						if(m_fAngleZ <= m_fBefore_AngZ)
							m_bBefore_AngZ_Decrease = true;
						else
							m_bBefore_AngZ_Decrease = false;

					}
				}
			}

			//오른쪽만땟을때 이것이 동작하게해야함. bool 변수 2개를 추가해서
			
			if(m_bRightButtonUp)
			{
				Engine::CMathMgr::MyRotationY(&m_pInfo->m_vDir, &m_pInfo->m_vDir, D3DXToRadian(m_fAngleY));
				Engine::CMathMgr::MyRotationX(&m_pInfo->m_vDir, &m_pInfo->m_vDir, D3DXToRadian(m_fAngleX));
				Engine::CMathMgr::MyRotationZ(&m_pInfo->m_vDir, &m_pInfo->m_vDir, D3DXToRadian(m_fAngleZ));

				D3DXVec3Normalize(&m_pInfo->m_vDir,&m_pInfo->m_vDir);

				if(m_pInfo->m_vDir.x > 0)
					m_pInfo->m_vPos.x += -1 * abs(m_pInfo->m_vDir.x*m_fInertiaX_Speed * fGetTime);
				else
					m_pInfo->m_vPos.x -= -1 * abs(m_pInfo->m_vDir.x*m_fInertiaX_Speed * fGetTime);//0.2f*abs(m_fAngleY);
			}
			//
			m_bRightAngZ_Stabilizing = false;

		}
	}
	

	if(Engine::Get_KeyMgr()->KeyDown(KEY_RBUTTON))
	{
		m_pInfo->m_eState = Engine::STATE_SPECIAL;
		m_bBomb = true;
		MAPOBJLIST::iterator	iterU = m_pMapObjlistUI->find(L"StaticCamera");
		OBJECTLIST::iterator iterUI  = iterU->second.begin();
		((CStaticCamera*)(*iterUI))->SetTargetDistance(200.f);
		((CStaticCamera*)(*iterUI))->SetAngleUp(D3DXToRadian(300.f));
		((CStaticCamera*)(*iterUI))->SetAngleRight(D3DXToRadian(30.f));
		((CStaticCamera*)(*iterUI))->SetbBombMove(true);
		m_vBombFragmentPos = D3DXVECTOR3(m_pInfo->m_vPos.x, m_pTerrainVertex[m_pTerrainCol->GetIndex()].vPos.y + 1.f, m_pInfo->m_vPos.z);
	}
	if(Engine::Get_KeyMgr()->KeyDown(KEY_2))
	{
		//-------머리생성-----
		m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_WireTex",((CWire*)m_pWire)->GetpVertex());

		m_pWireHead = CWireHead::Create(m_pDevice);
		((CWireHead*)m_pWireHead)->SetHeliInfo(m_pInfo);
		((CWireHead*)m_pWireHead)->SetParentMat(&m_pInfo->m_matWorldAng);
		MAPOBJLIST::iterator	iter = m_pMapObjlist->find(L"WireHead");
		if(iter == m_pMapObjlist->end())
		{
			(*m_pMapObjlist)[L"WireHead"] = OBJECTLIST();
		}
		
		////가슴리스트셋
		iter = m_pMapObjlist->find(L"WireChest");
		if(iter == m_pMapObjlist->end())
		{
			(*m_pMapObjlist)[L"WireChest"] = OBJECTLIST();
		}
		((CWireHead*)m_pWireHead)->SetWireChestList(&(*m_pMapObjlist)[L"WireChest"]);
		
		////배리스트셋
		iter = m_pMapObjlist->find(L"WireStomach");
		if(iter == m_pMapObjlist->end())
		{
			(*m_pMapObjlist)[L"WireStomach"] = OBJECTLIST();
		}
		((CWireHead*)m_pWireHead)->SetWireStomachList(&(*m_pMapObjlist)[L"WireStomach"]);

		////꼬리리스트셋
		iter = m_pMapObjlist->find(L"WireTail");
		if(iter == m_pMapObjlist->end())
		{
			(*m_pMapObjlist)[L"WireTail"] = OBJECTLIST();
		}
		((CWireHead*)m_pWireHead)->SetWireTailList(&(*m_pMapObjlist)[L"WireTail"]);

		////화물 훅셋
		iter = m_pMapObjlist->find(L"Freight_Hook");
		if(iter == m_pMapObjlist->end())
		{
			(*m_pMapObjlist)[L"Freight_Hook"] = OBJECTLIST();
		}


		
		////
		((CWireHead*)m_pWireHead)->CreateWireChest();

		((CWireChest*)((CWireHead*)m_pWireHead)->GetWireChest())->SetWireStomachList(((CWireHead*)m_pWireHead)->GetWireStomachList());
		((CWireChest*)((CWireHead*)m_pWireHead)->GetWireChest())->CreateWireStomach();
		

		((CWireStomach*)((CWireHead*)m_pWireHead)->GetWireChest()->Getm_WireStomach())->SetWireTailList(((CWireHead*)m_pWireHead)->GetWireTailList());
		((CWireStomach*)((CWireHead*)m_pWireHead)->GetWireChest()->Getm_WireStomach())->CreateWireTail();
		//((CWireStomach*)((CWireChest*)((CWireHead*)m_pTemp)->GetWireChest())->Getm_WireStomach())->set

		////
		(*m_pMapObjlist)[L"WireHead"].push_back(m_pWireHead);

		
		Engine::CTransform*	 TailTransform = ((CWireStomach*)((CWireHead*)m_pWireHead)->GetWireChest()->Getm_WireStomach())->GetWireTail()->GetTransform();
	

		//훅생성
		m_pTemp = CHook::Create(m_pDevice);
		((CHook*)m_pTemp)->SetParentInfo(TailTransform);
		((CHook*)m_pTemp)->SetFreight_HookList(&(*m_pMapObjlist)[L"Freight_Hook"]);
		iter = m_pMapObjlist->find(L"Hook");
		if(iter == m_pMapObjlist->end())
		{
			(*m_pMapObjlist)[L"Hook"] = OBJECTLIST();
		}
		(*m_pMapObjlist)[L"Hook"].push_back(m_pTemp);

	}

}


void CPlayer::Release(void)
{
	m_pInfoSubject->RemoveData(PLAYER_POS, &m_pInfo->m_vPos);
	m_pInfoSubject->RemoveData(PLAYER_DIR, &m_pInfo->m_vDir);
	m_pInfoSubject->RemoveData(PLAYER_HP, &m_pInfo->m_fHP);
	m_pInfoSubject->RemoveData(PLAYER_BULLET, &m_fBullet);
	m_pInfoSubject->RemoveData(PLAYER_STATE, &m_pInfo->m_eState);
	m_pInfoSubject->RemoveData(PLAYER_WORLD, &m_pInfo->m_matWorldNoSca);
	m_pInfoSubject->RemoveData(PLAYER_BOARDNUM, &m_iBoardNum);
	m_pInfoSubject->RemoveData(PLAYER_MAXBOARDNUM, &m_iMAXBoardNum);

	m_pInfoSubject->UnSubscribe(m_pCameraObserver);

	list<CNemo*>::iterator iter = m_NemoList.begin();
	list<CNemo*>::iterator iter_end = m_NemoList.end();

	for(; iter != iter_end; ++iter)
	{
		Engine::Safe_Delete(*iter);
	}

	Engine::Safe_Delete_Array(m_pVertex);
	Engine::Safe_Delete_Array(m_pConvertVertex);

	Engine::Safe_Delete(m_pCameraObserver);
	Engine::Safe_Delete(m_Dust);
	Engine::Safe_Delete(m_pWire);
	Engine::Safe_Delete(m_pBasicBullet);
	Engine::Safe_Delete(m_pMissileBullet);
	Engine::Safe_Delete(m_pMultiBullet);
	Engine::Safe_Delete(m_pBomb);
}

void CPlayer::TransformMatrix(void)
{
	const D3DXMATRIX* pmatView = m_pCameraObserver->GetView();
	NULL_CHECK(pmatView);
	const D3DXMATRIX* pmatProj = m_pCameraObserver->GetProj();
	NULL_CHECK(pmatProj);
	

	D3DXMATRIX matRotx, matRoty, matRotz;

	D3DXMatrixRotationZ(&matRotz, m_fAngleZ);
	D3DXMatrixRotationY(&matRoty, m_fAngleY);
	D3DXMatrixRotationX(&matRotx, m_fAngleX);
	
	//
	D3DXMATRIX RevolveActivate;
	//

	if(m_bHeliCol && m_fAngleZ != 0 && m_pInfo->m_eState == Engine::STATE_LIVE)
	{

		D3DXMATRIX matBackTrans,matOriginTrans,matRotRevolvez;


		if(m_fAngleRevolveZ > 0)
		{
			D3DXMatrixRotationZ(&matRotRevolvez, m_fAngleRevolveZ);
			D3DXMatrixTranslation(&matBackTrans,-m_vLeftLegPos.x,-m_vLeftLegPos.y,-m_vLeftLegPos.z);
			D3DXMatrixTranslation(&matOriginTrans,m_vLeftLegPos.x,m_vLeftLegPos.y,m_vLeftLegPos.z);
		}
		else if(m_fAngleRevolveZ < 0)
		{
			D3DXMatrixRotationZ(&matRotRevolvez, m_fAngleRevolveZ);
			D3DXMatrixTranslation(&matBackTrans,-m_vRightLegPos.x,-m_vRightLegPos.y,-m_vRightLegPos.z);
			D3DXMatrixTranslation(&matOriginTrans,m_vRightLegPos.x,m_vRightLegPos.y,m_vRightLegPos.z);
		}
		
		RevolveActivate = m_pInfo->m_matWorld * matBackTrans * matRotRevolvez * matOriginTrans;

		m_pInfo->m_matWorldAng = RevolveActivate;
		
		
		//원래 공전앵글z값의 절대값이 1보다작으면 앵글값을 0으로
		if(abs(m_fOriginAngleRevolveZ) < 0.9f)
		{
		
			if(m_fAngleRevolveZ > 0)
				m_fAngleRevolveZ -= 2.f * m_pTimeMgr->GetTime();
			else if(m_fAngleRevolveZ < 0)
				m_fAngleRevolveZ += 2.f * m_pTimeMgr->GetTime();

			if(abs(m_fAngleRevolveZ) < 0.1)
			{
				m_fAngleZ = 0.f;
				m_fAngleRevolveZ = 0.f;
				m_fOriginAngleRevolveZ = 0.f;
			}

		}
		//원래 공전앵글z값의 절대값이 1보다크면 반대로 도는 예외처리
		else if(abs(m_fOriginAngleRevolveZ) >= 0.9f)
		{
			if(m_fAngleRevolveZ > 0)
				m_fAngleRevolveZ += 2.f * m_pTimeMgr->GetTime();
			else if(m_fAngleRevolveZ < 0)
				m_fAngleRevolveZ -= 2.f * m_pTimeMgr->GetTime();

			if(abs(m_fAngleRevolveZ) > 1.5f)
			{
				//예외처리용
				if(m_fAngleRevolveZ > 0)
				{
					m_fAngleZ = 1.5f;
					m_fAngleRevolveZ = 1.5f;
				}
				else if(m_fAngleRevolveZ < 0)
				{
					m_fAngleZ = -1.5f;
					m_fAngleRevolveZ = -1.5f;
				}
				m_pInfo->m_fHP = 0.f;
			}
		}

	}
	else
	{
		m_fAngleRevolveZ = m_fAngleZ;
		m_fOriginAngleRevolveZ = m_fAngleRevolveZ;
		m_pInfo->m_matWorldAng = matRotz * matRoty * matRotx *  m_pInfo->m_matWorld;
	}


	m_vLeftLegPos = m_vOriginLeftLegPos;
	m_vRightLegPos = m_vOriginRightLegPos;
	//
	if(m_bHeliCol &&  m_fAngleRevolveZ != 0 && m_pInfo->m_eState == Engine::STATE_LIVE)
		m_pInfo->m_matWorldAng = m_pInfo->m_matWorld;
	//왼다리
	D3DXVec3TransformCoord(&m_vLeftLegPos, &m_vLeftLegPos, &m_pInfo->m_matWorldAng);
	//오른다리
	D3DXVec3TransformCoord(&m_vRightLegPos, &m_vRightLegPos, &m_pInfo->m_matWorldAng);
	//
	if(m_bHeliCol && m_fAngleRevolveZ != 0 && m_pInfo->m_eState == Engine::STATE_LIVE)
		m_pInfo->m_matWorldAng = RevolveActivate;

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVertex[i] = m_pVertex[i];
		
		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, &m_pInfo->m_matWorldAng);
		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatView);
		if(m_pConvertVertex[i].vPos.z < 1.f)
			m_pConvertVertex[i].vPos.z = 1.f;

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pmatProj);
	}
}

void CPlayer::JumpUpdate()
{
	MAPCOMPONENT::iterator	iter = m_mapComponent.begin();
	MAPCOMPONENT::iterator	iter_end = m_mapComponent.end();

	for( ;iter != iter_end ; ++iter)
	{
		if(iter->first != L"TerrainCol")
			iter->second->Update();
	}

}

void CPlayer::DustCreate()
{
	m_pTemp = CDust::Create(m_pDevice);
	((CEffect*)m_pTemp)->SetPos(D3DXVECTOR3(m_pInfo->m_vPos.x, m_pTerrainVertex[m_pTerrainCol->GetIndex()].vPos.y + 1.f, m_pInfo->m_vPos.z));

	((CEffect*)m_pTemp)->SetAngleZ(float(rand()%200));
	((CDust*)m_pTemp)->DustRotationY();


	MAPOBJLIST::iterator	iter = m_pMapObjlist->find(L"Dust");
	if(iter == m_pMapObjlist->end())
	{
		(*m_pMapObjlist)[L"Dust"] = OBJECTLIST();
	}
	(*m_pMapObjlist)[L"Dust"].push_back(m_pTemp);
}

void CPlayer::SmokeCreate()
{
	//m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_RcTex",((CEffect*)m_Dust)->GetpVertex());

	for(int i = 0; i < 3; ++i)
	{

		m_pTemp = CSmoke::Create(m_pDevice);
		((CEffect*)m_pTemp)->SetPos(m_pInfo->m_vPos + D3DXVECTOR3(rand()%30*0.1f,1.f,0.f));

		MAPOBJLIST::iterator	iter = m_pMapObjlist->find(L"Smoke");
		if(iter == m_pMapObjlist->end())
		{
			(*m_pMapObjlist)[L"Smoke"] = OBJECTLIST();
		}
		(*m_pMapObjlist)[L"Smoke"].push_back(m_pTemp);
	}
}



void CPlayer::BombCreate()
{
	//m_pResourceMgr->SetVertexInfo(Engine::RESOURCE_DYNAMIC, L"Buffer_Bomb",((CBullet*)m_pBomb)->GetpVertex());

	m_pTemp = CBomb::Create(m_pDevice);
	((CBullet*)m_pTemp)->SetPos(m_pInfo->m_vPos);
	((CBomb*)m_pTemp)->SetbFragmentCreate(&m_bFragmentCreate);

	MAPOBJLIST::iterator	iter = m_pMapObjlist->find(L"Bomb");
	if(iter == m_pMapObjlist->end())
	{
		(*m_pMapObjlist)[L"Bomb"] = OBJECTLIST();
	}
	(*m_pMapObjlist)[L"Bomb"].push_back(m_pTemp);
	//
	MAPOBJLIST::iterator	iterU = m_pMapObjlistUI->find(L"StaticCamera");
	OBJECTLIST::iterator iterUI  = iterU->second.begin();
	((CStaticCamera*)(*iterUI))->SetTargetInfo((const Engine::CTransform*)m_pTemp->GetComponent(L"Transform"));
	//
}

void CPlayer::AddNemo(CNemo* pNemo)
{
	m_NemoList.push_back(pNemo);
}

list<CNemo*>* CPlayer::GetNemoList(void)
{
	return &m_NemoList;
}

void CPlayer::SetScale(const D3DXVECTOR3& vScale)
{
	m_pInfo->m_vScale = vScale;
}

void CPlayer::PropellerCreate()
{
	//프로펠러
	m_pPropeller = CPropeller::Create(m_pDevice);
	((CPropeller*)m_pPropeller)->SetParentMatrix(&m_pInfo->m_matWorldAng);
	((CPropeller*)m_pPropeller)->SetHeliPosY(&m_pInfo->m_vPos.y);

	MAPOBJLIST::iterator	iter = m_pMapObjlist->find(L"Propeller");
	if(iter == m_pMapObjlist->end())
	{
		(*m_pMapObjlist)[L"Propeller"] = OBJECTLIST();
	}
	(*m_pMapObjlist)[L"Propeller"].push_back(m_pPropeller);

	//미니프로펠러
	m_pMiniPropeller = CMiniPropeller::Create(m_pDevice);
	((CMiniPropeller*)m_pMiniPropeller)->SetParentMatrix(&m_pInfo->m_matWorldAng);
	((CMiniPropeller*)m_pMiniPropeller)->SetHeliPosY(&m_pInfo->m_vPos.y);

	iter = m_pMapObjlist->find(L"MiniPropeller");
	if(iter == m_pMapObjlist->end())
	{
		(*m_pMapObjlist)[L"MiniPropeller"] = OBJECTLIST();
	}
	(*m_pMapObjlist)[L"MiniPropeller"].push_back(m_pMiniPropeller);
}


void CPlayer::WeaponCreate(wchar_t* BufferName, WEAPON_TYPE _eType)
{
	if(m_bOnce)
	{
		m_pWeapon = CWeapon::Create(m_pDevice);
		((CWeapon*)m_pWeapon)->SetParentMatrix(&m_pInfo->m_matWorldAng);
		((CWeapon*)m_pWeapon)->SetNoRotParMatrix(&m_pInfo->m_matWorld);
		((CWeapon*)m_pWeapon)->SetMapObjlist(m_pMapObjlist);
		((CWeapon*)m_pWeapon)->SetPlayer(this);
		((CWeapon*)m_pWeapon)->Initialize();

		MAPOBJLIST::iterator	iter = m_pMapObjlist->find(L"Weapon");
		if(iter == m_pMapObjlist->end())
			(*m_pMapObjlist)[L"Weapon"] = OBJECTLIST();

		(*m_pMapObjlist)[L"Weapon"].push_back(m_pWeapon);

		m_bOnce = false;
	}

	((CWeapon*)m_pWeapon)->SetType(_eType);
	((CWeapon*)m_pWeapon)->ChangeBuffer(BufferName);

}


void CPlayer::FragmentCreate()
{
	m_pTemp = CFragment::Create(m_pDevice);
	((CEffect*)m_pTemp)->SetPos(m_vBombFragmentPos);

	((CEffect*)m_pTemp)->SetAngleZ(float(rand()%200));

	//
	MAPOBJLIST::iterator	iter = m_pMapObjlist->find(L"Fragment");
	if(iter == m_pMapObjlist->end())
	{
		(*m_pMapObjlist)[L"Fragment"] = OBJECTLIST();
	}
	(*m_pMapObjlist)[L"Fragment"].push_back(m_pTemp);
}

void CPlayer::ChangeBuffer(wchar_t* BufferName)
{
	if(m_CurrentBuffer == BufferName )
		return ;

	m_CurrentBuffer = BufferName;

	MAPCOMPONENT::iterator iter = m_mapComponent.find(L"Buffer");
	Engine::Safe_Delete(iter->second);
	m_pBuffer = NULL;
	m_mapComponent.erase(iter);
	//

	//새버퍼추가
	m_wstrBufferName = BufferName;
	m_pResourceMgr->GetVertexInfo(Engine::RESOURCE_DYNAMIC, m_wstrBufferName , m_pVertex);

	Engine::CComponent*		pComponent = NULL;
	pComponent = m_pResourceMgr->CloneResource(Engine::RESOURCE_DYNAMIC, m_wstrBufferName);
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));
}

void CPlayer::SetHP(float rHP)
{
	m_pInfo->m_fHP = rHP;
}

void CPlayer::SetBoardNum()
{
	++m_iBoardNum;
}

int CPlayer::GetMAXBoardNum()
{
	return m_iMAXBoardNum;
}

int CPlayer::GetBoardNum()
{
	return m_iBoardNum;
}

void CPlayer::TreeCollision()
{
	D3DXVECTOR3 vPos;
	float fLength;
	MAPOBJLIST::iterator	iter = m_pMapObjlist->find(L"Tree");

	if(iter != m_pMapObjlist->end())
	{
		OBJECTLIST::iterator	iterBegin  = iter->second.begin();
		OBJECTLIST::iterator	iterEnd  = iter->second.end();

		for(;iterBegin != iterEnd; ++iterBegin)
		{
			vPos = ((CTree*)(*iterBegin))->GetPos() - m_pInfo->m_vPos;

			 fLength = D3DXVec3Length(&vPos);

			 if(fLength < 30.f)
			 {
				 if(m_pInfo->m_vPos.y > 20.f)
				 {	
					m_bTreeCollision = true;
					if(m_pInfo->m_vPos.x > ((CTree*)(*iterBegin))->GetPos().x)
						m_vDieMove= D3DXVECTOR3(-60.f,20.f,0.f);
					else if(m_pInfo->m_vPos.x < ((CTree*)(*iterBegin))->GetPos().x)
						m_vDieMove= D3DXVECTOR3(60.f,20.f,0.f);

					m_fDieAngleZ = 10.f;
					m_fDieAngleZ_Speed = 10.f;
					 m_pInfo->m_fHP = 0.f;
				 }
				 else
				 {
					 m_pInfo->m_fHP = 0.f;
				 }
				
			 }

		}
	}

	
}

