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
	//�� ���� nullptr �ʱ�ȭ
	for (ULONG i = 0; i < (ULONG)eSceneType::END; ++i)
		m_pArrScene[i] = nullptr;
}

CSceneMgr::~CSceneMgr()
{
	//�÷��̾� ����, ��� �� ����
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
	//���� �� ������Ʈ
	m_pCurScene->Update();
}

void CSceneMgr::LateUpdate()
{
	//���� �� ����Ʈ������Ʈ
	m_pCurScene->LateUpdate();
}

void CSceneMgr::Render()
{
	//���� �� ����
	m_pCurScene->Render();
}

void CSceneMgr::Initialize()
{
	//��� �� �����Ҵ� �� �̸� �ο�
	m_pArrScene[(ULONG)eSceneType::SCENE_ROGO] = new CScene_Rogo;
	m_pArrScene[(ULONG)eSceneType::SCENE_ROGO]->SetName(L"CScene_Rogo");

	m_pArrScene[(ULONG)eSceneType::SCENE_START] = new CScene_Start;
	m_pArrScene[(ULONG)eSceneType::SCENE_START]->SetName(L"CScene_Start");


	//���� �۾�����
	m_pArrScene[(ULONG)eSceneType::SCENE_RUNRUN] = new CScene_RUNRUN;
	m_pArrScene[(ULONG)eSceneType::SCENE_RUNRUN]->SetName(L"CScene_RUNRUN");

	m_pArrScene[(ULONG)eSceneType::SCENE_JUMPJUMP] = new CScene_JUMPJUMP;
	m_pArrScene[(ULONG)eSceneType::SCENE_JUMPJUMP]->SetName(L"CScene_JUMPJUMP");

	m_pArrScene[(ULONG)eSceneType::SCENE_ROUNDROUND] = new CScene_ROUNDROUND;
	m_pArrScene[(ULONG)eSceneType::SCENE_ROUNDROUND]->SetName(L"CScene_ROUNDROUND");


	
	m_pArrScene[(ULONG)eSceneType::SCENE_TOOL] = new CScene_Tool;
	m_pArrScene[(ULONG)eSceneType::SCENE_TOOL]->SetName(L"CScene_Tool");
	
	

	//����� ��� �� �����Լ� �۵�
	m_pCurScene = m_pArrScene[(ULONG)eSceneType::SCENE_ROGO];
	m_pCurScene->Enter();
}

void CSceneMgr::Release()
{
}

CObject* CSceneMgr::GetPlayer()
{
	//������� �÷��̾ �������� �Լ�
	if (m_pCurScene->GetvSceneObj()[(int)eObjectType::PLAYER].size() > 0)
		return m_pCurScene->GetvSceneObj()[(int)eObjectType::PLAYER].front();
	else
		return nullptr;
}


void CSceneMgr::ChangeScene(eSceneType _eNext)
{
	//�� ü���� �Լ�
	m_pCurScene->Exit();

	m_pCurScene = m_pArrScene[(int)_eNext];
	
	m_pCurScene->Enter();
}

