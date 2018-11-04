//*****************************************************************************
//!	@file	shader.cpp
//!	@brief	
//!	@note	�V�F�[�_�[�֘A����
//!	@author	
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include "common.h"
#include "main.h"
#include "manager.h"
#include "camera.h"
#include"sh.h"

//==============================================================================
//!	@fn		ShaderCompile
//!	@brief	�V�F�[�_�[�R���p�C��
//!	@param	const char* filename,�@ �V�F�[�_�[�t�@�C����
//!	@param	const char* entry,�@�@�@�G���g���֐���
//!	@param	const char* version,�@�@�V�F�[�_�[�o�[�W�����w��
//!	@param	LPD3DXBUFFER* code,�@�@ �R���p�C���ς݃R�[�h�@
//!	@param	LPD3DXCONSTANTTABLE* ctbl�@�萔�e�[�u��
//!	@retval	bool true �����@false ���s
//==============================================================================
bool Shader::ShaderCompile(
	const char* filename,
	const char* entry,
	const char* version,						// �o�[�W����
	LPD3DXBUFFER* code,							// �R���p�C���ς݃R�[�h
	LPD3DXCONSTANTTABLE* ctbl)					// �萔�e�[�u��
{
	LPD3DXBUFFER err = nullptr;

	HRESULT hr;

	// ���_�V�F�[�_���R���p�C��
	hr = D3DXCompileShaderFromFile(
		filename,		// �t�@�C����
		nullptr,		// �v���v���Z�b�T��`�ւ̃|�C���^ 
		nullptr,		// ID3DXInclude�i#include�^�����߁j
		entry,			// ���_�V�F�[�_�[�֐��� 
		version,		// ���_�V�F�[�_�[�̃o�[�W���� 
		0,				// �R���p�C���I�v�V����
		code,			// �R���p�C���ς݂̃V�F�[�_�[�R�[�h�iOUT�j
		&err,			// �R���p�C���G���[��񂪊i�[�����iOUT�j
		ctbl);			// �V�F�[�_�[���̃R���X�^���g�e�[�u���iOUT�j

						// �G���[����
	if (FAILED(hr))
	{
		if (err) {
			// �R���p�C���G���[����
			MessageBox(NULL, (LPSTR)err->GetBufferPointer(), "D3DXCompileShaderFromFile", MB_OK);
		}
		else {
			// ���̑��̃G���[
			MessageBox(NULL, "�V�F�[�_�[�t�@�C�����ǂݍ��߂܂���", "D3DXCompileShaderFromFile", MB_OK);
		}
		return false;
	}
	return true;
}

//==============================================================================
//!	@fn		VertexShaderCompile
//!	@brief	���_�V�F�[�_�[�I�u�W�F�N�g����
//!	@param	LPDIRECT3DDEVICE9 lpdevidce    �f�o�C�X�I�u�W�F�N�g
//!	@param	const char* filename,		   �V�F�[�_�[�t�@�C����
//!	@param	const char* entry,			   �G���g���[�֐���
//!	@param	const char* version,		   �o�[�W����
//!	@param	LPD3DXCONSTANTTABLE* ctbl,	�@ �萔�e�[�u��
//!	@param	LPDIRECT3DVERTEXSHADER9* vsh�@ ���_�V�F�[�_�[�I�u�W�F�N�g
//!	@retval	bool true �����@false ���s
//==============================================================================
bool Shader::VertexShaderCompile(
	const char* filename,						// �V�F�[�_�[�t�@�C����
	const char* entry,							// �G���g���[�֐���
	const char* version)						// �o�[�W����
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return false;
	}

	bool sts;
	LPD3DXBUFFER code;

	// �V�F�[�_�[�R���p�C��
	sts = ShaderCompile(filename,
		entry,
		version,
		&code,
		&m_VSConstantTable);
	if (!sts) {
		return false;
	}

	// ���_�V�F�[�_�[�I�u�W�F�N�g���쐬����
	HRESULT hr = pDevice->CreateVertexShader((DWORD*)code->GetBufferPointer(), &m_VertexShader);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "CreateVertexShader error", "CreateVertexShader", MB_OK);
		return false;
	}

	code->Release();

	return true;
}

//==============================================================================
//!	@fn		PixelShaderCompile
//!	@brief	�s�N�Z���V�F�[�_�[�I�u�W�F�N�g����
//!	@param	LPDIRECT3DDEVICE9 lpdevidce    �f�o�C�X�I�u�W�F�N�g
//!	@param	const char* filename,		   �V�F�[�_�[�t�@�C����
//!	@param	const char* entry,			   �G���g���[�֐���
//!	@param	const char* version,		   �o�[�W����
//!	@param	LPD3DXCONSTANTTABLE* ctbl,	�@ �萔�e�[�u��
//!	@param	LPDIRECT3DVPIXELSHADER9* psh�@ �s�N�Z���V�F�[�_�[�I�u�W�F�N�g
//!	@retval	bool true �����@false ���s
//==============================================================================
bool Shader::PixelShaderCompile(
	const char* filename,						// �V�F�[�_�[�t�@�C����
	const char* entry,							// �G���g���[�֐���
	const char* version)						// �o�[�W����
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return false;
	}

	bool sts;
	LPD3DXBUFFER code;

	// �V�F�[�_�[�R���p�C��
	sts = ShaderCompile(filename,
		entry,
		version,
		&code,
		&m_PSConstantTable);
	if (!sts) {
		return false;
	}

	// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g���쐬����
	HRESULT hr = pDevice->CreatePixelShader((DWORD*)code->GetBufferPointer(), &m_PixelShader);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "CreatePixelShader error", "CreatePixelShader", MB_OK);
		return false;
	}

	code->Release();

	return true;
}

void Shader::ShaderSet(D3DXMATRIX world)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

	// ���̐ݒ���
	D3DXVECTOR4		light_dir(0.0f, -1.0f, 0.0f, 0.0f);			// ���̕���
	D3DXVECTOR4		diffuse(1.0f, 1.0f, 1.0f, 1.0f);			// ���s�����̐F
	D3DXVECTOR4		ambient(0.2f, 0.2f, 0.2f, 0.2f);			// ����
	D3DXVECTOR4		specular(0.2f, 0.2f, 0.2f, 0.2f);			// �X�y�L������

	// ���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[���Z�b�g
	pDevice->SetVertexShader(m_VertexShader);
	pDevice->SetPixelShader(m_PixelShader);

	// �萔���Z�b�g(���_�V�F�[�_�[)
	m_VSConstantTable->SetMatrix(pDevice, "m_world", &world);
	m_VSConstantTable->SetMatrix(pDevice, "m_view", &CManager::GetCamera()->GetView());
	m_VSConstantTable->SetMatrix(pDevice, "m_projection", &CManager::GetCamera()->GetProjection());

	m_VSConstantTable->SetVector(pDevice, "m_diffuse", &diffuse);
	m_VSConstantTable->SetVector(pDevice, "m_ambient", &ambient);
	m_VSConstantTable->SetVector(pDevice, "m_specular", &specular);
	m_VSConstantTable->SetVector(pDevice, "m_light_dir", &light_dir);
}

//******************************************************************************
//	End of file.
//******************************************************************************
