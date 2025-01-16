#pragma once
#include "Scene.h"
class CScene_Tool :  public CScene
{
public:
	virtual void Render();
	virtual void Enter();
	virtual void Exit();
	virtual void Update();

public:
	CScene_Tool();
	virtual ~CScene_Tool();

public:
	POINT	m_tBlockPoint[DRAWPOINT_END];
	CTextures* m_pTool;
};

