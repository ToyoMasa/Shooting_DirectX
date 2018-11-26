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
class CFog;
class CEnemyManager;

enum GAME_RESULT
{
	GAME_CLEAR,
	GAME_OVER
};

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
	static CCamera* GetCamera();
	static CField* GetField() { return Field; }
	static void IncrementKillCount() { KillCount++; }
	static void GameEnd(GAME_RESULT result);
	static void CallPause();

private:
	static CPlayer* player;
	static CLight *Light;
	static CScene2D* Load;
	static CScene2D* LoadFrame;
	static CScene2D* LoadGage;
	static CScene2D* GameClear;
	static CScene2D* GameOver;
	static CScene2D* Mission;
	static CScene2D* Wanted;
	static CScene2D* Tutorial;
	static CScene2D* Tutorial2;
	static CScene2D* PauseWord;
	static CScene2D* Black;
	static CScene2D* HowToUse;
	static GAME_RESULT Result;
	static CSound* BGM;
	static CSound* GameEnd_SE;
	static CSound* ZombieVoice;
	static CFog*	Fog;
	static CField* Field;
	static CEnemyManager* EnemyManager;
	static bool Pause;
	static bool GameFinish;
	static int FrameCount;
	static int KillCount;
};

#endif // !_GAME_H_
