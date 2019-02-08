//======================================================================
//	ゲーム管理ヘッダ[manager]
//
//======================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

class CInputKeyboard;
class CInputMouse;
class CCamera;
class CLight;
class CMode;

static const float DRAW_DIST = 40.0f;

class CManager
{
public:
	CManager() {}
	~CManager() {}

	static bool Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	static void Uninit();
	static void Update();
	static void Draw();
	static void DrawShadow();

	static CInputKeyboard *GetInputKeyboard(void) { return m_InputKeyboard; }
	static CInputMouse *GetInputMouse(void) { return m_InputMouse; }
	static void SetMode(CMode* mode);
	static CMode* GetMode() { return m_Mode; }
	static void SetCamera(CCamera* camera) { m_UsingCamera = camera; }
	static CCamera* GetCamera() { return m_UsingCamera; }

	static void ChangeDebugMode();
	static void DebugCameraControll();
	static bool GetDebug() { return m_isDebug; }
	static bool GetFogEnable() { return m_isFogEnable; }
	static void SetSkinMeshDrawDist(float dist) { m_SkinMeshDrawDist = dist; }
	static float GetSkinMeshDrawDist() { return m_SkinMeshDrawDist; }

	static void GetCurrentRendertarget(
		LPDIRECT3DSURFACE9& currentsurface,		// 現在の描画サーフェスを保存するためのサーフェス
		LPDIRECT3DSURFACE9& currentzbuffer, 	// 現在のＺバッファを保存するためのサーフェス
		D3DVIEWPORT9& currentvp);				// 現在のビューポートを保存するためのもの
	static void SetRenderTarget(
		LPDIRECT3DSURFACE9 rendersurface,	// レンダーターゲット
		LPDIRECT3DSURFACE9 zbuffer,			// Ｚバッファ
		D3DVIEWPORT9&		vp); 			// ビューポート
	static void CreateShadow();
	static D3DXMATRIX GetLightCamera() { return m_LightCamera; }
	static D3DXMATRIX GetLightProjection() { return m_LightProjection; }

	static LPDIRECT3DTEXTURE9 GetShadowTexture() { return m_ShadowTex; }
	static void DrawQuad(LPDIRECT3DDEVICE9 device, float x, float y, float width, float height, LPDIRECT3DTEXTURE9 tex);

private:
	static CInputKeyboard*	m_InputKeyboard;	// キーボードへのポインタ
	static CInputMouse*		m_InputMouse;		// マウスへのポインタ
	static CLight*			m_Light;
	static CMode*			m_Mode;
	static CCamera*			m_UsingCamera;
	static CCamera*			m_DebugCamera;
	static CCamera*			m_TempCamera;
	// 投影テクスチャマップ作成用
	static LPDIRECT3DTEXTURE9	m_ShadowTex;				// 投影シャドウテクスチャ
	static LPDIRECT3DSURFACE9	m_ShadowSurface;			// 投影シャドウテクスチャサーフェス	
	static LPDIRECT3DSURFACE9	m_ShadowZbufferSurface;		// 投影テクスチャへレンダリングする際のＺバッファ（深度バッファ）
	static D3DXMATRIX			m_LightCamera;				// ライトの位置をカメラの位置にした時のビュー行列
	static D3DXMATRIX			m_LightProjection;			// プロジェクション変換行列を作る(投影シャドウ用)
	static bool				m_isDebug;
	static bool				m_isFogEnable;
	static float			m_SkinMeshDrawDist;
};

#endif // !_MANAGER_H_
