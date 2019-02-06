//------------------------------------------------
// グローバル変数
//------------------------------------------------

float4x4	g_world;					// ワールド変換行列
float4x4 	g_view;						// カメラ変換行列
float4x4 	g_projection;				// プロジェクション変換行列

//------------------------------------------------
// 頂点シェーダ
//------------------------------------------------
void main(
	float3 in_pos		: POSITION,
	out float4 out_pos : POSITION)
{
	// 座標変換
	out_pos = mul(float4(in_pos, 1.0f), g_world);
	out_pos = mul(out_pos, g_view);
	out_pos = mul(out_pos, g_projection);
}

//------------------------------------------------
// ピクセルシェーダ
//------------------------------------------------
void PS(out float4 out_color : COLOR0)
{
	out_color = float4(1, 1, 1, 0);
}
