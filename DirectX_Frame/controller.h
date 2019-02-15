//======================================================================
//	ゲームコントロールヘッダ[controller.h]
//
//======================================================================
#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

class CController
{
public:
	void Init();
	void Update();
	bool GetIsAble() { return m_isAble; }
	bool ButtonPress(WORD button);
	bool ButtonRelease(WORD button);
	bool ButtonTrigger(WORD button);
	float GetStickRX() { return m_JoyStickRX; }
	float GetStickRY() { return m_JoyStickRY; }
	float GetStickLX() { return m_JoyStickLX; }
	float GetStickLY() { return m_JoyStickLY; }
	static void Uninit();

private:
	XINPUT_STATE	m_State;
	XINPUT_STATE	m_OldState;
	DWORD			m_Result;
	float			m_JoyStickLX;
	float			m_JoyStickLY;
	float			m_JoyStickRX;
	float			m_JoyStickRY;
	int				m_Id;
	bool			m_isAble;
	static int		m_NumController;
};

#endif // !_CONTROLLER_H_