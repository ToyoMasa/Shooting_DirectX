//======================================================================
//	プレイヤーダッシュパターンヘッダ[playerPatternDash.h]
//
//======================================================================
#ifndef _PLAYER_PATTERN_DASH_H_
#define _PLAYER_PATTERN_DASH_H_


//*********************************************************
//	走っている時の処理
//*********************************************************
#include "playerPatternBase.h"

class CPlayerPatternDash : public CPlayerPatternBase
{
public:
	void Init(CPlayer* player)override;
	void Uninit()override {}
	void Update()override;

	virtual void Move(D3DXVECTOR2 move)override;
	virtual void Rotate(D3DXVECTOR2 rot)override;
	virtual void ADS()override;
	virtual void Shoot()override;
	virtual void Reload()override;
	virtual void ChangeWeapon()override;
};

#endif // !_PLAYER_PATTERN_DASH_H_
