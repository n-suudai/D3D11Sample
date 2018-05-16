
texture2D diffuseTexture : register(t0);
texture2D ambientTexture : register(t1);
texture2D specularTexture : register(t2);
texture2D emissiveTexture : register(t3);


sampler linearSampler : register(s0);


cbuffer CB0 : register(b0)
{
    float4x4 ModelViewProjection;   // �]���s��
    float4   LightDirection;        // ���s��      ���t�s��ŕϊ��ς�
    float4   EyeDirection;          // �����x�N�g�� ���t�s��ŕϊ��ς�

    float4   DiffuseColor;           // �f�B�t���[�Y -> ���_�J���[���g�p����
    float4   AmbientColor;           // �A���r�G���g
    float4   SpecularColor;          // �X�y�L����    w : power
    float4   EmissiveColor;          // �G�~�b�V�u
};


struct PS_INPUT
{
    float4 position : SV_POSITION;  // ���_
    float4 color    : COLOR;        // ���_�F
    float4 normal   : NORMAL;       // �@��
    float2 tex      : TEXCOORD;     // �e�N�X�`�����W
};


float4 main(PS_INPUT ps_input) : SV_TARGET
{
    //float4 diffuseColor  = diffuseTexture.Sample(linearSampler, ps_input.tex);
    //float4 ambientColor  = ambientTexture.Sample(linearSampler, ps_input.tex);
    //float4 specularColor = specularTexture.Sample(linearSampler, ps_input.tex);
    //float4 emissiveColor = emissiveTexture.Sample(linearSampler, ps_input.tex);

    float4 diffuseColor  = diffuseTexture.Sample(linearSampler, ps_input.tex);
    float4 ambientColor  = float4(1.0, 1.0, 1.0, 1.0);
    float4 specularColor = specularTexture.Sample(linearSampler, ps_input.tex);
    float4 emissiveColor = float4(1.0, 1.0, 1.0, 1.0);

    diffuseColor  = diffuseColor  * DiffuseColor;
    ambientColor  = ambientColor  * AmbientColor;
    specularColor = specularColor * SpecularColor;
    emissiveColor = emissiveColor * EmissiveColor;

    float3 light  = normalize(LightDirection.xyz);
    float3 eye    = normalize(EyeDirection.xyz);
    float diffuse = clamp(dot(ps_input.normal.xyz, light), 0.1, 1.0);

    float3 halfLE  = normalize(light + eye);
    float specular = pow(clamp(dot(ps_input.normal.xyz, halfLE), 0.0, 1.0), SpecularColor.w);

    float4 color = ps_input.color * diffuseColor * float4(diffuse, diffuse, diffuse, 1.0);
    color += ambientColor;
    color += float4(specular * specularColor.x, specular * specularColor.y, specular * specularColor.z, 1.0);
    color += emissiveColor;

    return color;
}

