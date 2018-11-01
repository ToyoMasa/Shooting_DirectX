//*****************************************************************************
//!	@file	shader.cpp
//!	@brief	
//!	@note	�V�F�[�_�[�֘A����
//!	@author	
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include	"sh.h"

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
bool ShaderCompile(const char* filename,
	const char* entry,
	const char* version,
	LPD3DXBUFFER* code,
	LPD3DXCONSTANTTABLE* ctbl)
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
bool VertexShaderCompile(
	LPDIRECT3DDEVICE9 device,					// �f�o�C�X�I�u�W�F�N�g
	const char* filename,						// �V�F�[�_�[�t�@�C����
	const char* entry,							// �G���g���[�֐���
	const char* version,						// �o�[�W����
	LPD3DXCONSTANTTABLE* ctbl,					// �萔�e�[�u��
	LPDIRECT3DVERTEXSHADER9* vsh)				// ���_�V�F�[�_�[�I�u�W�F�N�g
{
	bool sts;
	LPD3DXBUFFER code;

	// �V�F�[�_�[�R���p�C��
	sts = ShaderCompile(filename,
		entry,
		version,
		&code,
		ctbl);
	if (!sts) {
		return false;
	}

	// ���_�V�F�[�_�[�I�u�W�F�N�g���쐬����
	HRESULT hr = device->CreateVertexShader((DWORD*)code->GetBufferPointer(), vsh);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "CreateVertexShader error", "CreateVertexShader", MB_OK);
		return false;
	}

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
bool PixelShaderCompile(
	LPDIRECT3DDEVICE9 device,					// �f�o�C�X�I�u�W�F�N�g
	const char* filename,						// �V�F�[�_�[�t�@�C����
	const char* entry,							// �G���g���[�֐���
	const char* version,						// �o�[�W����
	LPD3DXCONSTANTTABLE* ctbl,					// �萔�e�[�u��
	LPDIRECT3DPIXELSHADER9* psh)				// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g
{
	bool sts;
	LPD3DXBUFFER code;

	// �V�F�[�_�[�R���p�C��
	sts = ShaderCompile(filename,
		entry,
		version,
		&code,
		ctbl);
	if (!sts) {
		return false;
	}

	// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g���쐬����
	HRESULT hr = device->CreatePixelShader((DWORD*)code->GetBufferPointer(), psh);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "CreatePixelShader error", "CreateVertexShader", MB_OK);
		return false;
	}

	return true;
}

//******************************************************************************
//	End of file.
//******************************************************************************
