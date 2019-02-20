//======================================================================
//	リザルト[result.cpp]　（2018/8/25）
//	Author : 豊村 昌俊
//======================================================================
#include <Windows.h>
#include <Xinput.h>
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
#include "controller.h"
#include "skybox.h"
#include "number.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "fade.h"
#include "number.h"
#include "fog.h"

CScene2D* CModeResult::ResultText = NULL;
CScene2D* CModeResult::KillNumText = NULL;
CScene2D *CModeResult::TextPressSpace = NULL;
CScene2D *CModeResult::TextPressStart = NULL;
CSound *CModeResult::BGM = NULL;
CSound *CModeResult::SE = NULL;
CNumber *CModeResult::ScreenKillCount = NULL;
CCamera *CModeResult::Camera = NULL;
CFog *CModeResult::Fog = NULL;
CField *CModeResult::Field = NULL;
CLight *CModeResult::Light = NULL;
int CModeResult::NumKillEnemy = 0;
int CModeResult::CountFrame = 0;

void CModeResult::Init()
{
	// テクスチャの初期化
	CTexture::Init();

	ResultText = CScene2D::Create(TEX_ID_RESULT, 193.0f, 103.0f);
	ResultText->Set(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 250.0f, 0.0f));
	ResultText->SetColor(D3DCOLOR_RGBA(186, 7, 7, 255));

	KillNumText = CScene2D::Create(TEX_ID_KILL_ENEMY_NUM, 335.0f, 103.0f);
	KillNumText->Set(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2.0f - 50.0f, 0.0f));
	KillNumText->SetColor(D3DCOLOR_RGBA(186, 7, 7, 255));

	TextPressSpace = CScene2D::Create(TEX_ID_PRESS_SPACE, 501.0f, 105.0f);
	TextPressSpace->Set(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 200.0f, 0.0f));
	TextPressSpace->SetColor(D3DCOLOR_RGBA(186, 7, 7, 255));
	TextPressSpace->SetVisible(false);

	TextPressStart = CScene2D::Create(TEX_ID_PRESS_START, 500.0f, 90.0f);
	TextPressStart->Set(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 200.0f, 0.0f));
	TextPressStart->SetColor(D3DCOLOR_RGBA(186, 7, 7, 255));
	TextPressStart->SetVisible(false);

	ScreenKillCount = CNumber::Create();
	ScreenKillCount->SetNum(NumKillEnemy);
	ScreenKillCount->Set(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2.0f + 50.0f, 0.0f));
	ScreenKillCount->SetColor(D3DCOLOR_RGBA(186, 7, 7, 255));

	BGM = CSound::Create(SOUND_LABEL_BGM_RESULT);
	BGM->Play(BGM_VOLUME);
	SE = CSound::Create(SOUND_LABEL_SE_TITLE);

	Fog->Set(D3DCOLOR_RGBA(18, 18, 36, 255), 0.15f);

	Camera = CCamera::Create();
	Camera->SetPos(D3DXVECTOR3(-27.41f, 2.51f, -86.44f));
	Camera->SetAt(D3DXVECTOR3(-23.28f, 2.36f, -86.12f));
	CManager::SetCamera(Camera);

	// ビルボードの準備
	CBillBoard::Init();

	// フィールド
	Field = CField::Create("data/output/map.txt");

	// ライト
	Light = CLight::Create(0);

	// 空
	CSkyBox::Create(NULL);

	CFade::FadeIn();
}

void CModeResult::Uninit()
{
	Camera->Release();

	Light->Release();

	CBillBoard::Uninit();

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

	CountFrame++;

	if (CManager::GetController()->GetIsAble())
	{
		TextPressSpace->SetVisible(false);
		TextPressStart->SetVisible(true);
	}
	else
	{
		TextPressSpace->SetVisible(true);
		TextPressStart->SetVisible(false);
	}

	if (CountFrame / 256 % 2 == 0.0f)
	{
		TextPressSpace->SetColor(D3DCOLOR_RGBA(186, 7, 7, CountFrame % 256));
		TextPressStart->SetColor(D3DCOLOR_RGBA(186, 7, 7, CountFrame % 256));
	}
	else
	{
		TextPressSpace->SetColor(D3DCOLOR_RGBA(186, 7, 7, 255 - (CountFrame % 256)));
		TextPressStart->SetColor(D3DCOLOR_RGBA(186, 7, 7, 255 - (CountFrame % 256)));
	}

	if (!CFade::GetFadeOut())
	{
		if (inputMouse->GetLeftTrigger() ||
			inputKeyboard->GetKeyTrigger(DIK_SPACE) ||
			CManager::GetController()->ButtonTrigger(XINPUT_GAMEPAD_A) ||
			CManager::GetController()->ButtonTrigger(XINPUT_GAMEPAD_START))
		{
			SE->Play(SE_VOLUME);
			CFade::FadeOut(new CModeTitle());
		}
	}
}

void CModeResult::Draw()
{
	CScene::DrawAll();

	if (CManager::GetDebug())
	{
		ImGui::Begin("Debug Window", 0);
		ImGui::Text("Kill = %d", NumKillEnemy);
		ImGui::End();
	}
}
