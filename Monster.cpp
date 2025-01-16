#include "pch.h"
#include "Monster.h"
#include "SceneMgr.h"
#include "Object.h"
#include "Define.h"
#include "ScrollMgr.h"

CMonster::CMonster() : fAttackRange(100), m_bHit(false)
{
    srand(static_cast<unsigned>(time(0)));
}

CMonster::~CMonster()
{
}

