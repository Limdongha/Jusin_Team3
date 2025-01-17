#pragma once

#include "Object.h"


class CJumpPad : public CObject
{
public:
	CJumpPad();
	~CJumpPad();

public:
	virtual void	Render() override;
	virtual void	Update() override;
	virtual void	Initialize() override;
	virtual void	LateUpdate() override;
	
	void			SetTargetPos(D3DXVECTOR3 _vTargetPos) { m_vTargetPos = _vTargetPos; }

private:

	D3DXVECTOR3     m_vGunPoint;
	D3DXVECTOR3     m_vOriginGunPoint;

	D3DXVECTOR3     m_vTargetPos;
};
