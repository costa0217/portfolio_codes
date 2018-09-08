#include "Bullet.h"

CBullet::CBullet()
{
}


CBullet::~CBullet()
{
}

void CBullet::Initialize()
{
	m_fRadius = 10.f;
	m_fSpeed = 1000.f;
	m_vScale = MyVector(1.f, 1.f, 1.f);
	m_eType = OBJ_EFFECT;
	m_vColor = MyVector(1.f, 1.f, 0.f);
}

int CBullet::Update(float fTime)
{
	m_fAccTime += fTime;
	m_vPos = m_vPos + m_vDir * m_fAccTime * m_fSpeed;

	if (m_fAccTime > 1.f)
		return -1;

	return 0;
}

void CBullet::Render()
{
	glLineWidth(1.f);
	glPushMatrix();
	{
		glTranslatef(m_vPos.x, m_vPos.y, m_vPos.z);
		glRotatef(m_vRot.x, 1.f, 0.f, 0.f);
		glRotatef(m_vRot.y, 0.f, 1.f, 0.f);
		glRotatef(m_vRot.z, 0.f, 0.f, 1.f);
		glScalef(m_vScale.x, m_vScale.y, m_vScale.z);

		glColor4f(m_vColor.x, m_vColor.y, m_vColor.z, 1.0f);
		glutSolidSphere(m_fRadius, 20, 20);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(m_vPos.x, m_vPos.y, m_vPos.z);

		//glGetFloatv(GL_MODELVIEW_MATRIX, m_mWorld);
		MyIdentityMatrix(&m_matWorld);
		MyTranslation(&m_matWorld, m_vPos.x, m_vPos.y, m_vPos.z);
		MyScaling(&m_matWorld, 1.f, 1.f, 1.f);

		//glColor4f(1.f, 1.f, 1.f, 1.0f);
		//glutWireCube(102);
	}
	glPopMatrix();
}

void CBullet::Release()
{
}
