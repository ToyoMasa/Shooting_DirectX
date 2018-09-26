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
		CController::m_NumController,       // DWORD         dwUserIndex
		&m_State); // XINPUT_STATE* pState

	m_NumController++;
}

void CController::Uninit()
{
	XInputEnable(false);
}
//
//void TitleController(void)
//{
//	ZeroMemory(&state, sizeof(XINPUT_STATE));
//
//	dwResult = XInputGetState(
//		0,       // DWORD         dwUserIndex
//		&state); // XINPUT_STATE* pState
//
//	if (dwResult == ERROR_SUCCESS)
//	{
//		if (GetTitleState() == SHOW_TITLE)
//		{
//			if (state.Gamepad.wButtons)
//			{
//				ChangeTitleState();
//				PlaySound(SOUND_LABEL_SE_BUTTON);
//			}
//		}
//		else
//		{
//			if (GamePadTrigger() & XINPUT_GAMEPAD_B)
//			{
//				ChangeTitleState();
//				PlaySound(SOUND_LABEL_SE_BUTTON);
//			}
//
//			if (GamePadTrigger() & XINPUT_GAMEPAD_DPAD_LEFT ||
//				GamePadTrigger() & XINPUT_GAMEPAD_DPAD_RIGHT ||
//				GamePadTrigger() & XINPUT_GAMEPAD_DPAD_UP ||
//				GamePadTrigger() & XINPUT_GAMEPAD_DPAD_DOWN)
//			{
//				ChangeGameMode();
//				PlaySound(SOUND_LABEL_SE_CURSOR);
//			}
//		}
//	}
//	else
//	{
//		if (GetKeyboardTrigger(DIK_RETURN))
//		{
//			ChangeTitle();
//		}
//	}
//
//	oldState = state;
//}
//
//void GameController(void)
//{
//	ZeroMemory(&state, sizeof(XINPUT_STATE));
//
//	dwResult = XInputGetState(
//		0,       // DWORD         dwUserIndex
//		&state); // XINPUT_STATE* pState
//
//	g_nCountStopFire++;
//
//	if (dwResult == ERROR_SUCCESS)
//	{
//		// Controller is connected 
//		// Zero value if thumbsticks are within the dead zone 
//		if ((state.Gamepad.sThumbLX < LEFT_THUMB_DEADZONE &&
//			state.Gamepad.sThumbLX > -LEFT_THUMB_DEADZONE) &&
//			(state.Gamepad.sThumbLY < LEFT_THUMB_DEADZONE &&
//				state.Gamepad.sThumbLY > -LEFT_THUMB_DEADZONE))
//		{
//			state.Gamepad.sThumbLX = 0;
//			state.Gamepad.sThumbLY = 0;
//			JoyStickLX = 0;
//			JoyStickLY = 0;
//		}
//		else
//		{
//			JoyStickLX = state.Gamepad.sThumbLX / 32768.0f;
//			JoyStickLY = state.Gamepad.sThumbLY / 32768.0f;
//		}
//
//		if ((state.Gamepad.sThumbRX < RIGHT_THUMB_DEADZONE &&
//			state.Gamepad.sThumbRX > -RIGHT_THUMB_DEADZONE) &&
//			(state.Gamepad.sThumbRY < RIGHT_THUMB_DEADZONE &&
//				state.Gamepad.sThumbRY > -RIGHT_THUMB_DEADZONE))
//		{
//			state.Gamepad.sThumbRX = 0;
//			state.Gamepad.sThumbRY = 0;
//			JoyStickRX = 0;
//			JoyStickRY = 0;
//		}
//		else
//		{
//			JoyStickRX = state.Gamepad.sThumbRX / 32768.0f;
//			JoyStickRY = state.Gamepad.sThumbRY / 32768.0f;
//		}
//
//
//		// ここから各種入力処理
//
//		// ポーズ遷移
//		if (GamePadTrigger() & XINPUT_GAMEPAD_START)
//		{
//			PlaySound(SOUND_LABEL_SE_BUTTON);
//			InitPause();
//			SetPause();
//		}
//
//		// 種投げモードのON/OFF
//		if (GamePadTrigger() & XINPUT_GAMEPAD_LEFT_SHOULDER)
//		{
//			if (GetNumSeed() > 0)
//			{
//				if (bSeedMode)
//				{
//					bSeedMode = false;
//				}
//				else
//				{
//					bSeedMode = true;
//				}
//			}
//		}
//
//		// リロード
//		if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_X) && !state.Gamepad.bRightTrigger)
//		{
//			IncreaseWater(0.3f);
//		}
//
//		// 照準の移動
//		PointerMove(JoyStickLX, JoyStickLY);
//		PointerMove(JoyStickRX, JoyStickRY);
//
//		if (state.Gamepad.sThumbRX || state.Gamepad.sThumbLX)
//		{
//			PlayerRotate();
//		}
//
//		if (!bSeedMode)
//		{
//			// 残りの水があるか
//			if (GetWater() > SHOT_WATER)
//			{
//				// 射撃
//				if (state.Gamepad.bRightTrigger && g_nCountStopFire > BULLET_RATE)
//				{
//					CreateBullet(GetPlayerPos(), GetPlayerFront());
//
//					g_nCountStopFire = 0;
//
//					// 残水量を減らす
//					DecreaseWater(SHOT_WATER);
//				}
//			}
//
//			// 残りの水があるか
//			if (GetWater() > SHOT_WATER_BOMB)
//			{
//				if (GamePadTrigger() & XINPUT_GAMEPAD_RIGHT_SHOULDER)
//				{
//					CreateWater(GetPlayerPos(), GetPointerPos());
//
//					// 残水量を減らす
//					DecreaseWater(SHOT_WATER_BOMB);
//				}
//			}
//		}
//		else
//		{
//			// トラップ設置
//			if ((GamePadTrigger() & XINPUT_GAMEPAD_RIGHT_SHOULDER) || state.Gamepad.bRightTrigger)
//			{
//				//			CreateTrap(GetPointerPos());
//				DecreaseSeed();
//				CreateSeed(GetPlayerPos(), GetPointerPos());
//				bSeedMode = false;
//			}
//		}
//
//		oldState = state;
//	}
//	else
//	{
//		// Controller is not connected 
//
//	}
//
//	float fMoveX = 0;
//	float fMoveY = 0;
//
//	if (GetKeyboardPress(DIK_LEFT))
//	{
//		fMoveX = -1.0f;
//	}
//	if (GetKeyboardPress(DIK_RIGHT))
//	{
//		fMoveX = 1.0f;
//	}
//	if (GetKeyboardPress(DIK_UP))
//	{
//		fMoveY = 1.0f;
//	}
//	if (GetKeyboardPress(DIK_DOWN))
//	{
//		fMoveY = -1.0f;
//	}
//
//	// 照準の移動
//	PointerMove(fMoveX, fMoveY);
//
//	// ポーズ遷移
//	if (GetKeyboardPress(DIK_P))
//	{
//		InitPause();
//		SetPause();
//	}
//	PlayerRotate();
//
//	if (GetKeyboardPress(DIK_SPACE) && g_nCountStopFire > BULLET_RATE)
//	{
//		// 残りの水があるか
//		if (GetWater() > 0.0f)
//		{
//			CreateBullet(GetPlayerPos(), GetPlayerFront());
//
//			g_nCountStopFire = 0;
//
//			// 残水量を減らす
//			DecreaseWater(SHOT_WATER);
//		}
//	}
//
//	if (GetKeyboardTrigger(DIK_J))
//	{
//		// 残りの水があるか
//		if (GetWater() > 0.0f)
//		{
//			CreateWater(GetPlayerPos(), GetPointerPos());
//
//			// 残水量を減らす
//			DecreaseWater(SHOT_WATER_BOMB);
//		}
//	}
//}
//
//void DuoGameController(void)
//{
//	ZeroMemory(&state, sizeof(XINPUT_STATE));
//	ZeroMemory(&state2, sizeof(XINPUT_STATE));
//
//	g_nCountStopFires[MALE]++;
//	g_nCountStopFires[FEMALE]++;
//
//	// 1Pの処理
//	dwResult = XInputGetState(
//		0,       // DWORD         dwUserIndex
//		&state); // XINPUT_STATE* pState
//
//	if (dwResult == ERROR_SUCCESS)
//	{
//		// Controller is connected 
//		// Zero value if thumbsticks are within the dead zone 
//		if ((state.Gamepad.sThumbLX < LEFT_THUMB_DEADZONE &&
//			state.Gamepad.sThumbLX > -LEFT_THUMB_DEADZONE) &&
//			(state.Gamepad.sThumbLY < LEFT_THUMB_DEADZONE &&
//				state.Gamepad.sThumbLY > -LEFT_THUMB_DEADZONE))
//		{
//			state.Gamepad.sThumbLX = 0;
//			state.Gamepad.sThumbLY = 0;
//			JoyStickLX = 0;
//			JoyStickLY = 0;
//		}
//		else
//		{
//			JoyStickLX = state.Gamepad.sThumbLX / 32768.0f;
//			JoyStickLY = state.Gamepad.sThumbLY / 32768.0f;
//		}
//
//		if ((state.Gamepad.sThumbRX < RIGHT_THUMB_DEADZONE &&
//			state.Gamepad.sThumbRX > -RIGHT_THUMB_DEADZONE) &&
//			(state.Gamepad.sThumbRY < RIGHT_THUMB_DEADZONE &&
//				state.Gamepad.sThumbRY > -RIGHT_THUMB_DEADZONE))
//		{
//			state.Gamepad.sThumbRX = 0;
//			state.Gamepad.sThumbRY = 0;
//			JoyStickRX = 0;
//			JoyStickRY = 0;
//		}
//		else
//		{
//			JoyStickRX = state.Gamepad.sThumbRX / 32768.0f;
//			JoyStickRY = state.Gamepad.sThumbRY / 32768.0f;
//		}
//
//
//		// ここから各種入力処理
//
//		// ポーズ遷移
//		if (GamePadTrigger() & XINPUT_GAMEPAD_START)
//		{
//			PlaySound(SOUND_LABEL_SE_BUTTON);
//			InitPause();
//			SetPause();
//		}
//
//		// 種投げモードのON/OFF
//		if (GamePadTrigger() & XINPUT_GAMEPAD_LEFT_SHOULDER)
//		{
//			if (GetNumSeed(MALE) > 0)
//			{
//				if (bSeedModes[MALE])
//				{
//					bSeedModes[MALE] = false;
//				}
//				else
//				{
//					bSeedModes[MALE] = true;
//				}
//			}
//		}
//
//		// リロード
//		if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_X) && !state.Gamepad.bRightTrigger)
//		{
//			IncreaseWater(MALE, 0.3f);
//		}
//
//		// 照準の移動
//		DuoPointerMove(MALE, JoyStickLX, JoyStickLY);
//		DuoPointerMove(MALE, JoyStickRX, JoyStickRY);
//
//		if (state.Gamepad.sThumbRX || state.Gamepad.sThumbLX)
//		{
//			PlayerRotate(MALE);
//		}
//
//		if (!bSeedModes[MALE])
//		{
//			// 残りの水があるか
//			if (GetWater(MALE) > SHOT_WATER)
//			{
//				// 射撃
//				if (state.Gamepad.bRightTrigger && g_nCountStopFires[MALE] > BULLET_RATE)
//				{
//					CreateBullet(GetPlayerPos(MALE), GetPlayerFront(MALE));
//
//					g_nCountStopFires[MALE] = 0;
//
//					// 残水量を減らす
//					DecreaseWater(MALE, SHOT_WATER);
//				}
//
//				// 残りの水があるか
//				if (GetWater(MALE) > SHOT_WATER_BOMB)
//				{
//					if (GamePadTrigger() & XINPUT_GAMEPAD_RIGHT_SHOULDER)
//					{
//						CreateWater(GetPlayerPos(MALE), GetDuoPointerPos(MALE));
//
//						// 残水量を減らす
//						DecreaseWater(MALE, SHOT_WATER_BOMB);
//					}
//				}
//			}
//		}
//		else
//		{
//			// トラップ設置
//			if ((GamePadTrigger() & XINPUT_GAMEPAD_RIGHT_SHOULDER) || state.Gamepad.bRightTrigger)
//			{
//				//			CreateTrap(GetPointerPos());
//				DecreaseSeed(MALE);
//				CreateSeed(GetPlayerPos(MALE), GetDuoPointerPos(MALE));
//				bSeedModes[MALE] = false;
//			}
//		}
//
//		oldState = state;
//	}
//	else
//	{
//		// Controller is not connected 
//
//	}
//
//	// 2Pの処理
//	dwResult = XInputGetState(
//		1,       // DWORD         dwUserIndex
//		&state2); // XINPUT_STATE* pState
//
//	if (dwResult == ERROR_SUCCESS)
//	{
//		// Controller is connected 
//		// Zero value if thumbsticks are within the dead zone 
//		if ((state2.Gamepad.sThumbLX < LEFT_THUMB_DEADZONE &&
//			state2.Gamepad.sThumbLX > -LEFT_THUMB_DEADZONE) &&
//			(state2.Gamepad.sThumbLY < LEFT_THUMB_DEADZONE &&
//				state2.Gamepad.sThumbLY > -LEFT_THUMB_DEADZONE))
//		{
//			state2.Gamepad.sThumbLX = 0;
//			state2.Gamepad.sThumbLY = 0;
//			JoyStickLX = 0;
//			JoyStickLY = 0;
//		}
//		else
//		{
//			JoyStickLX = state2.Gamepad.sThumbLX / 32768.0f;
//			JoyStickLY = state2.Gamepad.sThumbLY / 32768.0f;
//		}
//
//		if ((state2.Gamepad.sThumbRX < RIGHT_THUMB_DEADZONE &&
//			state2.Gamepad.sThumbRX > -RIGHT_THUMB_DEADZONE) &&
//			(state2.Gamepad.sThumbRY < RIGHT_THUMB_DEADZONE &&
//				state2.Gamepad.sThumbRY > -RIGHT_THUMB_DEADZONE))
//		{
//			state2.Gamepad.sThumbRX = 0;
//			state2.Gamepad.sThumbRY = 0;
//			JoyStickRX = 0;
//			JoyStickRY = 0;
//		}
//		else
//		{
//			JoyStickRX = state2.Gamepad.sThumbRX / 32768.0f;
//			JoyStickRY = state2.Gamepad.sThumbRY / 32768.0f;
//		}
//
//
//		// ここから各種入力処理
//
//		// ポーズ遷移
//		if (GamePad2Trigger() & XINPUT_GAMEPAD_START)
//		{
//			PlaySound(SOUND_LABEL_SE_BUTTON);
//			InitPause();
//			SetPause();
//		}
//
//		// 種投げモードのON/OFF
//		if (GamePad2Trigger() & XINPUT_GAMEPAD_LEFT_SHOULDER)
//		{
//			if (GetNumSeed(FEMALE) > 0)
//			{
//				if (bSeedModes[FEMALE])
//				{
//					bSeedModes[FEMALE] = false;
//				}
//				else
//				{
//					bSeedModes[FEMALE] = true;
//				}
//			}
//		}
//
//		// リロード
//		if ((state2.Gamepad.wButtons & XINPUT_GAMEPAD_X) && !state2.Gamepad.bRightTrigger)
//		{
//			IncreaseWater(FEMALE, 0.3f);
//		}
//
//		// 照準の移動
//		DuoPointerMove(FEMALE, JoyStickLX, JoyStickLY);
//		DuoPointerMove(FEMALE, JoyStickRX, JoyStickRY);
//
//		if (state2.Gamepad.sThumbRX || state2.Gamepad.sThumbLX)
//		{
//			PlayerRotate(FEMALE);
//		}
//
//		if (!bSeedModes[FEMALE])
//		{
//			// 残りの水があるか
//			if (GetWater(FEMALE) > SHOT_WATER)
//			{
//				// 射撃
//				if (state2.Gamepad.bRightTrigger && g_nCountStopFires[FEMALE] > BULLET_RATE)
//				{
//					CreateBullet(GetPlayerPos(FEMALE), GetPlayerFront(FEMALE));
//
//					g_nCountStopFires[FEMALE] = 0;
//
//					// 残水量を減らす
//					DecreaseWater(FEMALE, SHOT_WATER);
//				}
//
//
//				// 残りの水があるか
//				if (GetWater(MALE) > SHOT_WATER_BOMB)
//				{
//					if (GamePad2Trigger() & XINPUT_GAMEPAD_RIGHT_SHOULDER)
//					{
//						CreateWater(GetPlayerPos(FEMALE), GetDuoPointerPos(FEMALE));
//
//						// 残水量を減らす
//						DecreaseWater(FEMALE, SHOT_WATER_BOMB);
//					}
//				}
//			}
//		}
//		else
//		{
//			// トラップ設置
//			if ((GamePad2Trigger() & XINPUT_GAMEPAD_RIGHT_SHOULDER) || state2.Gamepad.bRightTrigger)
//			{
//				//			CreateTrap(GetPointerPos());
//				DecreaseSeed(FEMALE);
//				CreateSeed(GetPlayerPos(FEMALE), GetDuoPointerPos(FEMALE));
//				bSeedModes[FEMALE] = false;
//			}
//		}
//
//		oldState2 = state2;
//	}
//	else
//	{
//		// Controller is not connected 
//
//	}
//}
//
//void PauseController(void)
//{
//	ZeroMemory(&state, sizeof(XINPUT_STATE));
//
//	dwResult = XInputGetState(
//		0,       // DWORD         dwUserIndex
//		&state); // XINPUT_STATE* pState
//
//	if (dwResult == ERROR_SUCCESS)
//	{
//		// ポーズ遷移
//		if (GamePadTrigger() & XINPUT_GAMEPAD_START)
//		{
//			SetPause();
//			PlaySound(SOUND_LABEL_SE_BUTTON);
//		}
//
//		if (GamePadTrigger() & XINPUT_GAMEPAD_DPAD_UP)
//		{
//			AddCursor(-1);
//			PlaySound(SOUND_LABEL_SE_CURSOR);
//		}
//
//		if (GamePadTrigger() & XINPUT_GAMEPAD_DPAD_DOWN)
//		{
//			AddCursor(1);
//			PlaySound(SOUND_LABEL_SE_CURSOR);
//		}
//
//		if (GamePadTrigger() & XINPUT_GAMEPAD_B)
//		{
//			PlaySound(SOUND_LABEL_SE_BUTTON);
//
//			switch (GetCursorPos())
//			{
//			case 0:
//				SetPause();
//				break;
//			case 1:
//				BackTitle();
//				break;
//			default:
//				break;
//			}
//		}
//	}
//	else
//	{
//		if (GetKeyboardTrigger(DIK_P))
//		{
//			SetPause();
//		}
//
//		if (GetKeyboardTrigger(DIK_W) || GetKeyboardTrigger(DIK_UP))
//		{
//			AddCursor(-1);
//		}
//
//		if (GetKeyboardTrigger(DIK_S) || GetKeyboardTrigger(DIK_DOWN))
//		{
//			AddCursor(1);
//		}
//
//		if (GetKeyboardTrigger(DIK_RETURN))
//		{
//			switch (GetCursorPos())
//			{
//			case 0:
//				SetPause();
//				break;
//			case 1:
//				BackTitle();
//				break;
//			default:
//				break;
//			}
//		}
//	}
//
//	oldState = state;
//}
//
//void ResultController(void)
//{
//	ZeroMemory(&state, sizeof(XINPUT_STATE));
//
//	dwResult = XInputGetState(
//		0,       // DWORD         dwUserIndex
//		&state); // XINPUT_STATE* pState
//
//	if (dwResult == ERROR_SUCCESS)
//	{
//		if (state.Gamepad.wButtons)
//		{
//			ChangeResult();
//			PlaySound(SOUND_LABEL_SE_BUTTON);
//		}
//	}
//	else
//	{
//		if (GetKeyboardTrigger(DIK_RETURN))
//		{
//			ChangeResult();
//			PlaySound(SOUND_LABEL_SE_BUTTON);
//		}
//	}
//
//	oldState = state;
//}
//
//void RankingController(void)
//{
//	ZeroMemory(&state, sizeof(XINPUT_STATE));
//
//	dwResult = XInputGetState(
//		0,       // DWORD         dwUserIndex
//		&state); // XINPUT_STATE* pState
//
//	if (dwResult == ERROR_SUCCESS)
//	{
//		if (state.Gamepad.wButtons)
//		{
//			ChangeRanking();
//			PlaySound(SOUND_LABEL_SE_BUTTON);
//		}
//	}
//	else
//	{
//		if (GetKeyboardTrigger(DIK_RETURN))
//		{
//			ChangeRanking();
//			PlaySound(SOUND_LABEL_SE_BUTTON);
//		}
//	}
//
//	oldState = state;
//}
//
//float GetJoyStickState(void)
//{
//	return JoyStickLY;
//}
//
//bool GetSeedMode(void)
//{
//	return bSeedMode;
//}
//
//bool GetSeedMode(int index)
//{
//	return bSeedModes[index];
//}
//
//bool GetControllerStatus(void)
//{
//	if (dwResult == ERROR_SUCCESS)
//	{
//		return true;
//	}
//
//	return false;
//}

// トリガー入力
WORD CController::Trigger(void)
{
	WORD triKey = (m_State.Gamepad.wButtons ^ m_OldState.Gamepad.wButtons) & m_State.Gamepad.wButtons;

	return triKey;
}
