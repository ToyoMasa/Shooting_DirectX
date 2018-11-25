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
	static void CallPause();
	static CField* GetField() { return Field; }

private:
	static CPlayer* player;
	static CLight *m_Light;
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
	static int m_Count;
	static CSound* BGM;
	static CSound* GameEnd_SE;
	static CSound* ZombieVoice;
	static CFog*	Fog;
	static CField* Field;
	static CEnemyManager* EnemyManager;
};

#endif // !_GAME_H_
