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
	SOUND_LABEL_SE_TITLE,			// タイトルのSE
	SOUND_LABEL_SE_KNIFE,			// ナイフを振る音
	SOUND_LABEL_SE_ENEMY_ATTACK,	// 敵の攻撃音
	SOUND_LABEL_SE_STAB,			// ナイフで刺した音
	SOUND_LABEL_SE_GAMEOVER,		// ゲームオーバーのSE
	SOUND_LABEL_SE_GAMECLEAR,		// ゲームクリアのSE
	SOUND_LABEL_SE_FIND,			// 敵に見つかるSE
	SOUND_LABEL_SE_HIT,				// 敵の攻撃にヒットSE
	SOUND_LABEL_MAX
} SOUND_LABEL;

// 各音素材のパラメータ
static const SOUNDPARAM g_aParam[SOUND_LABEL_MAX] =
{
	{ "data/sounds/title_BGM.wav",		-1 },			// タイトルBGM
	{ "data/sounds/game_BGM.wav",		-1 },			// ゲームBGM
	{ "data/sounds/load_BGM.wav",		-1 },			// ロードBGM
	{ "data/sounds/result_BGM.wav",		-1 },			// リザルトBGM
	{ "data/sounds/title_SE.wav",		0  },			// タイトルSE
	{ "data/sounds/knife_attack.wav",	0 },			// ナイフを振るSE
	{ "data/sounds/enemy_attack.wav",	0 },			// 敵の攻撃SE
	{ "data/sounds/stab.wav",			0 },			// ナイフで刺したSE
	{ "data/sounds/gameover.wav",		0 },			// ゲームオーバーSE
	{ "data/sounds/gameclear.wav",		0 },			// ゲームクリアSE
	{ "data/sounds/find.wav",			0 },			// 敵に見つかるSE
	{ "data/sounds/punch_hit.wav",		0 },			// 敵の攻撃にヒットSE
};

static const int SOUND_MAX = 100;

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
