
#include "IncludeHeader.h"
#include "MainFrame.h"
#include "InputMgr.h"

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid TimerFunction(int value);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid MousePosition(int x, int y);

float fTime = 10.f;	//한프레임 시간(밀리세컨드 1초 = 1000밀리초)
CMainFrame* mainFrame;
#define MAIN_QUIT -1

CInputMgr*		pInputMgr = CInputMgr::GetInstance();
void main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));

	mainFrame = new CMainFrame();
	mainFrame->Initialize();

	//초기화 함수들
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치지정
	glutInitWindowSize(WINSIZEX, WINSIZEY); // 윈도우의 크기 지정
	glutCreateWindow("FrameWork!"); // 윈도우 생성 (윈도우 이름)

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수의 지정
	glutTimerFunc(fTime, TimerFunction, 1); // 타이머 함수 설정 

	glutMouseFunc(Mouse);
	glutPassiveMotionFunc(MousePosition);
	glutMotionFunc(MousePosition);
	glutKeyboardFunc(Keyboard);

	glutMainLoop();
}
GLvoid TimerFunction(int value)
{
	// 메인프레임 업데이트 구문
	//if (MAIN_QUIT == mainFrame->Update(fTime * 0.001f))
		//mainFrame->Release();
	mainFrame->Update(fTime * 0.001f);
	glutPostRedisplay();
	glutTimerFunc(fTime, TimerFunction, 1); // 타이머 함수 설정 
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
// 윈도우 출력 함수
GLvoid drawScene(GLvoid)
{
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f); // 바탕색을 'blue' 로 지정
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glClearDepth(1.f);

	// 메인프레임 업데이트 구문  //
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