#pragma once

class CObject;
class CEffect;

class CColliderMgr
{
	SINGLE(CColliderMgr)

public:
	void		Update();
	void		Initialize();
	void		Release();


public:
	bool		CollisionCircle(CObject* lhs, CObject* rhs, float _ScrollX, float _ScrollY);
	void		PlayerGravityEx(vector<CObject*> _ObjectGroup);
	void		ObjectGravityEx(vector<CObject*> _ObjectGroup);
	void		GetCurObjectTile(vector<CObject*> _ObjectGroup);
	void		GravityCondition(vector<CObject*> _ObjectGroup, vector<CObject*> _Tiles);
	void		GravityCondition(CObject* _ObjectGroup, vector<CObject*> _Tiles);
	bool		CheckXRange(CObject* tile, CObject* object);
	void		SetClosestTile(CObject* object, vector<CObject*> Tiles);
	bool		TileCompare(CObject* _Player, CObject* _CurrentTile, CObject* _NextTile);
	void		PlayerJump(vector<CObject*> _ObjectGroup);
	void		ObjectJump(vector<CObject*> _ObjectGroup);
	float		CalculateVelocityY(float jumpSpeed, float gravity, float time);

public:
	bool		Check_Rect(CObject* _player, CObject* _mob, float* _fX, float* _fY);
	void		CollisionEx(vector<CObject*> _Dst, vector<CObject*> _Src);

public:
	void		SetfTime(float _Time)		{ m_fTime = _Time; }
	void		SetfGravity(float _Gravity) { m_Gravity = _Gravity; }
	void		SetbPlayerDead(bool _bPlayerDead) { m_bPlayerDead = _bPlayerDead; }
	bool		GetbPlayerDead() { return m_bPlayerDead; }

private:
	float		m_Gravity;
	float		m_fTime;
	bool		m_bJumpLimit;
	bool		m_bEffectCreated;
	bool		m_bPlayerDead;

	bool		m_bSound;
	ULONG64		m_SoundStart;
};

