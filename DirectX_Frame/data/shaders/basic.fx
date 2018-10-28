//------------------------------------------------
// グローバル変数
//------------------------------------------------
float4x4 World;		// 変換行列
float4x4 View;		// 変換行列
float4x4 Proj;		// 変換行列
sampler		Sampler1 =
	sampler_state{
		MinFilter = LINEAR;
		MagFilter = LINEAR;
	};

//------------------------------------------------
// 頂点シェーダ
//------------------------------------------------
void VertexShader_Main(		float3 in_pos : POSITION,
			float4 in_color : COLOR0,
			float2 in_tex : TEXCOORD0,
 				out float4 out_pos : POSITION,
				out float4 out_color : COLOR0,
				out float2 out_tex : TEXCOORD0)
{
	// 座標変換
	out_pos = mul(in_pos, mul(mul(World, View), Proj));	// 頂点と変換行列を掛ける
	out_color = in_color;					// 色はそのまま
	out_tex = in_tex;					// テクスチャ座標の出力
}

//------------------------------------------------
// ピクセルシェーダ
//------------------------------------------------
void PixelShader_Main(		float4 in_color : COLOR0,
			float2 in_tex : TEXCOORD0,	// ピクセルのUV座標
			out float4 out_color : COLOR0)
{
    // 固定の色で出力
//	out_color = in_color;			// R G B A	

	// サンプラーからテクセルを取得
	float4 texcol = tex2D(Sampler1, in_tex);	

	//out_color = texcol;	// テクセル色を出力

	// 頂点カラーとブレンド
	//out_color = texcol * in_color;

	// 頂点カラーと加算
	//out_color = texcol + in_color;

	// テクセル色を反転
	out_color = 1.0f - texcol;
}

// -------------------------------------------------------------
// テクニック
// -------------------------------------------------------------
technique TVPShader
{
	pass pass0
	{
		// ステート設定
		// シェーダ設定
		VertexShader = compile vs_2_0 VertexShader_Main();	// 頂点シェーダの設定
		PixelShader = compile ps_2_0 PixelShader_Main();	// ピクセルシェーダの設定
	}
}