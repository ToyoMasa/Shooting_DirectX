//======================================================================
//	ライフルクラス [rifle.cpp]　（2018/9/29）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "sceneModel.h"
#include "weapon.h"
#include "rifle.h"

void CRifle::Init()
{

}

void CRifle::Update()
{

}

void CRifle::Shoot()
{

}

CRifle* CRifle::Create()
{
	CRifle* rifle = new CRifle();
	rifle->Init();

	return rifle;
}
