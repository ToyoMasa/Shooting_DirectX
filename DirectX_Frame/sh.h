//*****************************************************************************
//!	@file	shader.h
//!	@brief	シェーダー関連処理
//!	@note	
//!	@author
//*****************************************************************************
#pragma once

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include	<d3dx9.h>

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
bool ShaderCompile(
	const char* filename,						// シェーダーファイル名
	const char* entry,							// エントリ関数名
	const char* version,						// バージョン
	LPD3DXBUFFER* code,							// コンパイル済みコード
	LPD3DXCONSTANTTABLE* ctbl);					// 定数テーブル

bool VertexShaderCompile(
	LPDIRECT3DDEVICE9 device,					// デバイスオブジェクト
	const char* filename,						// シェーダーファイル名
	const char* entry,							// エントリー関数名
	const char* version,						// バージョン
	LPD3DXCONSTANTTABLE* ctbl,					// 定数テーブル
	LPDIRECT3DVERTEXSHADER9* vsh);				// 頂点シェーダーオブジェクト

bool PixelShaderCompile(
	LPDIRECT3DDEVICE9 device,					// デバイスオブジェクト
	const char* filename,						// シェーダーファイル名
	const char* entry,							// エントリー関数名
	const char* version,						// バージョン
	LPD3DXCONSTANTTABLE* ctbl,					// 定数テーブル
	LPDIRECT3DPIXELSHADER9* vsh);				// ピクセルシェーダーオブジェクト

//******************************************************************************
//	End of file.
//******************************************************************************
