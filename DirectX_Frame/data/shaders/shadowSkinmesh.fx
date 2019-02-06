//------------------------------------------------
// グローバル変数
//------------------------------------------------
float4x4 	g_matWorld[4] : WORLD;  // ワールド変換行列配列
float4x4	g_world;				// ワールド変換行列
float4x4 	g_view;					// カメラ変換行列
float4x4 	g_projection;			// プロジェクション変換行列
int 		g_blendNum;				// ブレンドする配列の数

//------------------------------------------------
// 頂点シェーダ
//------------------------------------------------
void main(
	float3 in_pos		: POSITION,
	float4 in_weight : BLENDWEIGHT,
	out float4 out_pos : POSITION)
{
	float Weight[4] = (float[4])in_weight;      	// 重みをfloatに分割します
	float LastBlendWeight = 0.0f;        		// 最後の行列に掛けられる重み
	float4x4 matCombWorld = 0.0f;           	// 合成ワールド変換行列

												// ワールド変換行列をブレンド
	for (int i = 0; i < g_blendNum; i++)
	{
		LastBlendWeight += Weight[i];   // 最後の重みをここで計算しておく
		matCombWorld += g_matWorld[i] * Weight[i];
	}

	// 最後の重みを足し算
	matCombWorld += g_matWorld[g_blendNum - 1] * (1.0f - LastBlendWeight);

	out_pos = mul(float4(in_pos, 1.0f), matCombWorld);		// 頂点座標（ローカル座標系）をワールド座標系に変換
	out_pos = mul(out_pos, g_view);							// 頂点座標（ワールド座標系）をカメラ座標系に変換
	out_pos = mul(out_pos, g_projection);					// 頂点座標（カメラ座標系）をスクリーン座標系に変換
}

//------------------------------------------------
// ピクセルシェーダ
//------------------------------------------------
void PS(out float4 out_color : COLOR0)
{
	out_color = float4(1, 1, 1, 0);
}
