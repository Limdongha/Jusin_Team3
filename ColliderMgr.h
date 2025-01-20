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
	void		SetbCollisionCooldown(bool _Cool) { m_bCollisionCooldown = _Cool; }
	void		SetbPlayerDead(bool _bPlayerDead) { m_bPlayerDead = _bPlayerDead; }
	bool		GetbCollisionCooldown() { return m_bCollisionCooldown; }
	bool		GetbPlayerDead() { return m_bPlayerDead; }

public:
	bool		KL_CheckCircle(CObject* _Src, CObject* _Dst);
	void		KL_CollisionCircle(CObject* _Src, vector <CObject*> _Dst);


public:
	void HW_CollisionCircle(CObject* _Src, vector<CObject*> _Dst);

	bool HW_CheckCircleEx(CObject* _Src, CObject* _Dst, D3DXVECTOR3* _collisionDir);
	bool HW_CheckCircle(CObject* _Src, CObject* _Dst);
	bool HW_Check_Rect(CObject* _Src, CObject* _Dst, float* pX, float* pY);





private:
	float		m_Gravity;
	float		m_fTime;
	bool		m_bJumpLimit;
	bool		m_bEffectCreated;
	bool		m_bPlayerDead;
	bool		m_bCollisionCooldown;

	bool		m_bSound;
	ULONG64		m_SoundStart;

private:
	steady_clock::time_point 		m_tStartTime;
	steady_clock::time_point		m_tCurTime;
};

