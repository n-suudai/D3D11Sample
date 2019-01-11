

Texture::Texture(
    const ComPtr<ID3D11Device>& device,
    const ComPtr<ID3D11DeviceContext> context
)
    : m_Device(device)
    , m_Context(context)
{

}

Texture::~Texture()
{

}

void Texture::Initialize(const std::string& fileName)
{
    Util::CreateTextureFromFile(
        m_Device,
        fileName,
        m_Texture2D,
        m_ShaderResourceView
    );
}

void Texture::Initialize(const unsigned char* pData, size_t length)
{
    Util::CreateTextureFromData(
        m_Device,
        (unsigned char*)pData,
        length,
        m_Texture2D,
        m_ShaderResourceView
    );
}

void Texture::Bind(u32 iSlot)
{
    ID3D11ShaderResourceView* pShaderResourceViews[] = {
        m_ShaderResourceView.Get()
    };

    m_Context->PSSetShaderResources(
        static_cast<UINT>(iSlot),
        _countof(pShaderResourceViews),
        pShaderResourceViews
    );
}


