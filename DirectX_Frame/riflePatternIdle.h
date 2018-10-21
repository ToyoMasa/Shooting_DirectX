//======================================================================
//	武器パターンヘッダ[weaponPatternIdle.h]
//
//======================================================================
#ifndef _RIFLE_PATTERN_IDLE_H_
#define _RIFLE_PATTERN_IDLE_H_

#include "weaponPatternBase.h"

class CRiflePatternIdle : public CWeaponPatternBase
{
public:
	CRiflePatternIdle() {}
	~CRiflePatternIdle() {}

	void Update(CWeapon* Weapon)override;
};

#endif // !_RIFLE_PATTERN_IDLE_H_
