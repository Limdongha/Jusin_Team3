#include "pch.h"
#include "Define.h"
#include "EventMgr.h"
#include "SceneMgr.h"
#include "KeyMgr.h"
#include "Define.h"
#include "Player.h"
#include "ScrollMgr.h"
#include "MainGame.h"

CEventMgr::CEventMgr()
{

}

CEventMgr::~CEventMgr()
{
	Release();
}

void CEventMgr::Update()
{
	//시작전 쓰레기통 비우기
	for (size_t i = 0; i < m_vecDead.size(); ++i)
	{
		if (nullptr != m_vecDead[i])
		{
			delete m_vecDead[i];
			m_vecDead[i] = nullptr;
		}
	}
	m_vecDead.clear();

	//이벤트 실행뿌리기
	for (size_t i = 0; i < m_vecEvent.size(); ++i)
	{
		Excute(m_vecEvent[i]);
	}
	m_vecEvent.clear();
}

//이벤트 실행함수
void CEventMgr::Excute(const tEvent& _eve)
{
	switch (_eve.eEvent)
	{
	case EVENT_TYPE::CREATE_OBJECT:
	{
		//lparam = ObjectAdress
		//wparm = GroupAdress
		CObject* pNewObj = (CObject*)_eve.lParm;
		eObjectType eType = (eObjectType)_eve.wParm;

		CSceneMgr::GetInst()->GetCurScene()->AddvObject(pNewObj, eType);
	}
		break;
	case EVENT_TYPE::DELETE_OBJECT:
	{
		//lparam : ObjectAdress
		//오브젝트 arrive를 false 상태로 변경a
		CObject* pDeadObj = (CObject*)_eve.lParm;
		pDeadObj->SetDead();

		if (find(m_vecDead.begin(), m_vecDead.end(), pDeadObj) == m_vecDead.end())
		{
			m_vecDead.push_back(pDeadObj); // 중복되지 않으면 추가
		}
	}
		break;
	case EVENT_TYPE::SCENE_CHANGE:
		//lparam : NextScene
		CSceneMgr::GetInst()->ChangeScene((eSceneType)_eve.lParm);
		break;

	default:
		break;
	}
}



//콜리젼 확인렌더(F1)
void CEventMgr::Render()
{

	if (CMainGame::GetInst()->GetiEventBox() == 1)
	{
		auto& player = CSceneMgr::GetInst()->GetCurScene()->GetvSceneObj()[(int)eObjectType::PLAYER];
		auto& monsters = CSceneMgr::GetInst()->GetCurScene()->GetvSceneObj()[(int)eObjectType::MONSTER];
		auto& Boss = CSceneMgr::GetInst()->GetCurScene()->GetvSceneObj()[(int)eObjectType::BOSSMONSTER];
		auto& head = CSceneMgr::GetInst()->GetCurScene()->GetvSceneObj()[(int)eObjectType::HEAD];

		HPEN newPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		HBRUSH newBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

		HPEN oldPen = (HPEN)SelectObject(g_memDC, newPen);
		HBRUSH oldBrush = (HBRUSH)SelectObject(g_memDC, newBrush);


		float ScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
		float ScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

		if (player.size() > 0)
		{
			Rectangle(g_memDC,
				int(player.front()->GetPos().fX - player.front()->GetScale().fX / 2.f + ScrollX),
				int(player.front()->GetPos().fY - player.front()->GetScale().fY / 2.f + ScrollY),
				int(player.front()->GetPos().fX + player.front()->GetScale().fX / 2.f + ScrollX),
				int(player.front()->GetPos().fY + player.front()->GetScale().fY / 2.f + ScrollY));
		}


		if (monsters.size() > 0)
		{
			for (auto& mob : monsters)
			{
				Rectangle(g_memDC,
					int(mob->GetPos().fX - mob->GetScale().fX / 2.f + ScrollX),
					int(mob->GetPos().fY - mob->GetScale().fY / 2.f + ScrollY),
					int(mob->GetPos().fX + mob->GetScale().fX / 2.f + ScrollX),
					int(mob->GetPos().fY + mob->GetScale().fY / 2.f + ScrollY));
			}
		}

		SelectObject(g_memDC, oldPen);
		SelectObject(g_memDC, oldBrush);

		DeleteObject(newBrush);
		DeleteObject(newPen);
	}
}

void CEventMgr::Initialize()
{
}
void CEventMgr::Release()
{
}
