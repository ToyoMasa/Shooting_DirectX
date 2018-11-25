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

class CManager
{
public:
	CManager() {}
	~CManager() {}

	static bool Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	static void Uninit();
	static void Update();
	static void Draw();

	static CInputKeyboard *GetInputKeyboard(void) { return m_InputKeyboard; }
	static CInputMouse *GetInputMouse(void) { return m_InputMouse; }
	static void SetMode(CMode* mode);
	static CMode* GetMode() { return m_Mode; }
	static void SetCamera(CCamera* camera) { m_UsingCamera = camera; }
	static CCamera* GetCamera() { return m_UsingCamera; }

	static void ChangeDebugMode();
	static void DebugCameraControll();
	static bool& GetDebug() { return m_isDebug; }
	static bool& GetFogEnable() { return m_isFogEnable; }

private:
	static CInputKeyboard*	m_InputKeyboard;	// キーボードへのポインタ
	static CInputMouse*		m_InputMouse;		// マウスへのポインタ
	static CLight*			m_Light;
	static CMode*			m_Mode;
	static CCamera*			m_UsingCamera;
	static CCamera*			m_DebugCamera;
	static CCamera*			m_TempCamera;
	static bool				m_isDebug;
	static bool				m_isFogEnable;
};

#endif // !_MANAGER_H_
