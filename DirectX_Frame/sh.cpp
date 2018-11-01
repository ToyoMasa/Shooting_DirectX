//*****************************************************************************
//!	@file	shader.cpp
//!	@brief	
//!	@note	シェーダー関連処理
//!	@author	
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include	"sh.h"

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
bool ShaderCompile(const char* filename,
	const char* entry,
	const char* version,
	LPD3DXBUFFER* code,
	LPD3DXCONSTANTTABLE* ctbl)
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
bool VertexShaderCompile(
	LPDIRECT3DDEVICE9 device,					// デバイスオブジェクト
	const char* filename,						// シェーダーファイル名
	const char* entry,							// エントリー関数名
	const char* version,						// バージョン
	LPD3DXCONSTANTTABLE* ctbl,					// 定数テーブル
	LPDIRECT3DVERTEXSHADER9* vsh)				// 頂点シェーダーオブジェクト
{
	bool sts;
	LPD3DXBUFFER code;

	// シェーダーコンパイル
	sts = ShaderCompile(filename,
		entry,
		version,
		&code,
		ctbl);
	if (!sts) {
		return false;
	}

	// 頂点シェーダーオブジェクトを作成する
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
//!	@brief	ピクセルシェーダーオブジェクト生成
//!	@param	LPDIRECT3DDEVICE9 lpdevidce    デバイスオブジェクト
//!	@param	const char* filename,		   シェーダーファイル名
//!	@param	const char* entry,			   エントリー関数名
//!	@param	const char* version,		   バージョン
//!	@param	LPD3DXCONSTANTTABLE* ctbl,	　 定数テーブル
//!	@param	LPDIRECT3DVPIXELSHADER9* psh　 ピクセルシェーダーオブジェクト
//!	@retval	bool true 成功　false 失敗
//==============================================================================
bool PixelShaderCompile(
	LPDIRECT3DDEVICE9 device,					// デバイスオブジェクト
	const char* filename,						// シェーダーファイル名
	const char* entry,							// エントリー関数名
	const char* version,						// バージョン
	LPD3DXCONSTANTTABLE* ctbl,					// 定数テーブル
	LPDIRECT3DPIXELSHADER9* psh)				// ピクセルシェーダーオブジェクト
{
	bool sts;
	LPD3DXBUFFER code;

	// シェーダーコンパイル
	sts = ShaderCompile(filename,
		entry,
		version,
		&code,
		ctbl);
	if (!sts) {
		return false;
	}

	// ピクセルシェーダーオブジェクトを作成する
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
