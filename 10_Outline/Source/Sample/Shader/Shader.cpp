#include "Shader.hpp"

Shader::Shader(
    const ComPtr<ID3D11Device>& device,
    const ComPtr<ID3D11DeviceContext> context
)
    : m_Device(device)
    , m_Context(context)
{

}

Shader::~Shader()
{

}


void Shader::Initialize(
    const char* pVS,
    const char* pPS,
    const char* pGS
)
{
    // 頂点シェーダー&入力レイアウト
    if (pVS != nullptr)
    {
        Util::CreateVertexShaderAndInputLayout(
            m_Device,
            pVS,
            Shape::Vertex::pInputElementDescs,
            Shape::Vertex::InputElementCount,
            m_VertexShader,
            m_InputLayout
        );
    }

    // ピクセルシェーダー
    if (pPS != nullptr)
    {
        Util::CreatePixelShader(
            m_Device,
            pPS,
            m_PixelShader
        );
    }

    // ジオメトリシェーダー
    if (pGS != nullptr)
    {
        Util::CreateGeometryShader(
            m_Device,
            pGS,
            m_GeometryShader
        );
    }
}

// シェーダーを適用
void Shader::Set()
{
    // 入力レイアウトを設定
    if (m_InputLayout)
    {
        m_Context->IASetInputLayout(m_InputLayout.Get());
    }

    // 頂点シェーダーの設定
    if (m_VertexShader)
    {
        m_Context->VSSetShader(m_VertexShader.Get(), nullptr, 0);
    }

    // ジオメトリシェーダーの設定
    if (m_GeometryShader)
    {
        m_Context->GSSetShader(m_GeometryShader.Get(), nullptr, 0);
    }

    // ピクセルシェーダーの設定
    if (m_PixelShader)
    {
        m_Context->PSSetShader(m_PixelShader.Get(), nullptr, 0);
    }
}

// シェーダーを適用解除
void Shader::Reset()
{
    // 入力レイアウトを設定
    m_Context->IASetInputLayout(nullptr);

    // 頂点シェーダーの設定
    m_Context->VSSetShader(nullptr, nullptr, 0);

    // ジオメトリシェーダーの設定
    m_Context->GSSetShader(nullptr, nullptr, 0);

    // ピクセルシェーダーの設定
    m_Context->PSSetShader(nullptr, nullptr, 0);
}

