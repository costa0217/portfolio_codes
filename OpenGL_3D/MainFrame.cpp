#include "MainFrame.h"
#include "IncludeHeader.h"
#include "GameObject.h"

#include "CollisioneMgr.h"

#include "Camera.h"
#include "MyTree.h"
#include "RailPoint.h"
#include "Rail.h"
#include "Coster.h"
#include "Weather.h"
#include "Ternel.h"
#include "Skybox.h"
#include "FireWork.h"

CMainFrame::CMainFrame()
{
}
CMainFrame::~CMainFrame()
{
	//Release();
}


void CMainFrame::Initialize()
{

	for (int i = 0; i < OBJ_END; ++i)
	{
		if (i == OBJ_MAP)
		{
			vecObjects[i].reserve(10000);
			continue;
		}
		vecObjects[i].reserve(20);
	}

	m_vPos = MyVector(0.f, 0.f, 0.f);
	m_vScale = MyVector(1.f, 1.f, 1.f);
	m_vRot = MyVector(0.f, 0.f, 0.f);

	

	CreateMapObject();
	CreateCharObject();

	m_pCamera = new CCamera;
	m_pCamera->m_pPlayer = &m_pPlayer[0];
	m_pCamera->m_vPos = MyVector(0.f, 500.f, 0.f);
	m_pCamera->m_vLook = MyVector(0.f, 0.f, 0.f);
}

int CMainFrame::Update(float fTime)
{
	int iResult = 0;

	KeyInput(fTime);
	for (int i = 0; i < OBJ_END; ++i)
	{
		vector<CGameObject*>::iterator iter = vecObjects[i].begin();
		for (; iter != vecObjects[i].end();)
		{
			iResult = (*iter)->Update(fTime);
			if (0 > iResult)
			{
				Safe_Delete((*iter));
				iter = vecObjects[i].erase(iter);
			}
			else
				++iter;
		}
	}

	if (m_bMakeRail == true)
	{
		m_fAccFireWorkTime += fTime;
		if (m_fAccFireWorkTime > 1.f)
		{
			m_fAccFireWorkTime = 0.f;
			CGameObject*	pObj = new CFireWork;
			pObj->m_vPos = MyVector(rand()%4000 - 2000.f, rand() % 2000 + 1000.f, rand() % 4000 - 2000.f);
			pObj->Initialize();
			vecObjects[OBJ_FIREWORK].push_back(pObj);
		}


		m_pCamera->InputData(fTime);
	}

	return iResult;
}

void CMainFrame::Render()
{


	m_pCamera->UpdateCamera();
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	{
		glTranslatef(m_vPos.x, m_vPos.y, m_vPos.z);
		glRotatef(m_vRot.x, 1.f, 0.f, 0.f);
		glRotatef(m_vRot.y, 0.f, 1.f, 0.f);
		glRotatef(m_vRot.z, 0.f, 0.f, 1.f);
		glScalef(m_vScale.x, m_vScale.y, m_vScale.z);

		if (true == m_bMakeRail)
		{
			glDisable(GL_LIGHTING);
			//glDisable(GL_LIGHT0);
			size_t vecsize = vecObjects[OBJ_EFFECT].size();
			for (size_t i = 0; i < vecsize; ++i)
				vecObjects[OBJ_EFFECT][i]->Render();

			vecObjects[OBJ_MAP].front()->Render();
			SettingLight();
		}

			
		//Terrain
		glColor4f(105.f / 255.f, 205.f / 255.f, 97.f / 255.f, 1.0f);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 4; ++i)
		{
			glNormal3f(0.0, 1.0f, 0.0);
			glVertex3f(m_vtx[i].x, m_vtx[i].y, m_vtx[i].z);
		}
		glEnd();
		//temp
		glLineWidth(3.f);

	
		for (int j = 0; j < OBJ_END; ++j)
		{
			size_t vecsize = vecObjects[j].size();
			if (j == OBJ_EFFECT)
				continue;

			for (size_t i = 0; i < vecsize; ++i)
			{
				if (m_bMakeRail == false && j == OBJ_EFFECT)
					continue;
				if (j == OBJ_MAP && i == 0)
					continue;

				vecObjects[j][i]->Render();
			}
		}
		RenderAxis();
	}
	glPopMatrix();

	UIRender();
}

void CMainFrame::Release()
{
}

void CMainFrame::UIRender()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(WINSIZEX * -0.5f, WINSIZEX * 0.5f, WINSIZEY * -0.5f, WINSIZEY * 0.5f, -100.0f, 10000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	{


	}
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
}

void CMainFrame::KeyInput(float fTime)
{
	if (GetAsyncKeyState('1') & 0x8000)
	{
		if (m_wKey == '1') return;

		//if(m_iHeight < 100)
			m_iHeight += 50;

		m_wKey = '1';
	}
	else if (GetAsyncKeyState('2') & 0x8000)
	{
		if (m_wKey == '2') return;

		if (m_iHeight > 0)
			m_iHeight -= 50;

		m_wKey = '2';
	}
	else if (GetAsyncKeyState('3') & 0x8000)
	{
		if (m_wKey == '3') return;

		m_bTernel = true;

		m_wKey = '3';
	}
	else if (GetAsyncKeyState('Q') & 0x8000)
	{
		if (m_wKey == 'Q') return;
		if (m_bMakeRail == true) return;
		if (vecObjects[OBJ_RAIL].empty()) return;


		CGameObject* pDeleteObj = vecObjects[OBJ_RAIL].back();

		Safe_Delete(pDeleteObj);
		vecObjects[OBJ_RAIL].pop_back();
		--m_iRailCnt;
		
		m_wKey = 'Q';
	}
	else if (GetAsyncKeyState('P') & 0x8000)
	{
		if (m_wKey == 'P') return;
		if (m_bMakeRail == false) return;

		for (int i = 0; i < 3; ++i)
		{
			((CCoster*)m_pPlayer[i])->m_bStart = true;
		}
		m_wKey = 'P';
	}
	else if (GetAsyncKeyState('E') & 0x8000)
	{
		if (m_wKey == 'E') return;
		if (m_bMakeRail == false) return;

		m_pCamera->m_bCosterEye ^= true;
		m_wKey = 'E';
	}
	else if (GetAsyncKeyState('R') & 0x8000)
	{
		if (m_wKey == 'R') return;
		if (m_bMakeRail == false) return;

		size_t vecEfxSize = vecObjects[OBJ_EFFECT].size();
		for (size_t i = 0; i < vecEfxSize; ++i)
		{
			vecObjects[OBJ_EFFECT][i]->m_iWeatherType < 2 ? ++vecObjects[OBJ_EFFECT][i]->m_iWeatherType : vecObjects[OBJ_EFFECT][i]->m_iWeatherType = 0;;
		}
		m_wKey = 'R';
	}
	else
	{
	   m_wKey = 0;
	}
}

void CMainFrame::RenderAxis()
{
	glLineWidth(2.f);
	glColor4f(1.f, 0.f, 0.f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(0, 0.f, 0.f);
	glVertex3f(100.f, 0.f, 0.f);
	glColor4f(0.f, 1.f, 0.f, 1.0f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 100.f, 0.f);
	glColor4f(0.f, 0.f, 1.f, 1.0f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, 100.f);
	glEnd();
	glLineWidth(1.f);
}

void CMainFrame::InputMousePoint(float x, float y)
{
	if (false == m_bMakeRail)
	{
		size_t nCollObjNum = vecObjects[OBJ_MAP_COLL].size();
		for (size_t i = 0; i < nCollObjNum; ++i)
		{
			if (GetVec3Dist(vecObjects[OBJ_MAP_COLL][i]->m_vPos, MyVector(x * 5.f, m_iHeight, y * -5.f)) < 200.f)
				return;
		}
		CGameObject*	pObj = new CRailPoint;
		pObj->m_vPos = MyVector(x * 5.f, m_iHeight, y * -5.f);
		if (vecObjects[OBJ_RAIL].empty())
		{
			pObj->m_vColor = MyVector(0.6f, 0.6f, 0.6f);
			pObj->m_bFirst = true;
		}
		else
		{
			pObj->m_vColor = MyVector(0.7f, 0.3f, 0.f);
		}
		((CRailPoint*)pObj)->m_iMyNum = m_iRailCnt;
		((CRailPoint*)pObj)->m_pVecRailPoint = &vecObjects[OBJ_RAIL];
		pObj->Initialize();

		vecObjects[OBJ_RAIL].push_back(pObj);

		// 터널 생성.
		if (m_bTernel == true)
		{
			if (m_iRailCnt > 3 && vecObjects[OBJ_MAP_TERNEL].empty())
			{
				CGameObject* pTernel = new CTernel;
				
				pTernel->m_vPos = (vecObjects[OBJ_RAIL][m_iRailCnt]->m_vPos + vecObjects[OBJ_RAIL][m_iRailCnt - 1]->m_vPos) * 0.5f;
				pTernel->Initialize();
				((CTernel*)pTernel)->m_iRailCnt = m_iRailCnt;
				vecObjects[OBJ_MAP_TERNEL].push_back(pTernel);
				m_bTernel = false;
			}
		}
		// 레일설치 종료조건
		float fDist = GetVec3Dist(vecObjects[OBJ_RAIL][0]->m_vPos, MyVector(pObj->m_vPos.x, 0.f, pObj->m_vPos.z));
		if (m_iRailCnt > 5 && fDist < 160)
		{
			pObj->m_vPos = vecObjects[OBJ_RAIL][0]->m_vPos;
			m_bMakeRail = true;
			m_pCamera->m_bFPStype = false;

			for (int i = 0; i < 3; ++i)
			{
				m_pPlayer[i] = new CCoster;
				m_pPlayer[i]->m_vPos = vecObjects[OBJ_RAIL][0]->m_vPos;
				m_pPlayer[i]->m_vDir = vecObjects[OBJ_RAIL][1]->m_vPos - vecObjects[OBJ_RAIL][0]->m_vPos;
				((CCoster*)m_pPlayer[i])->m_iCosterNum = i;
				m_pPlayer[i]->Initialize();
				((CCoster*)m_pPlayer[i])->m_pCamera = m_pCamera;
				((CCoster*)m_pPlayer[i])->m_pVecRailPoint = &vecObjects[OBJ_RAIL];
				((CCoster*)m_pPlayer[i])->m_pVecEffect = &vecObjects[OBJ_EFFECT];
				((CCoster*)m_pPlayer[i])->m_pPlayers = m_pPlayer;
				vecObjects[OBJ_COSTER].push_back(m_pPlayer[i]);
			}


		}

		MakePositioningRail();
		++m_iRailCnt;


	}

}
void CMainFrame::CreateMapObject()
{
	m_vtx[0] = MyVector(TerrainSize * 1.5f, 0.f, -TerrainSize* 1.5f);
	m_vtx[1] = MyVector(TerrainSize* 1.5f, 0.f, TerrainSize* 1.5f);
	m_vtx[2] = MyVector(-TerrainSize* 1.5f, 0.f, TerrainSize* 1.5f);
	m_vtx[3] = MyVector(-TerrainSize* 1.5f, 0.f, -TerrainSize* 1.5f);

	CGameObject*	pObj = new CSkybox;
	pObj->Initialize();
	vecObjects[OBJ_MAP].push_back(pObj);

	for (int i = 0; i < 40; ++i)
	{
		for (int j = 0; j < 40; ++j)
		{
			int nRand = rand() % 100;
			float fDist = sqrtf(pow(i * 150.f - 3000.f, 2) + pow(j * 150.f - 3000.f, 2));
			if (fDist < 2500.f)
			{
				if (nRand < 3)
				{
					CGameObject*	pObj = new CMyTree;
					pObj->m_vPos = MyVector(i * 150.f + rand() % 150 - 3000.f, 0.f, j * 150.f + rand() % 150 - 3000.f);
					pObj->Initialize();
					vecObjects[OBJ_MAP_COLL].push_back(pObj);
				}
			}
			else
			{
				if (nRand < 70)
				{
					CGameObject*	pObj = new CMyTree;
					pObj->m_vPos = MyVector(i * 200.f + rand() % 150 - 4000.f, 0.f, j * 200.f + rand() % 150 - 4000.f);
					pObj->Initialize();
					vecObjects[OBJ_MAP].push_back(pObj);
				}
			}
			if (nRand < 30)
			{
				CGameObject*	pObj = new CWeather;
				pObj->m_vPos = MyVector(i * 200.f + rand() % 200 - 4000.f, rand() % 2000 + 2000.f, j * 200.f + rand() % 200 - 4000.f);
				pObj->m_iWeatherType = 0.f;
				pObj->Initialize();
				vecObjects[OBJ_EFFECT].push_back(pObj);
			}
		}
	}

}

void CMainFrame::CreateCharObject()
{
	
}

void CMainFrame::MakePositioningRail()
{

	if (m_iRailCnt == 0) return;

	size_t nRailSize = vecObjects[OBJ_RAIL].size();
	for (size_t i = 1; i < nRailSize; ++i)
	{
		CRailPoint* pRailPoint = ((CRailPoint*)(vecObjects[OBJ_RAIL][i]));
		if (i == 1)
		{
			int nNext = nRailSize > 2 ? i + 1 : i;
			for (int j = 0; j < 10; ++j)
			{
				pRailPoint->m_vecRail[j]->m_vPos = MakeCatmullRom(j / 10.f, vecObjects[OBJ_RAIL][i - 1]->m_vPos,
					vecObjects[OBJ_RAIL][i - 1]->m_vPos, vecObjects[OBJ_RAIL][i]->m_vPos, vecObjects[OBJ_RAIL][nNext]->m_vPos);


			}
			for (int j = 0; j < 10; ++j)
			{
				MyVector vDir;
				j < 9 ? vDir = pRailPoint->m_vecRail[j + 1]->m_vPos - pRailPoint->m_vecRail[j]->m_vPos : vDir = pRailPoint->m_vPos - pRailPoint->m_vecRail[j]->m_vPos;
				MyNormalize(&vDir, &vDir);
				pRailPoint->m_vecRail[j]->m_vDir = vDir;
			}
		}
		else if (nRailSize - 1 == i)
		{
			for (int j = 0; j < 10; ++j)
			{
				pRailPoint->m_vecRail[j]->m_vPos = MakeCatmullRom(j / 10.f, vecObjects[OBJ_RAIL][i - 2]->m_vPos,
					vecObjects[OBJ_RAIL][i - 1]->m_vPos, vecObjects[OBJ_RAIL][i]->m_vPos, vecObjects[OBJ_RAIL][i]->m_vPos);
			}
			for (int j = 0; j < 10; ++j)
			{
				MyVector vDir;
				j < 9 ? vDir = pRailPoint->m_vecRail[j + 1]->m_vPos - pRailPoint->m_vecRail[j]->m_vPos : vDir = pRailPoint->m_vPos - pRailPoint->m_vecRail[j]->m_vPos;
				MyNormalize(&vDir, &vDir);
				pRailPoint->m_vecRail[j]->m_vDir = vDir;
			}
		}
		else
		{
			for (int j = 0; j < 10; ++j)
			{
				pRailPoint->m_vecRail[j]->m_vPos = MakeCatmullRom(j / 10.f, vecObjects[OBJ_RAIL][i - 2]->m_vPos,
					vecObjects[OBJ_RAIL][i - 1]->m_vPos, vecObjects[OBJ_RAIL][i]->m_vPos, vecObjects[OBJ_RAIL][i + 1]->m_vPos);

			}
			for (int j = 0; j < 10; ++j)
			{
				MyVector vDir;
				j < 9 ? vDir = pRailPoint->m_vecRail[j + 1]->m_vPos - pRailPoint->m_vecRail[j]->m_vPos : vDir = pRailPoint->m_vPos - pRailPoint->m_vecRail[j]->m_vPos;
				MyNormalize(&vDir, &vDir);
				pRailPoint->m_vecRail[j]->m_vDir = vDir;
			}
		}
	}

	size_t vecTernel = vecObjects[OBJ_MAP_TERNEL].size();
	for (size_t i = 0; i < vecTernel; ++i)
	{
		int nRailCnt = ((CTernel*)vecObjects[OBJ_MAP_TERNEL][i])->m_iRailCnt;
		vecObjects[OBJ_MAP_TERNEL][i]->m_vPos = ((CRailPoint*)(vecObjects[OBJ_RAIL][nRailCnt]))->m_vecRail[5]->m_vPos;
		vecObjects[OBJ_MAP_TERNEL][i]->m_vDir = ((CRailPoint*)(vecObjects[OBJ_RAIL][nRailCnt]))->m_vecRail[5]->m_vDir;
	}
}

void CMainFrame::SettingLight()
{
	GLfloat AmbientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat DiffuseLight[] = { 1.f, 1.f, 1.f, 1.0f };
	GLfloat SpecularLight[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat lightPos[] = { 0.f, -0.5f, -0.5f, 0.0 };  // 조명을 사용하도록 설정  
	glEnable(GL_LIGHTING);
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, AmbientLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLight);
	glEnable(GL_LIGHT0);

	glEnable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularLight);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}
