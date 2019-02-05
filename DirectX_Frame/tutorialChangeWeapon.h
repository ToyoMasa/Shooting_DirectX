//======================================================================
//	武器チェンジチュートリアルパターンヘッダ[tutorialChangeWeapon.h]
//
//======================================================================
#ifndef _TUTORIAL_CHANGE_WEAPON_H_
#define _TUTORIAL_CHANGE_WEAPON_H_

#include "tutorialBase.h"

class CTutorialChangeWeapon : public CTutorialBase
{
public:
	CTutorialChangeWeapon() {}
	~CTutorialChangeWeapon() {}

	void Init()override;
	void Uninit()override;
	void Update(CPlayer* player)override;
};

#endif // !_TUTORIAL_CHANGE_WEAPON_H_

