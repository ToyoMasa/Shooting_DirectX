//======================================================================
//	�ړ��`���[�g���A���p�^�[���w�b�_[tutorialNone.h]
//
//======================================================================
#ifndef _TUTORIAL_MOVE_H_
#define _TUTORIAL_MOVE_H_

#include "tutorialBase.h"

//*********************************************************
//	�ړ��`���[�g���A��
//*********************************************************
class CTutorialMove : public CTutorialBase
{
public:
	void Init()override;
	void Uninit()override;
	void Update(CPlayer* player)override;
};

#endif // !_TUTORIAL_MOVE_H_

