struct VSInput
{
    float2 position : POSITION;
    float2 tex      : TEXCOORD;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float2 tex      : TEXCOORD;
};

VSOutput main(VSInput In)
{
    VSOutput Out = (VSOutput)0;
    float4 position = float4(In.position.x, In.position.y, 0, 1);
    Out.position = position;
    Out.tex = In.tex;

    return Out;
}

