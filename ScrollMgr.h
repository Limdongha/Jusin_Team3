#pragma once
#include "Define.h"

class CScrollMgr
{
private:
	CScrollMgr();
	~CScrollMgr();

public:
	void			Update();

public:
	void			SetbShake01() { m_bShake01 = true; }
	bool			GetbShake01() { return m_bShake01; }
	void			SetbShake02() { m_bShake02 = true; }
	bool			GetbShake02() { return m_bShake02; }
	void			SetbShake03() { m_bShake03 = true; }
	bool			GetbShake03() { return m_bShake03; }

public:
	float			Get_ScrollX() { return m_fScrollX; }
	float			Get_ScrollY() { return m_fScrollY; }

	void			Add_ScrollX(float _fX) { m_fScrollX += _fX; }
	void			Add_ScrollY(float _fY) { m_fScrollY += _fY; }

	void			Set_ScrollX(float _fX) { m_fScrollX = _fX; }
	void			Set_ScrollY(float _fY) { m_fScrollY = _fY; }

	void			Scroll_Lock(float _LockX, float _LockY);


	void			Scroll_Shake01();
	void			Scroll_Shake02();
	void			Scroll_Shake03();

	void			Scroll_MoveX(float _fX);

public:
	static CScrollMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CScrollMgr;

		return m_pInstance;
	}

	static void	Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CScrollMgr* m_pInstance;

	float			    m_fScrollX;
	float			    m_fScrollY;
	int					m_iTime;
	int					m_iShakeTime01;
	int					m_iShakeTime02;
	int					m_iShakeTime03;
	bool				m_bShake01;
	bool				m_bShake02;
	bool				m_bShake03;
	float				m_fShakeTimeElapsed;
};

