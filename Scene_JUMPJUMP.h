#pragma once
#include "Scene.h"
class CTextures;

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


public:
	//타일 생성
	void				CreateRandomTile();
	void				CheckAndCreateTiles();
	void				UpdateMaxHeight();
	void				DeleteTile();


private:
	steady_clock::time_point 		m_tStartTime;
	steady_clock::time_point		m_tCurTime;

private:
	CTextures* m_pBG2;
	int		m_iMaxHeight;
	int		m_iTileY;
	int		m_iTileNum;
	int		m_iBG1;
	int		m_iBG2;
	bool	m_bExcuseOne;

};

