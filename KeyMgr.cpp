#include "pch.h"
#include "Define.h"
#include "KeyMgr.h"

int g_arrVK[static_cast<int>(KEY::LAST)]
{
	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
	'A', 'S', 'D', 'F', 'G', 'Z', 'X', 'C', 'V', 'B',
	VK_MENU, VK_CONTROL, VK_LSHIFT, VK_SPACE, VK_RETURN, VK_TAB, VK_F1, VK_ESCAPE,
	VK_LBUTTON, VK_RBUTTON
	//LAST
};


Ckeymgr::Ckeymgr()
{

}
Ckeymgr::~Ckeymgr()
{

}

void Ckeymgr::Initialize()
{
	for (int i = 0; i < static_cast<int>(KEY::LAST); ++i)
	{
		m_veckey.push_back(tKey_Info{ KEY_STATE::NONE, false });
	}
}

void Ckeymgr::Update()
{
	HWND hWnd = GetFocus();
	if (hWnd != nullptr)
	{
		for (int i = 0; i < static_cast<int>(KEY::LAST); ++i)
		{
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)
			{
				// 그 전에도 눌려있었다?
				if (m_veckey[i].bPrev)
				{
					m_veckey[i].eState = KEY_STATE::HOLD;
				}
				else
				{
					//이 전에 눌려있지않았다?
					m_veckey[i].eState = KEY_STATE::TAP;
				}
				m_veckey[i].bPrev = true;
			}
			else
			{
				//아무것도 안눌렀을때
				if (m_veckey[i].bPrev)
				{
					//이전에 눌렸었다? 지금 막 뗌
					m_veckey[i].eState = KEY_STATE::AWAY;
				}
				else
				{
					//아무것도 안누르고 그 전에도 안눌림
					m_veckey[i].eState = KEY_STATE::NONE;
				}
				m_veckey[i].bPrev = false;
			}
		}
	}
	else
	{
		//이 윈도우를 보고 있지않을 때
		for (int i = 0; i < static_cast<int>(KEY::LAST); ++i)
		{
			m_veckey[i].bPrev = false;
			if (m_veckey[i].eState == KEY_STATE::TAP || m_veckey[i].eState == KEY_STATE::AWAY)
			{
				m_veckey[i].eState = KEY_STATE::AWAY;
			}
			else if (m_veckey[i].eState == KEY_STATE::AWAY)
			{
				m_veckey[i].eState = KEY_STATE::NONE;
			}
		}
	}
}
