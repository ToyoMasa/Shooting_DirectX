//======================================================================
//	ADSチュートリアルパターンヘッダ[tutorialADS.h]
//
//======================================================================
#ifndef _TUTORIAL_ADS_H_
#define _TUTORIAL_ADS_H_

#include "tutorialBase.h"

//*********************************************************
//	銃を覗き込むチュートリアル
//*********************************************************
class CTutorialADS : public CTutorialBase
{
public:
	void Init()override;
	void Uninit()override;
	void Update(CPlayer* player)override;
};

#endif // !_TUTORIAL_ADS_H_

