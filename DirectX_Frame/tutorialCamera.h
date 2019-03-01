//======================================================================
//	移動チュートリアルパターンヘッダ[tutorialCamera.h]
//
//======================================================================
#ifndef _TUTORIAL_CAMERA_H_
#define _TUTORIAL_CAMERA_H_

#include "tutorialBase.h"

//*********************************************************
//	カメラ操作チュートリアル
//*********************************************************
class CTutorialCamera : public CTutorialBase
{
public:
	void Init()override;
	void Uninit()override;
	void Update(CPlayer* player)override;
};

#endif // !_TUTORIAL_CAMERA_H_

