
texture2D diffuseTexture : register(t0);
texture2D ambientTexture : register(t1);
texture2D specularTexture : register(t2);
texture2D emissiveTexture : register(t3);


sampler linearSampler : register(s0);


cbuffer CB0 : register(b0)
{
    float4x4 ModelViewProjection;   // 転送行列
    float4   LightDirection;        // 平行光      ※逆行列で変換済み
    float4   EyeDirection;          // 視線ベクトル ※逆行列で変換済み

    float4   DiffuseColor;           // ディフューズ -> 頂点カラーを使用する
    float4   AmbientColor;           // アンビエント
    float4   SpecularColor;          // スペキュラ    w : power
    float4   EmissiveColor;          // エミッシブ
};


struct PS_INPUT
{
    float4 position : SV_POSITION;  // 頂点
    float4 color    : COLOR;        // 頂点色
    float4 normal   : NORMAL;       // 法線
    float2 tex      : TEXCOORD;     // テクスチャ座標
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

