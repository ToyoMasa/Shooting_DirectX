//*****************************************************************************
//!	@file	shader.cpp
//!	@brief	
//!	@note	シェーダー関連処理
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
//!	@brief	シェーダーコンパイル
//!	@param	const char* filename,　 シェーダーファイル名
//!	@param	const char* entry,　　　エントリ関数名
//!	@param	const char* version,　　シェーダーバージョン指定
//!	@param	LPD3DXBUFFER* code,　　 コンパイル済みコード　
//!	@param	LPD3DXCONSTANTTABLE* ctbl　定数テーブル
//!	@retval	bool true 成功　false 失敗
//==============================================================================
bool Shader::ShaderCompile(
	const char* filename,
	const char* entry,
	const char* version,						// バージョン
	LPD3DXBUFFER* code,							// コンパイル済みコード
	LPD3DXCONSTANTTABLE* ctbl)					// 定数テーブル
{
	LPD3DXBUFFER err = nullptr;

	HRESULT hr;

	// 頂点シェーダをコンパイル
	hr = D3DXCompileShaderFromFile(
		filename,		// ファイル名
		nullptr,		// プリプロセッサ定義へのポインタ 
		nullptr,		// ID3DXInclude（#include疑似命令）
		entry,			// 頂点シェーダー関数名 
		version,		// 頂点シェーダーのバージョン 
		0,				// コンパイルオプション
		code,			// コンパイル済みのシェーダーコード（OUT）
		&err,			// コンパイルエラー情報が格納される（OUT）
		ctbl);			// シェーダー内のコンスタントテーブル（OUT）

						// エラー発生
	if (FAILED(hr))
	{
		if (err) {
			// コンパイルエラーあり
			MessageBox(NULL, (LPSTR)err->GetBufferPointer(), "D3DXCompileShaderFromFile", MB_OK);
		}
		else {
			// その他のエラー
			MessageBox(NULL, "シェーダーファイルが読み込めません", "D3DXCompileShaderFromFile", MB_OK);
		}
		return false;
	}
	return true;
}

//==============================================================================
//!	@fn		VertexShaderCompile
//!	@brief	頂点シェーダーオブジェクト生成
//!	@param	LPDIRECT3DDEVICE9 lpdevidce    デバイスオブジェクト
//!	@param	const char* filename,		   シェーダーファイル名
//!	@param	const char* entry,			   エントリー関数名
//!	@param	const char* version,		   バージョン
//!	@param	LPD3DXCONSTANTTABLE* ctbl,	　 定数テーブル
//!	@param	LPDIRECT3DVERTEXSHADER9* vsh　 頂点シェーダーオブジェクト
//!	@retval	bool true 成功　false 失敗
//==============================================================================
bool Shader::VertexShaderCompile(
	const char* filename,						// シェーダーファイル名
	const char* entry,							// エントリー関数名
	const char* version)						// バージョン
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return false;
	}

	bool sts;
	LPD3DXBUFFER code;

	// シェーダーコンパイル
	sts = ShaderCompile(filename,
		entry,
		version,
		&code,
		&m_VSConstantTable);
	if (!sts) {
		return false;
	}

	// 頂点シェーダーオブジェクトを作成する
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
//!	@brief	ピクセルシェーダーオブジェクト生成
//!	@param	LPDIRECT3DDEVICE9 lpdevidce    デバイスオブジェクト
//!	@param	const char* filename,		   シェーダーファイル名
//!	@param	const char* entry,			   エントリー関数名
//!	@param	const char* version,		   バージョン
//!	@param	LPD3DXCONSTANTTABLE* ctbl,	　 定数テーブル
//!	@param	LPDIRECT3DVPIXELSHADER9* psh　 ピクセルシェーダーオブジェクト
//!	@retval	bool true 成功　false 失敗
//==============================================================================
bool Shader::PixelShaderCompile(
	const char* filename,						// シェーダーファイル名
	const char* entry,							// エントリー関数名
	const char* version)						// バージョン
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return false;
	}

	bool sts;
	LPD3DXBUFFER code;

	// シェーダーコンパイル
	sts = ShaderCompile(filename,
		entry,
		version,
		&code,
		&m_PSConstantTable);
	if (!sts) {
		return false;
	}

	// ピクセルシェーダーオブジェクトを作成する
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

	// 光の設定情報
	D3DXVECTOR4		light_dir(0.0f, -1.0f, 0.0f, 0.0f);			// 光の方向
	D3DXVECTOR4		diffuse(1.0f, 1.0f, 1.0f, 1.0f);			// 平行光源の色
	D3DXVECTOR4		ambient(0.2f, 0.2f, 0.2f, 0.2f);			// 環境光
	D3DXVECTOR4		specular(0.2f, 0.2f, 0.2f, 0.2f);			// スペキュラ光

	// 頂点シェーダーとピクセルシェーダーをセット
	pDevice->SetVertexShader(m_VertexShader);
	pDevice->SetPixelShader(m_PixelShader);

	// 定数をセット(頂点シェーダー)
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
