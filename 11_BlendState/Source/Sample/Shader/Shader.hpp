#pragma once


class Shader
{
public:
    Shader(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3D11DeviceContext> context
    );

    virtual ~Shader();

    void Initialize(
        const char* pVS,
        const char* pPS,
        const char* pGS
    );

    // シェーダーを適用
    void Set();

    void Reset();

protected:
    ComPtr<ID3D11Device>                m_Device;               // デバイス
    ComPtr<ID3D11DeviceContext>         m_Context;              // デバイスコンテキスト
    ComPtr<ID3D11VertexShader>          m_VertexShader;         // 頂点シェーダー
    ComPtr<ID3D11PixelShader>           m_PixelShader;          // ピクセルシェーダー
    ComPtr<ID3D11GeometryShader>        m_GeometryShader;       // ジオメトリシェーダー
    ComPtr<ID3D11InputLayout>           m_InputLayout;          // 入力レイアウト
};

