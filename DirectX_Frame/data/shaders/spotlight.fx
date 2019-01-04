//// グローバル変数
////float4x4 	g_matWorld[4] : WORLD;  // ワールド変換行列配列
//float4x4	g_world;		// 変換行列
//float4x4	g_view;			// 変換行列
//float4x4	g_projection;		// 変換行列
////int 		g_blendNum;       	// ブレンドする配列の数
//bool		g_tex;			// テクスチャありなしフラグ　true:あり
//
//float3		g_lightposition;
//float		g_lightstrength;
//float3		g_lightdirection;
//float		g_lightcos; // スポットライトの角度のコサイン値
//float		g_lightdecay; // 減衰
//
////------------------------------------------------
//// サンプラー1
////------------------------------------------------
//sampler Sampler1 =
//sampler_state {
//	Texture = <g_texture>;
//	MinFilter = LINEAR;		// リニアフィルタ（縮小時）
//	MagFilter = LINEAR;		// リニアフィルタ（拡大時）
//};
//
//void main(float3 in_pos 	: POSITION,
//	float3 in_normal 		: NORMAL,
//	float2 in_tex1			: TEXCOORD0,
//	out float4 out_pos			: POSITION,
//	out float2 out_tex1			: TEXCOORD0,
//	out float3 out_normal 		: TEXCOORD1,
//	out float3 out_lightdir 	: TEXCOORD2)
//{
//
//	// 座標変換して出力（ワールド変換後の頂点も出力）
//	out_pos = mul(float4(in_pos, 1.0f), g_world);		// 頂点座標（ローカル座標系）をワールド座標系に変換	
//	out_pos = mul(out_pos, g_view);						// 頂点座標（ワールド座標系）をカメラ座標系に変換
//	out_pos = mul(out_pos, g_projection);				// 頂点座標（カメラ座標系）をスクリーン座標系に変換
//
//	// 法線をワールド空間上のベクトルに変換して、単位ベクトル化してから出力
//	out_normal = normalize(mul(in_normal, (float3x3)g_world));
//
//	out_lightdir = out_pos - g_lightposition;
//
//	// テクスチャ座標をそのまま出力する
//	out_tex1 = in_tex1;			// テクスチャ座標の出力
//
//}
//
//void PS(float2 in_tex1		: TEXCOORD0,	//テクスチャ座標入力
//	float3 in_normal 		: TEXCOORD1,	//ピクセルの法線入力
//	float3 in_lightdir 		: TEXCOORD2,
//	out float4 out_color 	: COLOR0)			//ピクセルカラー出力
//{
//	float4 color = float4(1.0f, 0.0f, 0.0f, 1.0f);
//	float ambient = 0.1f;
//
//	float3 ldir = normalize(in_lightdir);
//	float shade = 0;
//	float coneFactor = dot(ldir, normalize(g_lightdirection)); //< スポットライトの範囲内かどうか
//	if(coneFactor >= g_lightcos) 
//	{
//		shade = dot(normalize(in_normal), - ldir) * pow(coneFactor, g_lightdecay) * g_lightstrength; // 減衰とライトの強さを考慮
// 	}
//
//	//色の出力
//	if (g_tex)
//	{
//		float4 tex_color = tex2D(Sampler1, in_tex1);
//		out_color = tex_color * (shade + ambient);
//	}
//	else
//	{
//		out_color = color * (shade + ambient);
//	}
//}

float4x4	g_world;				// ワールド変換行列
float4x4 	g_view;					// カメラ変換行列
float4x4 	g_projection;				// プロジェクション変換行列

// main関数
void main(
	float3 in_pos			: POSITION,
	float3 in_normal		: NORMAL,
	float4 in_diff			: COLOR0,
	float2 in_tex			: TEXCOORD0,
	out float4 out_pos		: POSITION,
	out float4 out_diff		: COLOR0,
	out float2 out_tex		: TEXCOORD0,
	out float3 out_normal	: TEXCOORD1,
	out float3 out_posforps : TEXCOORD2)
{
	float3 lWorldNrm;
	float3 lViewNrm;


	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// 座標変換して出力（ワールド変換後の頂点も出力）
	out_pos = mul(float4(in_pos, 1.0f), g_world);		// 頂点座標（ローカル座標系）をワールド座標系に変換	
	out_posforps = out_pos.xyz;							// ワールド変換済み座標
	out_pos = mul(out_pos, g_view);						// 頂点座標（ワールド座標系）をカメラ座標系に変換
	out_pos = mul(out_pos, g_projection);				// 頂点座標（カメラ座標系）をスクリーン座標系に変換

	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )



	// 法線をビュー空間の角度に変換 =========================================( 開始 )

	// 法線をワールド空間上のベクトルに変換して、単位ベクトル化してから出力
	out_normal = normalize(mul(in_normal, (float3x3)g_world));

	// 法線をビュー空間の角度に変換 =========================================( 終了 )

	out_diff = in_diff;
	out_tex = in_tex;
}

// ライトパラメータ
float4	g_light_pos;               // 座標( ビュー空間 )
float3	g_light_dir;              // 方向( ビュー空間 )
float4	g_light_diff;                // ディフューズカラー
float4	g_light_specular;               // スペキュラカラー
float4	g_light_ambient;                // アンビエントカラーとマテリアルのアンビエントカラーを乗算したもの
float4	g_falloff_param;  // x:有効距離  y:スポットライト用FallOff  z:距離による減衰処理用パラメータ０  w:距離による減衰処理用パラメータ１
float4	g_light_param;      // x:距離による減衰処理用パラメータ２  y:スポットライト用パラメータ０( cos( Phi / 2.0f ) )  z:スポットライト用パラメータ１( 1.0f / ( cos( Theta / 2.0f ) - cos( Phi / 2.0f ) ) )

float4  g_mat_ambient;
float4	g_mat_emissive;		// エミッシブカラー + マテリアルアンビエントカラー * グローバルアンビエントカラー
float4	g_mat_diffuse;
float4	g_mat_specular;
float4	g_mat_power;
float	g_alpha;		// 不透明度等


//------------------------------------------------
// サンプラー1
//------------------------------------------------
sampler Sampler1 =
sampler_state {
	Texture = <g_texture>;
	MinFilter = LINEAR;		// リニアフィルタ（縮小時）
	MagFilter = LINEAR;		// リニアフィルタ（拡大時）
};

// main関数
void PS(
	float2 in_tex		: TEXCOORD0,
	float4 in_diff : COLOR0,
	float3 in_normal : TEXCOORD1,    // 法線( ビュー空間 )
	float3 in_posforps      : TEXCOORD2,    // 座標( ビュー空間 ))
	out float4 out_color	: COLOR0)
{
	float4 TextureDiffuseColor;
	float4 SpecularColor;
	float3 Normal;
	float DiffuseAngleGen;
	float4 TotalDiffuse;
	float4 TotalSpecular;
	float3 V_to_Eye;
	float3 TempF3;
	float Temp;
	float3 lLightTemp;
	float lLightDistancePow2;
	float lLightGen;
	float3 lLightDir;
	float lLightDirectionCosA;


	// 法線の準備
	Normal = normalize(in_normal);

	// 頂点座標から視点へのベクトルを正規化
	V_to_Eye = normalize(g_light_pos.xyz - in_posforps);

	// ディフューズカラーとスペキュラカラーの蓄積値を初期化
	TotalDiffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	TotalSpecular = float4(0.0f, 0.0f, 0.0f, 0.0f);


	// スポットライトの処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// ライト方向ベクトルの計算
	lLightDir = normalize(in_posforps - g_light_pos.xyz);


	// 距離・スポットライト減衰値計算 =======================================( 開始 )

	// 距離減衰計算 ------------------

	// 頂点とライト位置との距離の二乗を求める
	lLightTemp = in_posforps - g_light_pos.xyz;
	lLightDistancePow2 = length(lLightTemp) * length(lLightTemp);

	// 減衰率の計算 lLightGen = 1 / ( 減衰値0 + 減衰値1 * 距離 + 減衰値2 * ( 距離 * 距離 ) )
	lLightGen = 1.0f / (g_falloff_param.z + g_falloff_param.w * length(lLightTemp) + g_light_param.x * lLightDistancePow2);

	// --------------------------------


	// スポットライト減衰計算 --------

	// ライト方向ベクトルとライト位置から頂点位置へのベクトルの内積( 即ち Cos a )を計算 
	lLightDirectionCosA = dot(lLightDir, g_light_dir);

	// スポットライト減衰計算  pow( falloff, ( ( Cos a - Cos f ) / ( Cos q - Cos f ) ) )
	lLightGen *= saturate(pow(abs(max(lLightDirectionCosA - g_light_param.y, 0.0f) * g_light_param.z), g_falloff_param.y));

	// --------------------------------


	// 有効距離外だったら減衰率を最大にする処理
	lLightGen *= step(length(lLightTemp), g_falloff_param.x);

	// 距離・スポットライト減衰値計算 =======================================( 終了 )


	// ディフューズ色計算

	// ディフューズ角度減衰率計算
	DiffuseAngleGen = saturate(dot(Normal, -lLightDir));

	// ディフューズカラー蓄積値 += ( ライトのディフューズカラー * マテリアルディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの  ) * 距離・スポットライトの角度減衰率
	TotalDiffuse += (g_light_diff * in_diff * DiffuseAngleGen + g_light_ambient) * lLightGen;
	//TotalDiffuse = (g_light_diff * in_diff * DiffuseAngleGen + g_light_ambient);

	// スペキュラカラー計算

	// ハーフベクトルの計算
	TempF3 = normalize(V_to_Eye - lLightDir);

	// Temp = pow( max( 0.0f, N * H ), g_mat_power.x )
	Temp = pow(max(0.0f, dot(Normal, TempF3)), g_mat_power.x);

	// スペキュラカラー蓄積値 += Temp * 距離・スポットライトの角度減衰率 * ライトのスペキュラカラー
	TotalSpecular += Temp * lLightGen.x * g_light_specular;

	// スポットライトの処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )



	// 出力カラー計算 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// TotalDiffuse = ライトディフューズカラー蓄積値 + ( マテリアルのアンビエントカラーとグローバルアンビエントカラーを乗算したものとマテリアルエミッシブカラーを加算したもの )
	//TotalDiffuse += g_mat_ambient * g_mat_emissive;

	// SpecularColor = ライトのスペキュラカラー蓄積値 * マテリアルのスペキュラカラー
	SpecularColor = TotalSpecular * g_mat_specular;

	// 出力カラー = TotalDiffuse * テクスチャカラー + SpecularColor
	TextureDiffuseColor = tex2D(Sampler1, in_tex);
	out_color.rgb = TextureDiffuseColor.rgb * TotalDiffuse.rgb + SpecularColor.rgb;

	// アルファ値 = テクスチャアルファ * マテリアルのディフューズアルファ * 不透明度
	out_color.a = TextureDiffuseColor.a * in_diff.a * g_alpha;

	// 出力カラー計算 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )

}