//*****************************************************************************
//!	@file	shader.h
//!	@brief	�V�F�[�_�[�֘A����
//!	@note	
//!	@author
//*****************************************************************************
#pragma once

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include	<d3dx9.h>

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
bool ShaderCompile(
	const char* filename,						// �V�F�[�_�[�t�@�C����
	const char* entry,							// �G���g���֐���
	const char* version,						// �o�[�W����
	LPD3DXBUFFER* code,							// �R���p�C���ς݃R�[�h
	LPD3DXCONSTANTTABLE* ctbl);					// �萔�e�[�u��

bool VertexShaderCompile(
	LPDIRECT3DDEVICE9 device,					// �f�o�C�X�I�u�W�F�N�g
	const char* filename,						// �V�F�[�_�[�t�@�C����
	const char* entry,							// �G���g���[�֐���
	const char* version,						// �o�[�W����
	LPD3DXCONSTANTTABLE* ctbl,					// �萔�e�[�u��
	LPDIRECT3DVERTEXSHADER9* vsh);				// ���_�V�F�[�_�[�I�u�W�F�N�g

bool PixelShaderCompile(
	LPDIRECT3DDEVICE9 device,					// �f�o�C�X�I�u�W�F�N�g
	const char* filename,						// �V�F�[�_�[�t�@�C����
	const char* entry,							// �G���g���[�֐���
	const char* version,						// �o�[�W����
	LPD3DXCONSTANTTABLE* ctbl,					// �萔�e�[�u��
	LPDIRECT3DPIXELSHADER9* vsh);				// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g

//******************************************************************************
//	End of file.
//******************************************************************************
