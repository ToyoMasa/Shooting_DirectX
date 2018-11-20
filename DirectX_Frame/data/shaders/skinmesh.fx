// グローバル変数
float4x4	World;		// 変換行列
float4x4	View;		// 変換行列
float4x4	Proj;		// 変換行列
bool		g_tex;			// テクスチャありなしフラグ　true:あり

sampler		Sampler1 =
sampler_state {
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

// 頂点シェーダの関数
void VertexShader_Main(
	in float4 inPos		: POSITION,			// [入力] 座標（モデル空間）
	in float4 inDiff	: COLOR0,			// [入力] 色
	float2 in_tex : TEXCOORD0,

	out float4 outPos	: POSITION,			// [出力] 座標（射影空間）
	out float4 outDiff	: COLOR0,
	out float2 out_tex : TEXCOORD0)			// [出力] 色
{
	outPos  = mul(inPos, mul(mul(World, View), Proj));	// 頂点と変換行列を掛ける
	outDiff = inDiff;						// 色はそのまま	
	out_tex = in_tex;					// テクスチャ座標の出力
}

// ピクセルシェーダの関数
void PixelShader_Main(
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

// テクニックの定義
technique TVPShader		// テクニック名「TVPShader」
{
	pass pass0			// 1番目のパス名「pass0」
	{
		// ステート設定
		// シェーダ設定
		VertexShader = compile vs_2_0 VertexShader_Main();	// 頂点シェーダの設定
		PixelShader = compile ps_2_0 PixelShader_Main();	// ピクセルシェーダの設定
	}
}
