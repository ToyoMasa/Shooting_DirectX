//======================================================================
//	�����[�h�`���[�g���A���p�^�[���w�b�_[tutorialReload.h]
//
//======================================================================
#ifndef _TUTORIAL_RELOAD_H_
#define _TUTORIAL_RELOAD_H_

#include "tutorialBase.h"

//*********************************************************
//	�����[�h�`���[�g���A��
//*********************************************************
class CTutorialReload : public CTutorialBase
{
public:
	void Init()override;
	void Uninit()override;
	void Update(CPlayer* player)override;
};

#endif // !_TUTORIAL_RELOAD_H_

