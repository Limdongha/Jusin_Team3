#pragma once
#include "Object.h"

class HW_CMonster : public CObject
{
public:
	HW_CMonster();
	virtual ~HW_CMonster();


public:
	void		Initialize()override;
	void		Update()override;
	void		LateUpdate()override;
	void		Render()override;
	void		Release()override;

public:
	void Jumping();

public:
	float Get_Radius() { return m_fMonsterRadius; }
	float Get_fTopY() { return m_fTopY; }
	float Get_fBottomY() { return m_fBottomY; }
	


	float m_fTopY;
	float m_fBottomY;

private:
	D3DXVECTOR3	m_vPoint[4];
	D3DXVECTOR3 m_vTemp[4];

private:
	float m_fSpeed;
	float m_fMonsterRadius;


	TCHAR m_szPlayerHP[32];
	TCHAR m_szPoint[32];
};

