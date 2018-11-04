//======================================================================
//	�e�N�X�`���Ǘ�
//
//======================================================================
#include "common.h"
#include "main.h"
#include "texture.h"

//======================================================================
//	�ÓI�����o�ϐ��̏�����
//======================================================================

CTexture::~CTexture()
{
}

void CTexture::Init()
{
	for (int i = 0; i < TEX_ID_MAX; i++)
	{
		MAP_TEXTURES[i] = TEXTURE_SOURCE[i];
		MAP_LOADING_TEXTURES[MAP_TEXTURES[i]] = false;
		MAP_TEXTURE_USE_NUM[i] = 0;
	}
}

void CTexture::Load(int id)
{
	if (id >= 0)
	{
		if (!MAP_LOADING_TEXTURES[MAP_TEXTURES[id]])
		{
			TEXTURES[id].LoadTexture(id);
			MAP_LOADING_TEXTURES[MAP_TEXTURES[id]] = true;
		}

		// ���̃e�N�X�`���̎g�p�҂��C���N�������g
		MAP_TEXTURE_USE_NUM[id]++;
	}
}

void CTexture::Release(int id)
{
	// ���̃e�N�X�`���̎g�p�҂��f�N�������g
	MAP_TEXTURE_USE_NUM[id]--;

	if (MAP_TEXTURE_USE_NUM[id] <= 0)
	{
		TEXTURES[id].ReleaseTexture();
		MAP_LOADING_TEXTURES[MAP_TEXTURES[id]] = false;
	}
}

void CTexture::LoadTexture(int id)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��ݎ��Ƀf�o�C�X�̎擾�Ɏ��s���܂���", "�G���[", MB_OK);
		return;
	}

	if (m_Texture == NULL)
	{
		HRESULT hr;

		hr = D3DXCreateTextureFromFile(pDevice, MAP_TEXTURES[id].c_str(), &m_Texture);

		if (FAILED(hr))
		{
			MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);
			return;
		}
	}
}

void CTexture::ReleaseTexture()
{
	if (m_Texture != NULL)
	{
		m_Texture->Release();
		m_Texture = NULL;
	}
}

void CTexture::LoadAll()
{
	for (int i = 0; i < TEX_ID_MAX; i++)
	{
		if (!MAP_LOADING_TEXTURES[MAP_TEXTURES[i]])
		{
			TEXTURES[i].LoadTexture(i);
			MAP_LOADING_TEXTURES[MAP_TEXTURES[i]] = true;
		}
	}
}

void CTexture::ReleaseAll()
{
	for (int i = 0; i < TEX_ID_MAX; i++)
	{
		TEXTURES[i].ReleaseTexture();
		MAP_TEXTURE_USE_NUM[i] = 0;
		MAP_LOADING_TEXTURES[MAP_TEXTURES[i]] = true;
	}
}

LPDIRECT3DTEXTURE9 CTexture::GetTexture(int id)
{ 
	return TEXTURES[id].GetTexture(); 
}
