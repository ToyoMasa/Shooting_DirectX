//======================================================================
//	ゲームコントローラー処理　（2017/12/02）
//	Author : 豊村 昌俊
//======================================================================
#include <Windows.h>
#include <Xinput.h>
#include "common.h"
#include "main.h"
#include "camera.h"
#include "controller.h"

#pragma comment (lib, "xinput.lib")

#define LEFT_THUMB_DEADZONE (7849)
#define RIGHT_THUMB_DEADZONE (6881)
#define NUM_CONTROLLER (2)

//======================================================================
//	静的メンバ変数の初期化
//======================================================================
int CController::m_NumController = 0;

void CController::Init()
{
	ZeroMemory(&m_State, sizeof(XINPUT_STATE));

	m_Result = XInputGetState(
		0,       // DWORD         dwUserIndex
		&m_State); // XINPUT_STATE* pState

	m_NumController++;
}

void CController::Update()
{
	m_OldState = m_State;

	ZeroMemory(&m_State, sizeof(XINPUT_STATE));

	m_Result = XInputGetState(
		0,       // DWORD         dwUserIndex
		&m_State); // XINPUT_STATE* pState

	if (m_Result == ERROR_SUCCESS)
	{
		m_isAble = true; 
		
		if ((m_State.Gamepad.sThumbLX < LEFT_THUMB_DEADZONE &&
			m_State.Gamepad.sThumbLX > -LEFT_THUMB_DEADZONE) &&
			(m_State.Gamepad.sThumbLY < LEFT_THUMB_DEADZONE &&
				m_State.Gamepad.sThumbLY > -LEFT_THUMB_DEADZONE))
		{
			m_State.Gamepad.sThumbLX = 0;
			m_State.Gamepad.sThumbLY = 0;
			m_JoyStickLX = 0;
			m_JoyStickLY = 0;
		}
		else
		{
			m_JoyStickLX = m_State.Gamepad.sThumbLX / 32768.0f;
			m_JoyStickLY = m_State.Gamepad.sThumbLY / 32768.0f;
		}

		if ((m_State.Gamepad.sThumbRX < RIGHT_THUMB_DEADZONE &&
			m_State.Gamepad.sThumbRX > -RIGHT_THUMB_DEADZONE) &&
			(m_State.Gamepad.sThumbRY < RIGHT_THUMB_DEADZONE &&
				m_State.Gamepad.sThumbRY > -RIGHT_THUMB_DEADZONE))
		{
			m_State.Gamepad.sThumbRX = 0;
			m_State.Gamepad.sThumbRY = 0;
			m_JoyStickRX = 0;
			m_JoyStickRY = 0;
		}
		else
		{
			m_JoyStickRX = m_State.Gamepad.sThumbRX / 32768.0f;
			m_JoyStickRY = m_State.Gamepad.sThumbRY / 32768.0f;
		}
	}
	else
	{
		m_isAble = false;
		m_JoyStickRX = 0;
		m_JoyStickRY = 0;
	}
}

void CController::Uninit()
{
	XInputEnable(false);
}

// ボタン入力
bool CController::ButtonPress(WORD button)
{
	if (!m_isAble)
	{
		return false;
	}

	return m_State.Gamepad.wButtons & button;
}

//	ボタンリリース
bool CController::ButtonRelease(WORD button)
{
	if (!m_isAble)
	{
		return false;
	}
	WORD triKey = (m_State.Gamepad.wButtons ^ m_OldState.Gamepad.wButtons) & m_OldState.Gamepad.wButtons;

	return triKey & button;
}

// トリガー入力
bool CController::ButtonTrigger(WORD button)
{
	if (!m_isAble)
	{
		return false;
	}

	WORD triKey = (m_State.Gamepad.wButtons ^ m_OldState.Gamepad.wButtons) & m_State.Gamepad.wButtons;

	return (triKey & button);
}
