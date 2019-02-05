//======================================================================
//	DirectXテンプレ（main）
//
//======================================================================
#define _CRTDBG_MAP_ALLOC

#include <Windows.h>
#include <stdlib.h>
#include <crtdbg.h>
#include "common.h"
#include "main.h"
#include "texture.h"
#include "renderer.h"
#include "camera.h"
#include "light.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneModel.h"
#include "manager.h"
#include "input.h"

//======================================================================
//	定義
//======================================================================
#define CLASS_NAME "sample"						//クラスの名前
#define WINDOW_NAME "3Dゲーム"			//タイトルの名前

//======================================================================
//	グローバル変数
//======================================================================

//======================================================================
//	プロトタイプ宣言
//======================================================================
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	//ウィンドウプロシージャ

//======================================================================
//	main関数
//======================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// CRTメモリリーク検出用
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// CRTメモリリーク箇所検出
	//_CrtSetBreakAlloc((int)メモリ番号);

	WNDCLASSEX wcex = {
		sizeof(WNDCLASSEX),						//自分の構造体のサイズを記入する
		CS_VREDRAW | CS_HREDRAW,				//詳細はwebで
		WndProc,								//ウィンドウプロシージャ関数ポインタ（関数名だけでポインタになる）
		0,
		0,
		hInstance,								//WinMainの引数に宣言されている
		NULL,									//指定したいアイコンがあれば指定できる
		LoadCursor(NULL, IDC_ARROW),			//カーソルのアイコンを指定
		(HBRUSH)(COLOR_WINDOW + 1),				//Windowのクラウド領域の色を指定
		NULL,									//メニューネーム
		CLASS_NAME,								//クラスネーム（WindowのパーツをOSに伝えるための名前）
		NULL
	};

	RegisterClassEx(&wcex);						//Windowクラス登録

	RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	DWORD Style;

	int WinWidth = SCREEN_WIDTH + 16;
	int WinHeight = SCREEN_HEIGHT + 39;

	RECT dr;
	GetWindowRect(GetDesktopWindow(), &dr);

	int WinX;
	int WinY;

	if (((dr.bottom - WinHeight) > 0) && ((dr.right - WinWidth) > 0))
	{
		Style = WS_OVERLAPPEDWINDOW;
		RECT wr = { 0, 0, WinWidth, WinHeight };
		AdjustWindowRect(&wr, Style, false);
		WinWidth -= wr.left;
		WinHeight -= wr.top;
		WinY = (dr.bottom - WinHeight) / 2;
		WinX = (dr.right - WinWidth) / 2;
	}
	else
	{
		Style = WS_POPUPWINDOW;
		WinX = dr.top;
		WinX = dr.left;
		WinHeight = dr.bottom;
		WinWidth = dr.right;
	}

	HWND hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		WINDOW_NAME,
		Style,
		WinX,									//Windowの左上X座標、左上Y座標、幅（フレーム含む）
		WinY,
		(WinWidth),
		(WinHeight),
		NULL,									//親Windowへのハンドル
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hWnd, nShowCmd);					//表示

	UpdateWindow(hWnd);							//更新


	if (!CManager::Init(hInstance, hWnd, TRUE))
	{
		MessageBox(hWnd, "初期化に失敗しました。", "エラー", MB_OK);
	}

	MSG msg;									//宣言

	//======================================================================
	//	ゲームループ
	//======================================================================
	DWORD dwExecLastTime = 0;					//前フレームの時間
	DWORD dwCurrentTime = 0;					//現在の時間

	timeBeginPeriod(1);							//分解能を設定

	do {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//60fpsでループするようにする。
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				//ゲーム処理
				CManager::Update();

				CManager::Draw();

				dwExecLastTime = dwCurrentTime;
			}
		}
	} while (msg.message != WM_QUIT);

	// 終了処理
	CManager::Uninit();

	timeEndPeriod(1);							//分解能を戻す

	return (int)msg.wParam;						//決まり
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{
	case WM_DESTROY:							//×を押されるなどでウィンドウが閉じられたら
		PostQuitMessage(0);						//WM_QUITメッセージの送信（このメッセージが呼ばれたら強制終了する）
		break;

	case WM_KEYDOWN:							//キーボードからの入力取得
		switch (wParam)
		{
		case VK_ESCAPE:
			CInput::ChangeShowCursol();

			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_DEFBUTTON2);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);
			}

			CInput::ChangeShowCursol();

			break;

		default:
			break;
		}

		break;

	default:
		break;
	}

	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return true;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);	//これをしないと正常に終了しないかもしれない。逆に基本的な動きをさせたくない時には呼ばない
}
