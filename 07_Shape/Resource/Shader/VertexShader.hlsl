

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
    float4 color : COLOR;
    float4 normal : NORMAL;
    float2 tex : TEXCOORD;
};


struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float4 normal : NORMAL;
    float2 tex : TEXCOORD;
};


VS_OUTPUT main(VS_INPUT vs_input)
{
    VS_OUTPUT vs_output = (VS_OUTPUT)0;

    vs_output.position = mul(vs_input.position, World);
    vs_output.position = mul(vs_output.position, View);
    vs_output.position = mul(vs_output.position, Projection);
    vs_output.tex = vs_input.tex;
    vs_output.color = vs_input.color;
    vs_output.normal = vs_input.normal;

    return vs_output;
}

