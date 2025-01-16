#pragma once

class CEventMgr
{
	SINGLE(CEventMgr)

public:
	void		Update();
	void		Initialize();
	void		Release();
	void		Render();

public:
	void		AddEvent(const tEvent& _eve) { m_vecEvent.push_back(_eve); }
	void		Excute(const tEvent& _eve);

private:
	vector<tEvent>		m_vecEvent;
	vector<CObject*>	m_vecDead;

};

