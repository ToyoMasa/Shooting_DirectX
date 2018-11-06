//*****************************************************************************
//!	@file	shader.h
//!	@brief	�V�F�[�_�[�֘A����
//!	@note	
//!	@author
//*****************************************************************************
#pragma once

#include <string>

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include	<d3dx9.h>

// �V�F�[�_�[�t�@�C�����X�g
static const std::string SHADER_FILE[] =
{
	"data/shaders/shader.fx",
	"data/shaders/phong.fx",
	"data/shaders/basic.fx",
};

static const DWORD FVF_VERTEX_SHADER = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
class CShader
{
public:
	CShader()
	{
		m_VertexShader = nullptr;
		m_VSConstantTable = nullptr;

		m_PixelShader = nullptr;
		m_PSConstantTable = nullptr;
	}
	~CShader()
	{
		if (m_VertexShader)
		{
			m_VertexShader->Release();
		}
		if (m_VSConstantTable)
		{
			m_VSConstantTable->Release();
		}
		if (m_PixelShader)
		{
			m_PixelShader->Release();
		}
		if (m_PSConstantTable)
		{
			m_PSConstantTable->Release();
		}
	}
	bool ShaderCompile(
		const char* filename,						// �V�F�[�_�[�t�@�C����
		const char* entry,							// �G���g���֐���
		const char* version,						// �o�[�W����
		LPD3DXBUFFER* code,							// �R���p�C���ς݃R�[�h
		LPD3DXCONSTANTTABLE* ctbl);					// �萔�e�[�u��

	bool VertexShaderCompile(
		const char* filename,						// �V�F�[�_�[�t�@�C����
		const char* entry,							// �G���g���[�֐���
		const char* version);						// �o�[�W����

	bool PixelShaderCompile(
		const char* filename,						// �V�F�[�_�[�t�@�C����
		const char* entry,							// �G���g���[�֐���
		const char* version);						// �o�[�W����

	LPD3DXCONSTANTTABLE& GetVSTable() { return m_VSConstantTable; }
	LPD3DXCONSTANTTABLE& GetPSTable() { return m_PSConstantTable; }
	LPDIRECT3DVERTEXSHADER9& GetVS() { return m_VertexShader; }
	LPDIRECT3DPIXELSHADER9& GetPS() { return m_PixelShader; }

	void ShaderSet(D3DXMATRIX world);
private:
	LPDIRECT3DVERTEXSHADER9 m_VertexShader;			// ���_�V�F�[�_�[
	LPD3DXCONSTANTTABLE		m_VSConstantTable;		// �萔�e�[�u��

	LPDIRECT3DPIXELSHADER9	m_PixelShader;			// �s�N�Z���V�F�[�_�[
	LPD3DXCONSTANTTABLE		m_PSConstantTable;		// �萔�e�[�u��
};
//******************************************************************************
//	End of file.
//******************************************************************************
