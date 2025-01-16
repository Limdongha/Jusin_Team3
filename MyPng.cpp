#include "pch.h"
#include "MyPng.h"
CMyPng::CMyPng() : m_pImage(nullptr) {}

CMyPng::~CMyPng()
{
    Release();
}

void CMyPng::Release()
{
    Safe_Delete(m_pImage);
}
