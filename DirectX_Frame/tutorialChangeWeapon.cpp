//======================================================================
//	武器チェンジチュートリアルパターン [tutorialChangeWeapon.cpp]　（2019/2/5）
//	Author : 豊村 昌俊
//======================================================================
#include <Windows.h>
#include <Xinput.h>
#include "common.h"
#include "main.h"
#include "texture.h"
#include "tutorialChangeWeapon.h"
#include "tutorialNone.h"
#include "scene2D.h"
#include "player.h"
#include "manager.h"
#include "controller.h"

void CTutorialChangeWeapon::Init()
{
	m_Text = CScene2D::Create(TEX_ID_CHANGE_WEAPON_TEXT, 512.0f, 70.0f);
	m_Text->Set(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - 200.0f, 0.0f));
	m_Controller = CScene2D::Create(TEX_ID_MOUSE_WHEEL, 70.0f, 110.0f);
	m_Controller->Set(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - 100.0f, 0.0f));
	m_Controller->SetTexCoord(0.0f, 1.0f, 0.0f, 1.0f);
	m_Pad = CScene2D::Create(TEX_ID_PAD_BUTTONS, 100.0f, 100.0f);
	m_Pad->Set(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - 120.0f, 0.0f));
	m_Pad->SetTexCoord(0.5f, 1.0f, 0.5f, 1.0f);
	m_Pad->SetVisible(false);

	m_Count = 0;
	m_Alpha = 0;

	m_Text->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_Alpha));
	m_Controller->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_Alpha));
	m_Pad->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_Alpha));
}

void CTutorialChangeWeapon::Uninit()
{
	m_Text->Release();
	m_Controller->Release();
	m_Pad->Release();
}

void CTutorialChangeWeapon::Update(CPlayer* player)
{
	m_Count++;

	CController *controller;
	// コントローラーの取得
	controller = CManager::GetController();

	if (controller->GetIsAble())
	{
		m_Pad->SetVisible(true);
		m_Controller->SetVisible(false);
	}
	else
	{
		m_Pad->SetVisible(false);
		m_Controller->SetVisible(true);
	}
	if (m_Count <= TUTORIAL_DRAW_FRAME - TUTORIAL_FADE_FRAME)
	{
		if (m_Alpha < 255)
		{
			m_Alpha += 255 / TUTORIAL_FADE_FRAME;
		}
		if (m_Alpha > 255)
		{
			m_Alpha = 255;
		}
		m_Text->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_Alpha));
		m_Controller->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_Alpha));
		m_Pad->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_Alpha));
	}
	else if (m_Count >  TUTORIAL_DRAW_FRAME - TUTORIAL_FADE_FRAME)
	{
		m_Alpha -= 255 / TUTORIAL_FADE_FRAME;
		m_Text->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_Alpha));
		m_Controller->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_Alpha));
		m_Pad->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_Alpha));
	}

	if (m_Count > TUTORIAL_DRAW_FRAME)
	{
		player->ChangeTutorial(new CTutorialNone());
	}

}
