//======================================================================
//	武器パターンヘッダ[weaponPatternBase.h]
//
//======================================================================
#ifndef _WEAPON_PATTERN_BASE_H_
#define _WEAPON_PATTERN_BASE_H_

class CWeapon;

class CWeaponPatternBase
{
public:
	CWeaponPatternBase() {}
	~CWeaponPatternBase() {}

	virtual void Update(CWeapon* Weapon) = 0;
};

#endif // !_WEAPON_PATTERN_BASE_H_

