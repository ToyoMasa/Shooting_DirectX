//======================================================================
//	�ˌ��`���[�g���A���p�^�[���w�b�_[tutorialShoot.h]
//
//======================================================================
#ifndef _TUTORIAL_SHOOT_H_
#define _TUTORIAL_SHOOT_H_

#include "tutorialBase.h"

//*********************************************************
//	�ˌ��`���[�g���A��
//*********************************************************
class CTutorialShoot : public CTutorialBase
{
public:
	void Init()override;
	void Uninit()override;
	void Update(CPlayer* player)override;
};

#endif // !_TUTORIAL_SHOOT_H_

