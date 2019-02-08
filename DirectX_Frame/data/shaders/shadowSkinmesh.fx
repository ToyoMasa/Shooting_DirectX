//------------------------------------------------
// グローバル変数
//------------------------------------------------
float4x4 	g_matWorld[4] : WORLD;  // ワールド変換行列配列
float4x4	g_world;				// ワールド変換行列
float4x4 	g_view;					// カメラ変換行列
float4x4 	g_projection;			// プロジェクション変換行列
float4x4 	g_light_view;					// カメラ変換行列
float4x4 	g_light_projection;			// プロジェクション変換行列
int 		g_blendNum;				// ブレンドする配列の数
float3		g_light_pos;
float		g_far;
float		g_near;

//------------------------------------------------
// 頂点シェーダ
//------------------------------------------------
void main(
	float3 in_pos : POSITION,
	float4 in_weight : BLENDWEIGHT,
	float3 in_normal : NORMAL,
	out float4 out_pos : POSITION,
	out float4 out_ztex : TEXCOORD0)
{
	float Weight[4] = (float[4])in_weight;      // 重みをfloatに分割します
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

	// 法線ベクトルを変換
	float3 transNormal = mul(in_normal, (float3x3)matCombWorld);
	float3 shadowNormal = mul(transNormal, (float3x3)g_view);

	out_pos = mul(float4(in_pos, 1.0f), matCombWorld);		// 頂点座標（ローカル座標系）をワールド座標系に変換
	out_pos = mul(out_pos, g_light_view);							// 頂点座標（ワールド座標系）をカメラ座標系に変換
	out_pos = mul(out_pos, g_light_projection);

	out_ztex = out_pos;
	//float4 shadowPos = out_pos;

	//// 光源に対して裏向きの頂点を光線方向に移動させる
	//if (dot(shadowNormal, -g_light_pos) < 0)
	//{
	//	if (g_light_pos.z > 0)
	//	{
	//		shadowPos.xyz += g_light_pos * abs(g_far - shadowPos.z) / g_light_pos.z;
	//	}
	//	else
	//	{
	//		shadowPos.xyz += g_light_pos * abs(g_near - shadowPos.z) / g_light_pos.z;
	//	}
	//	out_pos = mul(shadowPos, g_projection);
	//}
	//else
	//{
	//	out_pos = mul(out_pos, g_projection);
	//}
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
