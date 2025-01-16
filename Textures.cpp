#include "pch.h"
#include "Textures.h"

CTextures::CTextures() : m_hDC(0), m_hBit(0)
{
}

CTextures::~CTextures()
{
	DeleteDC(m_hDC);
	DeleteObject(m_hBit);
}

void CTextures::Load(const TCHAR* _strFilePath)
{
	//파일로부터 로딩한 데이터로 비트맵 생성
	m_hBit = (HBITMAP)LoadImage(
		nullptr,
		_strFilePath,
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	assert(m_hBit);																	

	//비트맵과 연결할 DC
	m_hDC = CreateCompatibleDC(g_memDC);

	//비트맵과 연결
	HBITMAP hPreBit = (HBITMAP)SelectObject(m_hDC, m_hBit);
	DeleteObject(hPreBit);

	//비트맵 정보
	GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);
}
