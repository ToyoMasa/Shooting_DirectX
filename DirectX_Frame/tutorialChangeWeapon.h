//======================================================================
//	����`�F���W�`���[�g���A���p�^�[���w�b�_[tutorialChangeWeapon.h]
//
//======================================================================
#ifndef _TUTORIAL_CHANGE_WEAPON_H_
#define _TUTORIAL_CHANGE_WEAPON_H_

#include "tutorialBase.h"

//*********************************************************
//	����ύX�̃`���[�g���A��
//*********************************************************
class CTutorialChangeWeapon : public CTutorialBase
{
public:
	void Init()override;
	void Uninit()override;
	void Update(CPlayer* player)override;
};

#endif // !_TUTORIAL_CHANGE_WEAPON_H_

