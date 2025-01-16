#pragma once
#include "Scene.h"

class CTextures;

class CScene_Rogo :  public CScene
{
public:
	CScene_Rogo();
	virtual ~CScene_Rogo();

public:
	virtual void		Enter() override;
	virtual void		Exit() override;
	virtual void		Update() override;
	virtual void		Render() override;

private:
	CTextures*		m_Rogo;
	Image*			m_Text;

private:
	steady_clock::time_point 		m_tStartTime;
	steady_clock::time_point		m_tCurTime;
	float							m_fText;
};

