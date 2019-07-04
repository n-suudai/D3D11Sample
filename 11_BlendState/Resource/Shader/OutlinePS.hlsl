

struct PS_INPUT
{
    float4 position : SV_POSITION;  // ’¸“_
    float  edge     : COLOR;
};


float4 main(PS_INPUT ps_input) : SV_TARGET
{
    return float4(0,0,0,ps_input.edge);
}

