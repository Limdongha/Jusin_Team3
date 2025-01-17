#pragma once

#include "Object.h"

class KL_CPlayer : public CObject
{
public:
	KL_CPlayer();
	~KL_CPlayer();


public:
	virtual void	Render() override;
	virtual void	Update() override;
	virtual void	Initialize() override;
	virtual void	LateUpdate() override;

public:
	virtual void	KeyInput();
	virtual void	Change_Motion();
	void			Offset();
	void			JumpIng();
	void			SetTarget(CObject* _pObj) { m_pTarget = _pObj; }
	


	bool        m_bCol;

private:
	// point를 normal로 해서 이동 안먹히게, pos만 계속 이동하는 식으로
	D3DXVECTOR3     m_vPoint[4];
	D3DXVECTOR3     m_vOriginPoint[4];

	D3DXVECTOR3     m_vGunPoint;
	D3DXVECTOR3     m_vOriginGunPoint;

	D3DXVECTOR3		m_vOriginPos;

	CObject* m_pTarget; // 충돌할경우 충돌한 posv에 따라서 공전하도록
};
