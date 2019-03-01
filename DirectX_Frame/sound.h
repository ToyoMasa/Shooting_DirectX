//=============================================================================
//
// �T�E���h���� [sound.h]
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include <XAudio2.h>

//*****************************************************************************
// �p�����[�^�\���̒�`
//*****************************************************************************
typedef struct
{
	char *pFilename;	// �t�@�C����
	int nCntLoop;		// ���[�v�J�E���g
} SOUNDPARAM;

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************

typedef enum
{
	SOUND_LABEL_BGM_TITLE,			// �^�C�g��
	SOUND_LABEL_BGM_GAME,			// �Q�[��
	SOUND_LABEL_BGM_LOAD,			// ���[�h
	SOUND_LABEL_BGM_RESULT,			// ���U���g
	SOUND_LABEL_BGM_ZOMBIE_BREATH,	// �]���r�̑�����
	SOUND_LABEL_SE_TITLE,			// �^�C�g����SE
	SOUND_LABEL_SE_GAMEOVER,		// �Q�[���I�[�o�[��SE
	SOUND_LABEL_SE_GAMECLEAR,		// �Q�[���N���A��SE
	SOUND_LABEL_SE_ZOMBIE_DEATH,	// �]���r�̎��S���̉�
	SOUND_LABEL_SE_ZOMBIEVOICE1,	// �]���r�̐��P
	SOUND_LABEL_SE_ZOMBIEVOICE2,	// �]���r�̐��Q
	SOUND_LABEL_SE_ZOMBIEVOICE3,	// �]���r�̐��R
	SOUND_LABEL_SE_RIFLE_SHOT1,		// ���C�t���e���P
	SOUND_LABEL_SE_RIFLE_SHOT2,		// ���C�t���e���Q
	SOUND_LABEL_SE_RIFLE_SHOT3,		// ���C�t���e���R
	SOUND_LABEL_SE_RIFLE_SHOT4,		// ���C�t���e���S
	SOUND_LABEL_SE_RIFLE_SHOT5,		// ���C�t���e���T
	SOUND_LABEL_SE_RIFLE_SHOT6,		// ���C�t���e���U
	SOUND_LABEL_SE_SHOTGUN_SHOT,	// �V���b�g�K���e��
	SOUND_LABEL_SE_RELOAD,			// �����[�h��
	SOUND_LABEL_MAX
} SOUND_LABEL;

// �e���f�ނ̃p�����[�^
static const SOUNDPARAM g_aParam[SOUND_LABEL_MAX] =
{
	{ "data/sounds/title_BGM.wav",			-1 },			// �^�C�g��BGM
	{ "data/sounds/game_BGM.wav",			-1 },			// �Q�[��BGM
	{ "data/sounds/load_BGM.wav",			-1 },			// ���[�hBGM
	{ "data/sounds/result_BGM.wav",			-1 },			// ���U���gBGM
	{ "data/sounds/zombie_breath.wav",		-1 },			// �]���r�̑�����
	{ "data/sounds/title_SE.wav",			0 },			// �^�C�g��SE
	{ "data/sounds/gameover.wav",			0 },			// �Q�[���I�[�o�[SE
	{ "data/sounds/gameclear.wav",			0 },			// �Q�[���N���ASE
	{ "data/sounds/zombie_death.wav",		0 },			// �]���r�̎��S���̉�
	{ "data/sounds/zombie_voice1.wav",		0 },			// �]���r�̐��P
	{ "data/sounds/zombie_voice2.wav",		0 },			// �]���r�̐��Q
	{ "data/sounds/zombie_voice3.wav",		0 },			// �]���r�̐��R
	{ "data/sounds/rifle_shot1.wav",		0 },			// ���C�t���e���P
	{ "data/sounds/rifle_shot2.wav",		0 },			// ���C�t���e���Q
	{ "data/sounds/rifle_shot3.wav",		0 },			// ���C�t���e���R
	{ "data/sounds/rifle_shot4.wav",		0 },			// ���C�t���e���S
	{ "data/sounds/rifle_shot5.wav",		0 },			// ���C�t���e���T
	{ "data/sounds/rifle_shot6.wav",		0 },			// ���C�t���e���U
	{ "data/sounds/shotgun_shot.wav",		0 },			// �V���b�g�K���e��
	{ "data/sounds/reload.wav",				0 },			// �����[�hSE
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
	IXAudio2SourceVoice *m_SourceVoice;	// �\�[�X�{�C�X
	BYTE *m_DataAudio;					// �I�[�f�B�I�f�[�^
	DWORD m_SizeAudio;					// �I�[�f�B�I�f�[�^�T�C�Y
	SOUND_LABEL m_Label;				// ���x��

	static IXAudio2 *m_XAudio2;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	static IXAudio2MasteringVoice *m_MasteringVoice;		// �}�X�^�[�{�C�X
	static CSound* m_Sounds[SOUND_MAX];
};

#endif
