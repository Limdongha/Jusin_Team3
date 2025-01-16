#include "pch.h"
#include "Define.h"
#include "SceneMgr.h"
#include "Rogo.h"
#include "Scene_Start.h"
#include "Scene_Tool.h"
#include "Scene_RUNRUN.h"
#include "Scene_JUMPJUMP.h"
#include "Scene_ROUNDROUND.h"

CSceneMgr::CSceneMgr() : m_pCurScene(nullptr)
{
	//씬 전부 nullptr 초기화
	for (ULONG i = 0; i < (ULONG)eSceneType::END; ++i)
		m_pArrScene[i] = nullptr;
}

CSceneMgr::~CSceneMgr()
{
	//플레이어 삭제, 모든 씬 삭제
	m_pCurScene->DeletePlayerGroup();

	CSceneMgr::Release();
	for (int i = 0; i < (int)eSceneType::END; ++i)
	{
		delete m_pArrScene[i];
		m_pArrScene[i] = nullptr;
	}
}

void CSceneMgr::Update()
{
	//현재 씬 업데이트
	m_pCurScene->Update();
}

void CSceneMgr::LateUpdate()
{
	//현재 씬 레이트업데이트
	m_pCurScene->LateUpdate();
}

void CSceneMgr::Render()
{
	//현재 씬 렌더
	m_pCurScene->Render();
}

void CSceneMgr::Initialize()
{
	//모든 씬 동적할당 및 이름 부여
	m_pArrScene[(ULONG)eSceneType::SCENE_ROGO] = new CScene_Rogo;
	m_pArrScene[(ULONG)eSceneType::SCENE_ROGO]->SetName(L"CScene_Rogo");

	m_pArrScene[(ULONG)eSceneType::SCENE_START] = new CScene_Start;
	m_pArrScene[(ULONG)eSceneType::SCENE_START]->SetName(L"CScene_Start");


	//실제 작업공간
	m_pArrScene[(ULONG)eSceneType::SCENE_RUNRUN] = new CScene_RUNRUN;
	m_pArrScene[(ULONG)eSceneType::SCENE_RUNRUN]->SetName(L"CScene_RUNRUN");

	m_pArrScene[(ULONG)eSceneType::SCENE_JUMPJUMP] = new CScene_JUMPJUMP;
	m_pArrScene[(ULONG)eSceneType::SCENE_JUMPJUMP]->SetName(L"CScene_JUMPJUMP");

	m_pArrScene[(ULONG)eSceneType::SCENE_ROUNDROUND] = new CScene_ROUNDROUND;
	m_pArrScene[(ULONG)eSceneType::SCENE_ROUNDROUND]->SetName(L"CScene_ROUNDROUND");


	
	m_pArrScene[(ULONG)eSceneType::SCENE_TOOL] = new CScene_Tool;
	m_pArrScene[(ULONG)eSceneType::SCENE_TOOL]->SetName(L"CScene_Tool");
	
	

	//현재씬 등록 및 엔터함수 작동
	m_pCurScene = m_pArrScene[(ULONG)eSceneType::SCENE_ROGO];
	m_pCurScene->Enter();
}

void CSceneMgr::Release()
{
}

CObject* CSceneMgr::GetPlayer()
{
	//현재씬의 플레이어를 가져오는 함수
	if (m_pCurScene->GetvSceneObj()[(int)eObjectType::PLAYER].size() > 0)
		return m_pCurScene->GetvSceneObj()[(int)eObjectType::PLAYER].front();
	else
		return nullptr;
}


void CSceneMgr::ChangeScene(eSceneType _eNext)
{
	//씬 체인지 함수
	m_pCurScene->Exit();

	m_pCurScene = m_pArrScene[(int)_eNext];
	
	m_pCurScene->Enter();
}

