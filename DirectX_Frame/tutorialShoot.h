//======================================================================
//	�ˌ��`���[�g���A���p�^�[���w�b�_[tutorialShoot.h]
//
//======================================================================
#ifndef _TUTORIAL_SHOOT_H_
#define _TUTORIAL_SHOOT_H_

#include "tutorialBase.h"

class CTutorialShoot : public CTutorialBase
{
public:
	CTutorialShoot() {}
	~CTutorialShoot() {}

	void Init()override;
	void Uninit()override;
	void Update(CPlayer* player)override;
};

#endif // !_TUTORIAL_SHOOT_H_

