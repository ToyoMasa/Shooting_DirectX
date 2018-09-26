//======================================================================
//	ƒŠƒUƒ‹ƒg[result.h]
//
//======================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "mode.h"
#include "sound.h"

class CModeResult : public CMode
{
public:
	CModeResult(int numKill, int numSneak, bool clear) : CMode() 
	{
		m_NumKillEnemy = numKill;
		m_NumSneakKill = numSneak;
		m_bKilledTarget = clear;
	}
	~CModeResult() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	static CScene2D* m_ResultText;
	static CScene2D* m_RankText;
	static CScene2D* m_Text_PressSpace;
	static D3DCOLOR m_RankColor;
	static CSound* m_BGM;
	static CSound* m_SE;
	static int m_NumKillEnemy;
	static int m_NumSneakKill;
	static int m_Rank;
	static int m_Count;
	static bool m_bKilledTarget;
};

#endif // !_RESULT_H_
