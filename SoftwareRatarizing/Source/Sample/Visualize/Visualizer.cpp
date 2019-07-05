

Visualizer::Visualizer(
    const ComPtr<ID3D11Device>& device,
    const ComPtr<ID3D11DeviceContext>& context
)
    : m_Device(device)
    , m_Context(context)
    , m_Size({0})
    , m_IndexCount(0)
{
    Init();
}

void Visualizer::Init()
{
    // 頂点シェーダー & 入力レイアウト
    Util::CreateVertexShaderAndInputLayout(
        m_Device,
        "Resource\\Shader\\bin\\2DVS.cso",
        Vertex2D::pInputElementDescs,
        Vertex2D::InputElementCount,
        m_VertexShader,
        m_InputLayout
    );

    // ピクセルシェーダー
    Util::CreatePixelShader(
        m_Device,
        "Resource\\Shader\\bin\\2DPS.cso",
        m_PixelShader
    );

    // サンプラー
    Util::CreateSamplerState(
        m_Device,
        D3D11_FILTER_ANISOTROPIC,
        D3D11_TEXTURE_ADDRESS_WRAP,
        D3D11_TEXTURE_ADDRESS_WRAP,
        D3D11_TEXTURE_ADDRESS_WRAP,
        m_SamplerState
    );

    const std::vector<Vertex2D> vertices = {
        Vertex2D{ glm::vec2(-1.0f, -1.0f),    glm::vec2(0.0f, 0.0f) },
        Vertex2D{ glm::vec2(1.0f, -1.0f),     glm::vec2(1.0f, 0.0f) },
        Vertex2D{ glm::vec2(1.0f, 1.0f),      glm::vec2(1.0f, 1.0f) },
        Vertex2D{ glm::vec2(-1.0f, 1.0f),     glm::vec2(0.0f, 1.0f) }
    };
    const std::vector<u16> indices = { 0, 1, 2, 0, 2, 3 };

    // 頂点バッファ
    Util::CreateBuffer(
        m_Device,
        vertices.data(),
        static_cast<UINT>(vertices.size()) * sizeof(Vertex2D),
        D3D11_BIND_VERTEX_BUFFER,
        sizeof(Vertex2D),
        m_VertexBuffer
    );

    // インデックスバッファ
    Util::CreateBuffer(
        m_Device,
        indices.data(),
        static_cast<UINT>(indices.size()) * sizeof(u16),
        D3D11_BIND_INDEX_BUFFER,
        sizeof(u16),
        m_IndexBuffer
    );
    m_IndexCount = static_cast<UINT>(indices.size());
}

void Visualizer::Update(const Size2D& size, const u8* data, u32 dataLength)
{
    if (size.width != m_Size.width || size.height != m_Size.height)
    {
        m_Size = size;

        // テクスチャを作り直し
        Util::CreateTextureFromData(
            m_Device,
            DXGI_FORMAT_R8G8B8A8_UNORM,
            m_Size,
            data,
            dataLength,
            m_Texture2D,
            m_ShaderResourceView
        );

        //Util::CreateTextureFromData(
        //    m_Device,
        //    DXGI_FORMAT_R8G8B8A8_UNORM,
        //    m_Size,
        //    data,
        //    dataLength,
        //    m_Texture2D,
        //    m_ShaderResourceView,
        //    D3D11_USAGE_DYNAMIC,
        //    D3D11_CPU_ACCESS_WRITE
        //);
    }
    else
    {
        // テクスチャを更新
        // https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-updatesubresource
        m_Context->UpdateSubresource(
            m_Texture2D.Get(),
            0,
            nullptr,
            data,
            static_cast<UINT>(m_Size.width * 4),
            0
        );
        
        //D3D11_MAPPED_SUBRESOURCE mapped = { 0 };
        //ResultUtil result = m_Context->Map(
        //    m_Texture2D.Get(),
        //    0,
        //    D3D11_MAP_WRITE_DISCARD,
        //    0,
        //    &mapped
        //);

        //if (result)
        //{
        //    // すべての個所を更新するので全コピー
        //    memcpy(mapped.pData, data, dataLength);
        //}

        //m_Context->Unmap(m_Texture2D.Get(), 0);
    }
}

void Visualizer::Render()
{
    if (!m_ShaderResourceView) { return; }

    // 入力レイアウトを設定
    m_Context->IASetInputLayout(m_InputLayout.Get());

    // 頂点バッファを設定
    {
        constexpr UINT stride = sizeof(Vertex2D);
        UINT offset = 0;

        ID3D11Buffer* pVertexBuffers[] = {
            m_VertexBuffer.Get()
        };
        m_Context->IASetVertexBuffers(
            0,
            _countof(pVertexBuffers),
            pVertexBuffers,
            &stride,
            &offset
        );
    }

    // インデックスバッファを設定
    m_Context->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

    // プリミティブトポロジーの設定
    m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // 頂点シェーダーを設定
    m_Context->VSSetShader(m_VertexShader.Get(), nullptr, 0);

    // ピクセルシェーダーを設定
    m_Context->PSSetShader(m_PixelShader.Get(), nullptr, 0);

    // ピクセルシェーダーにサンプラーを設定
    {
        ID3D11SamplerState* pSamplerStates[] = {
            m_SamplerState.Get()
        };
        m_Context->PSSetSamplers(0, _countof(pSamplerStates), pSamplerStates);
    }

    // テクスチャをバインド
    {
        ID3D11ShaderResourceView* pShaderResourceViews[] = {
            m_ShaderResourceView.Get()
        };

        m_Context->PSSetShaderResources(
            0,
            _countof(pShaderResourceViews),
            pShaderResourceViews
        );
    }

    m_Context->DrawIndexed(m_IndexCount, 0, 0);
}

