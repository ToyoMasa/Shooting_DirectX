//======================================================================
//	カメラ操作チュートリアルパターン [tutorialCamera.cpp]　（2019/2/4）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "texture.h"
#include "tutorialMove.h"
#include "tutorialCamera.h"
#include "scene2D.h"
#include "player.h"

void CTutorialCamera::Init()
{
	m_Text = CScene2D::Create(TEX_ID_CAMERA_ROTATE_TEXT, 320.0f, 70.0f);
	m_Text->Set(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - 200.0f, 0.0f));
	m_Controller = CScene2D::Create(TEX_ID_MOUSE_MOVE, 105.0f, 144.0f);
	m_Controller->Set(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - 100.0f, 0.0f));
	m_Controller->SetTexCoord(0.0f, 1.0f, 0.0f, 1.0f);

	m_Count = 0;
	m_Alpha = 0;
}

void CTutorialCamera::Uninit()
{
	m_Text->Release();
	m_Controller->Release();
}

void CTutorialCamera::Update(CPlayer* player)
{
	m_Count++;

	int value = m_Count % 120;

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

	if (m_Count > 420)
	{
		player->ChangeTutorial(new CTutorialMove());
	}
	
}
