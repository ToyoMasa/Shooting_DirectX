//======================================================================
//	ゲーム管理
//
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
#include "mapmake.h"
#include "fade.h"
#include "sound.h"
#include "shader.h"

//======================================================================
//	静的メンバ変数
//======================================================================
CInputKeyboard *CManager::m_InputKeyboard = NULL;		// キーボードへのポインタ
CInputMouse *CManager::m_InputMouse = NULL;			// マウスへのポインタ
CLight		*CManager::m_Light;
CMode		*CManager::m_Mode = NULL;
CCamera		*CManager::m_UsingCamera = NULL;

bool CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	if (!CRenderer::Init(hWnd, bWindow))
	{
		return false;
	}

	CSound::Init();

	// imguiの初期化と設定
	CImGui::Init(hWnd, CRenderer::GetDevice());
	ImGui::StyleColorsClassic();

	// キーボード処理初期化
	m_InputKeyboard = new CInputKeyboard;
	m_InputKeyboard->Init(hInstance, hWnd);

	// マウス処理初期化
	m_InputMouse = new CInputMouse;
	m_InputMouse->Init(hInstance, hWnd);

	CManager::m_Mode = new CModeMapMake();
	//CManager::m_Mode = new CModeGame();

	if (CManager::m_Mode != NULL)
	{
		CManager::m_Mode->Init();
	}

	CFade::Init();

	return true;
}

void CManager::Uninit()
{
	CFade::Uninit();

	CManager::m_Mode->Uninit();
	delete CManager::m_Mode;

	// キーボードの開放処理
	if (m_InputKeyboard)
	{
		m_InputKeyboard->Uninit();
		delete m_InputKeyboard;
		m_InputKeyboard = NULL;
	}

	// マウスの開放処理
	if (m_InputMouse)
	{
		m_InputMouse->Uninit();
		delete m_InputMouse;
		m_InputMouse = NULL;
	}

	// サウンドを解放
	CSound::Uninit();

	// 使ったシェーダーを解放
	CShader::ReleaseAll();

	// imguiの終了処理
	CImGui::Uninit();

	// レンダラークラスの終了処理
	CRenderer::Uninit();
}

void CManager::Update()
{
	CImGui::BeginDraw();

	CFade::Update();

	// キーボード更新処理
	if (m_InputKeyboard)
	{
		m_InputKeyboard->Update();
	}

	// マウス更新処理
	if (m_InputMouse)
	{
		m_InputMouse->Update();
	}

	m_Mode->Update();

	m_UsingCamera->Update();
}

void CManager::Draw()
{
	HRESULT hr;

	hr = CRenderer::DrawBegin();

	//Direct3Dによる描画の開始
	if (SUCCEEDED(hr))
	{
		//描画
		m_Mode->Draw();

		//CFade::Draw();
	}

	CRenderer::DrawEnd();
}

void CManager::SetMode(CMode* mode)
{
	if (CManager::m_Mode != NULL)
	{
		CManager::m_Mode->Uninit();

		delete CManager::m_Mode;
		CManager::m_Mode = NULL;
	}

	CManager::m_Mode = mode;

	if (CManager::m_Mode != NULL)
	{
		CManager::m_Mode->Init();
	}
}

