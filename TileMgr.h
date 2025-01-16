#pragma once
#include "Tile.h"

class CTileMgr
{
	SINGLE(CTileMgr)

public:
	void Initialize() ;
	void Update() ;
	void Late_Update() ;
	void Render() ;
	void Release() ;

	void		Picking_Tile(POINT pt, int iDrawID, int iOption);

	void		Save_Tile();
	void		Load_Tile(const wchar_t* _Tile);


public:
	vector<CObject*>*	GetvecTile() { return &m_vecTile; }

private:

	vector<CObject*>	m_vecTile;

};

