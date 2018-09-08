#include "RailPoint.h"
#include "Rail.h"

CRailPoint::CRailPoint()
{
}


CRailPoint::~CRailPoint()
{
	vector<CGameObject*>::iterator iter = m_vecRail.begin();
	for (; iter != m_vecRail.end();)
	{
		Safe_Delete((*iter));
		iter = m_vecRail.erase(iter);
	}
}

void CRailPoint::Initialize()
{
	m_vScale = MyVector(1.f, 1.f, 1.f);
	m_fRadius = 40.f;
	
	m_vecRail.reserve(10);
	if (true == m_bFirst)
	{
		m_vScale = MyVector(4.f, 0.05f, 4.f);
		m_vPos.y += m_fRadius * 0.5f * m_vScale.y;
	}
	else
	{
		m_vPos.y += m_fRadius * 0.5f * m_vScale.y;
		for (int i = 0; i < 10; ++i)
		{
			CGameObject*	pObj = new CRail;
			pObj->Initialize();
			m_vecRail.push_back(pObj);
		}
	}
}

int CRailPoint::Update(float fTime)
{
	size_t VecSize = m_vecRail.size();
	for (size_t i = 0; i < VecSize; ++i)
	{
		m_vecRail[i]->Update(fTime);
	}
	return 0;
}

void CRailPoint::Render()
{
	if (true == m_bFirst)
	{
		glPushMatrix();
		{
			glTranslatef(m_vPos.x, m_vPos.y, m_vPos.z);
			glRotatef(m_vRot.x, 1.f, 0.f, 0.f);
			glRotatef(m_vRot.y, 0.f, 1.f, 0.f);
			glRotatef(m_vRot.z, 0.f, 0.f, 1.f);
			glScalef(m_vScale.x, m_vScale.y, m_vScale.z);

			glColor3f(m_vColor.x, m_vColor.y, m_vColor.z);
			glutSolidCube(40.f);
		}
		glPopMatrix();
	}
	else
	{
		glPushMatrix();
		{
			glTranslatef(m_vPos.x, m_vPos.y, m_vPos.z);
			glRotatef(m_vRot.x, 1.f, 0.f, 0.f);
			glRotatef(m_vRot.y, 0.f, 1.f, 0.f);
			glRotatef(m_vRot.z, 0.f, 0.f, 1.f);
			glScalef(m_vScale.x, m_vScale.y * 0.5f, m_vScale.z);

			glColor3f(m_vColor.x, m_vColor.y, m_vColor.z);
			glutSolidCube(m_fRadius * 0.5f);
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(m_vPos.x, m_vPos.y * 0.5f, m_vPos.z);
			glRotatef(m_vRot.x, 1.f, 0.f, 0.f);
			glRotatef(m_vRot.y, 0.f, 1.f, 0.f);
			glRotatef(m_vRot.z, 0.f, 0.f, 1.f);
			glScalef(40.f, m_vPos.y, 40.f);

			glColor3f(0.2f, 0.2f, 0.2f);
			glutSolidCube(1);
		}
		glPopMatrix();
	}
	size_t VecSize = m_vecRail.size();
	for (size_t i = 0; i < VecSize; ++i)
	{
		m_vecRail[i]->Render();
	}
}

void CRailPoint::Release()
{
}
