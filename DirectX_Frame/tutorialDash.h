//======================================================================
//	走るチュートリアルパターンヘッダ[tutorialDash.h]
//
//======================================================================
#ifndef _TUTORIAL_DASH_H_
#define _TUTORIAL_DASH_H_

#include "tutorialBase.h"

//*********************************************************
//	走るチュートリアル
//*********************************************************
class CTutorialDash : public CTutorialBase
{
public:
	void Init()override;
	void Uninit()override;
	void Update(CPlayer* player)override;
};

#endif // !_TUTORIAL_DASH_H_
