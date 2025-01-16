#pragma once
#include "Define.h"


class CMyPng
{
public:
    CMyPng();
    ~CMyPng();

public:
    void Release();
    Image* Get_Image() const { return m_pImage; }                                                                                                   

private:                                
    Image* m_pImage;
};                                                                                                                                                                                                                                 