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


public:
	int Get_Point() { return m_iPoint; }
	bool Get_GameOver() { return m_bGameOver; }



	void Set_Point(int _iPoint) { m_iPoint = _iPoint; }
	void Set_GameOver(bool _bGameOver) { m_bGameOver = _bGameOver; }



private:
	steady_clock::time_point 		m_tStartTime;
	steady_clock::time_point		m_tCurTime;


	CTextures* m_pBG2;
	int		m_iBG1;
	int		m_iBG2;

	int		m_iPoint;
	bool	m_bGameOver;
};

