#include "pch.h"
#include "Scene_Start.h"
#include "Player.h"
#include "Monster.h"
#include "KeyMgr.h"
#include "Define.h"
#include "SceneMgr.h"
#include "Func.h"
#include "Tile.h"
#include "EffectMgr.h"
#include "ResMgr.h"
#include "Textures.h"
#include "ScrollMgr.h"
#include "TileMgr.h"
#include "ColliderMgr.h"
#include "SoundMgr.h"
#include "UIMgr.h"

float	g_fVolume(0.f);

CScene_Start::CScene_Start() : m_tGirl(FRAME{}), m_pDoor(nullptr), m_pNPC_Girl(nullptr)
{


	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 8;
	m_tFrame.iMotion = 0;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount64();

	m_tGirl.iFrameStart = 0;
	m_tGirl.iFrameEnd = 6;
	m_tGirl.iMotion = 0;
	m_tGirl.dwSpeed = 100;
	m_tGirl.dwTime = GetTickCount64();
}

CScene_Start::~CScene_Start()
{
	DeleteAll();
	CSoundMgr::GetInst()->StopSound(SOUND_BGM);
}

// �ش���� ������Ʈ �߰�
void CScene_Start::Enter()
{
	m_fFade = 1.0f;
	m_bChangeScene = false;
	m_bDontCopy = false;

	//Ÿ�� �ҷ�����
	CTileMgr::GetInst()->Load_Tile(L"Scene_Start.dat");

	//�÷��̾��ȯ
	DeletePlayerGroup();
	SpawnPlayer(WINCX / 2 + 400, WINCY / 2 - 100.f);
	CUIMgr::GetInst()->SetUION(true);

}

void CScene_Start::Update()
{
	//���� ������ Ŀ����
	if (g_fVolume <= 1.f)
	{
		g_fVolume += 0.01f;
		CSoundMgr::GetInst()->SetChannelVolume(SOUND_BGM, g_fVolume);
	}

#pragma region �� ��ȯ


	FadeInOut();

	if (m_bChangeScene && m_fFade == 1.f)
	{
		Change_Scene(eSceneType::SCENE_ROGO);
	}
	auto& EnterTile = CSceneMgr::GetInst()->GetCurScene()->GetvSceneObj()[(int)eObjectType::TILEENTER];
	auto& Player = CSceneMgr::GetInst()->GetCurScene()->GetvSceneObj()[(int)eObjectType::PLAYER];

	bool DoorEnter = true;
	if (DoorEnter && KEY_TAP(KEY::UP))
	{
		m_bChangeScene = true;
	}


	//������ ����
	if (KEY_TAP(KEY::ESC))
	{
		PostQuitMessage(0);
	}

	if (KEY_TAP(KEY::SPACE))
	{
		Change_Scene(eSceneType::SCENE_TOOL);
	}

#pragma endregion



	CScrollMgr::Get_Instance()->Scroll_Lock(2000, 800);

	//������ �̵� (�� , ����)
	CScene::Move_Frame();
	CScene::Move_Frame(m_tGirl);

	//��������Ʈ
	CScene::Update();

	
}

void CScene_Start::Render()
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	//���
	//BitBlt(g_memDC, 0 + iScrollX, 0 + iScrollY, 2000, 800, m_pBG->GetDC(),	0, 0, SRCCOPY);
	Rectangle(g_memDC, 0, 0, WINCX, WINCY);


	CScene::Render();


	//���̵� ��, �ƿ�
	if (0 < m_fFade)
		AlphaBlend(m_pBlack, m_fFade);
}

void CScene_Start::Exit() 
{
	
	DeleteAll();
	CSoundMgr::GetInst()->StopSound(SOUND_BGM);

	auto& Stage01 = CSceneMgr::GetInst()->GetAllScene()[(ULONG)eSceneType::STAGE01];


	//�÷��̾� ���������ʰ� �������� 1�� ���� �ѱ��
	if (!m_bDontCopy)
	{
		for (auto& Player : GetvSceneObj()[(UINT)eObjectType::PLAYER])
		{
			Stage01->AddvObject(Player, eObjectType::PLAYER);
		}

		for (auto& UI : GetvSceneObj()[(UINT)eObjectType::UI])
		{
			Stage01->AddvObject(UI, eObjectType::UI);
		}
		GetvSceneObj()[(UINT)eObjectType::UI].clear();
	}
}
