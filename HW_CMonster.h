#pragma once
#include "Object.h"

class HW_CMonster : public CObject
{
public:
	HW_CMonster();
	~HW_CMonster();


public:
	void		Initialize()override;
	void		Update()override;
	void		LateUpdate()override;
	void		Render()override;
	void		Release()override;

};

