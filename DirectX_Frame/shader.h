//======================================================================
//	シェーダー[shader.h]
//
//======================================================================
#ifndef _SHADER_H_
#define _SHADER_H_

#include <string>
#include <d3dx9.h>

enum SHADER_ID
{
	SH_ID_DEFAULT,
	SH_ID_METAL
};

// シェーダーファイルリスト
static const std::string SHADER_FILE[] =
{
	"data/shaders/basic.fx",
	"data/shaders/metal.fx",
};

static const DWORD FVF_VERTEX_SHADER = (D3DFVF_XYZ | D3DFVF_DIFFUSE);

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
		const char* filename,						// シェーダーファイル名
		const char* entry,							// エントリ関数名
		const char* version,						// バージョン
		LPD3DXBUFFER* code,							// コンパイル済みコード
		LPD3DXCONSTANTTABLE* ctbl);					// 定数テーブル

	bool VertexShaderCompile(
		const char* filename,						// シェーダーファイル名
		const char* entry,							// エントリー関数名
		const char* version);						// バージョン

	bool PixelShaderCompile(
		const char* filename,						// シェーダーファイル名
		const char* entry,							// エントリー関数名
		const char* version);						// バージョン

	LPD3DXCONSTANTTABLE& GetVSTable() { return m_VSConstantTable; }
	LPD3DXCONSTANTTABLE& GetPSTable() { return m_PSConstantTable; }
	LPDIRECT3DVERTEXSHADER9& GetVS() { return m_VertexShader; }
	LPDIRECT3DPIXELSHADER9& GetPS() { return m_PixelShader; }

	virtual void ShaderSet(D3DXMATRIX world) {}
	virtual void SetMaterial(D3DMATERIAL9 const&mat) {}

	// 全てのシェーダーを解放
	static void ReleaseAll();
protected:
	LPDIRECT3DVERTEXSHADER9 m_VertexShader;			// 頂点シェーダー
	LPD3DXCONSTANTTABLE		m_VSConstantTable;		// 定数テーブル

	LPDIRECT3DPIXELSHADER9	m_PixelShader;			// ピクセルシェーダー
	LPD3DXCONSTANTTABLE		m_PSConstantTable;		// 定数テーブル
};

#endif // !_SHADER_H_
