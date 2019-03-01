//======================================================================
//	プレイヤー武器切り替えパターンヘッダ[playerPatternWeaponChange.h]
//
//======================================================================
#ifndef _PLAYER_PATTERN_WEAPON_CHANGE_H_
#define _PLAYER_PATTERN_WEAPON_CHANGE_H_

#include "playerPatternBase.h"

//*********************************************************
//	武器変更時の処理
//*********************************************************
class CPlayerPatternWeaponChange : public CPlayerPatternBase
{
public:
	void Init(CPlayer* player)override;
	void Uninit()override {}
	void Update()override;

	virtual void Move(D3DXVECTOR2 move)override;
	virtual void Rotate(D3DXVECTOR2 rot)override;
private:
	int	m_CountFrame;
};

#endif // !_PLAYER_PATTERN_WEAPON_CHANGE_H_
