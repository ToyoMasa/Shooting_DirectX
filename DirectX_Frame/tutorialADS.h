//======================================================================
//	ADSチュートリアルパターンヘッダ[tutorialADS.h]
//
//======================================================================
#ifndef _TUTORIAL_ADS_H_
#define _TUTORIAL_ADS_H_

#include "tutorialBase.h"

class CTutorialADS : public CTutorialBase
{
public:
	CTutorialADS() {}
	~CTutorialADS() {}

	void Init()override;
	void Uninit()override;
	void Update(CPlayer* player)override;
};

#endif // !_TUTORIAL_ADS_H_

