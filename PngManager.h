#pragma once
#include "Define.h"
class CPngManager
{
    SINGLE(CPngManager)

public:
    // PNG �̹��� �߰�
    bool Insert_Png(const WCHAR* filepath, const wstring& imgKey);
    // PNG �̹��� �˻�
    Image* Find_Png(const wstring& imgKey) const;
    void Initialize();
    void Release();


private:
    map<wstring, Image*> m_mapPng; // PNG �̹����� �����ϴ� ��
};