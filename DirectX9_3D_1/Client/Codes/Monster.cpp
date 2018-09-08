#include "stdafx.h"
#include "Monster.h"

#include "Export_Function.h"

#include "Line2D.h"
#include "Player.h"
#include "Collision.h"

#include "CollisionMgr.h"
#include "DamageFont.h"

#include "SoundMgr.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
: CGameObject(pGraphicDev)
, m_pMeshCom(NULL)
, m_pTransCom(NULL)
, m_pNaviCell(NULL)
, m_pEffect(NULL)
, m_pShadowEffect(NULL)
, m_fTimeDelta(0.f)
, m_fRenderTimeDelta(0.f)
, m_fAnimationSpeed(2.f)
, m_pSphere(NULL)
, m_bPlayerDistanceCheck(false)
, m_bPlayerAttack_Body_CollisionCheck(false)
, m_bPlayerEffect_Body_CollisionCheck(false)
, m_bPlayerAttack_MyAttack_CollisionCheck(false)
, m_iMonsterAnimationIndex(0)
, m_bCameraModeOn(false)
, m_vCollisionPos(0.f, 0.f, 0.f), m_fMaxDamageValue(0.f)
, m_fDownValue(0.f)
, m_fMonsterDownValue(0.f)
, m_iAnimationSoundCheck(0)
, m_fDistanceSize(1.f)
, m_bMoveCollCheck(true)
, m_pPlayer(CPlayer::GetInstance())
, m_fAngleX(0.f)
, m_fAngleZ(0.f)
, m_bAppear(true)
, m_bCameraModeCheck(false)
{
	m_tMonsterInfo.fDownValue = 25.f;
	CCollisionMgr::GetInstance()->Get_MonsterList()->push_back(this);
	ZeroMemory(m_bSoundPlay, sizeof(_bool) * 20);
	ZeroMemory(m_bCreateEffect, sizeof(_bool) * 50);
}

CMonster::~CMonster(void)
{

}

void CMonster::Render(void)
{
#ifdef _DEBUG
	if(m_pPlayer->m_bShowBounding)
	{
		Render_BoudingSphere();
		Render_AttackBox();	
		Render_DamageBox();	
	}
#endif
}

void CMonster::MonsterMoveFunction(const char* pFrameName)
{
	m_vOriginalPos = m_pTransCom->m_vPosition;
	m_pMeshCom->AnimationMove(pFrameName, m_pTransCom);

	ObjectBox_And_MyMoveBox();

	if(!CDynamicMesh_And_MyMoveBox())
		m_vAfterPos = m_pTransCom->m_vPosition;
	else
	{
		m_vAfterPos = m_vOriginalPos;
	}

	Check_Navigation();
}

void CMonster::MonsterMoveFunctionTwo(_bool bCompulsion, _bool bLeftAndRihgt, _bool bFrontAndBack)
{
	m_vOriginalPos = m_pTransCom->m_vPosition;
	m_pMeshCom->AnimationMoveTwo(m_pTransCom, bCompulsion, bLeftAndRihgt, bFrontAndBack, m_fDistanceSize);

	ObjectBox_And_MyMoveBox();

	if(!CDynamicMesh_And_MyMoveBox())
	{
		if(false == (m_pTransCom->m_vPosition.x <= FLT_MAX && m_pTransCom->m_vPosition.x >= -FLT_MAX))
			m_pTransCom->m_vPosition.x = (_int)(m_pTransCom->m_vPosition.x * 100 ) / 100.f;
		if(false == (m_pTransCom->m_vPosition.y <= FLT_MAX && m_pTransCom->m_vPosition.y >= -FLT_MAX))
			m_pTransCom->m_vPosition.y = (_int)(m_pTransCom->m_vPosition.y * 100 ) / 100.f;
		if(false == (m_pTransCom->m_vPosition.z <= FLT_MAX && m_pTransCom->m_vPosition.z >= -FLT_MAX))
			m_pTransCom->m_vPosition.z = (_int)(m_pTransCom->m_vPosition.z * 100 ) / 100.f;

		m_vAfterPos = m_pTransCom->m_vPosition;
	}
	else
	{
		if(false == (m_vOriginalPos.x <= FLT_MAX && m_vOriginalPos.x >= -FLT_MAX))
			m_vOriginalPos.x = (_int)(m_vOriginalPos.x * 100 ) / 100.f;
		if(false == (m_vOriginalPos.y <= FLT_MAX && m_vOriginalPos.y >= -FLT_MAX))
			m_vOriginalPos.y = (_int)(m_vOriginalPos.y * 100 ) / 100.f;
		if(false == (m_vOriginalPos.z <= FLT_MAX && m_vOriginalPos.z >= -FLT_MAX))
			m_vOriginalPos.z = (_int)(m_vOriginalPos.z * 100 ) / 100.f;

		m_vAfterPos = m_vOriginalPos;
	}

	Check_Navigation();
}

void CMonster::Check_Navigation()
{
	//처음에 있는 곳에 네비셀이 존재하면 받아온다//ㅊㅂㅇ
	if(NULL == m_pNaviCell)
	{
		list<Engine::CNaviCell*>::iterator iter = Engine::Get_NaviMesh()->Get_listNaviCell()->begin();
		list<Engine::CNaviCell*>::iterator iter_end = Engine::Get_NaviMesh()->Get_listNaviCell()->end();
		for(; iter != iter_end; ++iter)
		{
			_bool bInside = (*iter)->Inside_Point(&m_vAfterPos);
			if(true == bInside)
			{
				m_pNaviCell = (*iter);
				break;
			}
		}
	}
	else
	{
		_bool bInside = m_pNaviCell->CheckInFindNeighbor(&m_vAfterPos, &m_pNaviCell);

		if(true == bInside)
		{

			m_pTransCom->m_vPosition = m_vAfterPos;

			D3DXPLANE	CellPlane;
			D3DXPlaneFromPoints(&CellPlane, m_pNaviCell->Get_Point(Engine::CNaviCell::POINT_A), m_pNaviCell->Get_Point(Engine::CNaviCell::POINT_B), m_pNaviCell->Get_Point(Engine::CNaviCell::POINT_C));

			m_pTransCom->m_vPosition.y = (-(CellPlane.a * m_pTransCom->m_vPosition.x) - (CellPlane.c * m_pTransCom->m_vPosition.z) - CellPlane.d) / CellPlane.b;
		}
		else // 슬라이딩벡터
		{
			m_pTransCom->m_vPosition = m_vOriginalPos; // 움직이기 전 위치로 돌려놓고

			Engine::CLine2D* pLine = m_pNaviCell->Get_Line(&m_vAfterPos); // 움직인 후의 위치에 대해 판단한다.

			_vec3	vNormal = _vec3(-pLine->Get_VecNormal()->x, 0.f, -pLine->Get_VecNormal()->y);
			D3DXVec3Normalize(&vNormal, &vNormal);

			_vec3	vSlidingVector = (m_vAfterPos - m_vOriginalPos) - D3DXVec3Dot(&(m_vAfterPos - m_vOriginalPos), &vNormal) * vNormal;
			//D3DXVec3Normalize(&vSlidingVector, &vSlidingVector);

			//여기수정해야함.
			m_pTransCom->m_vPosition += vSlidingVector;
			//* (abs(D3DXVec3Length(&(m_vAfterPos - m_vOriginalPos)))) * m_fTimeDelta * 100; // 움직이기 전 위치에 슬라이딩벡터를 적용한다.

			//슬라이딩으로 이동한 후의 위치로 다시 검사.
			_bool bSlidingCheck = false;
			list<Engine::CNaviCell*>::iterator iter = Engine::Get_NaviMesh()->Get_listNaviCell()->begin();
			list<Engine::CNaviCell*>::iterator iter_end = Engine::Get_NaviMesh()->Get_listNaviCell()->end();
			for(; iter != iter_end; ++iter)
			{
				bSlidingCheck = (*iter)->Inside_Point(&m_pTransCom->m_vPosition);
				if(true == bSlidingCheck)
				{
					m_pNaviCell = (*iter);
					break;
				}
			}

			if(true == bSlidingCheck)
			{
				D3DXPLANE	CellPlane;
				D3DXPlaneFromPoints(&CellPlane, m_pNaviCell->Get_Point(Engine::CNaviCell::POINT_A), m_pNaviCell->Get_Point(Engine::CNaviCell::POINT_B), m_pNaviCell->Get_Point(Engine::CNaviCell::POINT_C));

				m_pTransCom->m_vPosition.y = (-(CellPlane.a * m_pTransCom->m_vPosition.x) - (CellPlane.c * m_pTransCom->m_vPosition.z) - CellPlane.d) / CellPlane.b;
			}
			else
			{
				m_pTransCom->m_vPosition -= vSlidingVector;
				//* (abs(D3DXVec3Length(&(m_vAfterPos - m_vOriginalPos)))) * m_fTimeDelta * 100;
			}
		}
	}
}

void CMonster::Load_AttackBox(TCHAR* pBoundingBoxPath)
{
	// Load Bounding Sphere & Box
	m_pSphere = new SPHEREINFO;

	HANDLE	hFile = CreateFile(pBoundingBoxPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD	dwByte;
	int iAnimationCnt = 0;	int iAniIndex = 0;	int iBoxCnt = 0;

	ZeroMemory(m_pSphere, sizeof(SPHEREINFO));

	ReadFile( hFile, m_pSphere, sizeof(SPHEREINFO), &dwByte, NULL );
	ReadFile( hFile, &iAnimationCnt, sizeof(int), &dwByte, NULL );

	for( int i = 0; i < iAnimationCnt; ++i )
	{
		ReadFile( hFile, &iAniIndex, sizeof(int), &dwByte, NULL );
		ReadFile( hFile, &iBoxCnt, sizeof(int), &dwByte, NULL );

		vector<CLIENTBOXINFO*>	vecBoxInfo;

		for( int j = 0; j < iBoxCnt; ++j )
		{ 
			BOXINFO*	pBoxInfo = new BOXINFO;
			CLIENTBOXINFO*	pBox = new CLIENTBOXINFO;
			ReadFile( hFile, (pBoxInfo), sizeof(BOXINFO), &dwByte, NULL );

			pBox->eBoxTag = pBoxInfo->eBoxTag;
			pBox->iAniIdx = pBoxInfo->iAniIdx;

			WideCharToMultiByte(CP_ACP, 0, pBoxInfo->pFrameName, 128, pBox->pFrameName, 128, NULL, NULL);

			pBox->vMin = pBoxInfo->vMin;
			pBox->vMax = pBoxInfo->vMax;

			pBox->fStartTime = pBoxInfo->fStartTime;
			pBox->fEndTime = pBoxInfo->fEndTime;

			//// 박스의 버퍼 생성
			if( FAILED( (*m_pGraphicDev).CreateVertexBuffer(sizeof(Engine::VTXCUBE) * 8, 0, Engine::VTXFVF_CUBE, D3DPOOL_MANAGED, &pBox->pVB, 0 ) ) )
			{
				MSG_BOX("LoadBox pVB Create Failed");
				return;
			}
			if( FAILED( (*m_pGraphicDev).CreateIndexBuffer(sizeof(Engine::INDEX16) * 12, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &pBox->pIB, NULL)))
			{
				MSG_BOX("LoadBox pVB Create Failed");
				return;
			}

			//// 버텍스 버퍼 설정
			Engine::VTXCUBE*			pVtxCube = NULL; 
			pBox->pVB->Lock(0, 0, (void**)&pVtxCube, 0);

			_vec3 vMin = pBox->vMin;
			_vec3 vMax = pBox->vMax;

			pVtxCube[0].vPos = D3DXVECTOR3(vMin.x, vMax.y, vMin.z);
			pVtxCube[0].vTexUV = pVtxCube[0].vPos;		

			pVtxCube[1].vPos = D3DXVECTOR3(vMax.x, vMax.y, vMin.z);
			pVtxCube[1].vTexUV = pVtxCube[1].vPos;	 

			pVtxCube[2].vPos = D3DXVECTOR3(vMax.x, vMin.y, vMin.z);
			pVtxCube[2].vTexUV = pVtxCube[2].vPos;	

			pVtxCube[3].vPos = D3DXVECTOR3(vMin.x, vMin.y, vMin.z);
			pVtxCube[3].vTexUV = pVtxCube[3].vPos;	

			pVtxCube[4].vPos = D3DXVECTOR3(vMin.x, vMax.y, vMax.z);
			pVtxCube[4].vTexUV = pVtxCube[4].vPos;

			pVtxCube[5].vPos = D3DXVECTOR3(vMax.x, vMax.y, vMax.z);
			pVtxCube[5].vTexUV = pVtxCube[5].vPos;	

			pVtxCube[6].vPos = D3DXVECTOR3(vMax.x, vMin.y, vMax.z);
			pVtxCube[6].vTexUV = pVtxCube[6].vPos;	

			pVtxCube[7].vPos = D3DXVECTOR3(vMin.x, vMin.y, vMax.z);
			pVtxCube[7].vTexUV = pVtxCube[7].vPos;

			pBox->pVB->Unlock();

			//// 인덱스 버퍼 설정
			Engine::INDEX16*			pIndex = NULL;
			pBox->pIB->Lock(0, 0, (void**)&pIndex, 0);

			//+x
			pIndex[0]._1 = 1;	pIndex[0]._2 = 5;	pIndex[0]._3 = 6;
			pIndex[1]._1 = 1;	pIndex[1]._2 = 6;	pIndex[1]._3 = 2;
			//-x
			pIndex[2]._1 = 4;	pIndex[2]._2 = 0;	pIndex[2]._3 = 3;
			pIndex[3]._1 = 4;	pIndex[3]._2 = 3;	pIndex[3]._3 = 7;
			//+y
			pIndex[4]._1 = 4;	pIndex[4]._2 = 5;	pIndex[4]._3 = 1;
			pIndex[5]._1 = 4;	pIndex[5]._2 = 1;	pIndex[5]._3 = 0;
			//-y
			pIndex[6]._1 = 3;	pIndex[6]._2 = 2;	pIndex[6]._3 = 6;
			pIndex[7]._1 = 3;	pIndex[7]._2 = 6;	pIndex[7]._3 = 7;
			//+z
			pIndex[8]._1 = 7;	pIndex[8]._2 = 6;	pIndex[8]._3 = 5;
			pIndex[9]._1 = 7;	pIndex[9]._2 = 5;	pIndex[9]._3 = 4;
			//-z
			pIndex[10]._1 = 0;	pIndex[10]._2 = 1;	pIndex[10]._3 = 2;
			pIndex[11]._1 = 0;	pIndex[11]._2 = 2;	pIndex[11]._3 = 3;

			pBox->pIB->Unlock();

			if( dwByte == 0 )
			{
				Engine::Safe_Delete(pBox);
				break;
			}
			Engine::Safe_Delete(pBoxInfo);

			vecBoxInfo.push_back(pBox);
		}
		m_mapAttackBox.insert(map<int, vector<CLIENTBOXINFO*>>::value_type(iAniIndex, vecBoxInfo));
	}
	CloseHandle(hFile);
	Engine::Safe_Delete(m_pSphere);
}

void CMonster::Load_DamageBox(TCHAR* pBoundingBoxPath)
{
	// Load Bounding Sphere & Box
	m_pSphere = new SPHEREINFO;

	HANDLE	hFile = CreateFile(pBoundingBoxPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD	dwByte;
	int iAnimationCnt = 0;	int iAniIndex = 0;	int iBoxCnt = 0;

	ZeroMemory(m_pSphere, sizeof(SPHEREINFO));

	ReadFile( hFile, m_pSphere, sizeof(SPHEREINFO), &dwByte, NULL );
	ReadFile( hFile, &iAnimationCnt, sizeof(int), &dwByte, NULL );

	for( int i = 0; i < iAnimationCnt; ++i )
	{
		ReadFile( hFile, &iAniIndex, sizeof(int), &dwByte, NULL );
		ReadFile( hFile, &iBoxCnt, sizeof(int), &dwByte, NULL );

		for( int j = 0; j < iBoxCnt; ++j )
		{ 
			BOXINFO*	pBoxInfo = new BOXINFO;
			CLIENTBOXINFO*	pBox = new CLIENTBOXINFO;
			ReadFile( hFile, (pBoxInfo), sizeof(BOXINFO), &dwByte, NULL );

			pBox->eBoxTag = pBoxInfo->eBoxTag;
			pBox->iAniIdx = pBoxInfo->iAniIdx;

			WideCharToMultiByte(CP_ACP, 0, pBoxInfo->pFrameName, 128, pBox->pFrameName, 128, NULL, NULL);

			pBox->vMin = pBoxInfo->vMin;
			pBox->vMax = pBoxInfo->vMax;

			pBox->fStartTime = pBoxInfo->fStartTime;
			pBox->fEndTime = pBoxInfo->fEndTime;

			//// 박스의 버퍼 생성
			if( FAILED( (*m_pGraphicDev).CreateVertexBuffer(sizeof(Engine::VTXCUBE) * 8, 0, Engine::VTXFVF_CUBE, D3DPOOL_MANAGED, &pBox->pVB, 0 ) ) )
			{
				MSG_BOX("LoadBox pVB Create Failed");
				return;
			}
			if( FAILED( (*m_pGraphicDev).CreateIndexBuffer(sizeof(Engine::INDEX16) * 12, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &pBox->pIB, NULL)))
			{
				MSG_BOX("LoadBox pVB Create Failed");
				return;
			}

			//// 버텍스 버퍼 설정
			Engine::VTXCUBE*			pVtxCube = NULL; 
			pBox->pVB->Lock(0, 0, (void**)&pVtxCube, 0);

			_vec3 vMin = pBox->vMin;
			_vec3 vMax = pBox->vMax;

			pVtxCube[0].vPos = D3DXVECTOR3(vMin.x, vMax.y, vMin.z);
			pVtxCube[0].vTexUV = pVtxCube[0].vPos;		

			pVtxCube[1].vPos = D3DXVECTOR3(vMax.x, vMax.y, vMin.z);
			pVtxCube[1].vTexUV = pVtxCube[1].vPos;	 

			pVtxCube[2].vPos = D3DXVECTOR3(vMax.x, vMin.y, vMin.z);
			pVtxCube[2].vTexUV = pVtxCube[2].vPos;	

			pVtxCube[3].vPos = D3DXVECTOR3(vMin.x, vMin.y, vMin.z);
			pVtxCube[3].vTexUV = pVtxCube[3].vPos;	

			pVtxCube[4].vPos = D3DXVECTOR3(vMin.x, vMax.y, vMax.z);
			pVtxCube[4].vTexUV = pVtxCube[4].vPos;

			pVtxCube[5].vPos = D3DXVECTOR3(vMax.x, vMax.y, vMax.z);
			pVtxCube[5].vTexUV = pVtxCube[5].vPos;	

			pVtxCube[6].vPos = D3DXVECTOR3(vMax.x, vMin.y, vMax.z);
			pVtxCube[6].vTexUV = pVtxCube[6].vPos;	

			pVtxCube[7].vPos = D3DXVECTOR3(vMin.x, vMin.y, vMax.z);
			pVtxCube[7].vTexUV = pVtxCube[7].vPos;

			pBox->pVB->Unlock();

			//// 인덱스 버퍼 설정
			Engine::INDEX16*			pIndex = NULL;
			pBox->pIB->Lock(0, 0, (void**)&pIndex, 0);

			//+x
			pIndex[0]._1 = 1;	pIndex[0]._2 = 5;	pIndex[0]._3 = 6;
			pIndex[1]._1 = 1;	pIndex[1]._2 = 6;	pIndex[1]._3 = 2;
			//-x
			pIndex[2]._1 = 4;	pIndex[2]._2 = 0;	pIndex[2]._3 = 3;
			pIndex[3]._1 = 4;	pIndex[3]._2 = 3;	pIndex[3]._3 = 7;
			//+y
			pIndex[4]._1 = 4;	pIndex[4]._2 = 5;	pIndex[4]._3 = 1;
			pIndex[5]._1 = 4;	pIndex[5]._2 = 1;	pIndex[5]._3 = 0;
			//-y
			pIndex[6]._1 = 3;	pIndex[6]._2 = 2;	pIndex[6]._3 = 6;
			pIndex[7]._1 = 3;	pIndex[7]._2 = 6;	pIndex[7]._3 = 7;
			//+z
			pIndex[8]._1 = 7;	pIndex[8]._2 = 6;	pIndex[8]._3 = 5;
			pIndex[9]._1 = 7;	pIndex[9]._2 = 5;	pIndex[9]._3 = 4;
			//-z
			pIndex[10]._1 = 0;	pIndex[10]._2 = 1;	pIndex[10]._3 = 2;
			pIndex[11]._1 = 0;	pIndex[11]._2 = 2;	pIndex[11]._3 = 3;

			pBox->pIB->Unlock();

			if( dwByte == 0 )
			{
				Engine::Safe_Delete(pBox);
				break;
			}
			Engine::Safe_Delete(pBoxInfo);

			m_vecDamageBox.push_back(pBox);
		}
	}
	CloseHandle(hFile);
}

void CMonster::Load_FootAnimationSync(TCHAR* pSyncPath)
{
	HANDLE	hFile = CreateFile(pSyncPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD	dwByte;
	int iAnimationCnt = 0;	int iAniIndex = 0;	int iListCnt = 0;

	ReadFile( hFile, &iAnimationCnt, sizeof(int), &dwByte, NULL );

	for( int i = 0; i < iAnimationCnt; ++i )
	{
		ReadFile( hFile, &iAniIndex, sizeof(int), &dwByte, NULL );

		vector<FOOTANISYNC*>	vecPart;	// 읽어온 세트들 담을 리스트
		float					fStartTime, fEndTime;
		bool					bLeft, bFront, bCompulsion;

		ReadFile( hFile, &iListCnt, sizeof(int), &dwByte, NULL );	// 애니메이션의 리스트 개수 읽고

		for( int j = 0; j < iListCnt; ++j )
		{
			FOOTANISYNC* pAniSync = new FOOTANISYNC;
			ReadFile( hFile, &fStartTime, sizeof(float), &dwByte, NULL );	// 시작 시간 읽고
			pAniSync->fStartTime = fStartTime;
			ReadFile( hFile, &fEndTime, sizeof(float), &dwByte, NULL );	// 끝 시간 읽고
			pAniSync->fEndTime = fEndTime;
			ReadFile( hFile, &bLeft, sizeof(bool), &dwByte, NULL );	// 속도 읽고
			pAniSync->bLeftFoot = bLeft;
			ReadFile( hFile, &bFront, sizeof(bool), &dwByte, NULL );	// 속도 읽고
			pAniSync->bFront = bFront;
			ReadFile( hFile, &bCompulsion, sizeof(bool), &dwByte, NULL );	// 속도 읽고
			pAniSync->bCompulsion = bCompulsion;

			vecPart.push_back(pAniSync);	
		}
		m_mapAniSync.insert(map<int, vector<FOOTANISYNC*>>::value_type( iAniIndex, vecPart ));
	}

	CloseHandle(hFile);
}

void CMonster::CollisionCheckFunction(void)
{
	m_bPlayerDistanceCheck					= false;
	m_bPlayerAttack_Body_CollisionCheck		= false;
	m_bPlayerEffect_Body_CollisionCheck		= false;
	m_bPlayerAttack_MyAttack_CollisionCheck	= false;

	PlayerDistanceCheck();

	if(m_bPlayerDistanceCheck == true)
	{
		PlayerAttackBox_And_MyBodyBox();
		PlayerEffectBox_And_MyBodyBox();
		PlayerAttackBox_And_MyAttackBox();
	}
}

void CMonster::PlayerDistanceCheck(void)
{
	_vec3 vPlayer	= m_pPlayer->Get_TransCom()->m_vPosition;
	_vec3 vMonster	= m_pTransCom->m_vPosition;

	if(D3DXVec3Length(&(vPlayer - vMonster)) < PlayerCollisionDistance)
	{
		m_bPlayerDistanceCheck = true;
	}
}

void CMonster::PlayerAttackBox_And_MyBodyBox(void)
{
	//여기에 플레이어공격 스피어와 나의 몸의 스피어 충돌을 먼저 검사하는 코드를 짤까?
	if(m_pPlayer->Get_ClientBoxInfo() != NULL)	//플레이어의 공격 박스가 존재 한다면
	{
		//플레이어의 공격박스가 Hit가 가능한 상태라면 참 아니면 거짓
		if( m_pPlayer->Get_Check_EndPeriodMulTime_Start() == false )
			return;

		if( m_pPlayer->Get_Check_EndPeriodMulTime_End() == true )
			return;

		//플레이어의 검을 이용하여 검사하는 충돌체크
		if(m_pPlayer->Get_PlayerState() == CPlayer::PS_BasicAttack
			|| m_pPlayer->Get_PlayerState() == CPlayer::PS_SmashAttack
			|| m_pPlayer->Get_PlayerState() == CPlayer::PS_SkillAttack)
		{
			//플레이어의 박스와 몬스터의 구가 충돌 했으면 true => Collision_AABB_Sphere => (사실 월드 상에서의 회전을 생각하여 만든 최대 AABB)
			if(Engine::CCollision::GetInstance()->Collision_AABB_Sphere(_vec3(0.f, -7.f, -5.f), _vec3(100.f, 7.f, 5.f), (*m_pPlayer->GetpAttachment_RH_Matrix() * m_pPlayer->Get_TransCom()->m_matWorld), m_pTransCom->m_vPosition, m_fRadius))
			{
				vector<CLIENTBOXINFO*>::iterator iter		= m_vecDamageBox.begin();
				vector<CLIENTBOXINFO*>::iterator iterEnd	= m_vecDamageBox.end();

				//플레이어의 검과 몬스터의 피격 박스 OBB - OBB를 검사한다.
				for( ; iter != iterEnd; ++iter)
				{
					const _matrix* pMonstrBox = m_pMeshCom->Get_FrameMatrix((*iter)->pFrameName);

					if(Engine::CCollision::GetInstance()->Collision_OBB_OBB(_vec3(0.f, -7.f, -5.f), _vec3(100.f, 7.f, 5.f), (*m_pPlayer->GetpAttachment_RH_Matrix() * m_pPlayer->Get_TransCom()->m_matWorld),(*iter)->vMin, (*iter)->vMax, (*pMonstrBox * m_pTransCom->m_matWorld)))
					{
						CMathMgr::GetInstance()->DamageDirectionDecision(m_pPlayer->Get_TransCom()->m_vPosition, m_pTransCom);
						//데미지 폰트를 좀더 랜덤하게 좌우로 띄어 자연스러운 효과를 준다.
						m_vCollisionPos = ((*iter)->vMin + (*iter)->vMax) / 2.f;
						D3DXVec3TransformCoord(&m_vCollisionPos, &m_vCollisionPos, &(*pMonstrBox * m_pTransCom->m_matWorld));
						float fGapX = m_vCollisionPos.x - m_pTransCom->m_vPosition.x;
						if(fGapX > 0.1f)
							m_vCollisionPos.x += 10.f + rand()%50;
						else if(fGapX < -0.1f)
							m_vCollisionPos.x -= 10.f + rand()%50;
						else
							m_vCollisionPos.x = m_vCollisionPos.x;

						m_bPlayerAttack_Body_CollisionCheck = true;

#pragma region 각도 예외처리
						m_fAngleX = 0.f;
						if(m_pPlayer->Get_MeshCom()->Get_CurrentAniIdx() == Player::battle_attack_01_a)
						{
							m_fAngleZ = _float(70 + rand()%40);
						}
						else if(m_pPlayer->Get_MeshCom()->Get_CurrentAniIdx() == Player::battle_attack_01_b)
						{
							m_fAngleZ = _float(80 + rand()%20);
						}
						else if(m_pPlayer->Get_MeshCom()->Get_CurrentAniIdx() == Player::battle_attack_02)
						{
							m_fAngleZ = _float(120 + rand()%15);
						}
						else if(m_pPlayer->Get_MeshCom()->Get_CurrentAniIdx() == Player::battle_attack_03)
						{
							m_fAngleZ = _float(60 + rand()%15);
						}
						else if(m_pPlayer->Get_MeshCom()->Get_CurrentAniIdx() == Player::battle_attack_04)
						{
							m_fAngleZ = _float(-15.f + rand()%15);
						}
						else if(m_pPlayer->Get_MeshCom()->Get_CurrentAniIdx() == Player::battle_attack_strong_01)
						{
							m_fAngleZ = _float(-45 - rand()%20);
						}
						else if(m_pPlayer->Get_MeshCom()->Get_CurrentAniIdx() == Player::battle_attack_strong_02_a)
						{
							m_fAngleX = _float(80 + rand()%20);
							m_fAngleZ = _float(-5 + rand()%5);
						}
						else if(m_pPlayer->Get_MeshCom()->Get_CurrentAniIdx() == Player::battle_attack_strong_03)
						{
							m_fAngleZ = _float(-80 - rand()%20);
						}
						else if(m_pPlayer->Get_MeshCom()->Get_CurrentAniIdx() == Player::battle_attack_strong_04_a)
						{
							m_fAngleZ = _float(-150 - rand()%15);
						}
						else if(m_pPlayer->Get_MeshCom()->Get_CurrentAniIdx() == Player::battle_attack_strong_04_b)
						{
							m_fAngleZ = _float(-150 - rand()%15);
						}
						else if(m_pPlayer->Get_MeshCom()->Get_CurrentAniIdx() == Player::battle_attack_strong_run_sprint)
						{
							m_fAngleX = _float(80 + rand()%20);
							m_fAngleZ = _float(-5 + rand()%5);
						}
						else if(m_pPlayer->Get_MeshCom()->Get_CurrentAniIdx() == Player::SP_attack_charge_slash_up_l)
						{
							m_fAngleZ = _float(10 + rand()%10);
						}
						else if(m_pPlayer->Get_MeshCom()->Get_CurrentAniIdx() == Player::SP_attack_charge_slash_up_r)
						{
							m_fAngleZ = _float(-10 - rand()%10);
						}
						else if(m_pPlayer->Get_MeshCom()->Get_CurrentAniIdx() == Player::SP_attack_execution)
						{
							m_fAngleZ = _float(-15.f + rand()%15);
						}
						else if(m_pPlayer->Get_MeshCom()->Get_CurrentAniIdx() == Player::SP_attack_hell)
						{
							m_fAngleZ = float(rand()%360);
						}
						else if(m_pPlayer->Get_MeshCom()->Get_CurrentAniIdx() == Player::SP_attack_judgement_l)
						{
							m_fAngleZ = 80.f + rand()%20;
						}
						else if(m_pPlayer->Get_MeshCom()->Get_CurrentAniIdx() == Player::SP_attack_judgement_r)
						{
							m_fAngleZ = 80.f + rand()%20;
						}
						else
							m_fAngleZ = 80.f + rand()%20;
#pragma endregion
					}
				}

			}
		}
		else if(m_pPlayer->Get_PlayerState() == CPlayer::PS_BreakAttack //플레이어의 공격 박스를 이용하여 충돌처리해야하는 부분
			|| m_pPlayer->Get_PlayerState() == CPlayer::PS_SmashAttack_0)
		{
			//플레이어의 공격 박스와 몬스터의 구가 충돌 했으면 true
			//플레이어의 공격 박스는 애초에 하나라는 전제하에 파싱을 하고 있으니 이렇게 짠다.
			CLIENTBOXINFO*	pPlayerAttackBoxInfo = m_pPlayer->Get_ClientBoxInfo();
			
			const _matrix* pPlayerBox = m_pPlayer->Get_MeshCom()->Get_FrameMatrix(pPlayerAttackBoxInfo->pFrameName);
			
			//공격박스의 AABB(사실 월드 상에서의 회전을 생각하여 만든 최대 AABB)와 스피어의 충돌을 체크한다.
			if(Engine::CCollision::GetInstance()->Collision_AABB_Sphere(pPlayerAttackBoxInfo->vMin, pPlayerAttackBoxInfo->vMax, (*pPlayerBox * m_pPlayer->Get_TransCom()->m_matWorld), m_pTransCom->m_vPosition, m_fRadius))
			{
				vector<CLIENTBOXINFO*>::iterator iter		= m_vecDamageBox.begin();
				vector<CLIENTBOXINFO*>::iterator iterEnd	= m_vecDamageBox.end();

				//플레이어의 검과 몬스터의 피격 박스 OBB - OBB를 검사한다.
				for( ; iter != iterEnd; ++iter)
				{
					const _matrix* pMonstrBox = m_pMeshCom->Get_FrameMatrix((*iter)->pFrameName);

					if(Engine::CCollision::GetInstance()->Collision_OBB_OBB(pPlayerAttackBoxInfo->vMin, pPlayerAttackBoxInfo->vMax, (*m_pPlayer->GetpAttachment_RH_Matrix() * m_pPlayer->Get_TransCom()->m_matWorld),(*iter)->vMin, (*iter)->vMax, (*pMonstrBox * m_pTransCom->m_matWorld)))
					{
						CMathMgr::GetInstance()->DamageDirectionDecision(m_pPlayer->Get_TransCom()->m_vPosition, m_pTransCom);
						
						//데미지 폰트를 좀더 랜덤하게 좌우로 띄어 자연스러운 효과를 준다.
						m_vCollisionPos = ((*iter)->vMin + (*iter)->vMax) / 2.f;
						D3DXVec3TransformCoord(&m_vCollisionPos, &m_vCollisionPos, &(*pMonstrBox * m_pTransCom->m_matWorld));
						float fGapX = m_vCollisionPos.x - m_pTransCom->m_vPosition.x;
						if(fGapX > 0.1f)
							m_vCollisionPos.x += 10.f + rand()%50;
						else if(fGapX < -0.1f)
							m_vCollisionPos.x -= 10.f + rand()%50;
						else
							m_vCollisionPos.x = m_vCollisionPos.x; 

						m_bPlayerAttack_Body_CollisionCheck = true;
					}
				}
			}
		}
	}
}

void CMonster::PlayerEffectBox_And_MyBodyBox(void)
{

}

void CMonster::PlayerAttackBox_And_MyAttackBox(void)
{
	if( (m_pPlayer->Get_PlayerState() == CPlayer::PS_BasicAttack || m_pPlayer->Get_PlayerState() == CPlayer::PS_BreakAttack || m_pPlayer->Get_PlayerState() == CPlayer::PS_SmashAttack || m_pPlayer->Get_PlayerState() == CPlayer::PS_SkillAttack)
		&& m_pPlayer->Get_ClientBoxInfo() != NULL)	//플레이어의 공격 박스가 존재 한다면
	{
		//플레이어의 어택 박스가 가능한 시간 인지 체크
		//플레이어의 공격박스가 Hit가 가능한 상태라면 참 아니면 거짓
		if( m_pPlayer->Get_Check_EndPeriodMulTime_Start() == false )
			return;

		if( m_pPlayer->Get_Check_EndPeriodMulTime_End() == true )
			return;

		//플레이어의 공격 스피어와 몬스터의 구가 충돌 했으면 true => 추후 플레이어 검의 스피어와 몬스터 검의 스피어로 바꾸어 볼까?
		if(Engine::CCollision::GetInstance()->Collision_AABB_Sphere(_vec3(0.f, -7.f, -5.f), _vec3(100.f, 7.f, 5.f), (*m_pPlayer->GetpAttachment_RH_Matrix() * m_pPlayer->Get_TransCom()->m_matWorld), m_pTransCom->m_vPosition, m_fRadius))
		{
			//추후에 타입으로 하여 해쉬맵에서 find하는 연산을 없애 최적화를 한다.
			//현재 몬스터 애니메이션의 공격 박스가 있는지 찾는다.
			boost::unordered_map<int, vector<CLIENTBOXINFO*>>::iterator MonsterAttackiter = m_mapAttackBox.find(m_iMonsterAnimationIndex);
			if(m_mapAttackBox.end() != MonsterAttackiter)	//공격 박스가 있으면 충돌을 검사한다.
			{
				//몬스터의 어택 박스가 가능한 시간 인지 체크
				vector<CLIENTBOXINFO*>::iterator Attackiter		= MonsterAttackiter->second.begin();
				vector<CLIENTBOXINFO*>::iterator AttackiterEnd	= MonsterAttackiter->second.end();
				
				for(; Attackiter != AttackiterEnd; ++Attackiter)
				{
					if( m_pMeshCom->Check_EndPeriodMulTime((*Attackiter)->fStartTime) == false)
						return;

					if( m_pMeshCom->Check_EndPeriodMulTime((*Attackiter)->fEndTime) == true)
						return;

					//여기서 일단 바로 OBB - OBB 검사를 하지만 추후 충돌박스들을 구로 바꾸면 지금이랑은 다른 구조로 최적화를 해야 할거 같다.
					const _matrix pMonsterAttackBoxMatrix = *m_pMeshCom->Get_FrameMatrix((*Attackiter)->pFrameName) * m_pTransCom->m_matWorld;

					if(Engine::CCollision::GetInstance()->Collision_OBB_OBB(_vec3(0.f, -7.f, -5.f), _vec3(100.f, 7.f, 5.f), (*m_pPlayer->GetpAttachment_RH_Matrix() * m_pPlayer->Get_TransCom()->m_matWorld)
						, (*Attackiter)->vMin, (*Attackiter)->vMax, pMonsterAttackBoxMatrix))
					{
						m_bPlayerAttack_MyAttack_CollisionCheck = true;
						//printf("Attack vs Attack here");
					}
				}


			}
		}
	}
}

bool CMonster::ObjectBox_And_MyMoveBox(void)
{
	list<CStaticObject*>::iterator iter		= CCollisionMgr::GetInstance()->Get_StaticObjectList()->begin();
	list<CStaticObject*>::iterator iterEnd	= CCollisionMgr::GetInstance()->Get_StaticObjectList()->end();

	for( ; iter != iterEnd; ++iter )
	{
		//Sphere - Sphere 충돌이 되면 서로 가까이 있다는 말이다.
		if(Engine::CCollision::GetInstance()->Collision_Sphere_Sphere((*iter)->Get_WorldCenter(), (*iter)->Get_WorldRadius(), m_pTransCom->m_vPosition, m_fMoveRadius))
		{
			if(Engine::CCollision::GetInstance()->Collision_OBB_WorldSphere_SlidingVector((*iter)->Get_LocalMin(), (*iter)->Get_LocalMax(), (*iter)->Get_TransCom()->m_matWorld, (*iter)->Get_TransCom()->m_vScale, m_pTransCom->m_vPosition, m_fMoveRadius, m_vOriginalPos, &m_pTransCom->m_vPosition))
			{
				//충돌시 해당 오브벡트의 충돌 면을 알아내어 슬라이딩 벡터를 고 구현해보자.
				return true;
			}

			/*if((*iter)->GetObjectRadianY() == D3DXToRadian(0) || (*iter)->GetObjectRadianY() == D3DXToRadian(90))
			{
				if(Engine::CCollision::GetInstance()->Collision_WorldAABB_WorldSphere((*iter)->Get_WorldMin(), (*iter)->Get_WorldMax(), m_pTransCom->m_vPosition, m_fMoveRadius))
				{
					return true;
				}
			}
			else
			{
				if(Engine::CCollision::GetInstance()->Collision_OBB_WorldSphere((*iter)->Get_LocalMin(), (*iter)->Get_LocalMax(), (*iter)->Get_TransCom()->m_matWorld, (*iter)->Get_TransCom()->m_vScale, m_pTransCom->m_vPosition, m_fMoveRadius))
					return true;
			}*/
		}
	}

	return false;
}

bool CMonster::CDynamicMesh_And_MyMoveBox(void)
{
	if(Engine::CCollision::GetInstance()->Collision_Sphere_Sphere(m_pPlayer->Get_TransCom()->m_vPosition, m_pPlayer->Get_MoveRadius(), m_pTransCom->m_vPosition, m_fMoveRadius))
		return true;

	list<CMonster*>::iterator iter		= CCollisionMgr::GetInstance()->Get_MonsterList()->begin();
	list<CMonster*>::iterator iterEnd	= CCollisionMgr::GetInstance()->Get_MonsterList()->end();

	for( ; iter != iterEnd; ++iter )
	{
		//충돌할건지 여부를 검사한다.
		if(false == (*iter)->Get_MoveCollCheck()) 
		{
			continue;
		}
		else
		{
			//Sphere - Sphere 충돌이 되면 서로 가까이 있다는 말이다.
			if(Engine::CCollision::GetInstance()->Collision_Sphere_Sphere((*iter)->Get_TransCom()->m_vPosition, (*iter)->Get_MoveRadius(), m_pTransCom->m_vPosition, m_fMoveRadius))
			{
				if((*iter) == this)
					continue;
				return true;
			}
		}
		
	}

	return false;
}

void CMonster::Initialize_Min_Max_Radius(void)
{
	Engine::CGameObject::Update(0.f);

	_vec3 vMin;
	_vec3 vMax;

	m_pMeshCom->Get_MinMax(&vMin, &vMax);
	m_vCenter = (vMin + vMax) / 2.f;	//센터 값은 로컬 좌표계로 구하고

	_matrix		matScale;
	D3DXMatrixScaling(&matScale, m_pTransCom->m_vScale.x, m_pTransCom->m_vScale.y, m_pTransCom->m_vScale.z);

	D3DXVec3TransformCoord(&vMin, &vMin, &matScale);
	D3DXVec3TransformCoord(&vMax, &vMax, &matScale);

	m_fRadius = D3DXVec3Length(&(vMax - vMin)) / 2.f; //반지름은 스케일 값을 생각해서 구한다.
}

void CMonster::Update_Monster_AnimationIndex()
{
	if( m_pMeshCom->Get_CurrentAniIdx() != m_iMonsterAnimationIndex )
		m_iMonsterAnimationIndex = m_pMeshCom->Get_CurrentAniIdx();
}

void CMonster::SoundEffectPlay(SOUND::EFF eSoundType, _float fCheckEndPeriodMulTime, _int iboolNumber, _float fVolume /*= 1.f*/)
{
	if(!m_bSoundPlay[iboolNumber] && m_pMeshCom->Check_EndPeriodMulTime(fCheckEndPeriodMulTime))
	{
		m_bSoundPlay[iboolNumber] = true;
		CSoundMgr::GetInstance()->PlayEffect(eSoundType, m_pTransCom->m_vPosition, fVolume);
	}
}
void CMonster::Play_Effect(TCHAR* pName, _float fCheckEndPeriodMulTime, const _matrix* pMatrix, _vec3 vLocalPos, _int iboolNumber, _bool bReStart, _bool bHDR, _vec3 vSize, _vec3 vAngle)
{
	if(!m_bCreateEffect[iboolNumber] && m_pMeshCom->Check_EndPeriodMulTime(fCheckEndPeriodMulTime))
	{
		_vec3 vCreateEffect(0.f, 0.f, 0.f);
		_matrix vCreateMatrix = *pMatrix * m_pTransCom->m_matWorld;
		D3DXVec3TransformCoord(&vCreateEffect, &vLocalPos, &vCreateMatrix);
		CEffectMgr::GetInstance()->Play_Effect(pName, vCreateEffect, bReStart, bHDR, vSize, vAngle);
		m_bCreateEffect[iboolNumber] = true;
	}
}
_ulong CMonster::Release(void)
{
	_ulong dwRefCnt = CGameObject::Release();

	if(0 == dwRefCnt)
	{	
		/////// 바운딩 스피어 & 박스 로드한 것 삭제
		Engine::Safe_Delete(m_pSphere);

		boost::unordered_map<int, vector<CLIENTBOXINFO*>>::iterator iter = m_mapAttackBox.begin();
		boost::unordered_map<int, vector<CLIENTBOXINFO*>>::iterator iter_end = m_mapAttackBox.end();

		for( ; iter != iter_end; )
		{
			vector<CLIENTBOXINFO*>::iterator iter_box = iter->second.begin();
			vector<CLIENTBOXINFO*>::iterator iter_box_end = iter->second.end();

			for( ; iter_box != iter_box_end; )
			{
				Engine::Safe_Release( (*iter_box)->pVB );
				Engine::Safe_Release( (*iter_box)->pIB );
				Engine::Safe_Delete( (*iter_box) );
				++iter_box;
			}
			iter->second.clear();
			iter = m_mapAttackBox.erase(iter);

			if( iter == iter_end )
				m_mapAttackBox.clear();
		}

		//////////////////////////////////////////////////////////////////////////
		vector<CLIENTBOXINFO*>::iterator iter_Vec = m_vecDamageBox.begin();
		vector<CLIENTBOXINFO*>::iterator iter_Vec_end = m_vecDamageBox.end();

		for( ; iter_Vec != iter_Vec_end; )
		{
			Engine::Safe_Release( (*iter_Vec)->pVB );
			Engine::Safe_Release( (*iter_Vec)->pIB );
			Engine::Safe_Delete( (*iter_Vec) );
			++iter_Vec;
		}
		if( iter_Vec == iter_Vec_end )
			m_vecDamageBox.clear();

	}

	return 0;
}

#ifdef _DEBUG
void CMonster::Render_AttackBox()
{
	boost::unordered_map<int, vector<CLIENTBOXINFO*>>::iterator iter_map = m_mapAttackBox.find(m_iMonsterAnimationIndex);	// 애니메이션 인덱스 넣어서 찾으면 됨.

	if( iter_map == m_mapAttackBox.end() )
		return;

	vector<CLIENTBOXINFO*>::iterator iter = iter_map->second.begin();
	vector<CLIENTBOXINFO*>::iterator iter_end = iter_map->second.end();

	// 항등행렬로 월드행렬을 설정해준다.
	_matrix matDefaultWorld;
	D3DXMatrixIdentity(&matDefaultWorld);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matDefaultWorld);

	for( ; iter != iter_end; ++iter )
	{
		if( m_pMeshCom->Check_EndPeriodMulTime((*iter)->fStartTime) == false )
			continue;

		if( m_pMeshCom->Check_EndPeriodMulTime((*iter)->fEndTime) == true )
			continue;

		const _matrix* m_pMatrix;
		m_pMatrix = m_pMeshCom->Get_FrameMatrix((*iter)->pFrameName);	// 선택한 뼈의 행렬을 얻어온다.

		// 바운딩 박스의 행렬 = 뼈의 행렬 * 메쉬의 행렬 
		_matrix matWorld;
		matWorld = (*m_pMatrix) * m_pTransCom->m_matWorld ;

		Engine::VTXCUBE*			pVtxCube = NULL; 
		(*iter)->pVB->Lock(0, 0, (void**)&pVtxCube, 0);

		_vec3 _vMin = (*iter)->vMin;
		_vec3 _vMax = (*iter)->vMax;

		pVtxCube[0].vPos = D3DXVECTOR3(_vMin.x, _vMax.y, _vMin.z);
		pVtxCube[0].vTexUV = pVtxCube[0].vPos;		

		pVtxCube[1].vPos = D3DXVECTOR3(_vMax.x, _vMax.y, _vMin.z);
		pVtxCube[1].vTexUV = pVtxCube[1].vPos;		

		pVtxCube[2].vPos = D3DXVECTOR3(_vMax.x, _vMin.y, _vMin.z);
		pVtxCube[2].vTexUV = pVtxCube[2].vPos;	

		pVtxCube[3].vPos = D3DXVECTOR3(_vMin.x, _vMin.y, _vMin.z);
		pVtxCube[3].vTexUV = pVtxCube[3].vPos;	

		pVtxCube[4].vPos = D3DXVECTOR3(_vMin.x, _vMax.y, _vMax.z);
		pVtxCube[4].vTexUV = pVtxCube[4].vPos;

		pVtxCube[5].vPos = D3DXVECTOR3(_vMax.x, _vMax.y, _vMax.z);
		pVtxCube[5].vTexUV = pVtxCube[5].vPos;	

		pVtxCube[6].vPos = D3DXVECTOR3(_vMax.x, _vMin.y, _vMax.z);
		pVtxCube[6].vTexUV = pVtxCube[6].vPos;	

		pVtxCube[7].vPos = D3DXVECTOR3(_vMin.x, _vMin.y, _vMax.z);
		pVtxCube[7].vTexUV = pVtxCube[7].vPos;

		// 큐브의 8개의 정점을 뼈의 로컬행렬과 메쉬의 월드행렬을 곱해준 행렬로 이동 시켜준다.(월드영역으로 변환)
		for(int i = 0; i < 8; ++i)
		{
			D3DXVec3TransformCoord(&pVtxCube[i].vPos, &pVtxCube[i].vPos, &matWorld);
		}

		(*iter)->pVB->Unlock();

		m_pGraphicDev->SetStreamSource(0, (*iter)->pVB, 0, sizeof(Engine::VTXCUBE));
		m_pGraphicDev->SetFVF(Engine::VTXFVF_CUBE);
		m_pGraphicDev->SetIndices((*iter)->pIB);
		m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
	}

}

void CMonster::Render_DamageBox()
{
	vector<CLIENTBOXINFO*>::iterator iter = m_vecDamageBox.begin();
	vector<CLIENTBOXINFO*>::iterator iter_end = m_vecDamageBox.end();

	// 항등행렬로 월드행렬을 설정해준다.
	_matrix matDefaultWorld;
	D3DXMatrixIdentity(&matDefaultWorld);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matDefaultWorld);

	for( ; iter != iter_end; ++iter )
	{
		if( m_pMeshCom->Check_EndPeriodMulTime((*iter)->fStartTime) == false )
			continue;

		if( m_pMeshCom->Check_EndPeriodMulTime((*iter)->fEndTime) == true )
			continue;

		const _matrix* m_pMatrix;
		m_pMatrix = m_pMeshCom->Get_FrameMatrix((*iter)->pFrameName);	// 선택한 뼈의 행렬을 얻어온다.

		// 바운딩 박스의 행렬 = 뼈의 행렬 * 메쉬의 행렬 
		_matrix matWorld;
		matWorld = (*m_pMatrix) * m_pTransCom->m_matWorld ;

		Engine::VTXCUBE*			pVtxCube = NULL; 
		(*iter)->pVB->Lock(0, 0, (void**)&pVtxCube, 0);

		_vec3 _vMin = (*iter)->vMin;
		_vec3 _vMax = (*iter)->vMax;

		pVtxCube[0].vPos = D3DXVECTOR3(_vMin.x, _vMax.y, _vMin.z);
		pVtxCube[0].vTexUV = pVtxCube[0].vPos;		

		pVtxCube[1].vPos = D3DXVECTOR3(_vMax.x, _vMax.y, _vMin.z);
		pVtxCube[1].vTexUV = pVtxCube[1].vPos;		

		pVtxCube[2].vPos = D3DXVECTOR3(_vMax.x, _vMin.y, _vMin.z);
		pVtxCube[2].vTexUV = pVtxCube[2].vPos;	

		pVtxCube[3].vPos = D3DXVECTOR3(_vMin.x, _vMin.y, _vMin.z);
		pVtxCube[3].vTexUV = pVtxCube[3].vPos;	

		pVtxCube[4].vPos = D3DXVECTOR3(_vMin.x, _vMax.y, _vMax.z);
		pVtxCube[4].vTexUV = pVtxCube[4].vPos;

		pVtxCube[5].vPos = D3DXVECTOR3(_vMax.x, _vMax.y, _vMax.z);
		pVtxCube[5].vTexUV = pVtxCube[5].vPos;	

		pVtxCube[6].vPos = D3DXVECTOR3(_vMax.x, _vMin.y, _vMax.z);
		pVtxCube[6].vTexUV = pVtxCube[6].vPos;	

		pVtxCube[7].vPos = D3DXVECTOR3(_vMin.x, _vMin.y, _vMax.z);
		pVtxCube[7].vTexUV = pVtxCube[7].vPos;

		// 큐브의 8개의 정점을 뼈의 로컬행렬과 메쉬의 월드행렬을 곱해준 행렬로 이동 시켜준다.(월드영역으로 변환)
		for(int i = 0; i < 8; ++i)
		{
			D3DXVec3TransformCoord(&pVtxCube[i].vPos, &pVtxCube[i].vPos, &matWorld);
		}

		(*iter)->pVB->Unlock();

		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		m_pGraphicDev->SetStreamSource(0, (*iter)->pVB, 0, sizeof(Engine::VTXCUBE));
		m_pGraphicDev->SetFVF(Engine::VTXFVF_CUBE);
		m_pGraphicDev->SetIndices((*iter)->pIB);
		m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
	}
}

void CMonster::Render_BoudingSphere()
{
	D3DXCreateSphere(m_pGraphicDev, m_pSphere->fRadius, 20, 20, &m_pSphereMesh, &m_pSphereBuffer );
	
	_matrix World;
	D3DXMatrixIdentity(&World);
	D3DXMatrixTranslation(&World, 0.f, m_pSphere->vPos.y, 0.f);

	World = World * m_pTransCom->m_matWorld;

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &( World ) );
	m_pSphereMesh->DrawSubset(0);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	Engine::Safe_Release(m_pSphereMesh);
	Engine::Safe_Release(m_pSphereBuffer);

	//무브 스피어
	D3DXCreateSphere(m_pGraphicDev, m_fMoveRadius, 30, 15, &m_pSphereMesh, &m_pSphereBuffer );

	_matrix MoveWorld;
	D3DXMatrixIdentity(&MoveWorld);
	D3DXMatrixTranslation(&MoveWorld, m_pTransCom->m_vPosition.x, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z);

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &( MoveWorld ) );
	m_pSphereMesh->DrawSubset(0);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	Engine::Safe_Release(m_pSphereMesh);
	Engine::Safe_Release(m_pSphereBuffer);
}
#endif