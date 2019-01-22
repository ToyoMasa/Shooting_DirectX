//======================================================================
//	�V�F�[�_�[[shader.h]
//
//======================================================================
#ifndef _SHADER_H_
#define _SHADER_H_

#include <string>
#include <d3dx9.h>

enum SHADER_ID
{
	SH_ID_METAL,
	SH_ID_SKINMESH,
	SH_ID_SKINMESH_SPOTLIGHT,
	SH_ID_NORMALMAP,
	SH_ID_NORMALMAP_SPOTLIGHT,
	SH_ID_SPOTLIGHT,
	SH_ID_FIELD,
	SH_ID_MAX
};

// �V�F�[�_�[�t�@�C�����X�g
static const std::string SHADER_FILE[] =
{
	"data/shaders/metal.fx",
	"data/shaders/skinmesh.fx",
	"data/shaders/skinmeshSpotlight.fx",
	"data/shaders/normalmap.fx",
	"data/shaders/normalmapSpotlight.fx",
	"data/shaders/spotlight.fx",
	"data/shaders/shader.fx",
};

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
	virtual ~CShader()
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

	virtual void ShaderSet(const D3DXMATRIX& world) {}
	virtual void SetMaterial(const D3DMATERIAL9& mat) {}

	// �S�ẴV�F�[�_�[�����
	static void ReleaseAll();
protected:
	LPDIRECT3DVERTEXSHADER9 m_VertexShader;			// ���_�V�F�[�_�[
	LPD3DXCONSTANTTABLE		m_VSConstantTable;		// �萔�e�[�u��

	LPDIRECT3DPIXELSHADER9	m_PixelShader;			// �s�N�Z���V�F�[�_�[
	LPD3DXCONSTANTTABLE		m_PSConstantTable;		// �萔�e�[�u��
};

#endif // !_SHADER_H_
