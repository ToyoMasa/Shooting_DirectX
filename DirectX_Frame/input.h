//=============================================================================
// 入力処理 [input.h]
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib, "DXGUID.LIB")

#include<windows.h>
#define DIRECTINPUT_VERSION (0x0800)		//dinput.hより前に書く
#include <dinput.h>

//*********************************************************
// 入力クラス
//*********************************************************
class CInput
{
public:
	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInst, HWND hWnd);
	virtual void Uninit(void);
	virtual HRESULT Update(void) = 0;		// ---> 純粋仮想関数化

protected:
	LPDIRECTINPUTDEVICE8		m_pDIDevice;		// IDirectInputDevice8インターフェースへのポインタ
	static LPDIRECTINPUT8		m_pDInput;			// IDirectInput8インターフェースへのポインタ
};

//*********************************************************
// キーボード入力クラス
//*********************************************************
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();

	HRESULT Init(HINSTANCE hInst, HWND hWnd);
	void Uninit(void);
	HRESULT Update(void);

	BOOL GetKeyPress(int nKey);
	BOOL GetKeyTrigger(int nKey);
	BOOL GetKeyRelease(int nKey);
	BOOL GetKeyRepeat(int nKey);
	void FlushKeyTrigger(int nKey);

private:
	BYTE			m_aKeyState[256];			// キーボードの状態を受け取るワーク
	BYTE			m_aKeyStateTrigger[256];	// トリガーワーク
	BYTE			m_aKeyStateRelease[256];	// リリースワーク
	BYTE			m_aKeyStateRepeat[256];		// リピートワーク
	int				m_aKeyStateRepeatCnt[256];	// リピートカウンタ
};

//*********************************************************
// マウス入力クラス
//*********************************************************
class CInputMouse : public CInput
{
public:
	CInputMouse();
	~CInputMouse();

	HRESULT Init(HINSTANCE hInst, HWND hWnd);
	void Uninit(void);
	HRESULT Update(void);

	BOOL GetLeftPress(void);
	BOOL GetLeftTrigger(void);
	BOOL GetLeftRelease(void);
	BOOL GetRightPress(void);
	BOOL GetRightTrigger(void);
	BOOL GetRightRelease(void);
	BOOL GetCenterPress(void);
	BOOL GetCenterTrigger(void);
	LONG GetAxisX(void);
	LONG GetAxisY(void);
	LONG GetAxisZ(void);
	POINT *GetPosWorld(void) { return &m_posMouseWorld; }

private:
	DIMOUSESTATE2	m_mouseState;				// マウスの状態を受け取るワーク
	DIMOUSESTATE2	m_mouseStateTrigger;		// トリガーワーク
	DIMOUSESTATE2	m_mouseStateRelease;		// トリガーリリースワーク
	POINT			m_posMouseWorld;			// マウスのスクリーン座標
};

#endif
