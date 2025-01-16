#include "pch.h"
#include "ResMgr.h"
#include "Textures.h"

CResMgr::CResMgr() : m_szFilePath(L"")
{

}

CResMgr::~CResMgr()
{
	map<wstring, CTextures*>::iterator iter = m_mapTex.begin();
	for (; iter != m_mapTex.end(); ++iter)
	{
		delete iter->second;
	}
}
void CResMgr::Initialize()
{
	//상대경로 받아오기
	GetCurrentDirectory(255, m_szFilePath);

	//경로 확인용
	//SetWindowText(g_hWnd, m_szFilePath);
}

CTextures* CResMgr::LoadTexture(const wstring& _strKey, const TCHAR* _strRelativePath)
{
	CTextures* pTex = FindTexture(_strKey);
	if (nullptr != pTex)
	{
		return pTex;
	}

	//상세 경로 완성
	//wstring strFilePath = m_szFilePath;
	//strFilePath += L"\\Content";
	//strFilePath = _strRelativePath;

	//텍스처 동적할당 후 로딩
	pTex = new CTextures;
	pTex->Load(_strRelativePath);

	//키와 경로 정보 기입
	pTex->SetKey(_strKey);
	pTex->SetRelativePath(_strRelativePath);

	//텍스처를 맵에 등록
	m_mapTex.insert(make_pair(_strKey, pTex));

	return pTex;
}

//해당 텍스처를 맵에서 찾기
CTextures* CResMgr::FindTexture(const wstring& _strKey)
{
	map<wstring, CTextures*>::iterator iter = m_mapTex.find(_strKey);

	if (iter == m_mapTex.end())
	{
		return nullptr;
	}

	return iter->second;
}
