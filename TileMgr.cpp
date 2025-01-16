#include "pch.h"
#include "TileMgr.h"
#include "ScrollMgr.h"
#include "Tile.h"
#include "SceneMgr.h"


CTileMgr::CTileMgr()
{
	m_vecTile.reserve(TILEX * TILEY);
}

CTileMgr::~CTileMgr()
{
	Release();
}

//이니셜라이즈하면 타일을 타일백터에 갯수만큼 전부 푸쉬백해줌
void CTileMgr::Initialize()
{
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			float fX = (float)(j * TILECX) + (TILECX >> 1);
			float fY = (float)(i * TILECY) + (TILECY >> 1);

			CObject* pTile = new CObject;
			pTile->Initialize();
			pTile->SetPos(tVec2{ fX, fY });
			m_vecTile.push_back(pTile);
		}
	}
}

//업데이트 뿌리기
void CTileMgr::Update()
{
	for (auto& pTile : m_vecTile)
		pTile->Update();
}

//레이트 업데이트 뿌리기
void CTileMgr::Late_Update()
{
	for (auto& pTile : m_vecTile)
		pTile->LateUpdate();
}

//렌더링뿌리기
void CTileMgr::Render()
{
	int		iScrollX = abs((int)CScrollMgr::Get_Instance()->Get_ScrollX() / TILECX);
	int		iScrollY = abs((int)CScrollMgr::Get_Instance()->Get_ScrollY() / TILECY);

	int		iMaxX = iScrollX + WINCX / TILECX + 2;
	int		iMaxY = iScrollY + WINCY / TILECY + 2;

	for (int i = iScrollY; i < iMaxY; ++i)
	{
		for (int j = iScrollX; j < iMaxX; ++j)
		{
			int		iIndex = i * TILEX + j;

			if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
				continue;

			m_vecTile[iIndex]->Render();
		}
	}




}

//벡터안에 있는 타일 전부 세이프딜리트
void CTileMgr::Release()
{
	auto& TileList = CSceneMgr::GetInst()->GetCurScene()->GetvSceneObj()[(ULONG)eObjectType::TILE];
	for (auto& tile : TileList)
	{
		Safe_Delete(tile);
	}
	TileList.clear();
	TileList.shrink_to_fit();
}

//타일 클릭시 변경됨
void CTileMgr::Picking_Tile(POINT pt, int iDrawID, int iOption)
{
	int		x = pt.x / TILECX;
	int		y = pt.y / TILECY;

	int	iIndex = y * TILEX + x;

	if (0 > iIndex || (size_t)iIndex >= m_vecTile.size())
		return;

	dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_DrawID(iDrawID);
	dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_Option(iOption);

}

//세이브파일
void CTileMgr::Save_Tile()
{
	HANDLE hFile = CreateFile(
		L"./Data/Tile.dat",
		GENERIC_WRITE,
		NULL, NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	int		iDrawID(0), iOption(0);
	DWORD	dwByte(0);

	for (auto& pTile : CSceneMgr::GetInst()->GetCurScene()->GetvSceneObj()[(ULONG)eObjectType::TILE])
	{
		iDrawID = dynamic_cast<CTile*>(pTile)->Get_DrawID();
		iOption = dynamic_cast<CTile*>(pTile)->Get_Option();

		WriteFile(hFile, pTile->Get_Info_Pointer(), sizeof(tObjInfo), &dwByte, NULL);
		WriteFile(hFile, &iDrawID, sizeof(int), &dwByte, NULL);
		WriteFile(hFile, &iOption, sizeof(int), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Tile Save", L"성공", MB_OK);

}

//로드 파일
void CTileMgr::Load_Tile(const wchar_t* _Tile)
{
	wstring filePath = L"./Data/";
	filePath += _Tile;

	HANDLE hFile = CreateFile(
		filePath.c_str(),
		GENERIC_READ,
		NULL, NULL,
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	int			iDrawID(0), iOption(0);
	DWORD		dwByte(0);
	tObjInfo	tTile{};

	Release();

	while (true)
	{
		bool a = ReadFile(hFile, &tTile, sizeof(tObjInfo), &dwByte, NULL);
		bool b = ReadFile(hFile, &iDrawID, sizeof(int), &dwByte, NULL);
		bool c = ReadFile(hFile, &iOption, sizeof(int), &dwByte, NULL);

		if (0 == dwByte)
			break;
		
		CTile* pTile = new CTile;
		pTile->Initialize();
		pTile->SetPos(tVec2{ tTile.tPos.fX, tTile.tPos.fY });
		pTile->Set_DrawID(iDrawID);
		pTile->Set_Option(iOption);

		if (pTile->Get_Option() == 0)
		{
			CSceneMgr::GetInst()->GetCurScene()->AddvObject(pTile, eObjectType::TILE);
		}
		else if (pTile->Get_Option() == 1)
		{
			CSceneMgr::GetInst()->GetCurScene()->AddvObject(pTile, eObjectType::TILEENTER);
		}
	}

	CloseHandle(hFile);
	//MessageBox(g_hWnd, L"Load Save", L"성공", MB_OK);

}
