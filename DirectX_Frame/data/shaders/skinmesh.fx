// グローバル変数
float4x4 	g_matWorld[4] : WORLD;  // ワールド変換行列配列
float4x4	g_world;		// 変換行列
float4x4	g_view;			// 変換行列
float4x4	g_projection;		// 変換行列
int 		g_blendNum;       	// ブレンドする配列の数
bool		g_tex;			// テクスチャありなしフラグ　true:あり

// 光関連
float3		g_light_dir;		// 平行光源の方向
float4		g_diffuse;		// デフューズ
float4		g_ambient;		// アンビエント
float4		g_specular;		// スペキュラ―

// マテリアル
float4		g_diffuse_mat;		// デフューズ
float4		g_emmisive_mat;		// エミッシブ
float4		g_ambient_mat;		// アンビエント
float4		g_specular_mat;		// スペキュラー
float		g_power;		// スペキュラパワー

sampler		Sampler1 =
sampler_state {
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

// 頂点シェーダの関数
void main(
	float4 inPos		: POSITION,		// [入力] 座標（モデル空間）
	float4 inDiff		: COLOR0,		// [入力] 色
	float4 inWeight 	: BLENDWEIGHT,
	float3 inNormal 	: NORMAL,
	float2 inTex 		: TEXCOORD0,

	out float4 outPos	: POSITION,		// [出力] 座標（射影空間）
	out float4 outDiff	: COLOR0,
	out float2 outTex 	: TEXCOORD0)		// [出力] 色
{
	float Weight[4] = (float[4])inWeight;      	// 重みをfloatに分割します
	float LastBlendWeight = 0.0f;        		// 最後の行列に掛けられる重み
	float4x4 matCombWorld = 0.0f;           	// 合成ワールド変換行列

	// ワールド変換行列をブレンド
	int i;
	for(i = 0; i < 4; i++)
	{
		LastBlendWeight += Weight[i];   // 最後の重みをここで計算しておく
		matCombWorld += g_matWorld[i] * Weight[i];
	}

	// 最後の重みを足し算
	matCombWorld += g_matWorld[g_blendNum-1] * (1.0f-LastBlendWeight);

	// 法線ベクトルを変換
	float3 transNormal = mul(inNormal, (float3x3)matCombWorld);
	//outNormal = normalize(transNormal);

	outPos  = mul(inPos, mul(mul(matCombWorld, g_view), g_projection));	// 頂点と変換行列を掛ける
	outDiff = inDiff;						// 色はそのまま	
	outTex = inTex;					// テクスチャ座標の出力

	float3 N;	// ワールド変換済み法線
	float3 L;	// 光のベクトル

	// 法線をワールド変換して正規化
	N = normalize(transNormal);

	// 平行光源の光を正規化
	L = normalize(-g_light_dir);	// 方向を反転している

	// 出力カラーを決める
	outDiff = g_ambient * g_ambient_mat + g_diffuse * g_diffuse_mat * max(0.0, dot(L, N));
	outDiff.w = 1.0f;
}

// ピクセルシェーダの関数
void PS(
	float4 inDiff	: COLOR0,	// [入力] 色
	float2 inTex : TEXCOORD0,		// ピクセルのUV座標
	out float4 outDiff : COLOR0)		// [出力] 色
{
	if (g_tex)
	{
		// サンプラーからテクセルを取得
		float4 texcol = tex2D(Sampler1, inTex);

		outDiff = inDiff * texcol;	// テクセル色を出力
		outDiff.a = 0;
	}
	else
	{
		outDiff = inDiff;	// テクセル色を出力
	}
}
