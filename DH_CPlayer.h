#pragma once
#include "Object.h"


class DH_CPlayer :  public CObject
{
public:
	DH_CPlayer();
	virtual ~DH_CPlayer();

public:
	virtual void	Render() override;
	virtual void	Update() override;
	virtual void	Initialize() override;
	virtual void	LateUpdate() override;

public:
	virtual void	KeyInput();
	virtual void	Change_Motion();
	void			Offset();

private:
	INFO			m_vPlayerInfo;
	D3DXVECTOR3		m_vPoint[4];
	D3DXVECTOR3		m_vOriginPoint[4];
};

