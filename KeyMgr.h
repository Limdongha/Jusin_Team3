#pragma once
enum class KEY_STATE
{
	NONE,
	TAP,
	HOLD,
	AWAY
};

enum class KEY
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	Q, W, E, R, T, Y, U, I, O, P,
	A, S, D, F, G, Z, X, C, V, B,

	ALT, CTRL, LSHIFT, SPACE, ENTER, TAP, F1, ESC,
	LBUTTEN, RBUTTEN,

	LAST
};

struct tKey_Info
{
	KEY_STATE	eState;
	bool		bPrev;
};
class Ckeymgr
{
	SINGLE(Ckeymgr)

public:
	void	Initialize();
	void	Update();

private:
	vector<tKey_Info> m_veckey;

public:
	KEY_STATE GetKEY_STATE(KEY WhatKey) { return m_veckey[int(WhatKey)].eState; }

};


