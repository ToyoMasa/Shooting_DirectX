//======================================================================
//	�T�E���h����
//
//======================================================================
#include "common.h"
#include "main.h"
#include "sound.h"

IXAudio2 *CSound::m_XAudio2 = NULL;
IXAudio2MasteringVoice *CSound::m_MasteringVoice = NULL;
CSound *CSound::m_Sounds[SOUND_MAX] = { NULL };

void CSound::Init()
{
	HRESULT hr;

	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2�I�u�W�F�N�g�̍쐬
	if (CSound::m_XAudio2 == NULL)
	{
		hr = XAudio2Create(&CSound::m_XAudio2, 0);
		if (FAILED(hr))
		{
			MessageBox(NULL, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// COM���C�u�����̏I������
			CoUninitialize();

			return;
		}
	}

	// �}�X�^�[�{�C�X�̐���
	if (CSound::m_MasteringVoice == NULL)
	{
		hr = CSound::m_XAudio2->CreateMasteringVoice(&CSound::m_MasteringVoice);
		if (FAILED(hr))
		{
			MessageBox(NULL, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

			if (CSound::m_XAudio2)
			{
				// XAudio2�I�u�W�F�N�g�̊J��
				CSound::m_XAudio2->Release();
				CSound::m_XAudio2 = NULL;
			}

			// COM���C�u�����̏I������
			CoUninitialize();

			return;
		}
	}

}
void CSound::Uninit(void)
{
	// �}�X�^�[�{�C�X�̔j��
	if (m_MasteringVoice != NULL)
	{
		m_MasteringVoice->DestroyVoice();
		m_MasteringVoice = NULL;
	}

	if (m_XAudio2)
	{
		// XAudio2�I�u�W�F�N�g�̊J��
		m_XAudio2->Release();
		m_XAudio2 = NULL;
	}

	// COM���C�u�����̏I������
	CoUninitialize();
}

HRESULT CSound::InitSound(SOUND_LABEL label)
{
	HRESULT hr;

	// �T�E���h�f�[�^�̏�����
	HANDLE hFile;
	DWORD dwChunkSize = 0;
	DWORD dwChunkPosition = 0;
	DWORD dwFiletype;
	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER buffer;

	// �o�b�t�@�̃N���A
	memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

	// �T�E���h�f�[�^�t�@�C���̐���
	hFile = CreateFile(g_aParam[label].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
		return HRESULT_FROM_WIN32(GetLastError());
	}
	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^��擪�Ɉړ�
		MessageBox(NULL, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
		return HRESULT_FROM_WIN32(GetLastError());
	}

	// WAVE�t�@�C���̃`�F�b�N
	hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
		return S_FALSE;
	}
	hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
		return S_FALSE;
	}
	if (dwFiletype != 'EVAW')
	{
		MessageBox(NULL, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);
		return S_FALSE;
	}

	// �t�H�[�}�b�g�`�F�b�N
	hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
		return S_FALSE;
	}
	hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
		return S_FALSE;
	}

	// �I�[�f�B�I�f�[�^�ǂݍ���
	hr = CheckChunk(hFile, 'atad', &m_SizeAudio, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
		return S_FALSE;
	}
	m_DataAudio = (BYTE*)malloc(m_SizeAudio);
	hr = ReadChunkData(hFile, m_DataAudio, m_SizeAudio, dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
		return S_FALSE;
	}

	// �\�[�X�{�C�X�̐���
	hr = m_XAudio2->CreateSourceVoice(&m_SourceVoice, &(wfx.Format));
	if (FAILED(hr))
	{
		MessageBox(NULL, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
		return S_FALSE;
	}

	// �o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_SizeAudio;
	buffer.pAudioData = m_DataAudio;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = g_aParam[label].nCntLoop;

	// �I�[�f�B�I�o�b�t�@�̓o�^
	m_SourceVoice->SubmitSourceBuffer(&buffer);

	m_Label = label;

	return S_OK;
}

void CSound::UninitSound(void)
{
	// �ꎞ��~
	if (m_SourceVoice)
	{
		// �ꎞ��~
		m_SourceVoice->Stop(0);

		// �\�[�X�{�C�X�̔j��
		m_SourceVoice->DestroyVoice();
		m_SourceVoice = NULL;

		// �I�[�f�B�I�f�[�^�̊J��
		free(m_DataAudio);
		m_DataAudio = NULL;
	}
}

void CSound::Release(void)
{
	for (int i = 0; i < SOUND_MAX; i++)
	{
		if (m_Sounds[i] == this)
		{
			m_Sounds[i]->UninitSound();

			delete m_Sounds[i];
			m_Sounds[i] = NULL;
			
			break;
		}
	}
}

void CSound::ReleaseAll(void)
{
	for (int i = 0; i < SOUND_MAX; i++)
	{
		if (m_Sounds[i] != NULL)
		{
			m_Sounds[i]->Release();
		}
	}
}

HRESULT CSound::Play()
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// �o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_SizeAudio;
	buffer.pAudioData = m_DataAudio;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = g_aParam[m_Label].nCntLoop;

	// ��Ԏ擾
	m_SourceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �Đ���
	 // �ꎞ��~
		m_SourceVoice->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		m_SourceVoice->FlushSourceBuffers();
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^
	m_SourceVoice->SubmitSourceBuffer(&buffer);

	// �Đ�
	m_SourceVoice->Start(0);

	return S_OK;
}

HRESULT CSound::Play(float volume)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// �o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_SizeAudio;
	buffer.pAudioData = m_DataAudio;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = g_aParam[m_Label].nCntLoop;

	// ��Ԏ擾
	m_SourceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �Đ���
	 // �ꎞ��~
		m_SourceVoice->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		m_SourceVoice->FlushSourceBuffers();
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^
	m_SourceVoice->SubmitSourceBuffer(&buffer);
	m_SourceVoice->SetVolume(volume);

	// �Đ�
	m_SourceVoice->Start(0);

	return S_OK;
}

void CSound::StopSound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	m_SourceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �Đ���
	 // �ꎞ��~
		m_SourceVoice->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		m_SourceVoice->FlushSourceBuffers();
	}
}

void CSound::StopSoundAll(void)
{
	// �ꎞ��~
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (m_Sounds[nCntSound] != NULL)
		{
			// �ꎞ��~
			m_Sounds[nCntSound]->m_SourceVoice->Stop(0);
		}
	}
}

HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}

CSound* CSound::Create(SOUND_LABEL label)
{
	CSound* sound = new CSound();
	sound->InitSound(label);

	return sound;
}
