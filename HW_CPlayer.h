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
	void Jumping();


public:
	float Get_Radius() { return fPlayerRadius; }
	int Get_JumpCount() { return m_iJumpCount; }
	float Get_JumpPower() { return m_fJumpPower; }
	const D3DXVECTOR3* Get_Temp() const { return m_vTemp; }


	
	void Set_JumpCount(int _JumpCount) { m_iJumpCount = _JumpCount; }
	void Set_JumpPower(int _JumpPower) { m_fJumpPower = _JumpPower; }
	void SetbDoubleJump(bool _bDoubleJump) { m_bDoubleJump = _bDoubleJump; }

private:
	void Key_Input();
	void OffSet();

public:
	static bool m_bColliderBox;

private:
	D3DXVECTOR3	m_vPoint[4];
	D3DXVECTOR3 m_vTemp[4];


	float m_fAngle;
	float m_fSpeed;
	float m_fJumpPower;
	float fPlayerRadius;
	bool  m_bDoubleJump;

	int m_iJumpCount;
};

