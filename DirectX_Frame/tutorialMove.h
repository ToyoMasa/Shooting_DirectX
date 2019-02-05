//======================================================================
//	移動チュートリアルパターンヘッダ[tutorialNone.h]
//
//======================================================================
#ifndef _TUTORIAL_MOVE_H_
#define _TUTORIAL_MOVE_H_

#include "tutorialBase.h"

class CTutorialMove : public CTutorialBase
{
public:
	CTutorialMove() {}
	~CTutorialMove() {}

	void Init()override;
	void Uninit()override;
	void Update(CPlayer* player)override;
};

#endif // !_TUTORIAL_MOVE_H_

