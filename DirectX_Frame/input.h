//=============================================================================
// ���͏��� [input.h]
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib, "DXGUID.LIB")

#include<windows.h>
#define DIRECTINPUT_VERSION (0x0800)		//dinput.h���O�ɏ���
#include <dinput.h>

//*********************************************************
// ���̓N���X
//*********************************************************
class CInput
{
public:
	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInst, HWND hWnd);
	virtual void Uninit(void);
	virtual HRESULT Update(void) = 0;		// ---> �������z�֐���

protected:
	LPDIRECTINPUTDEVICE8		m_pDIDevice;		// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^
	static LPDIRECTINPUT8		m_pDInput;			// IDirectInput8�C���^�[�t�F�[�X�ւ̃|�C���^
};

//*********************************************************
// �L�[�{�[�h���̓N���X
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
	BYTE			m_aKeyState[256];			// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
	BYTE			m_aKeyStateTrigger[256];	// �g���K�[���[�N
	BYTE			m_aKeyStateRelease[256];	// �����[�X���[�N
	BYTE			m_aKeyStateRepeat[256];		// ���s�[�g���[�N
	int				m_aKeyStateRepeatCnt[256];	// ���s�[�g�J�E���^
};

//*********************************************************
// �}�E�X���̓N���X
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
	DIMOUSESTATE2	m_mouseState;				// �}�E�X�̏�Ԃ��󂯎�郏�[�N
	DIMOUSESTATE2	m_mouseStateTrigger;		// �g���K�[���[�N
	DIMOUSESTATE2	m_mouseStateRelease;		// �g���K�[�����[�X���[�N
	POINT			m_posMouseWorld;			// �}�E�X�̃X�N���[�����W
};

#endif
