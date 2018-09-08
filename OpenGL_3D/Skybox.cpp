#include "Skybox.h"



CSkybox::CSkybox()
{
}


CSkybox::~CSkybox()
{
}

void CSkybox::Initialize()
{
	vRenderMax = MyVector(5000.f, 8000.f, 5000.f);
	vRenderMin = MyVector(-5000.f, -2000.f, -5000.f);

	pBytes[0] = LoadDIBitmap("Skybox_Front.bmp", &info);
	pBytes[1] = LoadDIBitmap("Skybox_Right.bmp", &info);
	pBytes[2] = LoadDIBitmap("Skybox_Back.bmp", &info);
	pBytes[3] = LoadDIBitmap("Skybox_Left.bmp", &info);
	pBytes[4] = LoadDIBitmap("Skybox_Top.bmp", &info);
	pBytes[5] = LoadDIBitmap("Skybox_Bottom.bmp", &info);
}

int CSkybox::Update(float fTime)
{
	return 0;
}

void CSkybox::Render()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 텍스처 모드 설정
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
	// 텍스처 매핑 활성화
	glEnable(GL_TEXTURE_2D);

	glColor3f(1.0f, 1.0f, 1.0f);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 256, 256, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes[0]);
	glBegin(GL_POLYGON);
	//앞면 반시계
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(vRenderMax.x, vRenderMin.y, vRenderMin.z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(vRenderMax.x, vRenderMax.y, vRenderMin.z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(vRenderMin.x, vRenderMax.y, vRenderMin.z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(vRenderMin.x, vRenderMin.y, vRenderMin.z);
	glEnd();

	glTexImage2D(GL_TEXTURE_2D, 0, 3, 256, 256, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes[1]);
	glBegin(GL_POLYGON);
	//우측면
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(vRenderMax.x, vRenderMin.y, vRenderMax.z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(vRenderMax.x, vRenderMax.y, vRenderMax.z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(vRenderMax.x, vRenderMax.y, vRenderMin.z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(vRenderMax.x, vRenderMin.y, vRenderMin.z);
	glEnd();

	glTexImage2D(GL_TEXTURE_2D, 0, 3, 256, 256, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes[2]);
	glBegin(GL_POLYGON);
	//뒷면
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(vRenderMin.x, vRenderMin.y, vRenderMax.z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(vRenderMin.x, vRenderMax.y, vRenderMax.z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(vRenderMax.x, vRenderMax.y, vRenderMax.z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(vRenderMax.x, vRenderMin.y, vRenderMax.z);
	glEnd();

	glTexImage2D(GL_TEXTURE_2D, 0, 3, 256, 256, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes[3]);
	glBegin(GL_POLYGON);
	//좌측면
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(vRenderMin.x, vRenderMin.y, vRenderMax.z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(vRenderMin.x, vRenderMax.y, vRenderMax.z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(vRenderMin.x, vRenderMax.y, vRenderMin.z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(vRenderMin.x, vRenderMin.y, vRenderMin.z);
	glEnd();

	glTexImage2D(GL_TEXTURE_2D, 0, 3, 256, 256, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes[4]);
	glBegin(GL_POLYGON);
	//윗면
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(vRenderMax.x, vRenderMax.y, vRenderMin.z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(vRenderMax.x, vRenderMax.y, vRenderMax.z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(vRenderMin.x, vRenderMax.y, vRenderMax.z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(vRenderMin.x, vRenderMax.y, vRenderMin.z);
	glEnd();

	glTexImage2D(GL_TEXTURE_2D, 0, 3, 256, 256, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes[5]);
	glBegin(GL_POLYGON);
	//아랫면
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(vRenderMin.x, vRenderMin.y, vRenderMin.z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(vRenderMin.x, vRenderMin.y, vRenderMax.z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(vRenderMax.x, vRenderMin.y, vRenderMax.z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(vRenderMax.x, vRenderMin.y, vRenderMin.z);
	glEnd();

}

void CSkybox::Release()
{

}

GLubyte * CSkybox::LoadDIBitmap(const char * filename, BITMAPINFO ** info)
{
	FILE *fp;
	GLubyte *bits;
	int bitsize, infosize;
	BITMAPFILEHEADER header;
	// 바이너리 읽기 모드로 파일을 연다
	if ((fp = fopen(filename, "rb")) == NULL)
		return NULL;
	// 비트맵 파일 헤더를 읽는다.
	if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1) {
		fclose(fp);
		return NULL;
	}
	// 파일이 BMP 파일인지 확인한다.
	if (header.bfType != 'MB') {
		fclose(fp);
		return NULL;
	}
	// BITMAPINFOHEADER 위치로 간다.
	infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
	// 비트맵 이미지 데이터를 넣을 메모리 할당을 한다.
	if ((*info = (BITMAPINFO *)malloc(infosize)) == NULL) {
		fclose(fp);
		exit(0);
		return NULL;
	}
	// 비트맵 인포 헤더를 읽는다.
	if (fread(*info, 1, infosize, fp) < (unsigned int)infosize) {
		free(*info);
		fclose(fp);
		return NULL;
	}
	// 비트맵의 크기 설정
	if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)
		bitsize = ((*info)->bmiHeader.biWidth *
		(*info)->bmiHeader.biBitCount + 7) / 8.0 *
		abs((*info)->bmiHeader.biHeight);
	// 비트맵의 크기만큼 메모리를 할당한다.
	if ((bits = (unsigned char *)malloc(bitsize)) == NULL) {
		free(*info);
		fclose(fp);
		return NULL;
	}
	// 비트맵 데이터를 bit(GLubyte 타입)에 저장한다.
	if (fread(bits, 1, bitsize, fp) < (unsigned int)bitsize) {
		free(*info); free(bits);
		fclose(fp);
		return NULL;
	}
	fclose(fp);
	return bits;
}
