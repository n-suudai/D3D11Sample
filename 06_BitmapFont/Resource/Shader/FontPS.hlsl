

Texture2DArray<float4> diffuseTextures : register(t0);
SamplerState linearSampler : register(s0);


struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
    uint   page : PAGE;
};


float4 main(PS_INPUT ps_input) : SV_TARGET
{
    float3 tex = float3(ps_input.tex, ps_input.page);
    float4 texColor = diffuseTextures.Sample(linearSampler, tex);
    return ps_input.color * texColor;
}

