//======================================================================
//	ƒŠƒUƒ‹ƒg[result.h]
//
//======================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "mode.h"
#include "sound.h"

class CNumber;

class CModeResult : public CMode
{
public:
	CModeResult(int numKill) : CMode() 
	{
		NumKillEnemy = numKill;
	}
	~CModeResult() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	static CScene2D* ResultText;
	static CScene2D* KillNumText;
	static CScene2D* TextPressSpace;
	static CScene2D* TextPressStart;
	static CSound* BGM;
	static CSound* SE;
	static CNumber* ScreenKillCount;
	static CCamera* Camera;
	static CFog* Fog;
	static CField* Field;
	static CLight* Light;
	static int NumKillEnemy;
	static int CountFrame;
};

#endif // !_RESULT_H_
