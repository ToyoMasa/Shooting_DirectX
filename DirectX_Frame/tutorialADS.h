//======================================================================
//	ADS�`���[�g���A���p�^�[���w�b�_[tutorialADS.h]
//
//======================================================================
#ifndef _TUTORIAL_ADS_H_
#define _TUTORIAL_ADS_H_

#include "tutorialBase.h"

//*********************************************************
//	�e��`�����ރ`���[�g���A��
//*********************************************************
class CTutorialADS : public CTutorialBase
{
public:
	void Init()override;
	void Uninit()override;
	void Update(CPlayer* player)override;
};

#endif // !_TUTORIAL_ADS_H_

