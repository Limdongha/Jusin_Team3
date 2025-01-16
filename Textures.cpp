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
	//���Ϸκ��� �ε��� �����ͷ� ��Ʈ�� ����
	m_hBit = (HBITMAP)LoadImage(
		nullptr,
		_strFilePath,
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	assert(m_hBit);																	

	//��Ʈ�ʰ� ������ DC
	m_hDC = CreateCompatibleDC(g_memDC);

	//��Ʈ�ʰ� ����
	HBITMAP hPreBit = (HBITMAP)SelectObject(m_hDC, m_hBit);
	DeleteObject(hPreBit);

	//��Ʈ�� ����
	GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);
}
