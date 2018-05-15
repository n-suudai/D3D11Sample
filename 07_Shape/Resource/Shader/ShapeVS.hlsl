

cbuffer CB0 : register(b0)
{
    float4x4 ModelViewProjection;   // 転送行列
    float4   LightDirection;        // 平行光      ※逆行列で変換済み
    float4   EyeDirection;          // 視線ベクトル ※逆行列で変換済み

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
    float4 color    : COLOR;        // 頂点色
    float4 normal   : NORMAL;       // 法線
    float2 tex      : TEXCOORD;     // テクスチャ座標
};


VS_OUTPUT main(VS_INPUT vs_input)
{
    VS_OUTPUT vs_output = (VS_OUTPUT)0;

    // 座標だけ変換してそのまま返す
    vs_output.position = mul(vs_input.position, ModelViewProjection);
    vs_output.color = vs_input.color;
    vs_output.normal = vs_input.normal;
    vs_output.tex = vs_input.tex;

    return vs_output;
}

