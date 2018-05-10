

cbuffer CB0 : register(b0)
{
    float4x4 World;
    float4x4 View;
    float4x4 Projection;
    float4x4 WorldInverse;
    float4   LightDirection;
};


Texture2D diffuseTexture : register(t0);
SamplerState linearSampler : register(s0);


struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float4 normal : NORMAL;
    float2 tex : TEXCOORD0;
};


float4 main(PS_INPUT ps_input) : SV_TARGET
{
    float3  lightInverse = normalize(mul(float4(LightDirection.xyz, 0.0), WorldInverse)).xyz;
    float diffuse = clamp(dot(ps_input.normal.xyz, lightInverse), 0.1, 1.0);
    float4 color = ps_input.color * float4(diffuse, diffuse, diffuse, 1.0);

    float4 texColor = diffuseTexture.Sample(linearSampler, ps_input.tex);
    return color * texColor;
}

