//======================================================================
//	リザルト[result.cpp]　（2018/8/25）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "mode.h"
#include "scene.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneModel.h"
#include "camera.h"
#include "light.h"
#include "manager.h"
#include "texture.h"
#include "billboard.h"
#include "field.h"
#include "player.h"
#include "enemy.h"
#include "input.h"
#include "skybox.h"
#include "number.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "fade.h"

#define KILL_POINT (5)
#define SNEAK_POINT (10)

CScene2D* CModeResult::m_ResultText = NULL;
CScene2D* CModeResult::m_RankText = NULL;
CScene2D *CModeResult::m_Text_PressSpace = NULL;
D3DCOLOR CModeResult::m_RankColor = D3DCOLOR_RGBA(255, 255, 255, 255);
CSound *CModeResult::m_BGM = NULL;
CSound *CModeResult::m_SE = NULL;
int CModeResult::m_NumKillEnemy = 0;
int CModeResult::m_NumSneakKill = 0;
int CModeResult::m_Rank = 0;
int CModeResult::m_Count = 0;
bool CModeResult::m_bKilledTarget = false;

void CModeResult::Init()
{
	// テクスチャの初期化
	CTexture::Init();

	m_ResultText = CScene2D::Create(TEX_ID_RESULT, 193.0f, 103.0f);
	m_ResultText->Set(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 250.0f, 0.0f));

	m_Text_PressSpace = CScene2D::Create(TEX_ID_PRESS_SPACE, 501.0f, 105.0f);
	m_Text_PressSpace->Set(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 200.0f, 0.0f));

	// ランクの計算
	int result = KILL_POINT * m_NumKillEnemy + SNEAK_POINT * m_NumSneakKill;
	if (m_bKilledTarget)
	{
		result += 50;
	}

	if (result > 100)
	{
		m_Rank = 0;
		m_RankColor = D3DCOLOR_RGBA(255, 0, 0, 255);
	}
	else if (result > 60)
	{
		m_Rank = 1;
		m_RankColor = D3DCOLOR_RGBA(0, 0, 255, 255);
	}
	else
	{
		m_Rank = 2;
		m_RankColor = D3DCOLOR_RGBA(255, 255, 0, 255);
	}

	m_RankText = CScene2D::Create(TEX_ID_RANK_A + m_Rank, 256, 256);
	m_RankText->SetColor(m_RankColor);
	m_RankText->Set(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0));

	m_BGM = CSound::Create(SOUND_LABEL_BGM_RESULT);
	m_BGM->Play();
	m_SE = CSound::Create(SOUND_LABEL_SE_TITLE);

	CFade::FadeIn();
}

void CModeResult::Uninit()
{
	// 全てのテクスチャの解放
	CTexture::ReleaseAll();

	CScene::ReleaseAll();

	CSound::ReleaseAll();
}

void CModeResult::Update()
{
	CInputKeyboard *inputKeyboard;
	CInputMouse *inputMouse;
	float mouseX, mouseY, mouseZ;

	// キーボード取得
	inputKeyboard = CManager::GetInputKeyboard();

	// マウス取得
	inputMouse = CManager::GetInputMouse();
	mouseX = (float)inputMouse->GetAxisX();
	mouseY = (float)inputMouse->GetAxisY();
	mouseZ = (float)inputMouse->GetAxisZ();

	CScene::UpdateAll();

	m_Count++;

	if (m_Count / 256 % 2 == 0.0f)
	{
		m_Text_PressSpace->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_Count % 256));
	}
	else
	{
		m_Text_PressSpace->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255 - (m_Count % 256)));
	}

	if (!CFade::GetFadeOut())
	{
		if (inputMouse->GetLeftTrigger() || inputKeyboard->GetKeyTrigger(DIK_SPACE))
		{
			m_SE->Play();
			CFade::FadeOut(new CModeTitle());
		}
	}
}

void CModeResult::Draw()
{
	CScene::DrawAll();

	CImGui::BeginDraw();

	ImGui::Begin("Debug Window", 0);
	ImGui::Text("Kill = %d SneakKill = %d", m_NumKillEnemy, m_NumSneakKill);
	ImGui::End();

	CImGui::EndDraw();
}
