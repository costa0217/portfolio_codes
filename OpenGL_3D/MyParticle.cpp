#include "MyParticle.h"



CMyParticle::CMyParticle()
{
}

CMyParticle::~CMyParticle()
{
}

void CMyParticle::Initialize()
{
	m_vColor = MyVector((rand() %  255) / 255.f, (rand() % 255) / 255.f, (rand() % 255) / 255.f);
}

int CMyParticle::Update(float fTime)
{
	if (m_fAccTime > m_fLifeTime)
		return -1;

	m_vPos = m_vPos + m_vDir * fTime * m_fSpeed;
	m_fAccTime += fTime;
	

	return 0;
}

void CMyParticle::Render()
{
	if (m_fAccTime > m_fLifeTime)
		return;

		glPushMatrix();
		{
			glTranslatef(m_vPos.x, m_vPos.y, m_vPos.z);
			glRotatef(m_vRot.x, 1.f, 0.f, 0.f);
			glRotatef(m_vRot.y, 0.f, 1.f, 0.f);
			glRotatef(m_vRot.z, 0.f, 0.f, 1.f);
			glScalef(1.f, 2.f, 1.f);

			glColor4f(m_vColor.x, m_vColor.y, m_vColor.z, 1.0f);

			glLineWidth(5.f);
			glBegin(GL_LINES);
			glNormal3f(m_vDir.x, m_vDir.y, m_vDir.z);
			glVertex3f(0, 0.f, 0.f);
			glNormal3f(m_vDir.x, m_vDir.y, m_vDir.z);
			glVertex3f(m_vDir.x * 50.f, m_vDir.y * 50.f, m_vDir.z * 50.f);
			glEnd();
			glLineWidth(1.f);
		}
		glPopMatrix();

}

void CMyParticle::Release()
{
}
