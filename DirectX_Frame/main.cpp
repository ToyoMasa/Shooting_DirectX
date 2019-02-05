//======================================================================
//	DirectX�e���v���imain�j
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
//	��`
//======================================================================
#define CLASS_NAME "sample"						//�N���X�̖��O
#define WINDOW_NAME "3D�Q�[��"			//�^�C�g���̖��O

//======================================================================
//	�O���[�o���ϐ�
//======================================================================

//======================================================================
//	�v���g�^�C�v�錾
//======================================================================
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	//�E�B���h�E�v���V�[�W��

//======================================================================
//	main�֐�
//======================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// CRT���������[�N���o�p
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// CRT���������[�N�ӏ����o
	//_CrtSetBreakAlloc((int)�������ԍ�);

	WNDCLASSEX wcex = {
		sizeof(WNDCLASSEX),						//�����̍\���̂̃T�C�Y���L������
		CS_VREDRAW | CS_HREDRAW,				//�ڍׂ�web��
		WndProc,								//�E�B���h�E�v���V�[�W���֐��|�C���^�i�֐��������Ń|�C���^�ɂȂ�j
		0,
		0,
		hInstance,								//WinMain�̈����ɐ錾����Ă���
		NULL,									//�w�肵�����A�C�R��������Ύw��ł���
		LoadCursor(NULL, IDC_ARROW),			//�J�[�\���̃A�C�R�����w��
		(HBRUSH)(COLOR_WINDOW + 1),				//Window�̃N���E�h�̈�̐F���w��
		NULL,									//���j���[�l�[��
		CLASS_NAME,								//�N���X�l�[���iWindow�̃p�[�c��OS�ɓ`���邽�߂̖��O�j
		NULL
	};

	RegisterClassEx(&wcex);						//Window�N���X�o�^

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
		WinX,									//Window�̍���X���W�A����Y���W�A���i�t���[���܂ށj
		WinY,
		(WinWidth),
		(WinHeight),
		NULL,									//�eWindow�ւ̃n���h��
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hWnd, nShowCmd);					//�\��

	UpdateWindow(hWnd);							//�X�V


	if (!CManager::Init(hInstance, hWnd, TRUE))
	{
		MessageBox(hWnd, "�������Ɏ��s���܂����B", "�G���[", MB_OK);
	}

	MSG msg;									//�錾

	//======================================================================
	//	�Q�[�����[�v
	//======================================================================
	DWORD dwExecLastTime = 0;					//�O�t���[���̎���
	DWORD dwCurrentTime = 0;					//���݂̎���

	timeBeginPeriod(1);							//����\��ݒ�

	do {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//60fps�Ń��[�v����悤�ɂ���B
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				//�Q�[������
				CManager::Update();

				CManager::Draw();

				dwExecLastTime = dwCurrentTime;
			}
		}
	} while (msg.message != WM_QUIT);

	// �I������
	CManager::Uninit();

	timeEndPeriod(1);							//����\��߂�

	return (int)msg.wParam;						//���܂�
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{
	case WM_DESTROY:							//�~���������ȂǂŃE�B���h�E������ꂽ��
		PostQuitMessage(0);						//WM_QUIT���b�Z�[�W�̑��M�i���̃��b�Z�[�W���Ă΂ꂽ�狭���I������j
		break;

	case WM_KEYDOWN:							//�L�[�{�[�h����̓��͎擾
		switch (wParam)
		{
		case VK_ESCAPE:
			CInput::ChangeShowCursol();

			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO | MB_DEFBUTTON2);
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

	return DefWindowProc(hWnd, uMsg, wParam, lParam);	//��������Ȃ��Ɛ���ɏI�����Ȃ���������Ȃ��B�t�Ɋ�{�I�ȓ��������������Ȃ����ɂ͌Ă΂Ȃ�
}
