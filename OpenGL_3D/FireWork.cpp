#include "FireWork.h"
#include "MyParticle.h"



CFireWork::CFireWork()
{
}


CFireWork::~CFireWork()
{
}

void CFireWork::Initialize()
{
	for (float phi = -90.0; phi <= 70.0; phi += 20.0)
	{
		for (float theta = -180.0; theta <= 180.0; theta += 20.0)
		{
			MyVector vDir = MyVector(sin(theta)*cos(phi), cos(theta)*cos(phi), sin(phi));
			if (Vec3Dot(vDir, m_vDir) >= 0)
			{
				CMyParticle* pObj = new CMyParticle(vDir, m_vPos);
				pObj->Initialize();
				m_vecParticle.push_back(pObj);
			}
			vDir = MyVector(sin(theta)*cos(phi + 20.0), cos(theta)*cos(phi + 20.0), sin(phi + 20.0));
			if (Vec3Dot(vDir, m_vDir) >= 0)
			{
				CMyParticle* pObj = new CMyParticle(vDir, m_vPos);
				pObj->Initialize();
				m_vecParticle.push_back(pObj);
			}
		}
	}
}

int CFireWork::Update(float fTime)
{
	vector<CMyParticle*>::iterator iter = m_vecParticle.begin();
	for (; iter != m_vecParticle.end();)
	{
		int iResult = (*iter)->Update(fTime);
		if (0 > iResult)
		{
			Safe_Delete((*iter));
			iter = m_vecParticle.erase(iter);
		}
		else
			++iter;
	}
	if (m_vecParticle.empty())
		return -1;
	return 0;
}

void CFireWork::Render()
{
	//GLfloat AmbientLight[] = { 1.0f, 0.2f, 0.2f, 1.0f };
	//GLfloat DiffuseLight[] = { 1.f, 0.2f, 0.2f, 1.0f };
	//GLfloat SpecularLight[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	//GLfloat lightPos[] = { m_vPos.x, m_vPos.y, m_vPos.z, 1.0 };  // 조명을 사용하도록 설정  
	////glEnable(GL_LIGHTING);
	////glLightModelfv(GL_LIGHT_MODEL_AMBIENT, AmbientLight);
	//glLightfv(GL_LIGHT1, GL_AMBIENT, AmbientLight);
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, DiffuseLight);
	//glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
	//glLightfv(GL_LIGHT1, GL_SPECULAR, SpecularLight);
	//glEnable(GL_LIGHT1);

	//glEnable(GL_COLOR_MATERIAL);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularLight);
	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	size_t VecSize = m_vecParticle.size();
	for (size_t i = 0; i < VecSize; ++i)
	{
		m_vecParticle[i]->Render();
	}
}

void CFireWork::Release()
{
}
