#pragma once
#include "Scene.h"
class CScene_RUNRUN :  public CScene
{
public:
	CScene_RUNRUN();
	virtual ~CScene_RUNRUN();

public:
	virtual void		Enter() override;
	virtual void		Exit() override;
	virtual void		Update() override;
	virtual void		Render() override;


private:
	steady_clock::time_point 		m_tStartTime;
	steady_clock::time_point		m_tCurTime;


	CTextures* m_pBG2;
	int		m_iBG1;
	int		m_iBG2;

};

