//======================================================================
//	�Q�[���R���g���[���[�����@�i2017/12/02�j
//	Author : �L�� ���r
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
//	�ÓI�����o�ϐ��̏�����
//======================================================================
int CController::m_NumController = 0;

void CController::Init()
{
	ZeroMemory(&m_State, sizeof(XINPUT_STATE));

	m_Result = XInputGetState(
		CController::m_NumController,       // DWORD         dwUserIndex
		&m_State); // XINPUT_STATE* pState

	m_NumController++;
}

void CController::Uninit()
{
	XInputEnable(false);
}

// �g���K�[����
WORD CController::Trigger(void)
{
	WORD triKey = (m_State.Gamepad.wButtons ^ m_OldState.Gamepad.wButtons) & m_State.Gamepad.wButtons;

	return triKey;
}
