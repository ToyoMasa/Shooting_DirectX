//======================================================================
//	計算 [mathutil.cpp]　（2018/9/21）
//	Author : 豊村 昌俊
//======================================================================
#include	<d3dx9.h>

float FloatRandom()
{
	return rand() / 32767.0f;
}

void MakeWorldMatrix(D3DXMATRIX& mat,const D3DXVECTOR3& angle,const D3DXVECTOR3& trans)
{
	D3DXMATRIX matx, maty, matz, mattrans;;
	D3DXMatrixRotationX(&matx, D3DX_PI*angle.x / 180.0f);
	D3DXMatrixRotationY(&maty, D3DX_PI*angle.y / 180.0f);
	D3DXMatrixRotationZ(&matz, D3DX_PI*angle.z / 180.0f);
	mat = matx*maty*matz;
	D3DXMatrixTranslation(&mattrans, trans.x, trans.y, trans.z);
	
	mat = mat*mattrans;
}

void MakeWorldMatrix(D3DXMATRIX& mat, D3DXMATRIX& mattotal,const D3DXVECTOR3& angle, const D3DXVECTOR3& trans)
{
	D3DXMATRIX matx, maty, matz, mattrans;;

	D3DXMatrixRotationX(&matx, D3DX_PI*angle.x / 180.0f);			// X軸回転行列を生成
	D3DXMatrixRotationY(&maty, D3DX_PI*angle.y / 180.0f);			// Y軸回転行列を生成
	D3DXMatrixRotationZ(&matz, D3DX_PI*angle.z / 180.0f);			// Z軸回転行列を生成
	mat = matx*maty*matz;											// 合成

	D3DXMatrixTranslation(&mattrans, trans.x, trans.y, trans.z);	// 平行移動行列を生成

	mat = mat*mattrans;
	mattotal = mat*mattotal;
}

bool IsCanIsee(const D3DXVECTOR3& pos,const D3DXMATRIX& matcamera,const D3DXMATRIX& matprojection,D3DXVECTOR3& ans)
{

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

void WorldToScreen(
	const LPDIRECT3DDEVICE9 lpdev,
	const D3DXMATRIX& matCamera,
	const D3DXMATRIX& matProjection,
	const D3DXVECTOR3& wp,
	D3DXVECTOR3& sp)
{

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
