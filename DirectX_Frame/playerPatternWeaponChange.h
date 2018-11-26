//======================================================================
//	プレイヤー武器切り替えパターンヘッダ[playerPatternWeaponChange.h]
//
//======================================================================
#ifndef _PLAYER_PATTERN_WEAPON_CHANGE_H_
#define _PLAYER_PATTERN_WEAPON_CHANGE_H_

#include "playerPatternBase.h"

class CPlayerPatternWeaponChange : public CPlayerPatternBase
{
public:
	CPlayerPatternWeaponChange() {}
	~CPlayerPatternWeaponChange() {}

	void Init(CPlayer* player)override;
	void Uninit(CPlayer* player)override {}
	void Update(CPlayer* player)override;
private:
	int	m_CountFrame;
};

#endif // !_PLAYER_PATTERN_WEAPON_CHANGE_H_
