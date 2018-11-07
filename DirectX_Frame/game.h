//======================================================================
//	ÉQÅ[ÉÄâÊñ [game.h]
//
//======================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "mode.h"
#include "sound.h"

static const int ENEMY_NUM = 100;

class CPlayer;
class CEnemy;
class CBillBoard;
class CScene2D;
class CLight;
class CSound;

class CModeGame : public CMode
{
public:
	CModeGame() : CMode() {}
	~CModeGame() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();

	static CPlayer* GetPlayer() { return player; }
	static void PlayerDied();
	static void TargetKilled();
	static CCamera* GetCamera();
	static void MakeMap();
	static void IncrementNumKill() { m_NumKill++; }
	static void IncrementNumSneak() { m_NumSneak++; }
	static void CallPause();

private:
	static CBillBoard* tree1;
	static CBillBoard* tree2;
	static CPlayer* player;
	static CEnemy* enemy[ENEMY_NUM];
	static CEnemy* Target;
	static CLight *m_Light;
	static bool m_PlayerDied;
	static bool m_TargetDied;
	static bool m_PlayBGM;
	static bool m_Pause;
	static CScene2D* Load;
	static CScene2D* LoadFrame;
	static CScene2D* LoadGage;
	static CScene2D* GameClear;
	static CScene2D* GameOver;
	static CScene2D* Mission;
	static CScene2D* Wanted;
	static CScene2D* Tutorial;
	static CScene2D* Tutorial2;
	static CScene2D* Pause;
	static CScene2D* Black;
	static CScene2D* HowToUse;
	static int m_NumKill;
	static int m_NumSneak;
	static int m_CountResult;
	static int m_Count;
	static CSound* BGM;
	static CSound* GameEnd_SE;
};

#endif // !_GAME_H_
