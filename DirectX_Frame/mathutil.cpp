//*****************************************************************************
//!	@file	matuutil.cpp
//!	@brief	
//!	@note	数学系のユーティリティ関数群
//!	@author	
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include	<d3dx9.h>

//==============================================================================
//!	@fn		MakeWorldMatrix
//!	@brief	角度と平行移動量から行列を作る
//!	@param　行列（出力）　　	
//!	@param　	角度（入力）
//!	@param　	平行移動量（入力）
//!	@retval	なし
//==============================================================================
void MakeWorldMatrix(D3DXMATRIX& mat,const D3DXVECTOR3& angle,const D3DXVECTOR3& trans){
	D3DXMATRIX matx, maty, matz, mattrans;;
	D3DXMatrixRotationX(&matx, D3DX_PI*angle.x / 180.0f);
	D3DXMatrixRotationY(&maty, D3DX_PI*angle.y / 180.0f);
	D3DXMatrixRotationZ(&matz, D3DX_PI*angle.z / 180.0f);
	mat = matx*maty*matz;
	D3DXMatrixTranslation(&mattrans, trans.x, trans.y, trans.z);
	
	mat = mat*mattrans;
}

//==============================================================================
//!	@fn		MakeWorldMatrix
//!	@brief	角度と平行移動量から行列を作る(積算行列を計算)
//!	@param　行列（出力）　　	
//!	@param　積算行列（出力）　　	
//!	@param　角度（入力）
//!	@param　平行移動量（入力）
//!	@retval	なし
//==============================================================================
void MakeWorldMatrix(D3DXMATRIX& mat, D3DXMATRIX& mattotal,const D3DXVECTOR3& angle, const D3DXVECTOR3& trans){
	D3DXMATRIX matx, maty, matz, mattrans;;

	D3DXMatrixRotationX(&matx, D3DX_PI*angle.x / 180.0f);			// X軸回転行列を生成
	D3DXMatrixRotationY(&maty, D3DX_PI*angle.y / 180.0f);			// Y軸回転行列を生成
	D3DXMatrixRotationZ(&matz, D3DX_PI*angle.z / 180.0f);			// Z軸回転行列を生成
	mat = matx*maty*matz;											// 合成

	D3DXMatrixTranslation(&mattrans, trans.x, trans.y, trans.z);	// 平行移動行列を生成

	mat = mat*mattrans;
	mattotal = mat*mattotal;
}

//==============================================================================
//!	@fn		IsCanIsee
//!	@brief	画面内に存在しているかをチェック
//!	@param　位置座標（ワールド）　　	
//!	@param　カメラ行列　	
//!	@param　プロジェクション変換行列
//!	@param　スクリーン座標
//!	@retval	true 画面内　false 画面外
//==============================================================================
bool IsCanIsee(const D3DXVECTOR3& pos,const D3DXMATRIX& matcamera,const D3DXMATRIX& matprojection,D3DXVECTOR3& ans){

	D3DXMATRIX mat;

	// カメラ変換行列とプロジェクション変換行列の合成
	mat = matcamera*matprojection;

	// スクリーン座標に変換
	D3DXVec3TransformCoord(&ans, &pos, &mat);

	if (ans.z < 0.0f){
		return false;
	}
	if (ans.z > 1.0f){
		return false;
	}
	if (ans.x < -1.0f)
	{
		return false;
	}
	if (ans.x > 1.0f)
	{
		return false;
	}
	if (ans.y < -1.0f)
	{
		return false;
	}
	if (ans.y > 1.0f)
	{
		return false;
	}
	return true;
}

//==============================================================================
//!	@fn		WorldToScreen
//!	@brief	ワールド座標をスクリーン座標に変換する
//!	@param　デバイスオブジェクト　　	
//!	@param　カメラ行列　　	
//!	@param　プロジェクション変換行列　	
//!	@param　ワールド座標系での座標（返還前）
//!	@param　スクリーン座標
//!	@retval	なし
//==============================================================================
void WorldToScreen(
	const LPDIRECT3DDEVICE9 lpdev,
	const D3DXMATRIX& matCamera,
	const D3DXMATRIX& matProjection,
	const D3DXVECTOR3& wp,
	D3DXVECTOR3& sp){

	D3DXMATRIX		mat;
	D3DXMATRIX		matViewPort;		// ビューポート行列

	D3DVIEWPORT9	vp;

	// ビューポート取得
	lpdev->GetViewport(&vp);

	// スクリーン幅、高さ取得
	DWORD screen_width = vp.Width;
	DWORD screen_height = vp.Height;

	// ビューポート行列作成
	D3DXMatrixIdentity(&matViewPort);
	matViewPort._11 = screen_width / 2.0f;
	matViewPort._22 = -static_cast<float>(screen_height) / 2.0f;
	matViewPort._41 = screen_width / 2.0f;
	matViewPort._42 = screen_height / 2.0f;

	// 行列を合成（カメラ行列＊プロジェクション変換行列＊ビューポート行列）
	mat = matCamera*matProjection*matViewPort;
	// 行列×ベクトル
	D3DXVec3TransformCoord(&sp, &wp, &mat);
}

//******************************************************************************
//	End of file.
//******************************************************************************
