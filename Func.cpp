#include "pch.h"
#include "EventMgr.h"

//이벤트 등록함수
void Create_Object(CObject* _pObj, eObjectType _eGroup)
{
	tEvent evn = {};
	evn.eEvent = EVENT_TYPE::CREATE_OBJECT;
	evn.lParm = (DWORD_PTR)_pObj;
	evn.wParm = (DWORD_PTR)_eGroup;

	CEventMgr::GetInst()->AddEvent(evn);
}

void Delete_Object(CObject* _pObj)
{
	tEvent evn = {};
	evn.eEvent = EVENT_TYPE::DELETE_OBJECT;
	evn.lParm = (DWORD_PTR)_pObj;

	CEventMgr::GetInst()->AddEvent(evn);
}

void Change_Scene(eSceneType _eNext)
{
	tEvent evn = {};
	evn.eEvent = EVENT_TYPE::SCENE_CHANGE;
	evn.lParm = (DWORD_PTR)_eNext;

	CEventMgr::GetInst()->AddEvent(evn);
}
