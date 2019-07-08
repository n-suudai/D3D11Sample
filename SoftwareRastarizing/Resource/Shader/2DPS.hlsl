struct PSInput
{
    float4 position : SV_POSITION;
    float2 tex      : TEXCOORD;
};

Texture2D    Texture : register(t0);
SamplerState Sampler : register(s0);


float4 main(PSInput In) : SV_TARGET
{
    return Texture.Sample(Sampler, In.tex);
}

