#include "pch.h"
#include "UIMgr.h"
#include "SceneMgr.h"
#include "KeyMgr.h"
#include "UI.h"
#include "PanelUI.h"
#include "BtnUI.h"
#include "Player.h"
#include "SceneMgr.h"

CUIMgr::CUIMgr() : m_FocusedUI(nullptr) , m_UION(true)
{

}

CUIMgr::~CUIMgr()
{
	//UI �Ҹ��ϱ�
	auto& UI = CSceneMgr::GetInst()->GetCurScene()->GetvSceneObj()[(int)eObjectType::UI];
	if(UI.size() > 0)
	{
		for (int i = 0; i < UI.size(); ++i)
		{
			delete UI[i];
			UI[i] = nullptr;
		}
		UI.clear();
	}
}

void CUIMgr::Update()
{
	if (m_UION)
	{
		m_FocusedUI = GetFocusUI();

		if (!m_FocusedUI)
			return;

		bool bLTap = KEY_TAP(KEY::LBUTTEN);
		bool bLAway = KEY_AWAY(KEY::LBUTTEN);

		CUI* pTarget = GetTargetedUI(m_FocusedUI);

		if (nullptr != pTarget)
		{
			pTarget->MouseOn();

			if (bLTap)
			{
				pTarget->MouseLDown();
				pTarget->m_bLDown = true;
			}
			else if (bLAway)
			{
				pTarget->MouseLUp();

				if (pTarget->m_bLDown)
				{
					pTarget->MouseLClicked();
				}

				pTarget->m_bLDown = false;
			}
		}
	}
}

void CUIMgr::Initialize()
{

}

void CUIMgr::SetFocusedUI(CUI* _pUI)
{
	if (m_FocusedUI == _pUI || nullptr == _pUI)
	{
		m_FocusedUI = _pUI;
		return;
	}

	m_FocusedUI = _pUI;

	vector<CObject*>& vecUI = CSceneMgr::GetInst()->GetCurScene()->GetUIGroup();

	vector<CObject*>::iterator iter = vecUI.begin();

	for (; iter != vecUI.end(); ++iter)
	{
		if (m_FocusedUI == *iter)
		{
			break;
		}
	}

	vecUI.erase(iter);
	vecUI.push_back(m_FocusedUI);
}

CUI* CUIMgr::GetTargetedUI(CUI* _pParentUI)
{
	bool bLAway = KEY_AWAY(KEY::LBUTTEN);
	CUI* pTargetUI = nullptr;

	//1. �θ� ���� , ��� �ڽĵ��� �˻��Ѵ�.
	static list<CUI*> queue;
	static vector<CUI*> vecNonTarget;
	queue.clear();
	vecNonTarget.clear();


	//ť�� �ֻ��� �θ� UI �ֱ�
	queue.push_back(_pParentUI);

	//ť�� ���� �� ���� ��ȸ
	while (!queue.empty())
	{
		//ť���� �������ϳ� ������
		CUI* pUI = queue.front();
		queue.pop_front();

		//ť�� �� �տ� ���콺�� �÷����� �ٷ� Ÿ��
		if (pUI->IsMouseOn())
		{
			if (nullptr != pTargetUI)
			{
				vecNonTarget.push_back(pTargetUI);
			}
			pTargetUI = pUI;
		}
		else
		{
			vecNonTarget.push_back(pUI);
		}

		vector<CUI*>& vecChild = pUI->GetChildUI();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			queue.push_back(vecChild[i]);
		}

	}


	//���� ��ư ���� ������ üũ�� �ٽ� �����Ѵ�.
	if (bLAway)
	{
		for (size_t i = 0; i < vecNonTarget.size(); ++i)
		{
			vecNonTarget[i]->m_bLDown = false;
		}
	}

	return pTargetUI;
}

CUI* CUIMgr::GetFocusUI()
{
	vector<CObject*>& vecUI = CSceneMgr::GetInst()->GetCurScene()->GetUIGroup();

	bool bLTap = KEY_TAP(KEY::LBUTTEN);

	CUI* pFocusedUI = m_FocusedUI;

	if (!bLTap)
	{
		return pFocusedUI;
	}

	//���ʹ�ư TAP�� �߻��ߴٴ� ����
	vector<CObject*>::iterator targetiter = vecUI.end();
	vector<CObject*>::iterator iter = vecUI.begin();

	for (; iter != vecUI.end(); ++iter)
	{
		if (((CUI*)*iter)->IsMouseOn())
		{
			targetiter = iter;
		}
	}

	if (vecUI.end() == targetiter)
	{
		return nullptr;
	}

	pFocusedUI = (CUI*)*targetiter;

	vecUI.erase(targetiter);
	vecUI.push_back(pFocusedUI);

	return pFocusedUI;
}
