

cbuffer CB0 : register(b0)
{
    float4x4 World;
    float4x4 View;
    float4x4 Projection;
    float4x4 WorldInverse;
    float4   LightDirection;
};


struct VS_INPUT
{
    float4 position : POSITION0;
    float4 color : COLOR0;
    float4 normal : NORMAL0;
    float2 tex : TEXCOORD0;
};


struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD0;
};


VS_OUTPUT main(VS_INPUT vs_input)
{
    VS_OUTPUT vs_output = (VS_OUTPUT)0;

    vs_output.position = mul(vs_input.position, World);
    vs_output.position = mul(vs_output.position, View);
    vs_output.position = mul(vs_output.position, Projection);
    vs_output.tex = vs_input.tex;

    float3  lightInverse = normalize(mul(float4(LightDirection.xyz, 0.0), WorldInverse)).xyz;
    float diffuse = clamp(dot(vs_input.normal.xyz, lightInverse), 0.1, 1.0);
    vs_output.color = vs_input.color * float4(diffuse, diffuse, diffuse, 1.0);

    return vs_output;
}

