#pragma once

class CTextures;

class CMouseMgr
{
	SINGLE(CMouseMgr)

public:
	void		Update();
	void		Render();
	void		Initialize();
	void		Release();

public:
	POINT		GetPos() { return m_tPos; }

public:
	void		SetPos(POINT _pos) { m_tPos.x = _pos.x; m_tPos.y = _pos.y; }

public:
	float		CulMouseDistance(POINT _mx,tVec2 _px);

private:
	POINT		m_tPos;
	CTextures*  m_pMouseTex;
};

