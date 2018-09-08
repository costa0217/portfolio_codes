#include "stdafx.h"
#include "SelectCloth.h"

#include "Export_Function.h"

#include "Player.h"
#include "SelectDye.h"

#include "SoundMgr.h"

CSelectCloth::CSelectCloth(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_iSelectPart(C_END)
, m_iUpper(-1)
, m_iLower(-1)
, m_iHand(-1)
, m_iFoot(-1)
, m_iIndex(-1)
, m_iHair(-1)
, m_iSword(-1)
, m_bPlay(false)
, m_fClick(0.f)
, m_fClickOverTime(0.f)
{

}

CSelectCloth::~CSelectCloth(void)
{

}

HRESULT CSelectCloth::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_UI(Alpha)");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_fSizeX = 550.f;
	m_fSizeY = 550.f;	

	for( int i = 0; i < 6; ++i )
	{
		for( int j = 0; j < 3; ++j )
		{
			int fTop = 85 + (50*i);
			SetRect(&m_rcUI[i][j], 40 + ( 55 * j ) , fTop, 76 + ( 55 * j ) , fTop + 40);
		}
	}
	SetRect(&m_rcPoseUI[0], 40 + ( 55 * 3 ) , 85 + (50*0), 76 + ( 55 * 3 ) , 85 + (50*0) + 40);
	SetRect(&m_rcPoseUI[1], 40 + ( 55 * 3 ) , 85 + (50*1), 76 + ( 55 * 3 ) , 85 + (50*1) + 40);
	SetRect(&m_rcPoseUI[2], 40 + ( 55 * 3 ) , 85 + (50*2), 76 + ( 55 * 3 ) , 85 + (50*2) + 40);


	return S_OK;
}

HRESULT CSelectCloth::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Material Component
	D3DMATERIAL9			MtrlInfo;
	MtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlInfo.Ambient = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	MtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	pComponent = m_pMtrlCom = Engine::CMaterial::Create(MtrlInfo);
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Material", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCom = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_DYE, L"Buffer_SelectCloth");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_DYE, L"Texture_SelectCloth");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	Create_Icon();
	Set_IconColor();

	m_pHairMesh[0] = (Engine::CPlayerDynamicMesh*)Engine::Clone_Resource(RESOURCE_DYE, L"Mesh_Skeleton_Hair");
	m_pHairMesh[1] = (Engine::CPlayerDynamicMesh*)Engine::Clone_Resource(RESOURCE_DYE, L"Mesh_Heremon_Hair");
	m_pHairMesh[2] = (Engine::CPlayerDynamicMesh*)Engine::Clone_Resource(RESOURCE_DYE, L"Mesh_Pirate_Hair");

	m_pUpperMesh[0] = (Engine::CPlayerDynamicMesh*)Engine::Clone_Resource(RESOURCE_DYE, L"Mesh_Skeleton_Upper");
	m_pUpperMesh[1] = (Engine::CPlayerDynamicMesh*)Engine::Clone_Resource(RESOURCE_DYE, L"Mesh_Heremon_Upper");
	m_pUpperMesh[2] = (Engine::CPlayerDynamicMesh*)Engine::Clone_Resource(RESOURCE_DYE, L"Mesh_Pirate_Upper");

	m_pLowerMesh[0] = (Engine::CPlayerDynamicMesh*)Engine::Clone_Resource(RESOURCE_DYE, L"Mesh_Skeleton_Lower");
	m_pLowerMesh[1] = (Engine::CPlayerDynamicMesh*)Engine::Clone_Resource(RESOURCE_DYE, L"Mesh_Heremon_Lower");
	m_pLowerMesh[2] = (Engine::CPlayerDynamicMesh*)Engine::Clone_Resource(RESOURCE_DYE, L"Mesh_Pirate_Lower");

	m_pHandMesh[0] = (Engine::CPlayerDynamicMesh*)Engine::Clone_Resource(RESOURCE_DYE, L"Mesh_Skeleton_Hand");
	m_pHandMesh[1] = (Engine::CPlayerDynamicMesh*)Engine::Clone_Resource(RESOURCE_DYE, L"Mesh_Heremon_Hand");
	m_pHandMesh[2] = (Engine::CPlayerDynamicMesh*)Engine::Clone_Resource(RESOURCE_DYE, L"Mesh_Pirate_Hand");

	m_pFootMesh[0] = (Engine::CPlayerDynamicMesh*)Engine::Clone_Resource(RESOURCE_DYE, L"Mesh_Skeleton_Foot");
	m_pFootMesh[1] = (Engine::CPlayerDynamicMesh*)Engine::Clone_Resource(RESOURCE_DYE, L"Mesh_Heremon_Foot");
	m_pFootMesh[2] = (Engine::CPlayerDynamicMesh*)Engine::Clone_Resource(RESOURCE_DYE, L"Mesh_Pirate_Foot");

	_matrix matColor;
	D3DXMatrixIdentity(&matColor);
	matColor._11 = 1.f;	matColor._12 = 1.f;	matColor._13 = 1.f;
	matColor._21 = 1.f;	matColor._22 = 1.f;	matColor._23 = 1.f;
	matColor._31 = 1.f;	matColor._32 = 1.f;	matColor._33 = 1.f;
	memcpy( g_tSwordInfo.matColor, matColor, sizeof(_matrix));

	m_pSwordMesh[0] = CSword::Create(m_pGraphicDev, g_tSwordInfo.matColor, L"Mesh_Sword");
	m_pSwordMesh[1] = CSword::Create(m_pGraphicDev, g_tSwordInfo.matColor, L"Mesh_Sword3");
	m_pSwordMesh[2] = CSword::Create(m_pGraphicDev, g_tSwordInfo.matColor, L"Mesh_Sword2");

	CPlayer::GetInstance()->Set_UpperMesh(m_pUpperMesh[0]);
	CPlayer::GetInstance()->Set_LowerMesh(m_pLowerMesh[0]);
	CPlayer::GetInstance()->Set_HandMesh(m_pHandMesh[0]);
	CPlayer::GetInstance()->Set_FootMesh(m_pFootMesh[0]);
	CPlayer::GetInstance()->Undraw();

	return S_OK;
}

_int CSelectCloth::Update(const _float& fTimeDelta)
{
	Engine::CGameObject::Update(fTimeDelta);

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_UI, this);

	D3DXMatrixOrthoLH(&m_matOrtho, WINSIZEX, WINSIZEY, 0.f, 1.f);

	if( m_bPlay )
	{
		m_fClickOverTime += fTimeDelta;

		if( m_fClickOverTime >= m_fClick + 0.5f )
		{
			m_fClick = 0.f;
			m_fClickOverTime = 0.f;
			m_bPlay = false;
		}
	}

	POINT			ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	TCHAR path[128];
	lstrcpy( path, L"../Bin/Resources/Mesh/DynamicMesh/Look/" );
	TCHAR name[128];
	TCHAR Origname[128];
	lstrcpy( name, L"Mesh_" );
	lstrcpy( Origname, L"Mesh_" );

	if(PtInRect(&m_rcPoseUI[0], ptMouse) && !m_bPlay &&( Engine::GetDIMouseState(Engine::CInput::DIM_LBUTTON) & 0x80 ) )
	{
		CPlayer::GetInstance()->Set_PlayerAnimationType(Player::battle_attack_01_a);
	}
	else if(PtInRect(&m_rcPoseUI[1], ptMouse) && !m_bPlay &&( Engine::GetDIMouseState(Engine::CInput::DIM_LBUTTON) & 0x80 ) )
	{
		CPlayer::GetInstance()->Set_PlayerAnimationType(Player::battle_attack_04);
	}
	else if(PtInRect(&m_rcPoseUI[2], ptMouse) && !m_bPlay &&( Engine::GetDIMouseState(Engine::CInput::DIM_LBUTTON) & 0x80 ) )
	{
		CPlayer::GetInstance()->Set_PlayerAnimationType(Player::kick);
	}

	for( int i = 0; i < C_END; ++i )
	{
		for( int j = 0; j < 3; ++j )
		{
			if(PtInRect(&m_rcUI[i][j], ptMouse) && !m_bPlay &&( Engine::GetDIMouseState(Engine::CInput::DIM_LBUTTON) & 0x80 ) )
			{
				printf("마우스 %d\n", (int)m_bPlay);

				if( !m_bPlay )
				{
					m_fClick = fTimeDelta;
					m_bPlay = true;
					printf("마우스 %d\n", (int)m_bPlay);
					CSoundMgr::GetInstance()->PlayEffect(SOUND::Click, _vec3(0, 0, 0), 0.7f);
				}


				m_iSelectPart = i;

				TCHAR FileName[128];
				TCHAR OrigFileName[128];

				switch(j)
				{
				case 0:
					lstrcat(path, L"Skeleton/");
					lstrcat(name, L"Skeleton_");
					lstrcat(Origname, L"Skeleton_");
					lstrcpy(FileName, L"Skeleton_");
					lstrcpy(OrigFileName, L"Skeleton_");
					break;
				case 1:
					lstrcat(path, L"Heremon/");
					lstrcat(name, L"Heremon_");
					lstrcat(Origname, L"Heremon_");
					lstrcpy(FileName, L"Heremon_");
					lstrcpy(OrigFileName, L"Heremon_");
					break;
				case 2:
					lstrcat(path, L"Pirate/");
					lstrcat(name, L"Pirate_");
					lstrcat(Origname, L"Pirate_");
					lstrcpy(FileName, L"Pirate_");
					lstrcpy(OrigFileName, L"Pirate_");
					break;

				}
				switch( i )
				{
				case 0:
					CPlayer::GetInstance()->Set_HairMesh(m_pHairMesh[j]);
					if( m_iHair == -1 )
					{
						_matrix matColor;
						D3DXMatrixIdentity(&matColor);
						matColor._11 = 1.f;	matColor._12 = 1.f; matColor._13 = 1.f;
						matColor._21 = 1.f;	matColor._22 = 1.f;	matColor._23 = 1.f;
						matColor._31 = 1.f;	matColor._32 = 1.f;	matColor._33 = 1.f;
						CPlayer::GetInstance()->Set_HairColor(matColor);
					}
					m_iHair = j;
					lstrcat(Origname, L"Hair");
					lstrcat(OrigFileName, L"Head.x");
					CPlayer::GetInstance()->Set_MeshPath(i, path);
					CPlayer::GetInstance()->Set_OrigMeshName(i, Origname);
					CPlayer::GetInstance()->Set_OrigMeshFileName(i, OrigFileName);

					break;

				case 1:
					m_iUpper = j;
					lstrcat(name, L"Upper_Broken");
					lstrcat(Origname, L"Upper");
					lstrcat(FileName, L"Upper_Broken.x");
					lstrcat(OrigFileName, L"Upper.x");

					if( j == 0 )
						CPlayer::GetInstance()->Undraw();
					else
						CPlayer::GetInstance()->Draw();

					if( j == 1 )
						CPlayer::GetInstance()->Draw_Hand2();
					else
						CPlayer::GetInstance()->Undraw_Hand2();

					CPlayer::GetInstance()->Set_MeshPath(i, path);
					CPlayer::GetInstance()->Set_MeshName(i, name);
					CPlayer::GetInstance()->Set_MeshFileName(i, FileName);

					CPlayer::GetInstance()->Set_OrigMeshName(i, Origname);
					CPlayer::GetInstance()->Set_OrigMeshFileName(i, OrigFileName);
					CPlayer::GetInstance()->Set_UpperMesh(m_pUpperMesh[j]);
					break;
				case 2:
					m_iHand = j;
					lstrcat(name, L"Hand_Broken");
					lstrcat(Origname, L"Hand");
					lstrcat(FileName, L"Hand_Broken.x");
					lstrcat(OrigFileName, L"Hand.x");

					CPlayer::GetInstance()->Set_MeshPath(i, path);
					CPlayer::GetInstance()->Set_MeshName(i, name);
					CPlayer::GetInstance()->Set_MeshFileName(i, FileName);
					CPlayer::GetInstance()->Set_OrigMeshName(i, Origname);
					CPlayer::GetInstance()->Set_OrigMeshFileName(i, OrigFileName);
					CPlayer::GetInstance()->Set_HandMesh(m_pHandMesh[j]);
					break;
				case 3:
					m_iLower = j;
					lstrcat(name, L"Lower_Broken");
					lstrcat(Origname, L"Lower");
					lstrcat(FileName, L"Lower_Broken.x");
					lstrcat(OrigFileName, L"Lower.x");

					CPlayer::GetInstance()->Set_MeshPath(i, path);
					CPlayer::GetInstance()->Set_MeshName(i, name);
					CPlayer::GetInstance()->Set_OrigMeshName(i, Origname);
					CPlayer::GetInstance()->Set_MeshFileName(i, FileName);
					CPlayer::GetInstance()->Set_OrigMeshFileName(i, OrigFileName);
					CPlayer::GetInstance()->Set_LowerMesh(m_pLowerMesh[j]);
					break;
				case 4:
					m_iFoot = j;
					lstrcat(name, L"Foot_Broken");
					lstrcat(Origname, L"Foot");
					lstrcat(FileName, L"Foot_Broken.x");
					lstrcat(OrigFileName, L"Foot.x");

					CPlayer::GetInstance()->Set_MeshPath(i, path);
					CPlayer::GetInstance()->Set_MeshName(i, name);
					CPlayer::GetInstance()->Set_OrigMeshName(i, Origname);
					CPlayer::GetInstance()->Set_MeshFileName(i, FileName);
					CPlayer::GetInstance()->Set_OrigMeshFileName(i, OrigFileName);
					CPlayer::GetInstance()->Set_FootMesh(m_pFootMesh[j]);
					break;
				case 5:
					m_iSword = j;
					Set_SwordInfo();
					break;
				}
				break;
			}
			if( m_bPlay )
				break;
		}
		if( m_bPlay )
			break;
	}


	Update_Icon(fTimeDelta);

	if( m_iSword != -1 )
		m_pSwordMesh[m_iSword]->Update(fTimeDelta);

	return 0;
}

void CSelectCloth::Set_SwordInfo()
{
	switch( m_iSword )
	{
	case 0:
		lstrcpy(g_tSwordInfo.szMeshName, L"Mesh_Sword");
		break;
	case 1:
		lstrcpy(g_tSwordInfo.szMeshName, L"Mesh_Sword3");
		break;
	case 2:
		lstrcpy(g_tSwordInfo.szMeshName, L"Mesh_Sword2");
		break;
	}
}

void CSelectCloth::Render(void)
{
	if(NULL == m_pEffect)
		return;

	Set_ContantTable();

	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);

	m_pBufferCom->Render(NULL);

	m_pEffect->EndPass();
	m_pEffect->End();	

	m_pEffect->SetTexture("g_BaseTexture", NULL);

	Render_Icon();

	if( m_iSword != -1 )
		m_pSwordMesh[m_iSword]->Render();

}

void CSelectCloth::Create_Icon()
{
	for( int i = 0; i < 5/*6*/; ++i )
	{
		m_pEqIcon[0][i] = CDyeIconColor::Create(Engine::Get_GraphicDev(), L"Texture_EqIconSkeleton", -740, 345 - (50*i), 32, 32);
		m_pEqIcon[0][i]->Set_Select(i);

		m_pEqIcon[1][i] = CDyeIconColor::Create(Engine::Get_GraphicDev(), L"Texture_EqIconHeremon", -690, 345 - (50*i), 32, 32);
		m_pEqIcon[1][i]->Set_Select(i);

		m_pEqIcon[2][i] = CDyeIconColor::Create(Engine::Get_GraphicDev(), L"Texture_EqIconPirate", -635, 345 - (50*i), 32, 32);
		m_pEqIcon[2][i]->Set_Select(i);
	}

	m_pEqIcon[0][5] = CDyeIconColor::Create(Engine::Get_GraphicDev(), L"Texture_SwordIconSkeleton", -740, 345 - (50*5), 32, 32);
	m_pEqIcon[0][5]->Set_Select(0);
	m_pEqIcon[1][5] = CDyeIconColor::Create(Engine::Get_GraphicDev(), L"Texture_SwordIconHeremon", -690, 345 - (50*5), 32, 32);
	m_pEqIcon[1][5]->Set_Select(0);
	m_pEqIcon[2][5] = CDyeIconColor::Create(Engine::Get_GraphicDev(), L"Texture_SwordIconNight", -635, 345 - (50*5), 32, 32);
	m_pEqIcon[2][5]->Set_Select(0);

	m_pPoseIcon[0] = CDyeIcon::Create(Engine::Get_GraphicDev(), L"Texture_PoseIcon", -585, 345 - (50*0), 32, 32);
	m_pPoseIcon[0]->Set_Select(0);
	m_pPoseIcon[1] = CDyeIcon::Create(Engine::Get_GraphicDev(), L"Texture_PoseIcon", -585, 345 - (50*1), 32, 32);
	m_pPoseIcon[1]->Set_Select(1);
	m_pPoseIcon[2] = CDyeIcon::Create(Engine::Get_GraphicDev(), L"Texture_PoseIcon", -585, 345 - (50*2), 32, 32);
	m_pPoseIcon[2]->Set_Select(2);
}

void CSelectCloth::Update_Icon(const _float& fTimeDelta)
{
	for( int i = 0; i < 6; ++i )
	{
		m_pEqIcon[0][i]->Update(fTimeDelta);
		m_pEqIcon[1][i]->Update(fTimeDelta);
		m_pEqIcon[2][i]->Update(fTimeDelta);
	}
	m_pPoseIcon[0]->Update(fTimeDelta);
	m_pPoseIcon[1]->Update(fTimeDelta);
	m_pPoseIcon[2]->Update(fTimeDelta);

}

void CSelectCloth::Render_Icon()
{
	for( int i = 0; i < 6; ++i )
	{
		m_pEqIcon[0][i]->Render();
		m_pEqIcon[1][i]->Render();
		if( i != 5 )
			m_pEqIcon[2][i]->Render();
	}
	m_pPoseIcon[0]->Render();
	m_pPoseIcon[1]->Render();
	m_pPoseIcon[2]->Render();
}

CSelectCloth* CSelectCloth::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSelectCloth*		pGameObj = new CSelectCloth(pGraphicDev);

	if(FAILED(pGameObj->Initialize()))
	{
		MSG_BOX("CSelectCloth Create Faild");
		::Safe_Release(pGameObj);
	}
	return pGameObj;
}

void CSelectCloth::Set_ContantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView;
	D3DXMatrixIdentity(&matView);

	matView._11 = m_fSizeX;
	matView._22 = m_fSizeY;
	matView._33 = 1.f;

	matView._41 = -520.f;
	matView._42 = 170.f;
	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &m_matOrtho);
	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture", 0);
}

void CSelectCloth::Set_IconColor()
{
	_matrix matHair[3];	_matrix matUpper[3];	_matrix matHand[3];
	_matrix matLower[3];	_matrix matFoot[3];	_matrix matSword[3];

	matHair[0]._11 = 0.f;	matHair[0]._12 = 0.f;	matHair[0]._13 = 0.f;
	matHair[0]._21 = 1.f;	matHair[0]._32 = 0.f;	matHair[0]._23 = 0.f;
	matHair[0]._31 = 0.f;	matHair[0]._32 = 0.f;	matHair[0]._33 = 0.f;
	matHair[1]._11 = 0.f;	matHair[1]._12 = 0.f;	matHair[1]._13 = 0.f;
	matHair[1]._21 = 1.f;	matHair[1]._32 = 0.f;	matHair[1]._23 = 0.f;
	matHair[1]._31 = 0.f;	matHair[1]._32 = 0.f;	matHair[1]._33 = 0.f;
	matHair[2]._11 = 0.f;	matHair[2]._12 = 0.f;	matHair[2]._13 = 0.f;
	matHair[2]._21 = 1.f;	matHair[2]._32 = 0.f;	matHair[2]._23 = 0.f;
	matHair[2]._31 = 0.f;	matHair[2]._32 = 0.f;	matHair[2]._33 = 0.f;
	m_pEqIcon[0][0]->Set_Color(matHair[0]);
	m_pEqIcon[1][0]->Set_Color(matHair[1]);
	m_pEqIcon[2][0]->Set_Color(matHair[2]);

	matUpper[0]._11 = 0.f;	matUpper[0]._12 = 0.f;	matUpper[0]._13 = 0.f;
	matUpper[0]._21 = 1.f;	matUpper[0]._32 = 0.f;	matUpper[0]._23 = 0.f;
	matUpper[0]._31 = 0.f;	matUpper[0]._32 = 0.f;	matUpper[0]._33 = 0.f;
	matUpper[1]._11 = 0.f;	matUpper[1]._12 = 0.f;	matUpper[1]._13 = 0.f;
	matUpper[1]._21 = 1.f;	matUpper[1]._32 = 0.f;	matUpper[1]._23 = 0.f;
	matUpper[1]._31 = 0.f;	matUpper[1]._32 = 0.f;	matUpper[1]._33 = 0.f;
	matUpper[2]._11 = 0.f;	matUpper[2]._12 = 0.f;	matUpper[2]._13 = 0.f;
	matUpper[2]._21 = 1.f;	matUpper[2]._32 = 0.f;	matUpper[2]._23 = 0.f;
	matUpper[2]._31 = 0.f;	matUpper[2]._32 = 0.f;	matUpper[2]._33 = 0.f;
	m_pEqIcon[0][1]->Set_Color(matUpper[0]);
	m_pEqIcon[1][1]->Set_Color(matUpper[1]);
	m_pEqIcon[2][1]->Set_Color(matUpper[2]);

	matHand[0]._11 = 0.f;	matHand[0]._12 = 0.f;	matHand[0]._13 = 0.f;
	matHand[0]._21 = 1.f;	matHand[0]._32 = 0.f;	matHand[0]._23 = 0.f;
	matHand[0]._31 = 0.f;	matHand[0]._32 = 0.f;	matHand[0]._33 = 0.f;
	matHand[1]._11 = 0.f;	matHand[1]._12 = 0.f;	matHand[1]._13 = 0.f;
	matHand[1]._21 = 1.f;	matHand[1]._32 = 0.f;	matHand[1]._23 = 0.f;
	matHand[1]._31 = 0.f;	matHand[1]._32 = 0.f;	matHand[1]._33 = 0.f;
	matHand[2]._11 = 0.f;	matHand[2]._12 = 0.f;	matHand[2]._13 = 0.f;
	matHand[2]._21 = 1.f;	matHand[2]._32 = 0.f;	matHand[2]._23 = 0.f;
	matHand[2]._31 = 0.f;	matHand[2]._32 = 0.f;	matHand[2]._33 = 0.f;
	m_pEqIcon[0][2]->Set_Color(matHand[0]);
	m_pEqIcon[1][2]->Set_Color(matHand[1]);
	m_pEqIcon[2][2]->Set_Color(matHand[2]);

	matLower[0]._11 = 0.f;	matLower[0]._12 = 0.f;	matLower[0]._13 = 0.f;
	matLower[0]._21 = 1.f;	matLower[0]._22 = 0.f;	matLower[0]._23 = 0.f;
	matLower[0]._31 = 0.f;	matLower[0]._32 = 0.f;	matLower[0]._33 = 0.f;
	matLower[1]._11 = 0.f;	matLower[1]._12 = 0.f;	matLower[1]._13 = 0.f;
	matLower[1]._21 = 1.f;	matLower[1]._22 = 0.f;	matLower[1]._23 = 0.f;
	matLower[1]._31 = 0.f;	matLower[1]._32 = 0.f;	matLower[1]._33 = 0.f;
	matLower[2]._11 = 0.f;	matLower[2]._12 = 0.f;	matLower[2]._13 = 0.f;
	matLower[2]._21 = 1.f;	matLower[2]._22 = 0.f;	matLower[2]._23 = 0.f;
	matLower[2]._31 = 0.f;	matLower[2]._32 = 0.f;	matLower[2]._33 = 0.f;
	m_pEqIcon[0][3]->Set_Color(matLower[0]);
	m_pEqIcon[1][3]->Set_Color(matLower[1]);
	m_pEqIcon[2][3]->Set_Color(matLower[2]);

	matFoot[0]._11 = 0.f;	matFoot[0]._12 = 0.f;	matFoot[0]._13 = 0.f;
	matFoot[0]._21 = 1.f;	matFoot[0]._22 = 0.f;	matFoot[0]._23 = 0.f;
	matFoot[0]._31 = 0.f;	matFoot[0]._32 = 0.f;	matFoot[0]._33 = 0.f;
	matFoot[1]._11 = 0.f;	matFoot[1]._12 = 0.f;	matFoot[1]._13 = 0.f;
	matFoot[1]._21 = 1.f;	matFoot[1]._22 = 0.f;	matFoot[1]._23 = 0.f;
	matFoot[1]._31 = 0.f;	matFoot[1]._32 = 0.f;	matFoot[1]._33 = 0.f;
	matFoot[2]._11 = 0.f;	matFoot[2]._12 = 0.f;	matFoot[2]._13 = 0.f;
	matFoot[2]._21 = 1.f;	matFoot[2]._22 = 0.f;	matFoot[2]._23 = 0.f;
	matFoot[2]._31 = 0.f;	matFoot[2]._32 = 0.f;	matFoot[2]._33 = 0.f;
	m_pEqIcon[0][4]->Set_Color(matFoot[0]);
	m_pEqIcon[1][4]->Set_Color(matFoot[1]);
	m_pEqIcon[2][4]->Set_Color(matFoot[2]);

	matSword[0]._11 = 0.f;	matSword[0]._12 = 0.f;	matSword[0]._13 = 0.f;
	matSword[0]._21 = 1.f;	matSword[0]._22 = 1.f;	matSword[0]._23 = 1.f;
	matSword[0]._31 = 0.f;	matSword[0]._32 = 0.f;	matSword[0]._33 = 0.f;

	matSword[1]._11 = 0.f;	matSword[1]._12 = 0.f;	matSword[1]._13 = 0.f;
	matSword[1]._21 = 1.f;	matSword[1]._22 = 1.f;	matSword[1]._23 = 1.f;
	matSword[1]._31 = 0.f;	matSword[1]._32 = 0.f;	matSword[1]._33 = 0.f;

	matSword[2]._11 = 0.f;	matSword[2]._12 = 0.f;	matSword[2]._13 = 0.f;
	matSword[2]._21 = 1.f;	matSword[2]._22 = 1.f;	matSword[2]._23 = 1.f;
	matSword[2]._31 = 0.f;	matSword[2]._32 = 0.f;	matSword[2]._33 = 0.f;
	m_pEqIcon[0][5]->Set_Color(matSword[0]);
	m_pEqIcon[1][5]->Set_Color(matSword[1]);
	m_pEqIcon[2][5]->Set_Color(matSword[2]);
}

_ulong CSelectCloth::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		for( int i = 0; i < 3; ++i )
		{
			Engine::Safe_Release(m_pPoseIcon[i]);
			for( int j = 0; j < 6; ++j )
			{
				Engine::Safe_Release(m_pEqIcon[i][j]);
			}
		}

		for( int k = 0; k < 3; ++k )
		{
			//if( i != m_iUpper )
			Engine::Safe_Release(m_pSwordMesh[k]);
		}
		for( int k = 0; k < 3; ++k )
		{
			//if( i != m_iUpper )
			Engine::Safe_Release(m_pHairMesh[k]);
		}
		for( int k = 0; k < 3; ++k )
		{
			//if( i != m_iUpper )
			Engine::Safe_Release(m_pUpperMesh[k]);
		}
		for( int k = 0; k < 3; ++k )
		{
			//if( i != m_iLower )
			Engine::Safe_Release(m_pLowerMesh[k]);
		}
		for( int k = 0; k < 3; ++k )
		{
			//if( i != m_iHand )
			Engine::Safe_Release(m_pHandMesh[k]);
		}
		for( int k = 0; k < 3; ++k )
		{
			//if( k != m_iFoot )
			Engine::Safe_Release(m_pFootMesh[k]);
		}

		delete this;
	}

	return dwRefCnt;
}
