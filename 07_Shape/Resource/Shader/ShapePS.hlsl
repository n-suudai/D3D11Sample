

cbuffer CB0 : register(b0)
{
    float4x4 ModelViewProjection;   // 転送行列
    float4   LightDirection;        // 平行光      ※逆行列で変換済み
    float4   EyeDirection;          // 視線ベクトル ※逆行列で変換済み
    float4   AmbientColor;          // 環境光の色
};


struct PS_INPUT
{
    float4 position : SV_POSITION;  // 頂点
    float4 color    : COLOR;        // 頂点色
    float4 normal   : NORMAL;       // 法線
};


float4 main(PS_INPUT ps_input) : SV_TARGET
{
    float3 light = normalize(LightDirection.xyz);
    float diffuse = clamp(dot(ps_input.normal.xyz, light), 0.1, 1.0);
    float4 color = ps_input.color * float4(diffuse, diffuse, diffuse, 1.0);

    return color;
}

