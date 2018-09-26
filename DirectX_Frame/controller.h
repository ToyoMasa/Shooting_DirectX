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
	WORD Trigger();
	static void Uninit();

private:
	XINPUT_STATE m_State;
	XINPUT_STATE m_OldState;
	DWORD		 m_Result;
	float		 m_JoyStickLX;
	float		 m_JoyStickLY;
	float		 m_JoyStickRX;
	float		 m_JoyStickRY;
	int			 m_Id;
	static int   m_NumController;
};

#endif // !_CONTROLLER_H_