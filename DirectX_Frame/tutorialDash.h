//======================================================================
//	����`���[�g���A���p�^�[���w�b�_[tutorialDash.h]
//
//======================================================================
#ifndef _TUTORIAL_DASH_H_
#define _TUTORIAL_DASH_H_

#include "tutorialBase.h"

//*********************************************************
//	����`���[�g���A��
//*********************************************************
class CTutorialDash : public CTutorialBase
{
public:
	void Init()override;
	void Uninit()override;
	void Update(CPlayer* player)override;
};

#endif // !_TUTORIAL_DASH_H_
