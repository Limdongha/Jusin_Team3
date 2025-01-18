#include "pch.h"
#include "HW_CLineMgr.h"

HW_CLineMgr* HW_CLineMgr:: m_pInstance = nullptr;


HW_CLineMgr::HW_CLineMgr()
{
}

HW_CLineMgr::~HW_CLineMgr()
{
	Release();
}



void HW_CLineMgr::Initialize()
{
	LINEPOINT tLinePoint[4]
	{
		{ 0.f , 450.f},
		{ 400.f , 450.f},
		{ 700.f , 450.f },
		{ 1000.f , 450.f }
	};

	m_Linelist.push_back(new HW_CLine(tLinePoint[0], tLinePoint[1]));
	m_Linelist.push_back(new HW_CLine(tLinePoint[1], tLinePoint[2]));
	m_Linelist.push_back(new HW_CLine(tLinePoint[2], tLinePoint[3]));

}

void HW_CLineMgr::Render()
{
	for (auto& pLine : m_Linelist)
		pLine->Render();
}

void HW_CLineMgr::Release()
{
	for_each(m_Linelist.begin(), m_Linelist.end(), Safe_Delete<HW_CLine*>);
	m_Linelist.clear();
}

bool HW_CLineMgr::Collision_Line(float _fX, float* pY)
{

	if (m_Linelist.empty())
		return false;

	HW_CLine* pTargetLine = nullptr;

	for (auto& pLine : m_Linelist)
	{
		if (_fX >= pLine->Get_Info().tLPoint.fX &&
			_fX < pLine->Get_Info().tRPoint.fX)
		{
			pTargetLine = pLine;
		}
	}

	if (!pTargetLine)
		return false;


	float	x1 = pTargetLine->Get_Info().tLPoint.fX;
	float	y1 = pTargetLine->Get_Info().tLPoint.fY;
	float	x2 = pTargetLine->Get_Info().tRPoint.fX;
	float	y2 = pTargetLine->Get_Info().tRPoint.fY;

	*pY = ((y2 - y1) / (x2 - x1)) * (_fX - x1) + y1 - 30;

	return true;
}