#pragma once
#include "Source.h"

class CResMgr; //전방선언

class CTextures : public CSource
{
private:
	HDC			m_hDC;
	HBITMAP		m_hBit;
	BITMAP		m_bitInfo;

public:
	void		Load(const TCHAR* _strFilePath);

public:
	UINT		GetWidth()  { return m_bitInfo.bmWidth; }
	UINT		GetHeight() { return m_bitInfo.bmHeight; }
	HDC			GetDC()		{ return m_hDC; }

private:
	CTextures();
	~CTextures();

	friend class CResMgr;
};

