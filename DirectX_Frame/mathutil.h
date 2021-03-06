//======================================================================
//	計算処理[mathutil.h]
//
//======================================================================
#ifndef _MATHUTIL_H_
#define _MATHUTIL_H_

#include	<d3dx9.h>

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
float FloatRandom();
void MakeWorldMatrix(D3DXMATRIX& mat, const D3DXVECTOR3& angle, const D3DXVECTOR3& trans);
void MakeWorldMatrix(D3DXMATRIX& mat, D3DXMATRIX& mattotal, const D3DXVECTOR3& angle, const D3DXVECTOR3& trans);
bool IsCanIsee(const D3DXVECTOR3& pos, const D3DXMATRIX& matcamera, const D3DXMATRIX& matprojection,D3DXVECTOR3& );
void WorldToScreen(const LPDIRECT3DDEVICE9 lpdev, const D3DXMATRIX& matCamera, const D3DXMATRIX& matProjection, const D3DXVECTOR3& wp, D3DXVECTOR3& sp);

#endif // !_MATHUTIL_H_
