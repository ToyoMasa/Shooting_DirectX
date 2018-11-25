// グローバル変数
//float4x4 	g_matWorld[4] : WORLD;  // ワールド変換行列配列
float4x4	g_world;		// 変換行列
float4x4	g_view;			// 変換行列
float4x4	g_projection;		// 変換行列
//int 		g_blendNum;       	// ブレンドする配列の数
bool		g_tex;			// テクスチャありなしフラグ　true:あり

float3		g_lightposition;
float		g_lightstrength;
float3		g_lightdirection;
float		g_lightcos; // スポットライトの角度のコサイン値
float		g_lightdecay; // 減衰

//------------------------------------------------
// サンプラー1
//------------------------------------------------
sampler Sampler1 =
sampler_state {
	Texture = <g_texture>;
	MinFilter = LINEAR;		// リニアフィルタ（縮小時）
	MagFilter = LINEAR;		// リニアフィルタ（拡大時）
};

void main(float3 in_pos 	: POSITION,
	float3 in_normal 		: NORMAL,
	float2 in_tex1			: TEXCOORD0,
	out float4 out_pos			: POSITION,
	out float2 out_tex1			: TEXCOORD0,
	out float3 out_normal 		: TEXCOORD1,
	out float3 out_lightdir 	: TEXCOORD2)
{

	// 座標変換して出力（ワールド変換後の頂点も出力）
	out_pos = mul(float4(in_pos, 1.0f), g_world);		// 頂点座標（ローカル座標系）をワールド座標系に変換	
	out_pos = mul(out_pos, g_view);						// 頂点座標（ワールド座標系）をカメラ座標系に変換
	out_pos = mul(out_pos, g_projection);				// 頂点座標（カメラ座標系）をスクリーン座標系に変換

	// 法線をワールド空間上のベクトルに変換して、単位ベクトル化してから出力
	out_normal = normalize(mul(in_normal, (float3x3)g_world));

	out_lightdir = out_pos - g_lightposition;

	// テクスチャ座標をそのまま出力する
	out_tex1 = in_tex1;			// テクスチャ座標の出力

}

void PS(float2 in_tex1		: TEXCOORD0,	//テクスチャ座標入力
	float3 in_normal 		: TEXCOORD1,	//ピクセルの法線入力
	float3 in_lightdir 		: TEXCOORD2,
	out float4 out_color 	: COLOR0)			//ピクセルカラー出力
{
	float4 color = float4(1.0f, 0.0f, 0.0f, 1.0f);
	float ambient = 0.1f;

	float3 ldir = normalize(in_lightdir);
	float shade = 0;
	float coneFactor = dot(ldir, normalize(g_lightdirection)); //< スポットライトの範囲内かどうか
	if(coneFactor > g_lightcos) 
	{
		shade = dot(normalize(in_normal), - ldir) * pow(coneFactor, g_lightdecay) * g_lightstrength; //< 減衰とライトの強さを考慮
 	}

	//色の出力
	if (g_tex)
	{
		float4 tex_color = tex2D(Sampler1, in_tex1);
		out_color = tex_color * (shade + ambient);
	}
	else
	{
		out_color = color * (shade + ambient);
	}
}