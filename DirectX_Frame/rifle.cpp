//======================================================================
//	���C�t���N���X [rifle.cpp]�@�i2018/9/29�j
//	Author : �L�� ���r
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
