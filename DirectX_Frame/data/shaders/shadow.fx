//------------------------------------------------
// グローバル変数
//------------------------------------------------
float4x4	g_world;					// ワールド変換行列
float4x4 	g_view;						// カメラ変換行列
float4x4 	g_projection;				// プロジェクション変換行列
float4		g_light_pos;
float		g_far;
float		g_near;

//------------------------------------------------
// 頂点シェーダ
//------------------------------------------------
void main(
	float3 in_pos : POSITION,
	//float3 in_normal : NORMAL,
	out float4 out_pos : POSITION,
	out float4 out_ztex : TEXCOORD0)
{
	// 普通にワールドビュー射影行列をする
	out_pos = mul(float4(in_pos, 1.0f), g_world);		// 頂点座標（ローカル座標系）をワールド座標系に変換
	out_pos = mul(out_pos, g_view);							// 頂点座標（ワールド座標系）をカメラ座標系に変換
	out_pos = mul(out_pos, g_projection);

	// テクスチャ座標を頂点に合わせる
	out_ztex = out_pos;
}

//------------------------------------------------
// ピクセルシェーダ
//------------------------------------------------
void PS(
	float4 in_ztex : TEXCOORD0,
	out float4 out_color : COLOR0)
{
	out_color = in_ztex.z / in_ztex.w;
}
