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

CScene2D* CModeTitle::m_TitleLogo = NULL;
CScene2D *CModeTitle::m_Text_PressSpace = NULL;
CSceneSkinMesh *CModeTitle::m_Mesh = NULL;
CCamera *CModeTitle::m_Camera = NULL;
CSound *CModeTitle::m_BGM = NULL;
CSound *CModeTitle::m_SE = NULL;
int CModeTitle::m_Count = 0;
CScene2D *CModeTitle::Load = NULL;
CScene2D *CModeTitle::LoadFrame = NULL;
CScene2D *CModeTitle::LoadGage = NULL;

CWall* wall;

void CModeTitle::Init()
{
	// テクスチャの初期化
	CTexture::Init();

	Load = CScene2D::Create(TEX_ID_NOWLOADING, 1545 / 2.0f, 414 / 2.0f);
	Load->Set(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0));

	LoadFrame = CScene2D::Create(TEX_ID_LOADFRAME, 960, 64);
	LoadFrame->Set(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 200.0f, 0));

	LoadGage = CScene2D::Create(TEX_ID_LOADGAGE, 950, 54);
	LoadGage->Set(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 200.0f, 0));

	HRESULT hr;
	hr = CRenderer::DrawBegin();

	// ロード画面を描画
	if (SUCCEEDED(hr))
	{
		//描画
		Load->Draw();
		LoadFrame->Draw();

		CRenderer::DrawEnd();
	}

	m_TitleLogo = CScene2D::Create(TEX_ID_TITLE, 1153.0f, 323.0f);
	m_TitleLogo->Set(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 150.0f, 0.0f));

	m_Text_PressSpace = CScene2D::Create(TEX_ID_PRESS_SPACE, 501.0f, 105.0f);
	m_Text_PressSpace->Set(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 200.0f, 0.0f));

	m_Camera = CCamera::Create();
	m_Camera->SetPos(D3DXVECTOR3(-0.2f, 1.7f, -1.0f));
	m_Camera->SetAt(D3DXVECTOR3(0.0f, 1.5f, 0.0f));
	CManager::SetCamera(m_Camera);

	{
		hr = CRenderer::DrawBegin();
		LoadGage->SetSize(D3DXVECTOR2(950 * 0.3f, 54));
		LoadGage->Set(D3DXVECTOR3((SCREEN_WIDTH / 2 - 950 / 2.0f) + 950 * 0.3f / 2.0f, SCREEN_HEIGHT / 2 + 200.0f, 0));

		// ロード画面を描画
		if (SUCCEEDED(hr))
		{
			//描画
			Load->Draw();
			LoadFrame->Draw();
			LoadGage->Draw();

			CRenderer::DrawEnd();
		}
	}

	m_Mesh = CSceneSkinMesh::Create(SM_ID_PLAYER);
	m_Mesh->ChangeAnim(PLAYER_IDLE, 0.0f);

	{
		hr = CRenderer::DrawBegin();
		LoadGage->SetSize(D3DXVECTOR2(950 * 0.75f, 54));
		LoadGage->Set(D3DXVECTOR3((SCREEN_WIDTH / 2 - 950 / 2.0f) + 950 * 0.75f / 2.0f, SCREEN_HEIGHT / 2 + 200.0f, 0));

		// ロード画面を描画
		if (SUCCEEDED(hr))
		{
			//描画
			Load->Draw();
			LoadFrame->Draw();
			LoadGage->Draw();

			CRenderer::DrawEnd();
		}
	}

	// フェードイン
	CFade::FadeIn();

	m_BGM = CSound::Create(SOUND_LABEL_BGM_TITLE);
	m_BGM->Play();

	m_Count = 0;

	// ロード画面を解放
	Load->Release();
	LoadFrame->Release();
	LoadGage->Release();

	//wall = CWall::Create(D3DXVECTOR3(0, 0, 0), 1, 1, 1, 0);
	//wall->SetShader(&test);
	//m_Mesh->SetShader(&test);
}

void CModeTitle::Uninit()
{
	// 全てのテクスチャの解放
	CTexture::ReleaseAll();

	CScene::ReleaseAll();

	m_Camera->Release();

	CSound::ReleaseAll();
}

void CModeTitle::Update()
{
	CInputKeyboard *inputKeyboard;
	CInputMouse *inputMouse;
	float mouseX, mouseY, mouseZ;

	m_Camera->Update();

	// キーボード取得
	inputKeyboard = CManager::GetInputKeyboard();

	// マウス取得
	inputMouse = CManager::GetInputMouse();
	mouseX = (float)inputMouse->GetAxisX();
	mouseY = (float)inputMouse->GetAxisY();
	mouseZ = (float)inputMouse->GetAxisZ();

	m_Count++;

	if (m_Count / 256 % 2 == 0.0f)
	{
		m_Text_PressSpace->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_Count % 256));
	}
	else
	{
		m_Text_PressSpace->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255 - (m_Count % 256)));
	}

	CScene::UpdateAll();

	if (!CFade::GetFadeOut())
	{
		if (inputMouse->GetLeftTrigger() || inputKeyboard->GetKeyTrigger(DIK_SPACE))
		{
			m_SE = CSound::Create(SOUND_LABEL_SE_TITLE);
			m_SE->Play();
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

	CImGui::BeginDraw();

	CImGui::EndDraw();
}
