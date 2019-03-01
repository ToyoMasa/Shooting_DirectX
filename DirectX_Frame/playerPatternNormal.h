//======================================================================
//	プレイヤー通常時パターンヘッダ[playerPatternNormal.h]
//
//======================================================================
#ifndef _PLAYER_PATTERN_NORMAL_H_
#define _PLAYER_PATTERN_NORMAL_H_

#include "playerPatternBase.h"

//*********************************************************
//	通常時の処理
//*********************************************************
class CPlayerPatternNormal : public CPlayerPatternBase
{
public:
	void Init(CPlayer* player)override;
	void Uninit()override{}
	void Update()override;

	virtual void Move(D3DXVECTOR2 move)override;
	virtual void Rotate(D3DXVECTOR2 rot)override;
	virtual void Dash(float moveZ)override;
	virtual void ADS()override;
	virtual void Shoot()override;
	virtual void Reload()override;
	virtual void ChangeWeapon()override;
};

#endif // !_PLAYER_PATTERN_NORMAL_H_
