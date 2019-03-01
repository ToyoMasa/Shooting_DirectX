//======================================================================
//	プレイヤーパターンヘッダ[playerPatternBase.h]
//
//======================================================================
#ifndef _PLAYER_PATTERN_BASE_H_
#define _PLAYER_PATTERN_BASE_H_

static const float WALK_HEAT = 0.4f / 60.0f;
static const float CONTROLLER_MAG = 10.0f;

class CPlayerPatternBase
{
public:
	virtual void Init(CPlayer* player) = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;

	virtual void Move(D3DXVECTOR2 move);
	virtual void Rotate(D3DXVECTOR2 rot);
	virtual void Dash(float moveZ);
	virtual void ADS();
	virtual void Shoot();
	virtual void Reload();
	virtual void ChangeWeapon();

protected:
	CPlayer*	m_Player;
};

#endif // !_PLAYER_PATTERN_BASE_H_

