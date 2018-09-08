
#include "IncludeHeader.h"
#include "MainFrame.h"
#include "InputMgr.h"

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid TimerFunction(int value);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid MousePosition(int x, int y);

float fTime = 10.f;	//�������� �ð�(�и������� 1�� = 1000�и���)
CMainFrame* mainFrame;
#define MAIN_QUIT -1

CInputMgr*		pInputMgr = CInputMgr::GetInstance();
void main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));

	mainFrame = new CMainFrame();
	mainFrame->Initialize();

	//�ʱ�ȭ �Լ���
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // ���÷��� ��� ����
	glutInitWindowPosition(100, 100); // �������� ��ġ����
	glutInitWindowSize(WINSIZEX, WINSIZEY); // �������� ũ�� ����
	glutCreateWindow("FrameWork!"); // ������ ���� (������ �̸�)

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ��� ����
	glutTimerFunc(fTime, TimerFunction, 1); // Ÿ�̸� �Լ� ���� 

	glutMouseFunc(Mouse);
	glutPassiveMotionFunc(MousePosition);
	glutMotionFunc(MousePosition);
	glutKeyboardFunc(Keyboard);

	glutMainLoop();
}
GLvoid TimerFunction(int value)
{
	// ���������� ������Ʈ ����
	//if (MAIN_QUIT == mainFrame->Update(fTime * 0.001f))
		//mainFrame->Release();
	mainFrame->Update(fTime * 0.001f);
	glutPostRedisplay();
	glutTimerFunc(fTime, TimerFunction, 1); // Ÿ�̸� �Լ� ���� 
}
GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mainFrame->InputMousePoint(x - WINSIZEX * 0.5f, WINSIZEY * 0.5f - y);
	}
}
GLvoid	MousePosition(int x, int y)
{
	pInputMgr->UpdateInput(x, y);
}
GLvoid Keyboard(unsigned char key, int x, int y)
{

}
// ������ ��� �Լ�
GLvoid drawScene(GLvoid)
{
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f); // �������� 'blue' �� ����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glClearDepth(1.f);

	// ���������� ������Ʈ ����  //
	mainFrame->Render();
	/////////////////////////////

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutSwapBuffers();
}
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glFrustum(-1.0, 1.0, -1.0, 1.0, -1.5, 20.0);
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 30000.0);
	//glTranslatef(0.f,50.f, -50);
	//glOrtho(WINSIZEX * -0.5f , WINSIZEX * 0.5f, WINSIZEY * -0.5f, WINSIZEY * 0.5f, -5000.0f, 10000.0f);
	glMatrixMode(GL_MODELVIEW);
}