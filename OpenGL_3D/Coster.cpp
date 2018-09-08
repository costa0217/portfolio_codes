#include "Coster.h"
#include "RailPoint.h"
#include "Rail.h"
#include "Bullet.h"

CCoster::CCoster()
{
}
CCoster::~CCoster()
{
}

void CCoster::Initialize()
{
	m_vScale = MyVector(2.f, 1.f, 1.2f);
	m_fRadius = 40.f;
	m_vPos.y = m_fRadius * 0.5f * m_vScale.y;
	m_vColor = MyVector(1.f, 0.f, 0.f);

	m_fSpeed = 500.f;
	m_fAccTime = 0.f;

	MyNormalize(&m_vDir, &m_vDir);

	m_fStartTime = m_iCosterNum * 0.2f;
}

int CCoster::Update(float fTime)
{
	if (m_bStart)
	{
		if(m_iCosterNum == 0)
			KeyInput(fTime);
		m_fAccTime += fTime;
		if (m_fAccTime >= m_fStartTime)
		{
			CRailPoint* pRailPoint = ((CRailPoint*)(*m_pVecRailPoint)[m_iRailPointNum + 1]);

			MyVector	vNextPos;
			m_iRailNum < 9 ? vNextPos = pRailPoint->m_vecRail[m_iRailNum + 1]->m_vPos : vNextPos = pRailPoint->m_vPos;

			m_vDir = vNextPos - m_vPos;
			MyNormalize(&m_vDir, &m_vDir);

			MyVector vTemp = MyVector(m_vDir.x, m_vDir.y, 0.f);
			MyNormalize(&vTemp, &vTemp);
			if (acosf(Vec3Dot(vTemp, MyVector(1.f, 0.f, 0.f))) < M_PI * 0.2f){
				m_fAccSpeed = 1.f;
			}else if (m_vDir.y < 0){
				m_fAccSpeed = 1.5f;
			}else{
				m_fAccSpeed = 0.7f;
			}

			m_vPos = m_vPos + m_vDir * m_fSpeed * fTime * m_fAccSpeed;
			if (GetVec3Dist(m_vPos, vNextPos) < 5.f)
			{
				m_vPos = vNextPos;
				++m_iRailNum;
				if (m_iRailNum == 10)
				{
					m_iRailNum = 0;
					++m_iRailPointNum;
					if (m_iRailPointNum + 1 == (*m_pVecRailPoint).size())
					{
						m_iRailPointNum = 0;
					}
				}
			}
			MyVector vDirXZ = MyVector(m_vDir.x, 0.f, m_vDir.z);
			MyNormalize(&vDirXZ, &vDirXZ);
			float fYAngle = acosf(Vec3Dot(vDirXZ, MyVector(1.f, 0.f, 0.f)));
			if (vDirXZ.z > 0)
			{
				fYAngle = M_PI * 2.f - fYAngle;
			}
			fYAngle *= 180 / M_PI;
			m_vRot.y = fYAngle;
		}

	}
	return 0;
}

void CCoster::Render()
{
	glPushMatrix();
	{
		glTranslatef(m_vPos.x, m_vPos.y, m_vPos.z);
		glRotatef(m_vRot.x, 1.f, 0.f, 0.f);
		glRotatef(m_vRot.y, 0.f, 1.f, 0.f);
		glRotatef(m_vRot.z, 0.f, 0.f, 1.f);
		glScalef(m_vScale.x, m_vScale.y, m_vScale.z);

		glColor3f(m_vColor.x, m_vColor.y, m_vColor.z);
		glutSolidCube(m_fRadius);
	}
	glPopMatrix();
	if (m_iCosterNum != 0)
	{
		glLineWidth(5.f);
		glColor3f(0.f, 0.24f, 0.6f);
		glBegin(GL_LINES);
		glNormal3f(0.0, 1.0f, 0.0);
		glVertex3f(m_pPlayers[m_iCosterNum - 1]->m_vPos.x, m_pPlayers[m_iCosterNum - 1]->m_vPos.y, m_pPlayers[m_iCosterNum - 1]->m_vPos.z);
		glNormal3f(0.0, 1.0f, 0.0);
		glVertex3f(m_vPos.x, m_vPos.y, m_vPos.z);
		glEnd();
		glLineWidth(1.f);
	}
}

void CCoster::Release()
{
}

void CCoster::KeyInput(float fTime)
{
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (m_wKey == VK_SPACE) return;

		CGameObject*	pBullet = new CBullet;
		pBullet->m_vPos = m_vPos;
		pBullet->m_vDir = m_vDir;
		pBullet->Initialize();
		m_pVecEffect->push_back(pBullet);
		m_wKey = VK_SPACE;
	}
	else
	{
		m_wKey = 0;
	}
}
