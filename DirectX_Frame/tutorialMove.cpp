//======================================================================
//	移動チュートリアルパターン [tutorialMove.cpp]　（2019/2/1）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "texture.h"
#include "tutorialMove.h"
#include "tutorialCamera.h"
#include "tutorialNone.h"
#include "scene2D.h"
#include "player.h"

void CTutorialMove::Init()
{
	m_Text = CScene2D::Create(TEX_ID_MOVE_TEXT, 128.0f, 70.0f);
	m_Text->Set(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - 200.0f, 0.0f));
	m_Controller = CScene2D::Create(TEX_ID_WASD_RED, 128.0f, 85.3f);
	m_Controller->Set(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - 100.0f, 0.0f));
	m_Controller->SetTexCoord(0.0f, 0.5f, 0.0f, 0.5f);

	m_Count = 0;
	m_Alpha = 0;
}

void CTutorialMove::Uninit()
{
	m_Text->Release();
	m_Controller->Release();
}

void CTutorialMove::Update(CPlayer* player)
{
	m_Count++;

	if (m_Count < 120)
	{
		m_Alpha += 255 / 120;
		m_Text->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_Alpha));
		m_Controller->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_Alpha));
	}
	else if (m_Count == 120)
	{
		m_Alpha = 255;
		m_Text->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_Alpha));
		m_Controller->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_Alpha));
	}
	else if (m_Count > 300)
	{
		m_Alpha -= 255 / 120;
		m_Text->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_Alpha));
		m_Controller->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_Alpha));
	}

	int value = m_Count % 120;
	
	if (value < 30)
	{
		m_Controller->SetTexCoord(0.0f, 0.5f, 0.0f, 0.5f);
	}
	else if (value < 60)
	{
		m_Controller->SetTexCoord(0.5f, 1.0f, 0.0f, 0.5f);
	}
	else if (value < 90)
	{
		m_Controller->SetTexCoord(0.0f, 0.5f, 0.5f, 1.0f);
	}
	else
	{
		m_Controller->SetTexCoord(0.5f, 1.0f, 0.5f, 1.0f);
	}

	if (m_Count > 420)
	{
		player->ChangeTutorial(new CTutorialNone());
	}
}
