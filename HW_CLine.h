#pragma once
#include "Define.h"

class HW_CLine
{

public:
	HW_CLine();
	HW_CLine(LINEPOINT& tLeft, LINEPOINT& tRight);
	~HW_CLine();

public:
	LINE Get_Info() { return m_tInfo; }


	void Render();


private:
	LINE m_tInfo;



};

