#pragma once
#include "Object.h"

class CMonster :  public CObject
{
public:
	CMonster();
	virtual ~CMonster();

public:
	float	GetfAttackRange() { return fAttackRange; }
	bool	GetbHit() { return m_bHit; }

public:
	void	SetfAttackRange(float _ARange) { fAttackRange = _ARange; }
	void	SetbHit(bool _bHit) { m_bHit = _bHit; }

private:
	float		fAttackRange;
	bool		m_bHit;
};

