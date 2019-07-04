

cbuffer CB0 : register(b0)
{
    float4x4 Model;                 // モデル転送行列
    float4x4 ModelViewProjection;   // 転送行列
    float4   LightPosition;         // 点光源      ※逆行列で変換済み
    float4   EyePosition;           // 視点        ※逆行列で変換済み

    float4   DiffuseColor;           // ディフューズ
    float4   AmbientColor;           // アンビエント
    float4   SpecularColor;          // スペキュラ    w : power
    float4   EmissiveColor;          // エミッシブ
};


struct VS_INPUT
{
    float4 position : POSITION;     // 頂点
    float4 color    : COLOR;        // 頂点色
    float4 normal   : NORMAL;       // 法線
    float2 tex      : TEXCOORD;     // テクスチャ座標
};


struct VS_OUTPUT
{
    float4 position : SV_POSITION;  // 頂点
    float  edge     : COLOR;
};


VS_OUTPUT main(VS_INPUT vs_input)
{
    VS_OUTPUT vs_output = (VS_OUTPUT)0;

    vs_output.position = mul(vs_input.position, ModelViewProjection);
    vs_output.edge = 1.0;

    return vs_output;
}