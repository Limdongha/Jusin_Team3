#include "pch.h"
#include "PngManager.h"

CPngManager::CPngManager()
{
}

CPngManager::~CPngManager()
{
	//Release();
}

bool CPngManager::Insert_Png(const WCHAR* filepath, const wstring& imgKey)
{
	// �ߺ� Ű Ȯ��
	if (m_mapPng.find(imgKey) != m_mapPng.end())
	{
		return false;
	}

	// PNG �̹��� �ε�
	Image* pImage = Image::FromFile(filepath);

	if (pImage && pImage->GetLastStatus() == Ok)
	{
		m_mapPng.insert({ imgKey,pImage });
		return true;
	}

	// ���� �� �޸� ����
	Safe_Delete(pImage);
	return false;
}

Image* CPngManager::Find_Png(const wstring& imgKey) const
{
	auto iter = m_mapPng.find(imgKey);
	if (iter != m_mapPng.end())
	{
		return iter->second;
	}

	return nullptr;
}

void CPngManager::Initialize()
{
}

void CPngManager::Release()
{
	for (auto& img : m_mapPng)
	{
		Safe_Delete(img.second);
	}
	m_mapPng.clear();
}
