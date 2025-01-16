#pragma once
#include "Define.h"
class CTextures;

class CMainGame
{
SINGLE(CMainGame)

public:
	void		Progress(HDC _hDC);

public:
	void		Update();
	void		LateUpdate();
	void		Render(HDC _hDC);
	void		Initialize(HDC _hDC);
	void		Release();

public:
	HBITMAP		m_hBit;

private:
	ULONG64		m_dwTime;
	int			m_iFPS;
	TCHAR		m_wcBuff[32];

public:
	//콜리젼 확인 ON/OFF
	int			GetiEventBox() { return m_iEventBox; }
	void		SetiEventBox(int _ev) { m_iEventBox = _ev; }

private:
	CTextures*	m_pBGTex;
	int			m_iEventBox;
};

