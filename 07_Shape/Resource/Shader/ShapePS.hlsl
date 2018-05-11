

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
    float3 eye = normalize(EyeDirection.xyz);
    float diffuse = clamp(dot(ps_input.normal.xyz, light), 0.1, 1.0);
    float3 halfLE = normalize(light + eye);
    float specular = pow(clamp(dot(ps_input.normal.xyz, halfLE), 0.0, 1.0), 50.0);
    float4 color = ps_input.color * float4(diffuse, diffuse, diffuse, 1.0);
    color += AmbientColor;
    color += float4(specular, specular, specular, 1.0);
    return color;
}
