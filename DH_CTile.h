#pragma once
#include "Object.h"
class DH_CTile : public CObject
{
public:
	DH_CTile();
	virtual ~DH_CTile();

public:
	int			Get_DrawID() { return m_iDrawID; }
	int			Get_Option() { return m_iOption; }

	void		Set_DrawID(int _iDrawID) { m_iDrawID = _iDrawID; }
	void		Set_Option(int _iOption) { m_iOption = _iOption; }
	void		Set_UniqTile(bool _Uniq) { m_UniqTile = _Uniq; }

public:
	virtual void	Initialize() override;
	virtual void	Release() override;
	virtual void	Update();
	virtual void	LateUpdate();
	virtual void	Render();

public:
	void AlphaBlend(Image* _pImage, float _alpha);

private:
	int		m_iDrawID;
	int		m_iOption;

	//로컬 , 월드 좌표
	INFO			m_vTileInfo;
	D3DXVECTOR3		m_vTilePos;
	D3DXVECTOR3		m_vTileOriginPos;


private:
	bool		m_UniqTile;
};

