//======================================================================
//	ゲーム管理
//
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
#include "mapmake.h"
#include "fade.h"
#include "sound.h"
#include "shader.h"
#include "waypoint.h"

static const int TEXMAP_SIZE = 2048;
static float CAMERA_SPEED = 0.1f;

//======================================================================
//	静的メンバ変数
//======================================================================
CInputKeyboard			*CManager::m_InputKeyboard = NULL;		// キーボードへのポインタ
CInputMouse				*CManager::m_InputMouse = NULL;			// マウスへのポインタ
CController				*CManager::m_Controller = NULL;
CLight					*CManager::m_Light;
CMode					*CManager::m_Mode = NULL;
CCamera					*CManager::m_UsingCamera = NULL;
CCamera					*CManager::m_DebugCamera = NULL;
CCamera					*CManager::m_TempCamera = NULL;
bool					CManager::m_isDebug = false;
bool					CManager::m_isFogEnable = true;
float					CManager::m_SkinMeshDrawDist = DRAW_DIST;
LPDIRECT3DTEXTURE9		CManager::m_ShadowTex = nullptr;				// 投影シャドウテクスチャ
LPDIRECT3DSURFACE9		CManager::m_ShadowSurface = nullptr;			// 投影シャドウテクスチャサーフェス	
LPDIRECT3DSURFACE9		CManager::m_ShadowZbufferSurface = nullptr;		// 投影テクスチャへレンダリングする際のＺバッファ（深度バッファ）
D3DXMATRIX				CManager::m_LightCamera;
D3DXMATRIX				CManager::m_LightProjection;

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

	m_Controller = new CController;
	m_Controller->Init();

	// スキンメッシュの一括ロード
	CSceneSkinMesh::LoadAll();

	// フェード処理
	CFade::Init();

	//CManager::m_Mode = new CModeMapMake();
	//CManager::m_Mode = new CModeGame();
	CManager::m_Mode = new CModeTitle();

	// デバッグカメラのセット
	m_DebugCamera = CCamera::Create();

	if (CManager::m_Mode != NULL)
	{
		CManager::m_Mode->Init();
	}

	// スキンメッシュモデルの描画距離
	m_SkinMeshDrawDist = DRAW_DIST;

	// 投影テクスチャオブジェクト(レンダーターゲット及びそれに合わせたＺバッファ)の作成
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return false;
	}

	HRESULT	hr;
	// テクスチャを作成する
	hr = pDevice->CreateTexture(
		TEXMAP_SIZE, TEXMAP_SIZE, 1, D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_ShadowTex,
		nullptr);

	if (FAILED(hr))
	{
		MessageBox(nullptr, "ERROR!!", "テクスチャ(レンダーターゲット)生成エラー", MB_OK);
		return false;
	}

	hr = m_ShadowTex->GetSurfaceLevel(0, &m_ShadowSurface);
	if (FAILED(hr)) {
		MessageBox(nullptr, "ERROR!!", "サーフェス(レンダーターゲット)取得エラー", MB_OK);
		return false;
	}

	// レンダーターゲットに合わせたＺバッファを作成
	hr = pDevice->CreateDepthStencilSurface(TEXMAP_SIZE, TEXMAP_SIZE, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, &m_ShadowZbufferSurface, nullptr);
	if (FAILED(hr)) {
		MessageBox(nullptr, "ERROR!!", "Ｚバッファ生成エラー", MB_OK);
		return false;
	}

	return true;
}

void CManager::Uninit()
{
	// スキンメッシュファイルの一括リリース
	CSceneSkinMesh::ReleaseFileAll();

	CFade::Uninit();

	CManager::m_Mode->Uninit();
	delete CManager::m_Mode;

	// デバッグカメラの解放
	m_DebugCamera->Release();

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

	// コントローラーの解放処理
	if (m_Controller)
	{
		m_Controller->Uninit();
		delete m_Controller;
		m_Controller = NULL;
	}

	// サウンドを解放
	CSound::Uninit();

	// 使ったシェーダーを解放
	CShader::ReleaseAll();

	// imguiの終了処理
	CImGui::Uninit();

	if (m_ShadowTex != nullptr) {
		m_ShadowTex->Release();
		m_ShadowTex = nullptr;
	}

	if (m_ShadowSurface != nullptr) {
		m_ShadowSurface->Release();
		m_ShadowSurface = nullptr;
	}

	if (m_ShadowZbufferSurface != nullptr) {
		m_ShadowZbufferSurface->Release();
		m_ShadowZbufferSurface = nullptr;
	}

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

	m_Controller->Update();

	if (m_InputKeyboard->GetKeyTrigger(DIK_AT))
	{
		ChangeDebugMode();
	}

	m_Mode->Update();

	// デバッグ中はデバッグカメラを有効に
	if (m_isDebug)
	{
		DebugCameraControll();
	}

	m_UsingCamera->Update();

	// キーボード取得
	CInputKeyboard *inputKeyboard;
	inputKeyboard = CManager::GetInputKeyboard();

	if (inputKeyboard->GetKeyTrigger(DIK_M))
	{
		CInput::ChangeShowCursol();
	}

}

void CManager::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	HRESULT hr;

	hr = CRenderer::DrawBegin();

	//Direct3Dによる描画の開始
	if (SUCCEEDED(hr))
	{
		//各種行列の設定
		pDevice->SetTransform(D3DTS_VIEW, &m_UsingCamera->GetView());
		pDevice->SetTransform(D3DTS_PROJECTION, &m_UsingCamera->GetProjection());

		DrawShadow();

		pDevice->SetRenderState(D3DRS_FOGENABLE, m_isFogEnable); //フォグ

		//描画
		m_Mode->Draw();

		// シャドウマップテクスチャ
		//DrawQuad(pDevice, 10, 10, 200, 200, m_ShadowTex);

		if (CManager::GetDebug())
		{
			ImGui::Begin("Debug Window");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		CImGui::EndDraw();

		CFade::Draw();
	}

	CRenderer::DrawEnd();
}

void CManager::DrawShadow()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	LPDIRECT3DSURFACE9 oldsurface;
	LPDIRECT3DSURFACE9 oldzbuffer;
	D3DVIEWPORT9	oldviewport;

	// 現在のレンダリングターゲット、Ｚバッファ、ビューポートを取得（後で戻すため）
	GetCurrentRendertarget(oldsurface, oldzbuffer, oldviewport);

	// 投影シャドウ作成（光源の位置にカメラを置いて、テクスチャにレンダリング）
	CreateShadow();

	// レンダーターゲットを元に戻す
	SetRenderTarget(oldsurface, oldzbuffer, oldviewport);
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

	m_isDebug = false;
	m_TempCamera = NULL;
}

void CManager::ChangeDebugMode()
{
	m_isDebug = !m_isDebug; 

	CWayPoint::Debug();

	if (m_isDebug)
	{
		m_TempCamera = m_UsingCamera;
		m_UsingCamera = m_DebugCamera;
	}
	else
	{
		if (m_TempCamera)
		{
			m_UsingCamera = m_TempCamera;
		}
	}
}

void CManager::DebugCameraControll()
{
	ImGui::Begin("Debug");
	ImGui::Checkbox("FogEnable", &m_isFogEnable);
	ImGui::SliderFloat("CameraSpeed", &CAMERA_SPEED, 0.0f, 5.0f);
	ImGui::End();

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

	// カメラ操作
	if (inputMouse->GetRightPress())
	{
		D3DXVECTOR3 camerapos = m_DebugCamera->GetPos();
		float moveX = 0.0f, moveY = 0.0f, moveZ = 0.0f;
		if (inputKeyboard->GetKeyPress(DIK_A))
		{
			moveX = -1.0f;
		}
		if (inputKeyboard->GetKeyPress(DIK_D))
		{
			moveX = 1.0f;
		}
		if (inputKeyboard->GetKeyPress(DIK_Q))
		{
			moveY = -1.0f;
		}
		if (inputKeyboard->GetKeyPress(DIK_E))
		{
			moveY = 1.0f;
		}
		if (inputKeyboard->GetKeyPress(DIK_W))
		{
			moveZ = 1.0f;
		}
		if (inputKeyboard->GetKeyPress(DIK_S))
		{
			moveZ = -1.0f;
		}

		camerapos = camerapos + m_DebugCamera->GetRight() * moveX * CAMERA_SPEED;
		camerapos = camerapos + m_DebugCamera->GetUp() * moveY * CAMERA_SPEED;
		camerapos = camerapos + m_DebugCamera->GetFront() * moveZ * CAMERA_SPEED;

		m_DebugCamera->Move(camerapos - m_DebugCamera->GetPos());

		// 回転
		m_DebugCamera->Rotation(PI * mouseX * VALUE_ROTATE_MOUSE, PI * mouseY * VALUE_ROTATE_MOUSE);
	}
}

void CManager::GetCurrentRendertarget(
	LPDIRECT3DSURFACE9& currentsurface,		// 現在の描画サーフェスを保存するためのサーフェス
	LPDIRECT3DSURFACE9& currentzbuffer, 	// 現在のＺバッファを保存するためのサーフェス
	D3DVIEWPORT9& currentvp) 				// 現在のビューポートを保存するためのもの
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	// 現在のサーフェスを取得
	pDevice->GetRenderTarget(0, &currentsurface);

	// 現在のビューポートを取得
	pDevice->GetViewport(&currentvp);

	// 現在のＺバッファを取得
	pDevice->GetDepthStencilSurface(&currentzbuffer);
}

void CManager::SetRenderTarget(
	LPDIRECT3DSURFACE9 rendersurface,	// レンダーターゲット
	LPDIRECT3DSURFACE9 zbuffer,			// Ｚバッファ
	D3DVIEWPORT9&		vp) 			// ビューポート
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	// 投影テクスチャ作成用にレンダリングターゲットをセット
	pDevice->SetRenderTarget(0, rendersurface);

	// 投影テクスチャ作成用にＺバッファセット
	pDevice->SetDepthStencilSurface(zbuffer);

	// 投影テクスチャ作成用に　ビューポートセット
	pDevice->SetViewport(&vp);
}

void CManager::CreateShadow()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	// ビューポート
	D3DVIEWPORT9 vp = { 0, 0, TEXMAP_SIZE, TEXMAP_SIZE, 0.0f, 1.0f };

	// レンダーターゲット設定
	SetRenderTarget(m_ShadowSurface, m_ShadowZbufferSurface, vp);
	// ターゲットバッファのクリア、Ｚバッファのクリア
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 0), 1.0f, 0);

	D3DXVECTOR3	lightPos = m_UsingCamera->GetPos();
	D3DXVECTOR3 at = lightPos;
	D3DXVECTOR3 front = m_UsingCamera->GetFront();
	//lightPos -= front * 5.0f;
	/*
	if (lightPos.y < 0.1f)
	{
		lightPos.y = 0.1f;
	}*/

	front.y = 0;
	D3DXVec3Normalize(&front, &front);
	lightPos -= front * 3.0f;
	at += front * 30.0f;
	at.y = 0;
	D3DXVECTOR3 up(0, 1, 0);
	lightPos.y += 0.5f;

	D3DXMatrixLookAtLH(&m_LightCamera, &lightPos, &at, &up);
	D3DXMatrixPerspectiveFovLH(&m_LightProjection, D3DXToRadian(m_UsingCamera->GetFov()), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.001f, 100.0f);

	// シャドウボリューム描画
	CScene::DrawShadowAll();
}

void CManager::DrawQuad(LPDIRECT3DDEVICE9 device, float x, float y, float width, float height, LPDIRECT3DTEXTURE9 tex) {

	// 頂点フォーマットの定義
	struct	VERTEX {
		float		x, y, z;
		float		rhw;
		float		tu, tv;
	};

	static VERTEX vertex[4];

	vertex[0].x = (float)x;
	vertex[0].y = (float)y;
	vertex[1].x = (float)(vertex[0].x + width);
	vertex[1].y = (float)vertex[0].y;
	vertex[2].x = (float)(vertex[0].x + width);
	vertex[2].y = (float)(vertex[0].y + height);
	vertex[3].x = (float)vertex[0].x;
	vertex[3].y = (float)(vertex[0].y + height);

	vertex[0].rhw = 1;
	vertex[1].rhw = 1;
	vertex[2].rhw = 1;
	vertex[3].rhw = 1;

	// 表示テクスチャ座標セット
	vertex[0].tu = 0.0f;
	vertex[0].tv = 0.0f;
	vertex[1].tu = 1.0f;
	vertex[1].tv = 0.0f;
	vertex[2].tu = 1.0f;
	vertex[2].tv = 1.0f;
	vertex[3].tu = 0.0f;
	vertex[3].tv = 1.0f;

	// テクスチャをセット
	device->SetTexture(0, tex);

	// 頂点フォーマットをセット
	device->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);

	// ４角形を描画
	device->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		vertex,
		sizeof(struct VERTEX));
}