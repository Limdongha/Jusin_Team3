#pragma once
#include "Scene.h"


class CScene_Start :  public CScene
{
public:
	CScene_Start();
	virtual ~CScene_Start();

public:
	virtual void		Enter() override;
	virtual void		Exit() override;
	virtual void		Update() override;
	virtual void		Render() override;

public:
	CTextures*	m_pDoor;
	CTextures*	m_pNPC_Girl;
	CTextures*	m_pKey;

private:
	FRAME		m_tGirl;
};

