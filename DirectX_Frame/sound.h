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
	SOUND_LABEL_SE_TITLE,			// �^�C�g����SE
	SOUND_LABEL_SE_KNIFE,			// �i�C�t��U�鉹
	SOUND_LABEL_SE_ENEMY_ATTACK,	// �G�̍U����
	SOUND_LABEL_SE_STAB,			// �i�C�t�Ŏh������
	SOUND_LABEL_SE_GAMEOVER,		// �Q�[���I�[�o�[��SE
	SOUND_LABEL_SE_GAMECLEAR,		// �Q�[���N���A��SE
	SOUND_LABEL_SE_FIND,			// �G�Ɍ�����SE
	SOUND_LABEL_SE_HIT,				// �G�̍U���Ƀq�b�gSE
	SOUND_LABEL_MAX
} SOUND_LABEL;

// �e���f�ނ̃p�����[�^
static const SOUNDPARAM g_aParam[SOUND_LABEL_MAX] =
{
	{ "data/sounds/title_BGM.wav",		-1 },			// �^�C�g��BGM
	{ "data/sounds/game_BGM.wav",		-1 },			// �Q�[��BGM
	{ "data/sounds/load_BGM.wav",		-1 },			// ���[�hBGM
	{ "data/sounds/result_BGM.wav",		-1 },			// ���U���gBGM
	{ "data/sounds/title_SE.wav",		0  },			// �^�C�g��SE
	{ "data/sounds/knife_attack.wav",	0 },			// �i�C�t��U��SE
	{ "data/sounds/enemy_attack.wav",	0 },			// �G�̍U��SE
	{ "data/sounds/stab.wav",			0 },			// �i�C�t�Ŏh����SE
	{ "data/sounds/gameover.wav",		0 },			// �Q�[���I�[�o�[SE
	{ "data/sounds/gameclear.wav",		0 },			// �Q�[���N���ASE
	{ "data/sounds/find.wav",			0 },			// �G�Ɍ�����SE
	{ "data/sounds/punch_hit.wav",		0 },			// �G�̍U���Ƀq�b�gSE
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
	IXAudio2SourceVoice *m_SourceVoice;	// �\�[�X�{�C�X
	BYTE *m_DataAudio;					// �I�[�f�B�I�f�[�^
	DWORD m_SizeAudio;					// �I�[�f�B�I�f�[�^�T�C�Y
	SOUND_LABEL m_Label;				// ���x��

	static IXAudio2 *m_XAudio2;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	static IXAudio2MasteringVoice *m_MasteringVoice;		// �}�X�^�[�{�C�X
	static CSound* m_Sounds[SOUND_MAX];
};

#endif
