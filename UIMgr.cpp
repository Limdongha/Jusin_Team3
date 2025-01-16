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
	//UI 소멸하기
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

	//1. 부모 포함 , 모든 자식들을 검사한다.
	static list<CUI*> queue;
	static vector<CUI*> vecNonTarget;
	queue.clear();
	vecNonTarget.clear();


	//큐에 최상위 부모 UI 넣기
	queue.push_back(_pParentUI);

	//큐가 없을 때 까지 순회
	while (!queue.empty())
	{
		//큐에서 데이터하나 꺼내기
		CUI* pUI = queue.front();
		queue.pop_front();

		//큐의 맨 앞에 마우스가 올려지면 바로 타겟
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


	//왼쪽 버튼 떼면 눌렀던 체크를 다시 해제한다.
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

	//왼쪽버튼 TAP이 발생했다는 전제
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
