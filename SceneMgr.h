#pragma once
#include "Scene.h"
class CEventMgr;



class CSceneMgr
{
	SINGLE(CSceneMgr)

public:
	void		Update();
	void		LateUpdate();
	void		Render();
	void		Initialize();
	void		Release();



public:
	CScene*		GetCurScene()	{ return m_pCurScene; }
	CScene**	GetAllScene() { return m_pArrScene; }

	CObject*	GetPlayer();
	
private:
	//씬 변경 접근제한
	void		ChangeScene(eSceneType _eNext);
	friend class CEventMgr;


private:
	CScene* m_pArrScene[(ULONG)eSceneType::END];
	CScene* m_pCurScene;

};

