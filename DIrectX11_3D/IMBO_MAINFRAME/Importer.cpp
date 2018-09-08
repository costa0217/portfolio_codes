#include "stdafx.h"
#include "Importer.h"

bool CImporter::Begin(string outputPath) {
	m_inputPath = outputPath;
	m_in = ifstream(m_inputPath, ios::in);

	m_pFileImporter = new CTxtImporter();

	return true;
}

bool CImporter::End() {
	m_in.close();

	delete m_pFileImporter;
	m_pFileImporter = nullptr;

	return true;
}

//const WCHAR* CImporter::ReadWCHAR() {
//	wstring ws = m_pFileImporter->ReadWstring(m_in);
//	return ws.c_str();
//}
//
//const char* CImporter::ReadCHAR() {
//	wstring ws = m_pFileImporter->ReadWstring(m_in);
//	string s;
//	s.assign(ws.begin(), ws.end());
//	return s.c_str();
//}
const wstring CImporter::ReadWstring() {
	string s = m_pFileImporter->Readstring(m_in);
	wstring ws{ L"" };
	ws.assign(s.cbegin(), s.cend());
	return ws;
}

const string CImporter::Readstring() {
	string s = m_pFileImporter->Readstring(m_in);
	return s;
}
UINT CImporter::ReadUINT() {
	return m_pFileImporter->ReadUINT(m_in);
}

int CImporter::ReadInt() {
	return m_pFileImporter->ReadInt(m_in);
}

float CImporter::ReadFloat() {
	return m_pFileImporter->ReadFloat(m_in);
}

XMFLOAT4X4 CImporter::ReadFloat4x4() {
	XMFLOAT4X4 xmf4x4;
	xmf4x4._11 = ReadFloat(); xmf4x4._12 = ReadFloat(); xmf4x4._13 = ReadFloat(); xmf4x4._14 = ReadFloat();
	xmf4x4._21 = ReadFloat(); xmf4x4._22 = ReadFloat(); xmf4x4._23 = ReadFloat(); xmf4x4._24 = ReadFloat();
	xmf4x4._31 = ReadFloat(); xmf4x4._32 = ReadFloat(); xmf4x4._33 = ReadFloat(); xmf4x4._34 = ReadFloat();
	xmf4x4._41 = ReadFloat(); xmf4x4._42 = ReadFloat(); xmf4x4._43 = ReadFloat(); xmf4x4._44 = ReadFloat();
	return xmf4x4;
}

XMMATRIX CImporter::ReadXMMatrix() {
	return XMLoadFloat4x4(&ReadFloat4x4());
}

bool CImporter::ReadBool(){
	return m_pFileImporter->ReadBool(m_in);
}

void CImporter::ReadSpace() {
	m_pFileImporter->ReadSpace(m_in);
}

void CImporter::ReadEnter() {
	m_pFileImporter->ReadEnter(m_in);
}

Pixel24 * CImporter::ReadBitmap24(const WCHAR * fileName){
	BITMAPFILEHEADER    bf;
	BITMAPINFOHEADER    bi;

	ifstream in;
	in.open(fileName, ios_base::in | ios_base::binary);
	in.read((char*)&bf, sizeof(BITMAPFILEHEADER));
	in.read((char*)&bi, sizeof(BITMAPINFOHEADER));
	UINT BUFSIZE = bi.biWidth *	bi.biHeight;
	Pixel24* pPixel = new Pixel24[BUFSIZE];
	in.read((char*)pPixel, BUFSIZE*3);
	Pixel24* rgbTemp = new Pixel24[BUFSIZE];
	for (int i = 0; i < BUFSIZE; ++i) {
		rgbTemp[i].r = pPixel[BUFSIZE - 1 - i].r;
		rgbTemp[i].g = pPixel[BUFSIZE - 1 - i].g;
		rgbTemp[i].b = pPixel[BUFSIZE - 1 - i].b;
	}
	delete pPixel;

	in.close();

	return rgbTemp;
}

Pixel24 * CImporter::ReadBitmap24(const char * fileName){
	BITMAPFILEHEADER    bf;
	BITMAPINFOHEADER    bi;

	ifstream in;
	in.open(fileName, ios_base::in | ios_base::binary);
	if (in.fail()) return nullptr;

	in.read((char*)&bf, sizeof(BITMAPFILEHEADER));
	in.read((char*)&bi, sizeof(BITMAPINFOHEADER));
	Pixel24* pPixel = new Pixel24[bi.biWidth*bi.biHeight];
	in.read((char*)pPixel, bi.biSizeImage);
	in.close();

	//UINT BUFSIZE = bi.biWidth*bi.biHeight;
	//Pixel24* rgbTemp = new Pixel24[BUFSIZE];
	//for (int i = 0; i < BUFSIZE; ++i) {
	//	rgbTemp[i].r = pPixel[BUFSIZE - 1 - i].r;
	//	rgbTemp[i].g = pPixel[BUFSIZE - 1 - i].g;
	//	rgbTemp[i].b = pPixel[BUFSIZE - 1 - i].b;
	//}
	//delete[] pPixel;

	return pPixel;
}

Pixel32 * CImporter::ReadBitmap32(const WCHAR * fileName){
	//BITMAPFILEHEADER    bf;
	//BITMAPINFOHEADER    bi;
	//
	//ifstream in;
	//in.open(fileName, ios_base::in | ios_base::binary);
	//in.read((char*)&bf, sizeof(BITMAPFILEHEADER));
	//in.read((char*)&bi, sizeof(BITMAPINFOHEADER));
	//char* pData = new char[bi.biSizeImage];
	//Pixel32* pPixel = new Pixel32[bi.biWidth*bi.biHeight];
	//in.read((char*)pPixel, bi.biSizeImage);
	//in.close();
	//
	return nullptr;
}
