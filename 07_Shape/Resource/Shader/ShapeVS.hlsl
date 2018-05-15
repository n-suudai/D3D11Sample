

cbuffer CB0 : register(b0)
{
    float4x4 ModelViewProjection;   // �]���s��
    float4   LightDirection;        // ���s��      ���t�s��ŕϊ��ς�
    float4   EyeDirection;          // �����x�N�g�� ���t�s��ŕϊ��ς�

    float4   DiffuseColor;           // �f�B�t���[�Y
    float4   AmbientColor;           // �A���r�G���g
    float4   SpecularColor;          // �X�y�L����    w : power
    float4   EmissiveColor;          // �G�~�b�V�u
};


struct VS_INPUT
{
    float4 position : POSITION;     // ���_
    float4 color    : COLOR;        // ���_�F
    float4 normal   : NORMAL;       // �@��
    float2 tex      : TEXCOORD;     // �e�N�X�`�����W
};


struct VS_OUTPUT
{
    float4 position : SV_POSITION;  // ���_
    float4 color    : COLOR;        // ���_�F
    float4 normal   : NORMAL;       // �@��
    float2 tex      : TEXCOORD;     // �e�N�X�`�����W
};


VS_OUTPUT main(VS_INPUT vs_input)
{
    VS_OUTPUT vs_output = (VS_OUTPUT)0;

    // ���W�����ϊ����Ă��̂܂ܕԂ�
    vs_output.position = mul(vs_input.position, ModelViewProjection);
    vs_output.color = vs_input.color;
    vs_output.normal = vs_input.normal;
    vs_output.tex = vs_input.tex;

    return vs_output;
}

