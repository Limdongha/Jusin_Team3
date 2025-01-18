#pragma once
#include "Object.h"

class HW_CPlayer : public CObject
{
public:
	HW_CPlayer();
	virtual ~HW_CPlayer();

public:
	void		Initialize()override;
	void		Update()override;
	void		LateUpdate()override;
	void		Render()override;
	void		Release()override;

private:
	void Key_Input();
	void Jumping();
	void OffSet();

private:
	D3DXVECTOR3	m_vPoint[4];
	D3DXVECTOR3 m_vTemp[4];
	
	
	float m_fAngle;
	float m_fSpeed;
	float m_fJumpPower;
	float m_fTime;
	


	bool m_bJump;

};

