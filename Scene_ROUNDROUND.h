#pragma once
#include "Scene.h"
class CScene_ROUNDROUND : public CScene
{
public:
	CScene_ROUNDROUND();
	virtual ~CScene_ROUNDROUND();

public:
	virtual void		Enter() override;
	virtual void		Exit() override;
	virtual void		Update() override;
	virtual void		Render() override;


private:
	steady_clock::time_point 		m_tStartTime;
	steady_clock::time_point		m_tCurTime;
};

