//======================================================================
//	サウンド処理
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

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	if (CSound::m_XAudio2 == NULL)
	{
		hr = XAudio2Create(&CSound::m_XAudio2, 0);
		if (FAILED(hr))
		{
			MessageBox(NULL, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

			// COMライブラリの終了処理
			CoUninitialize();

			return;
		}
	}

	// マスターボイスの生成
	if (CSound::m_MasteringVoice == NULL)
	{
		hr = CSound::m_XAudio2->CreateMasteringVoice(&CSound::m_MasteringVoice);
		if (FAILED(hr))
		{
			MessageBox(NULL, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

			if (CSound::m_XAudio2)
			{
				// XAudio2オブジェクトの開放
				CSound::m_XAudio2->Release();
				CSound::m_XAudio2 = NULL;
			}

			// COMライブラリの終了処理
			CoUninitialize();

			return;
		}
	}

}
void CSound::Uninit(void)
{
	// マスターボイスの破棄
	if (m_MasteringVoice != NULL)
	{
		m_MasteringVoice->DestroyVoice();
		m_MasteringVoice = NULL;
	}

	if (m_XAudio2)
	{
		// XAudio2オブジェクトの開放
		m_XAudio2->Release();
		m_XAudio2 = NULL;
	}

	// COMライブラリの終了処理
	CoUninitialize();
}

HRESULT CSound::InitSound(SOUND_LABEL label)
{
	HRESULT hr;

	// サウンドデータの初期化
	HANDLE hFile;
	DWORD dwChunkSize = 0;
	DWORD dwChunkPosition = 0;
	DWORD dwFiletype;
	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER buffer;

	// バッファのクリア
	memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

	// サウンドデータファイルの生成
	hFile = CreateFile(g_aParam[label].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
		return HRESULT_FROM_WIN32(GetLastError());
	}
	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		MessageBox(NULL, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
		return HRESULT_FROM_WIN32(GetLastError());
	}

	// WAVEファイルのチェック
	hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
		return S_FALSE;
	}
	hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
		return S_FALSE;
	}
	if (dwFiletype != 'EVAW')
	{
		MessageBox(NULL, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
		return S_FALSE;
	}

	// フォーマットチェック
	hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
		return S_FALSE;
	}
	hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
		return S_FALSE;
	}

	// オーディオデータ読み込み
	hr = CheckChunk(hFile, 'atad', &m_SizeAudio, &dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
		return S_FALSE;
	}
	m_DataAudio = (BYTE*)malloc(m_SizeAudio);
	hr = ReadChunkData(hFile, m_DataAudio, m_SizeAudio, dwChunkPosition);
	if (FAILED(hr))
	{
		MessageBox(NULL, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
		return S_FALSE;
	}

	// ソースボイスの生成
	hr = m_XAudio2->CreateSourceVoice(&m_SourceVoice, &(wfx.Format));
	if (FAILED(hr))
	{
		MessageBox(NULL, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
		return S_FALSE;
	}

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_SizeAudio;
	buffer.pAudioData = m_DataAudio;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = g_aParam[label].nCntLoop;

	// オーディオバッファの登録
	m_SourceVoice->SubmitSourceBuffer(&buffer);

	m_Label = label;

	return S_OK;
}

void CSound::UninitSound(void)
{
	// 一時停止
	if (m_SourceVoice)
	{
		// 一時停止
		m_SourceVoice->Stop(0);

		// ソースボイスの破棄
		m_SourceVoice->DestroyVoice();
		m_SourceVoice = NULL;

		// オーディオデータの開放
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

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_SizeAudio;
	buffer.pAudioData = m_DataAudio;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = g_aParam[m_Label].nCntLoop;

	// 状態取得
	m_SourceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
	 // 一時停止
		m_SourceVoice->Stop(0);

		// オーディオバッファの削除
		m_SourceVoice->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	m_SourceVoice->SubmitSourceBuffer(&buffer);

	// 再生
	m_SourceVoice->Start(0);

	return S_OK;
}

HRESULT CSound::Play(float volume)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_SizeAudio;
	buffer.pAudioData = m_DataAudio;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = g_aParam[m_Label].nCntLoop;

	// 状態取得
	m_SourceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
	 // 一時停止
		m_SourceVoice->Stop(0);

		// オーディオバッファの削除
		m_SourceVoice->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	m_SourceVoice->SubmitSourceBuffer(&buffer);
	m_SourceVoice->SetVolume(volume);

	// 再生
	m_SourceVoice->Start(0);

	return S_OK;
}

void CSound::StopSound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	m_SourceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
	 // 一時停止
		m_SourceVoice->Stop(0);

		// オーディオバッファの削除
		m_SourceVoice->FlushSourceBuffers();
	}
}

void CSound::StopSoundAll(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (m_Sounds[nCntSound] != NULL)
		{
			// 一時停止
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
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
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
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
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
