//======================================================================
//	チュートリアルベースパターン [tutorialBase.cpp]　（2019/2/1）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "texture.h"
#include "tutorialBase.h"

void CTutorialBase::LoadTexture()
{
	CTexture::Load(TEX_ID_DASH_TEXT);
	CTexture::Load(TEX_ID_GUNSHOT_TEXT);
	CTexture::Load(TEX_ID_GUNUP_TEXT);
	CTexture::Load(TEX_ID_CAMERA_ROTATE_TEXT);
	CTexture::Load(TEX_ID_CHANGE_WEAPON_TEXT);
	CTexture::Load(TEX_ID_MOUSE_LEFT);
	CTexture::Load(TEX_ID_MOUSE_RIGHT);
	CTexture::Load(TEX_ID_MOUSE_WHEEL);
	CTexture::Load(TEX_ID_WASD_RED);
	CTexture::Load(TEX_ID_SHIFT);
}

void CTutorialBase::ReleaseTexture()
{
	CTexture::Release(TEX_ID_DASH_TEXT);
	CTexture::Release(TEX_ID_GUNSHOT_TEXT);
	CTexture::Release(TEX_ID_GUNUP_TEXT);
	CTexture::Release(TEX_ID_CAMERA_ROTATE_TEXT);
	CTexture::Release(TEX_ID_CHANGE_WEAPON_TEXT);
	CTexture::Release(TEX_ID_MOUSE_LEFT);
	CTexture::Release(TEX_ID_MOUSE_RIGHT);
	CTexture::Release(TEX_ID_MOUSE_WHEEL);
	CTexture::Release(TEX_ID_WASD_RED);
	CTexture::Release(TEX_ID_SHIFT);
}
