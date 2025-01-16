#pragma once
#include "Define.h"
class CPngManager
{
    SINGLE(CPngManager)

public:
    // PNG 이미지 추가
    bool Insert_Png(const WCHAR* filepath, const wstring& imgKey);
    // PNG 이미지 검색
    Image* Find_Png(const wstring& imgKey) const;
    void Initialize();
    void Release();


private:
    map<wstring, Image*> m_mapPng; // PNG 이미지를 관리하는 맵
};