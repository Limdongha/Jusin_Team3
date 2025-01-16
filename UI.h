#pragma once
#include "Object.h"
class CUIMgr;


class CUI :  public CObject
{
public:
	CUI();
	virtual ~CUI();

public:

	virtual void Update();
	virtual void Render();
	virtual void Initialize();
	virtual void LateUpdate();

public:
	//마우스 연동 함수
	virtual void MouseOn();
	virtual void MouseLDown();
	virtual void MouseLUp();
	virtual void MouseLClicked();

private:
	//자식 UI 들을 담는 벡터
	vector<CUI*>	m_vecChildUI;

	//부모를 담는 포인터
	CUI*			m_pParnetUI;

	//최종 UI 위치
	tVec2			m_vFinalPos;

public:
	//부모 주소 겟함수
	CUI* GetParent() { return m_pParnetUI; }

	//이 객체를 부모로 정해주는 함수 
	void	AddParent(CUI* _pUI) { m_vecChildUI.push_back(_pUI); _pUI->m_pParnetUI = this; }
	void	SetPinalPos(tVec2 _tFin) { m_vFinalPos = _tFin; }
	
	tVec2	GetPinalPos()	{ return m_vFinalPos; }
	bool	IsMouseOn()		{ return m_bMouseOn; }
	bool	IsbLDown()		{ return m_bLDown; }

	vector<CUI*>& GetChildUI() { return m_vecChildUI; }

public:
	void MouseOnCheck();
	void UpdateChild();
	void RenderChild();
	void LateUpdateChild();

public:
	void SetVisible(bool visible)	{ m_bVisible = visible; }
	bool IsVisible() const			{ return m_bVisible; }

protected:
	bool m_bMouseOn;
	bool m_bLDown;
	bool m_bVisible;


	friend class CUIMgr;
};

