#include "MyTree.h"



CMyTree::CMyTree()
{
}


CMyTree::~CMyTree()
{
}

void CMyTree::Initialize()
{
	m_vScale = MyVector(1.f, 2.f, 1.f);
	m_fRadius = 40.f;
	m_vPos.y += m_fRadius * 0.5f * m_vScale.y;

	m_vColor = MyVector(rand() % 30 / 255.f, 0.3f + rand() % 80  / 255.f, rand() % 30 / 255.f);
}

int CMyTree::Update(float fTime)
{
	return 0;
}

void CMyTree::Render()
{
	glPushMatrix();
	{
		glTranslatef(m_vPos.x, m_vPos.y, m_vPos.z);
		glRotatef(m_vRot.x, 1.f, 0.f, 0.f);
		glRotatef(m_vRot.y, 0.f, 1.f, 0.f);
		glRotatef(m_vRot.z, 0.f, 0.f, 1.f);

		glPushMatrix();
		{
			glTranslatef(0.f, m_fRadius * 0.5f, 0.f);
			glPushMatrix();
			{
				glTranslatef(0.f, m_fRadius * 0.5f, 0.f);
				glPushMatrix();
				{
					glScalef(1.f, 1.3f, 1.f);
					glTranslatef(0.f, m_fRadius * 0.5f, 0.f);
					glRotatef(270.f, 1.f, 0.f, 0.f);

					glColor4f(m_vColor.x, m_vColor.y + 0.5f, m_vColor.z, 1.0f);
					glutSolidCone(m_fRadius, 70, 10, 10);
				}
				glPopMatrix();
				glScalef(1.2f, 1.1f, 1.2f);
				glRotatef(270.f, 1.f, 0.f, 0.f);
				glColor4f(m_vColor.x, m_vColor.y + 0.3f, m_vColor.z, 1.0f);
				glutSolidCone(m_fRadius, 70, 10, 10);
			}
			glPopMatrix();
			glScalef(1.5f, 1.f, 1.5f);
			glRotatef(270.f, 1.f, 0.f, 0.f);

			glColor4f(m_vColor.x, m_vColor.y + 0.2f, m_vColor.z, 1.0f);
			glutSolidCone(m_fRadius, 70, 10, 10);
		}
		glPopMatrix();
		glScalef(m_vScale.x, m_vScale.y, m_vScale.z);
		glColor4f(0.8f, 0.3f, 0.2f, 1.0f);
		glutSolidCube(m_fRadius);

	}
	glPopMatrix();
}

void CMyTree::Release()
{
}
