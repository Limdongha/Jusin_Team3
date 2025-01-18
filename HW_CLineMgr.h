#pragma once
#include "HW_CLine.h"

class HW_CLineMgr
{

private:
	HW_CLineMgr();
	~HW_CLineMgr();

public:
	void	Initialize();
	void	Render();
	void	Release();

public:
	bool Collision_Line(float _fX, float* pY);

public:
	static HW_CLineMgr* Get_Instance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new HW_CLineMgr;
		}

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
	static HW_CLineMgr* m_pInstance;

	list<HW_CLine*> m_Linelist;


};

