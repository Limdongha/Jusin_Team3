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
	//���콺 ���� �Լ�
	virtual void MouseOn();
	virtual void MouseLDown();
	virtual void MouseLUp();
	virtual void MouseLClicked();

private:
	//�ڽ� UI ���� ��� ����
	vector<CUI*>	m_vecChildUI;

	//�θ� ��� ������
	CUI*			m_pParnetUI;

	//���� UI ��ġ
	tVec2			m_vFinalPos;

public:
	//�θ� �ּ� ���Լ�
	CUI* GetParent() { return m_pParnetUI; }

	//�� ��ü�� �θ�� �����ִ� �Լ� 
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

