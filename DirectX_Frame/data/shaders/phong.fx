// -------------------------------------------------------------
// グローバル変数
// -------------------------------------------------------------
float4x4 World;		// 変換行列
float4x4 View;		// 変換行列
float4x4 Proj;		// 変換行列

float4x4 mWVP;

float4 vLightDir; // ライトの方向
float4 vColor;  // ライト＊メッシュの色
float3 vEyePos;  // カメラの位置（ローカル座標系）

// -------------------------------------------------------------
// 頂点シェーダからピクセルシェーダに渡すデータ
// -------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos   : POSITION;
    float4 Color  : COLOR0;
    float3 N   : TEXCOORD0;
    float3 Eye   : TEXCOORD1;
float2 Tex : TEXCOORD2;
};
//テクスチャ
texture Tex;
sampler Samp = sampler_state
{
 Texture =<Tex>;
 MinFilter =LINEAR;
 MagFilter =LINEAR;
 MipFilter =NONE;

 AddressU =Clamp;
 AddressV =Clamp;
};
// -------------------------------------------------------------
// シーンの描画
// -------------------------------------------------------------
VS_OUTPUT VertexShader_Main(
      float4 Pos    : POSITION,          // ローカル位置座標
      float4 Normal : NORMAL,            // 法線ベクトル
float2 Tex : TEXCOORD
){
 VS_OUTPUT Out = (VS_OUTPUT)0;        // 出力データ
 
 // 座標変換
 //out_pos = mul(in_pos, mul(mul(World, View), Proj));	// 頂点と変換行列を掛ける
 Out.Pos = mul(Pos, mul(mul(World, View), Proj));	// 頂点と変換行列を掛ける
 
 // 拡散光＋環境光
 float amb = -vLightDir.w; // 環境光の強さ
 float3 L = -vLightDir; // ローカル座標系でのライトベクトル
 Out.Color = vColor * max(amb, dot(Normal, -vLightDir));
 
 // 鏡面反射用のベクトル
 Out.N   = Normal.xyz;
 Out.Eye = vEyePos - Pos.xyz;
Out.Tex = Tex;//テクスチャ座標
 
 return Out;
}
// -------------------------------------------------------------
float4 PixelShader_Main(VS_OUTPUT In) : COLOR
{  
 float3 L = -vLightDir.xyz;
 float3 H = normalize(L + normalize(In.Eye)); // ハーフベクトル
 float3 N = normalize(In.N);
 
   // return (In.Color+ pow(max(0,dot(N, H)), 10))*tex2D( Samp, In. Tex);// 拡散光＋環境光
 return In.Color*1.5*tex2D( Samp, In. Tex)+ pow(max(0,dot(N, H)), 40); 
    //+ pow(max(0,dot(N, H)), 10); // 鏡面反射光
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