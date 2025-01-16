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
	//����� �޾ƿ���
	GetCurrentDirectory(255, m_szFilePath);

	//��� Ȯ�ο�
	//SetWindowText(g_hWnd, m_szFilePath);
}

CTextures* CResMgr::LoadTexture(const wstring& _strKey, const TCHAR* _strRelativePath)
{
	CTextures* pTex = FindTexture(_strKey);
	if (nullptr != pTex)
	{
		return pTex;
	}

	//�� ��� �ϼ�
	//wstring strFilePath = m_szFilePath;
	//strFilePath += L"\\Content";
	//strFilePath = _strRelativePath;

	//�ؽ�ó �����Ҵ� �� �ε�
	pTex = new CTextures;
	pTex->Load(_strRelativePath);

	//Ű�� ��� ���� ����
	pTex->SetKey(_strKey);
	pTex->SetRelativePath(_strRelativePath);

	//�ؽ�ó�� �ʿ� ���
	m_mapTex.insert(make_pair(_strKey, pTex));

	return pTex;
}

//�ش� �ؽ�ó�� �ʿ��� ã��
CTextures* CResMgr::FindTexture(const wstring& _strKey)
{
	map<wstring, CTextures*>::iterator iter = m_mapTex.find(_strKey);

	if (iter == m_mapTex.end())
	{
		return nullptr;
	}

	return iter->second;
}
