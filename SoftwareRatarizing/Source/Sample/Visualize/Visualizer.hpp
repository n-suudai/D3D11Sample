#pragma once



class Visualizer
{
public:
    using Vertex2D = Vertex_PositionTexture2D;

    Visualizer(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3D11DeviceContext>& context
    );

    ~Visualizer() = default;

    void Init();

    void Update(const Size2D& size, const u8* data, u32 dataLength);

    void Render();

protected:
    ComPtr<ID3D11Device>         m_Device;
    ComPtr<ID3D11DeviceContext>  m_Context;

    Size2D m_Size;

    ComPtr<ID3D11Texture2D>             m_Texture2D;            // テクスチャ
    ComPtr<ID3D11ShaderResourceView>    m_ShaderResourceView;   // シェーダーリソースビュー

    ComPtr<ID3D11Buffer>        m_VertexBuffer;
    ComPtr<ID3D11Buffer>        m_IndexBuffer;
    UINT                        m_IndexCount;
    ComPtr<ID3D11InputLayout>   m_InputLayout;
    ComPtr<ID3D11VertexShader>  m_VertexShader;
    ComPtr<ID3D11PixelShader>   m_PixelShader;
    ComPtr<ID3D11SamplerState>  m_SamplerState;
};

