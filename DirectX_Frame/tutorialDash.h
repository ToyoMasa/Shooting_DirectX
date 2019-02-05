//======================================================================
//	走るチュートリアルパターンヘッダ[tutorialDash.h]
//
//======================================================================
#ifndef _TUTORIAL_DASH_H_
#define _TUTORIAL_DASH_H_

#include "tutorialBase.h"

class CTutorialDash : public CTutorialBase
{
public:
	CTutorialDash() {}
	~CTutorialDash() {}

	void Init()override;
	void Uninit()override;
	void Update(CPlayer* player)override;
};

#endif // !_TUTORIAL_DASH_H_
