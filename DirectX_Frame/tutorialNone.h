//======================================================================
//	チュートリアルなしパターンヘッダ[tutorialNone.h]
//
//======================================================================
#ifndef _TUTORIAL_NONE_H_
#define _TUTORIAL_NONE_H_

#include "tutorialBase.h"

class CTutorialNone : public CTutorialBase
{
public:
	CTutorialNone() {}
	~CTutorialNone() {}

	void Init()override {}
	void Uninit()override {}
	void Update(CPlayer* player)override {}
};

#endif // !_TUTORIAL_NONE_H_

