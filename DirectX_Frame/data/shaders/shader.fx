// グローバル変数
float4x4 World;		// 変換行列
float4x4 View;		// 変換行列
float4x4 Proj;		// 変換行列

// 頂点シェーダの関数
void VertexShader_Main(
	in float4 inPos		: POSITION,			// [入力] 座標（モデル空間）
	in float4 inDiff	: COLOR0,			// [入力] 色

	out float4 outPos	: POSITION,			// [出力] 座標（射影空間）
	out float4 outDiff	: COLOR0)			// [出力] 色
{
	outPos  = mul(inPos, mul(mul(World, View), Proj));	// 頂点と変換行列を掛ける
	outDiff = inDiff;						// 色はそのまま	
}

// ピクセルシェーダの関数
void PixelShader_Main(
	in  float4 inDiff	: COLOR0,			// [入力] 色
	out float4 outDiff : COLOR0)			// [出力] 色
{
	outDiff = inDiff;
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
