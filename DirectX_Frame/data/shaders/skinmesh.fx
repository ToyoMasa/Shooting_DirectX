// グローバル変数
float4x4 	g_matWorld[4] : WORLD;   // ワールド変換行列配列
float4x4	g_world;		// 変換行列
float4x4	g_view;		// 変換行列
float4x4	g_projection;		// 変換行列
int 		g_BlendNum;       // ブレンドする配列の数
bool		g_tex;			// テクスチャありなしフラグ　true:あり

sampler		Sampler1 =
sampler_state {
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

// 頂点シェーダの関数
void main(
	in float4 inPos		: POSITION,			// [入力] 座標（モデル空間）
	in float4 inDiff	: COLOR0,			// [入力] 色
	float4 	inWeight : BLENDWEIGHT,
	float2 in_tex : TEXCOORD0,

	out float4 outPos	: POSITION,			// [出力] 座標（射影空間）
	out float4 outDiff	: COLOR0,
	out float2 out_tex : TEXCOORD0)			// [出力] 色
{
	float Weight[4] = (float[4])inWeight;      // 重みをfloatに分割します
	float LastBlendWeight = 0.0f;        // 最後の行列に掛けられる重み
	float4x4 matCombWorld = 0.0f;           // 合成ワールド変換行列

	// ワールド変換行列をブレンド
	for(int i=0; i < g_BlendNum - 1; i++)
	{
	LastBlendWeight += Weight[i];   // 最後の重みをここで計算しておく
	matCombWorld += g_matWorld[i] * Weight[i];
	}

	// 最後の重みを足し算
	matCombWorld += g_matWorld[g_BlendNum-1] * (1.0f-LastBlendWeight);


	outPos  = mul(inPos, mul(mul(matCombWorld, g_view), g_projection));	// 頂点と変換行列を掛ける
	outDiff = inDiff;						// 色はそのまま	
	out_tex = in_tex;					// テクスチャ座標の出力
}

// ピクセルシェーダの関数
void PS(
	in  float4 inDiff	: COLOR0,			// [入力] 色
	float2 in_tex : TEXCOORD0,	// ピクセルのUV座標
	out float4 outDiff : COLOR0)			// [出力] 色
{
	if (g_tex)
	{
		// サンプラーからテクセルを取得
		float4 texcol = tex2D(Sampler1, in_tex);

		outDiff = texcol;	// テクセル色を出力
	}
	else
	{
		outDiff = inDiff;	// テクセル色を出力
	}
}
