

cbuffer CB0 : register(b0)
{
    float4x4 ModelViewProjection;   // �]���s��
    float4   LightDirection;        // ���s��      ���t�s��ŕϊ��ς�
    float4   EyeDirection;          // �����x�N�g�� ���t�s��ŕϊ��ς�
    float4   AmbientColor;          // �����̐F
};


struct VS_INPUT
{
    float4 position : POSITION;     // ���_
    float4 color    : COLOR;        // ���_�F
    float4 normal   : NORMAL;       // �@��
};


struct VS_OUTPUT
{
    float4 position : SV_POSITION;  // ���_
    float4 color    : COLOR;        // ���_�F
    float4 normal   : NORMAL;       // �@��
};


VS_OUTPUT main(VS_INPUT vs_input)
{
    VS_OUTPUT vs_output = (VS_OUTPUT)0;

    // ���W�����ϊ����Ă��̂܂ܕԂ�
    vs_output.position = mul(vs_input.position, ModelViewProjection);
    vs_output.color = vs_input.color;
    vs_output.normal = vs_input.normal;

    return vs_output;
}

