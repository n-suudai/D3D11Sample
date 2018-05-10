

cbuffer CB0 : register(b0)
{
    float4x4 ModelViewProjection;   // �]���s��
    float4   LightDirection;        // ���s��      ���t�s��ŕϊ��ς�
    float4   EyeDirection;          // �����x�N�g�� ���t�s��ŕϊ��ς�
    float4   AmbientColor;          // �����̐F
};


struct PS_INPUT
{
    float4 position : SV_POSITION;  // ���_
    float4 color    : COLOR;        // ���_�F
    float4 normal   : NORMAL;       // �@��
};


float4 main(PS_INPUT ps_input) : SV_TARGET
{
    float3 light = normalize(LightDirection.xyz);
    float diffuse = clamp(dot(ps_input.normal.xyz, light), 0.1, 1.0);
    float4 color = ps_input.color * float4(diffuse, diffuse, diffuse, 1.0);

    return color;
}

