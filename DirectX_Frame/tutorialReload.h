//======================================================================
//	リロードチュートリアルパターンヘッダ[tutorialReload.h]
//
//======================================================================
#ifndef _TUTORIAL_RELOAD_H_
#define _TUTORIAL_RELOAD_H_

#include "tutorialBase.h"

class CTutorialReload : public CTutorialBase
{
public:
	CTutorialReload() {}
	~CTutorialReload() {}

	void Init()override;
	void Uninit()override;
	void Update(CPlayer* player)override;
};

#endif // !_TUTORIAL_RELOAD_H_

