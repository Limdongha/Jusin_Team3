#include "pch.h"
#include "Scene_Tool.h"
#include "Object.h"
#include "KeyMgr.h"
#include "Define.h"
#include "ResMgr.h"
#include "Textures.h"
#include "KeyMgr.h"
#include "TileMgr.h"
#include "Tile.h"
#include "SceneMgr.h"
#include "ScrollMgr.h"

CScene_Tool::CScene_Tool()
{
	ZeroMemory(&m_tBlockPoint, sizeof(m_tBlockPoint));

	m_pTool = CResMgr::GetInst()->LoadTexture
	(L"StageTool", L"./\\Content\\Textures\\BG\\ROGO.bmp");
}

CScene_Tool::~CScene_Tool()
{
}

void CScene_Tool::Render()
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();


	BitBlt(
		g_memDC,
		0 + iScrollX,
		0 + iScrollY,
		3000,
		1200,
		m_pTool->GetDC(),
		0,
		0,
		SRCCOPY
	);

	//그리드 그리기
	for (int i = 0; i < TILEX; ++i)
	{
		MoveToEx(g_memDC, int(i * TILECX + iScrollX), int(0 + iScrollY), nullptr);
		LineTo(g_memDC, int(i * TILECX + iScrollX), int(TILEY * TILECX + iScrollY));
	}
	for (int i = 0; i < TILEY; ++i)
	{
		MoveToEx(g_memDC, int(0 + iScrollX), int(i * TILECY + iScrollY), nullptr);
		LineTo(g_memDC, int(TILEY * TILECY + iScrollX), int(i * TILECY + iScrollY));
	}

	//렌더 레이어
	for (auto& pObj : GetvRenderObj()[(int)RENDERID::RENDER_Tile])
		pObj->Render();

	auto aa = GetvRenderObj()[(int)RENDERID::RENDER_Tile].size();
	if(GetvRenderObj()[(int)RENDERID::RENDER_Tile].size() > 0)
		GetvRenderObj()[(int)RENDERID::RENDER_Tile].clear();
}

void CScene_Tool::Enter()
{
}

void CScene_Tool::Exit()
{
}

void CScene_Tool::Update()
{
	CScene::Update();

	//마우스 포인터 값 가져오기
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	//스크롤값 포함
	ptMouse.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	ptMouse.y -= (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	//좌클릭 시 , 좌표를 블럭 사이즈만큼 나누기 연산 (딱 떨어지게)
	if (KEY_TAP(KEY::LBUTTEN))
	{
		if ((!m_tBlockPoint[HEAD].x) && (!m_tBlockPoint[HEAD].y))
		{
			m_tBlockPoint[HEAD].x = ptMouse.x - (ptMouse.x % long(TILECX));
			m_tBlockPoint[HEAD].y = ptMouse.y - (ptMouse.y % long(TILECY));
		}
	}
	//좌클릭 떼면 생성
	if (KEY_AWAY(KEY::LBUTTEN))
	{
		CTile* pTile = new CTile;
		pTile->SetPos(
			tVec2{
				float(m_tBlockPoint[HEAD].x + TILECX * 0.5f),
				float(m_tBlockPoint[HEAD].y + TILECY * 0.5f),
			});
		pTile->Initialize();
		Create_Object(pTile, eObjectType::TILE);
		ZeroMemory(&m_tBlockPoint, sizeof(m_tBlockPoint));
	}

	//우클릭 시 , 좌표를 블럭 사이즈만큼 나누기 연산 (딱 떨어지게)
	if (KEY_TAP(KEY::RBUTTEN))
	{
		if ((!m_tBlockPoint[HEAD].x) && (!m_tBlockPoint[HEAD].y))
		{
			m_tBlockPoint[HEAD].x = ptMouse.x - (ptMouse.x % long(TILECX));
			m_tBlockPoint[HEAD].y = ptMouse.y - (ptMouse.y % long(TILECY));
		}
	}
	//우클릭 떼면 생성
	if (KEY_AWAY(KEY::RBUTTEN))
	{
		CTile* pTile = new CTile;
		pTile->SetPos(
			tVec2{
				float(m_tBlockPoint[HEAD].x + TILECX * 0.5f),
				float(m_tBlockPoint[HEAD].y + TILECY * 0.5f),
			});
		pTile->Initialize();
		pTile->Set_Option(1);
		Create_Object(pTile, eObjectType::TILE);
		ZeroMemory(&m_tBlockPoint, sizeof(m_tBlockPoint));
	}

	//화면 움직이기
	if (KEY_HOLD(KEY::LEFT))
		CScrollMgr::Get_Instance()->Add_ScrollX(5);

	if (KEY_HOLD(KEY::RIGHT))
		CScrollMgr::Get_Instance()->Add_ScrollX(-5);

	if (KEY_HOLD(KEY::UP))
		CScrollMgr::Get_Instance()->Add_ScrollY(5);

	if (KEY_HOLD(KEY::DOWN))
		CScrollMgr::Get_Instance()->Add_ScrollY(-5);

	//세이브 키
	if (KEY_HOLD(KEY::R)) 
	{
		CTileMgr::GetInst()->Save_Tile();
	}

	//로드 키
	if (KEY_HOLD(KEY::T))
	{
		CTileMgr::GetInst()->Load_Tile(L"Scene_Start.dat");
	}
	//윈도우 종료
	if (KEY_TAP(KEY::ESC))
	{
		PostQuitMessage(0);
	}

	if (KEY_TAP(KEY::ENTER))
	{
		Change_Scene(eSceneType::SCENE_START);
	}
	CScrollMgr::Get_Instance()->Scroll_Lock(3000, 1200);
}

