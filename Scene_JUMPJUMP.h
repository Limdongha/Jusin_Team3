#pragma once
#include "Scene.h"
class CScene_JUMPJUMP : public CScene
{
public:
	CScene_JUMPJUMP();
	virtual ~CScene_JUMPJUMP();

public:
	virtual void		Enter() override;
	virtual void		Exit() override;
	virtual void		Update() override;
	virtual void		Render() override;


private:
	steady_clock::time_point 		m_tStartTime;
	steady_clock::time_point		m_tCurTime;
};

