//======================================================================
//	タイトル[title.cpp]　（2018/8/25）
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "mode.h"
#include "scene.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneModel.h"
#include "SkinMeshFile.h"
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
#include "fade.h"
#include "PlayerAnim.h"
#include "wall.h"

CScene2D* CModeTitle::TitleLogo = NULL;
CScene2D *CModeTitle::Text_PressSpace = NULL;
CSceneSkinMesh *CModeTitle::m_Mesh = NULL;
CCamera *CModeTitle::Camera = NULL;
CSound *CModeTitle::BGM = NULL;
CSound *CModeTitle::SE = NULL;
int CModeTitle::Count = 0;
CScene2D *CModeTitle::Load = NULL;
CScene2D *CModeTitle::LoadFrame = NULL;
CScene2D *CModeTitle::LoadGage = NULL;

void CModeTitle::Init()
{
	// テクスチャの初期化
	CTexture::Init();

	TitleLogo = CScene2D::Create(TEX_ID_TITLE, 1153.0f, 323.0f);
	TitleLogo->Set(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 150.0f, 0.0f));

	Text_PressSpace = CScene2D::Create(TEX_ID_PRESS_SPACE, 501.0f, 105.0f);
	Text_PressSpace->Set(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 200.0f, 0.0f));

	Camera = CCamera::Create();
	Camera->SetPos(D3DXVECTOR3(-0.2f, 1.7f, -1.0f));
	Camera->SetAt(D3DXVECTOR3(0.0f, 1.5f, 0.0f));
	CManager::SetCamera(Camera);

	// フェードイン
	CFade::FadeIn();

	BGM = CSound::Create(SOUND_LABEL_BGM_TITLE);
	BGM->Play();

	Count = 0;
}

void CModeTitle::Uninit()
{
	// 全てのテクスチャの解放
	CTexture::ReleaseAll();

	CScene::ReleaseAll();

	Camera->Release();

	CSound::ReleaseAll();
}

void CModeTitle::Update()
{
	CInputKeyboard *inputKeyboard;
	CInputMouse *inputMouse;
	float mouseX, mouseY, mouseZ;

	Camera->Update();

	// キーボード取得
	inputKeyboard = CManager::GetInputKeyboard();

	// マウス取得
	inputMouse = CManager::GetInputMouse();
	mouseX = (float)inputMouse->GetAxisX();
	mouseY = (float)inputMouse->GetAxisY();
	mouseZ = (float)inputMouse->GetAxisZ();

	Count++;

	if (Count / 256 % 2 == 0.0f)
	{
		Text_PressSpace->SetColor(D3DCOLOR_RGBA(255, 255, 255, Count % 256));
	}
	else
	{
		Text_PressSpace->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255 - (Count % 256)));
	}

	CScene::UpdateAll();

	if (!CFade::GetFadeOut())
	{
		if (inputMouse->GetLeftTrigger() || inputKeyboard->GetKeyTrigger(DIK_SPACE))
		{
			SE = CSound::Create(SOUND_LABEL_SE_TITLE);
			SE->Play();
			CFade::FadeOut(new CModeGame());
		}
	}
}

void CModeTitle::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	CScene::DrawAll();

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
