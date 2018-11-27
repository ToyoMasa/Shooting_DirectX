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
#include "field.h"
#include "title.h"
#include "game.h"
#include "fade.h"
#include "PlayerAnim.h"
#include "wall.h"
#include "fog.h"

CScene2D *CModeTitle::TitleLogo = NULL;
CScene2D *CModeTitle::TextPressSpace = NULL;
CSceneSkinMesh *CModeTitle::m_Mesh = NULL;
CSound *CModeTitle::BGM = NULL;
CSound *CModeTitle::SE = NULL;
CScene2D *CModeTitle::Load = NULL;
CScene2D *CModeTitle::LoadFrame = NULL;
CScene2D *CModeTitle::LoadGage = NULL;
CCamera *CModeTitle::Camera = NULL;
CFog *CModeTitle::Fog = NULL;
CField *CModeTitle::Field = NULL;
CLight *CModeTitle::Light = NULL;
int CModeTitle::Count = 0;

void CModeTitle::Init()
{
	// テクスチャの初期化
	CTexture::Init();

	// フェードイン
	CFade::FadeIn();

	TitleLogo = CScene2D::Create(TEX_ID_TITLE, 390.0f * 2.0f, 90.0f * 2.0f);
	TitleLogo->Set(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100.0f, 0.0f));
	TitleLogo->SetColor(D3DCOLOR_RGBA(186, 7, 7, 255));

	TextPressSpace = CScene2D::Create(TEX_ID_PRESS_SPACE, 420.0f, 90.0f);
	TextPressSpace->Set(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 200.0f, 0.0f));
	TextPressSpace->SetColor(D3DCOLOR_RGBA(186, 7, 7, 255));

	Camera = CCamera::Create();
	Camera->SetPos(D3DXVECTOR3(-27.41f, 2.51f, -86.44f));
	Camera->SetAt(D3DXVECTOR3(-23.28f, 2.36f, -86.12f));
	CManager::SetCamera(Camera);

	BGM = CSound::Create(SOUND_LABEL_BGM_TITLE);
	BGM->Play();

	Fog->Set(D3DCOLOR_RGBA(18, 18, 36, 255), 0.15f);

	// フィールド
	Field = CField::Create("data/output/map.txt");

	// ライト
	Light = CLight::Create(0);

	// 空
	CSkyBox::Create(NULL);

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
		TextPressSpace->SetColor(D3DCOLOR_RGBA(186, 7, 7, Count % 256));
	}
	else
	{
		TextPressSpace->SetColor(D3DCOLOR_RGBA(186, 7, 7, 255 - (Count % 256)));
	}

	CScene::UpdateAll();

	if (!CFade::GetFadeOut())
	{
		if (!CManager::GetDebug())
		{
			if (inputMouse->GetLeftTrigger() || inputKeyboard->GetKeyTrigger(DIK_SPACE))
			{
				SE = CSound::Create(SOUND_LABEL_SE_TITLE);
				SE->Play();
				CFade::FadeOut(new CModeGame());
			}
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

	CScene::DrawAll();

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
