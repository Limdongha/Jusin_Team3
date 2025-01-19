#pragma once




#define PI			3.141592f
#define	PURE		= 0

extern HWND g_hWnd;
extern HDC g_memDC;


extern float g_fVolume;

#define SQUARE(X)				(X)*(X)
#define RADIAN(Degree)			(Degree)*PI/180.f
#define DEGREE(Radian)			(Radian)*180.f/PI
#define PERCENT(X)				(X) - rand()%100 > 0

#define SCROLLX CScrollMgr::Get_Instance()->Get_ScrollX()
#define SCROLLY CScrollMgr::Get_Instance()->Get_ScrollY()
#define GETPLAYER		auto& Player    = *CSceneMgr::GetInst()->GetPlayer();
#define GETKNITEBOSS	auto& KniteBoss = *CSceneMgr::GetInst()->GetKniteBoss();
#define GETKWIZARDBOSS	auto& WizardBoss = *CSceneMgr::GetInst()->GetWizardBoss();
#define TILECX		64
#define TILECY		64

#define TILEX		50
#define TILEY		50

#define WINCX		800
#define WINCY		600



#define CinInt if (cin.fail()){cin.clear();cin.ignore(1000, '\n');}

template<typename T>
void Safe_Delete(T& Temp)
{
	if (Temp)
	{
		delete Temp;
		Temp = nullptr;
	}
}


#define SINGLE(type) public:\
							static type* GetInst()\
							{\
								static type mgr;\
								return &mgr;\
							}\
					private:\
						type();\
						~type();


#define KEY_CHECK(key, state) Ckeymgr::GetInst()->GetKEY_STATE(key) == state
#define KEY_HOLD(key) KEY_CHECK(key, KEY_STATE::HOLD)
#define KEY_TAP(key) KEY_CHECK(key, KEY_STATE::TAP)
#define KEY_AWAY(key) KEY_CHECK(key, KEY_STATE::AWAY)
#define KEY_NONE(key) KEY_CHECK(key, KEY_STATE::NONE)

//다이렉트 구조체
struct INFO
{
	D3DXVECTOR3		vDir;
	D3DXVECTOR3		vLook;
	D3DXVECTOR3		vPos;

	D3DXMATRIX		matWorld;
};


//사운드 채널
enum CHANNELID
{
	SOUND_EFFECT,
	SOUND_PROGRESS,
	SOUND_WALK,
	SOUND_BERSERKER,
	SOUND_UI,
	SOUND_ENT,
	SOUND_HUMAN,
	SOUND_MONSTER,
	SOUND_KNITEBOSS,
	SOUND_KNITEBOSSEFFECT,
	SOUND_WIZARDBOSS,
	SOUND_WIZARDBOSSEFFECT,
	SOUND_BGM,
	SOUND_END 
};

enum class WIZARDBOSSSTATE
{
	IDLE,
	ATTACK01,
	ATTACK02,
	ATTACK02_READY,
	ULTIMATE,
	DEAD,
	FALLING,
	DISSAPPEAR,
	APPEAR,
	LANDING,
	POTION,
	WALK,
	BACKSTEP,

	END
};

enum class KNITEBOSSSTATE
{
	IDLE,
	SUPERMAN,
	JUMPATTACK,
	MISSILE,
	POINTING,
	POTION,
	SWING,
	BACKSTEP,
	CHARGING,
	FALLING,
	LANDING,
	READYSTAB,
	STABBING,
	REALSTABBING,
	KNEE,
	DEAD,

	END
};


enum class NORMALTREESTATE
{
	IDLE,
	MOVE,
	BATTLEMODE,
	ATTACK,
	HIT,
	DEAD,

	END
};

enum class DEALTREESTATE
{
	IDLE,
	MOVE,
	BATTLEMODE,
	ATTACK,
	HIT,
	DEAD,

	END
};

enum class CHERRYTREESTATE
{
	IDLE,
	MOVE,
	BATTLEMODE,
	ATTACK,
	HIT,
	DEAD,

	END
};

enum class KNITESTATE
{
	IDLE,
	MOVE,
	BATTLEMODE,
	ATTACK,
	HIT,
	DEAD,

	END
};

enum class KNITENAMEDSTATE
{
	IDLE,
	MOVE,
	BATTLEMODE,
	ATTACK,
	SKILL,
	HIT,
	DEAD,

	END
};

enum class ARCHERSTATE
{
	IDLE,
	MOVE,
	BATTLEMODE,
	ATTACK,
	HIT,
	DEAD,

	END
};

//프레임구조체
typedef struct tagFrame
{
	int			iFrameStart;
	int			iFrameEnd;
	int			iMotion;
	DWORD		dwSpeed;
	ULONGLONG	dwTime;

}FRAME;

//렌더레이어
enum class RENDERID
{
	RENDER_BACKGROUND,
	RENDER_NPC,
	RENDER_GAMEOBJECT,
	RENDER_MONSTER,
	RENDER_BOSSMONSTER,
	RENDER_PLAYER,
	RENDER_EFFECT,
	RENDER_Tile,


	RENDER_UI,
	RENDER_END
};

enum class eEffectName
{
	BASEATTACK,
	BERSERKERATTACK,
	BERSERKERATTACKPARTICLE,
	BERSERKERSKILL01,
	BERSERKERSKILL02,
	BERSERKERSKILL03,
	SKULLREBORN,
	PLAYERHIT,
	THROWHEAD,
	NOEFFECT,
	DOUBLEJUMP,
	REBORN,
	DASH,
	BERSERKERDASH,
	KNITEBOSSCHARGING,
	KNITEBOSSREALSTABBING,
	KNITEBOSSJUMPATTACK,
	KNITEBOSSMISSILE,
	KNITEBOSSSWING,
	KNITEBOSSSPOTION,
	BOSSSDEAD01,
	BOSSSDEAD02,
	WIZARDBOSSMISSILE,
	WIZARDBOSSMISSILE2,
	WIZARDBOSSFIREBALL,
	WIZARDBOSSFIREBLOW,
	WIZARDBOSSFIRERING01,
	WIZARDBOSSFIRERING02,
	WIZARDBOSSFIRERING03,
	WIZARDBOSSULTIMATE,
	WIZARDBOSSORA,
	ARCHERATTACKLINE,
	ARCHERATTACK,
	DEALTREEATTACK,
	KNITENAMEDSKILL,
	CHERRYTREEATTACK,

	END
};

//이펙트 종류
enum class eEffect
{
	PLAYERATTACK,
	MONSTERATTACK,
	EFFECT,

	END
};

//OBJ STATE
enum class STATE
{
	IDLE,
	WALK,
	FALLING,
	JUMP,
	JUMPATTACK,
	ATTACK,
	ATTACK2,
	ATTACK3,
	DASH,
	SKILL1,
	SKILL2,
	CHANGE,
	HIT,
	DEAD,


	END 
};

//Player STATE
enum class PLAYERSTATE
{
	HEAD,
	NOHEAD,
	BERSERKER,
	
	END
};

// float 2개 구조체
struct tVec2
{
	float fX;
	float fY;

#pragma region 구조체 연산자오버로딩

	tVec2& operator+(tVec2 _other)
	{
		fX += _other.fX;
		fY += _other.fY;
		return *this;
	}
	tVec2& operator-(tVec2 _other)
	{
		fX -= _other.fX;
		fY -= _other.fY;
		return *this;
	}
	tVec2& operator*(tVec2 _other)
	{
		fX *= _other.fX;
		fY *= _other.fY;
		return *this;
	}
	tVec2& operator/(tVec2 _other)
	{
		fX /= _other.fX;
		fY /= _other.fY;
		return *this;
	}

#pragma endregion
};

// 벡터 2개 구조체
struct tObjInfo
{
	tVec2 tPos;
	tVec2 tScale;
};

//오브젝트 그룹화
enum class eObjectType
{
	DEFAULT,
	PLAYER,
	NPC,
	HEAD,
	MONSTER,
	BOSSMONSTER,
	BOSSMONSTERMISSILE,
	BOSSWIZARDMISSILE,
	ATTACK_PLAYER,
	ATTACK_MONSTER,
	SHILD,
	TILE,
	TILEENTER,
	UI,


	END = 32
};

//씬 종류
enum class eSceneType
{
	DEFAULT,
	SCENE_RUNRUN,
	SCENE_JUMPJUMP,
	SCENE_ROUNDROUND,
	SCENE_ROGO,
	SCENE_START,
	SCENE_TOOL,
	STAGE01,
	STAGE02,
	STAGE03,
	BossA,

	END
};

//이벤트 종류
enum class EVENT_TYPE
{
	CREATE_OBJECT,
	DELETE_OBJECT,
	SCENE_CHANGE,

	END
};

//이벤트 구조체
struct tEvent
{
	EVENT_TYPE  eEvent;
	DWORD_PTR	lParm;
	DWORD_PTR	wParm;
};

//툴 에디터
enum DRAWPOINT { HEAD, TAIL, DRAWPOINT_END };


typedef struct tagLinePoint
{
	float fX, fY;

	tagLinePoint() { ZeroMemory(this, sizeof(tagLinePoint)); }
	tagLinePoint(float _fX, float _fY)
		: fX(_fX), fY(_fY)
	{
	}

}LINEPOINT;

typedef struct tagLine
{
	LINEPOINT tLPoint;
	LINEPOINT tRPoint;

	tagLine() { ZeroMemory(this, sizeof(tagLine)); }
	tagLine(LINEPOINT& _tLPoint, LINEPOINT& _tRPoint)
		: tLPoint(_tLPoint), tRPoint(_tRPoint)
	{
	}
}LINE;