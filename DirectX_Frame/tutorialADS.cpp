//======================================================================
//	ADSチュートリアルパターン [tutorialADS.cpp]　（2019/2/5）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "texture.h"
#include "tutorialReload.h"
#include "tutorialADS.h"
#include "scene2D.h"
#include "player.h"

void CTutorialADS::Init()
{
	m_Text = CScene2D::Create(TEX_ID_GUNUP_TEXT, 320.0f, 70.0f);
	m_Text->Set(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - 200.0f, 0.0f));
	m_Controller = CScene2D::Create(TEX_ID_MOUSE_RIGHT, 70.0f, 110.0f);
	m_Controller->Set(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - 100.0f, 0.0f));
	m_Controller->SetTexCoord(0.0f, 1.0f, 0.0f, 1.0f);

	m_Count = 0;
	m_Alpha = 0;

	m_Text->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_Alpha));
	m_Controller->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_Alpha));
}

void CTutorialADS::Uninit()
{
	m_Text->Release();
	m_Controller->Release();
}

void CTutorialADS::Update(CPlayer* player)
{
	m_Count++;

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
	}
	else if (m_Count >  TUTORIAL_DRAW_FRAME - TUTORIAL_FADE_FRAME)
	{
		m_Alpha -= 255 / TUTORIAL_FADE_FRAME;
		m_Text->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_Alpha));
		m_Controller->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_Alpha));
	}

	if (m_Count > TUTORIAL_DRAW_FRAME)
	{
		player->ChangeTutorial(new CTutorialReload());
	}

}
