//=============================================================================
//
// サウンド処理 [sound.h]
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include <XAudio2.h>

//*****************************************************************************
// パラメータ構造体定義
//*****************************************************************************
typedef struct
{
	char *pFilename;	// ファイル名
	int nCntLoop;		// ループカウント
} SOUNDPARAM;

//*****************************************************************************
// サウンドファイル
//*****************************************************************************

typedef enum
{
	SOUND_LABEL_BGM_TITLE,			// タイトル
	SOUND_LABEL_BGM_GAME,			// ゲーム
	SOUND_LABEL_BGM_LOAD,			// ロード
	SOUND_LABEL_BGM_RESULT,			// リザルト
	SOUND_LABEL_BGM_ZOMBIE_BREATH,	// ゾンビの息遣い
	SOUND_LABEL_SE_TITLE,			// タイトルのSE
	SOUND_LABEL_SE_GAMEOVER,		// ゲームオーバーのSE
	SOUND_LABEL_SE_GAMECLEAR,		// ゲームクリアのSE
	SOUND_LABEL_SE_ZOMBIE_DEATH,	// ゾンビの死亡時の音
	SOUND_LABEL_SE_ZOMBIEVOICE1,	// ゾンビの声１
	SOUND_LABEL_SE_ZOMBIEVOICE2,	// ゾンビの声２
	SOUND_LABEL_SE_ZOMBIEVOICE3,	// ゾンビの声３
	SOUND_LABEL_SE_RIFLE_SHOT1,		// ライフル銃声１
	SOUND_LABEL_SE_RIFLE_SHOT2,		// ライフル銃声２
	SOUND_LABEL_SE_RIFLE_SHOT3,		// ライフル銃声３
	SOUND_LABEL_SE_RIFLE_SHOT4,		// ライフル銃声４
	SOUND_LABEL_SE_RIFLE_SHOT5,		// ライフル銃声５
	SOUND_LABEL_SE_RIFLE_SHOT6,		// ライフル銃声６
	SOUND_LABEL_SE_SHOTGUN_SHOT,	// ショットガン銃声
	SOUND_LABEL_SE_RELOAD,			// リロード音
	SOUND_LABEL_MAX
} SOUND_LABEL;

// 各音素材のパラメータ
static const SOUNDPARAM g_aParam[SOUND_LABEL_MAX] =
{
	{ "data/sounds/title_BGM.wav",			-1 },			// タイトルBGM
	{ "data/sounds/game_BGM.wav",			-1 },			// ゲームBGM
	{ "data/sounds/load_BGM.wav",			-1 },			// ロードBGM
	{ "data/sounds/result_BGM.wav",			-1 },			// リザルトBGM
	{ "data/sounds/zombie_breath.wav",		-1 },			// ゾンビの息遣い
	{ "data/sounds/title_SE.wav",			0 },			// タイトルSE
	{ "data/sounds/gameover.wav",			0 },			// ゲームオーバーSE
	{ "data/sounds/gameclear.wav",			0 },			// ゲームクリアSE
	{ "data/sounds/zombie_death.wav",		0 },			// ゾンビの死亡時の音
	{ "data/sounds/zombie_voice1.wav",		0 },			// ゾンビの声１
	{ "data/sounds/zombie_voice2.wav",		0 },			// ゾンビの声２
	{ "data/sounds/zombie_voice3.wav",		0 },			// ゾンビの声３
	{ "data/sounds/rifle_shot1.wav",		0 },			// ライフル銃声１
	{ "data/sounds/rifle_shot2.wav",		0 },			// ライフル銃声２
	{ "data/sounds/rifle_shot3.wav",		0 },			// ライフル銃声３
	{ "data/sounds/rifle_shot4.wav",		0 },			// ライフル銃声４
	{ "data/sounds/rifle_shot5.wav",		0 },			// ライフル銃声５
	{ "data/sounds/rifle_shot6.wav",		0 },			// ライフル銃声６
	{ "data/sounds/shotgun_shot.wav",		0 },			// ショットガン銃声
	{ "data/sounds/reload.wav",				0 },			// リロードSE
};

static const int SOUND_MAX = 100;
static const float BGM_VOLUME = 1.0f;
static const float SHOT_SE_VOLUME = 0.5f;
static const float SE_VOLUME = 1.0f; 
static const float ZOMBIE_VOICE_VOLUME = 0.5f;


class CSound
{
public:
	CSound()
	{
		for (int i = 0; i < SOUND_MAX; i++)
		{
			if (m_Sounds[i] == NULL)
			{
				m_Sounds[i] = this;
				break;
			}
		}
	}
	HRESULT InitSound(SOUND_LABEL label);
	void UninitSound(void);
	void Release(void);
	HRESULT Play();
	HRESULT Play(float volume);
	void StopSound(SOUND_LABEL label);
	static void StopSoundAll(void);

	static void Init(void);
	static void Uninit(void);
	static void ReleaseAll(void);
	static CSound* Create(SOUND_LABEL label);

	static HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	static HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

private:
	IXAudio2SourceVoice *m_SourceVoice;	// ソースボイス
	BYTE *m_DataAudio;					// オーディオデータ
	DWORD m_SizeAudio;					// オーディオデータサイズ
	SOUND_LABEL m_Label;				// ラベル

	static IXAudio2 *m_XAudio2;								// XAudio2オブジェクトへのインターフェイス
	static IXAudio2MasteringVoice *m_MasteringVoice;		// マスターボイス
	static CSound* m_Sounds[SOUND_MAX];
};

#endif
