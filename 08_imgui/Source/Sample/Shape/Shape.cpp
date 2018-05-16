

static constexpr f32 PIH = 1.5707963267948966192313216916398f;
static constexpr f32 PI = 3.1415926535897932384626433832795f;
static constexpr f32 PI2 = 6.2831853071795864769252867665590f;
static constexpr f32 DIV_PI = 0.31830988618379067153776752674503f;
static constexpr f32 DIV_PI2 = 0.15915494309189533576888376337251f;


void Shape::ConstantBuffer::Initialize()
{
    ModelViewProjection = glm::mat4x4(1);
    LightDirection = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    EyeDirection = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    DiffuseColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    AmbientColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
    SpecularColor = glm::vec4(1.0f, 1.0f, 1.0f, 50.0f);
    EmissiveColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
}


Shape::Shape(
    const ComPtr<ID3D11Device>& device,
    const ComPtr<ID3D11DeviceContext> context
)
    : m_Device(device)
    , m_Context(context)
{
    m_ConstantBufferData.Initialize();
}

Shape::~Shape()
{

}

// トーラスとして初期化
void Shape::InitializeAsTorus(u16 row, u16 column, f32 irad, f32 orad, const glm::vec4* pColor)
{
    std::vector<Vertex> vertices;   // 頂点配列
    std::vector<u16>    indices;    // インデックス配列

    // 頂点配列を作成
    for (u16 ix = 0; ix <= row; ++ix)
    {
        f32 r = PI * 2.0f / row * ix;
        f32 rr = std::cosf(r);
        f32 ry = std::sinf(r);

        for (u16 iy = 0; iy <= column; ++iy)
        {
            f32 tr = PI * 2.0f / column * iy;

            Vertex v;
            // 頂点
            v.Position.x = (rr * irad + orad) * std::cosf(tr);
            v.Position.y = ry * irad;
            v.Position.z = (rr * irad + orad) * std::sinf(tr);
            v.Position.w = 1.0f;

            // 頂点色
            if (pColor != nullptr)
            {
                v.Color.x = pColor->x;
                v.Color.y = pColor->y;
                v.Color.z = pColor->z;
                v.Color.w = pColor->w;
            }
            else
            {
                glm::vec3 color = glm::rgbColor(glm::vec3(360.0f / column * iy, 1.0f, 1.0f));
                v.Color.x = color.x;
                v.Color.y = color.y;
                v.Color.z = color.z;
                v.Color.w = 1.0f;
            }

            // 法線
            v.Normal.x = rr * std::cosf(tr);
            v.Normal.y = ry;
            v.Normal.z = rr * std::sinf(tr);
            v.Normal.w = 1.0f;

            // テクスチャ座標
            v.Texture.x = 1.0f / column * iy;
            v.Texture.y = 1.0f / row * ix + 0.5f;
            if (v.Texture.y > 1.0f)
            {
                v.Texture.y -= 1.0f;
            }
            v.Texture.y = 1.0f - v.Texture.y;

            vertices.push_back(v);
        }
    }

    // インデックス配列を作成
    for (u16 ix = 0; ix < row; ++ix)
    {
        for (u16 iy = 0; iy < column; ++iy)
        {
            u16 r = (column + 1) * ix + iy;

            indices.push_back(r);
            indices.push_back(r + column + 1);
            indices.push_back(r + 1);

            indices.push_back(r + column + 1);
            indices.push_back(r + column + 2);
            indices.push_back(r + 1);
        }
    }

    // 初期化
    Initialize(
        vertices,
        indices,
        "Resource\\Shader\\bin\\ShapeVS.cso",
        "Resource\\Shader\\bin\\ShapePS.cso"
    );
}

// 球体として初期化
void Shape::InitializeAsSphere(u16 row, u16 column, f32 rad, const glm::vec4* pColor)
{
    std::vector<Vertex> vertices;   // 頂点配列
    std::vector<u16>    indices;    // インデックス配列

    // 頂点配列を作成
    for (u16 ix = 0; ix <= row; ix++)
    {
        f32 r = PI / row * ix;
        f32 ry = std::cosf(r);
        f32 rr = std::sinf(r);
        for (u16 iy = 0; iy <= column; iy++)
        {
            Vertex v;
            f32 tr = PI * 2 / column * iy;
            
            // 頂点
            v.Position.x = rr * rad * std::cosf(tr);
            v.Position.y = ry * rad;
            v.Position.z = rr * rad * std::sinf(tr);
            v.Position.w = 1.0f;

            // 頂点色
            if (pColor != nullptr)
            {
                v.Color.x = pColor->x;
                v.Color.y = pColor->y;
                v.Color.z = pColor->z;
                v.Color.w = pColor->w;
            }
            else
            {
                glm::vec3 color = glm::rgbColor(glm::vec3(360.0f / row * ix, 1.0f, 1.0f));
                v.Color.x = color.x;
                v.Color.y = color.y;
                v.Color.z = color.z;
                v.Color.w = 1.0f;
            }

            // 法線
            v.Normal.x = rr * std::cosf(tr);
            v.Normal.y = ry;
            v.Normal.z = rr * std::sinf(tr);
            v.Normal.w = 1.0f;

            // テクスチャ座標
            v.Texture.x = 1.0f - 1.0f / column * iy;
            v.Texture.y = 1.0f / row * ix;

            vertices.push_back(v);
        }
    }

    // インデックス配列を作成
    for (u16 ix = 0; ix < row; ix++)
    {
        for (u16 iy = 0; iy < column; iy++)
        {
            u16 r = (column + 1) * ix + iy;
            indices.push_back(r);
            indices.push_back(r + 1);
            indices.push_back(r + column + 2);
            indices.push_back(r);
            indices.push_back(r + column + 2);
            indices.push_back(r + column + 1);
        }
    }

    // 初期化
    Initialize(
        vertices,
        indices,
        "Resource\\Shader\\bin\\ShapeVS.cso",
        "Resource\\Shader\\bin\\ShapePS.cso"
    );
}

void Shape::Update(
    const glm::vec3* pTranslate,
    const glm::vec3* pRotate,
    const glm::vec3* pScale
)
{
    m_ModelMatrix = glm::mat4x4(1.0f);

    if (pTranslate != nullptr)
    {
        m_ModelMatrix = glm::translate(m_ModelMatrix, *pTranslate);
        m_bUpdateContantBuffer = true;
    }

    if (pRotate != nullptr)
    {
        m_ModelMatrix = glm::rotate(m_ModelMatrix, pRotate->x, glm::vec3(1.0f, 0.0f, 0.0f));
        m_ModelMatrix = glm::rotate(m_ModelMatrix, pRotate->y, glm::vec3(0.0f, 1.0f, 0.0f));
        m_ModelMatrix = glm::rotate(m_ModelMatrix, pRotate->z, glm::vec3(0.0f, 0.0f, 1.0f));
        m_bUpdateContantBuffer = true;
    }

    if (pScale != nullptr)
    {
        m_ModelMatrix = glm::scale(m_ModelMatrix, *pScale);
        m_bUpdateContantBuffer = true;
    }

    // 定数バッファデータ更新
    if(m_bUpdateContantBuffer)
    {
        glm::mat4x4 inverseModelMatrix = glm::inverse(m_ModelMatrix);

        m_ConstantBufferData.LightDirection = inverseModelMatrix * m_LightDirection;
        glm::normalize(m_ConstantBufferData.LightDirection);

        m_ConstantBufferData.EyeDirection = inverseModelMatrix * m_EyeDirection;
        glm::normalize(m_ConstantBufferData.EyeDirection);

        m_ConstantBufferData.ModelViewProjection = m_ProjectionMatrix * m_ViewMatrix * m_ModelMatrix;
        m_ConstantBufferData.ModelViewProjection = glm::transpose(m_ConstantBufferData.ModelViewProjection);
    }
}

void Shape::Draw()
{
    // 入力レイアウトを設定
    m_Context->IASetInputLayout(m_InputLayout.Get());

    // 頂点バッファを設定
    {
        constexpr UINT stride = sizeof(Vertex);
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

    // 頂点シェーダーの設定
    m_Context->VSSetShader(m_VertexShader.Get(), nullptr, 0);

    // 頂点シェーダーに定数バッファを設定
    {
        // 定数バッファを更新
        if (m_bUpdateContantBuffer)
        {
            m_Context->UpdateSubresource(
                m_ConstantBuffer.Get(),
                0,
                nullptr,
                &m_ConstantBufferData,
                0,
                0
            );
            m_bUpdateContantBuffer = false;
        }

        ID3D11Buffer* pConstantBuffers[] = {
            m_ConstantBuffer.Get()
        };
        m_Context->VSSetConstantBuffers(0, _countof(pConstantBuffers), pConstantBuffers);
    }

    // ピクセルシェーダーを設定
    m_Context->PSSetShader(m_PixelShader.Get(), nullptr, 0);

    // ピクセルシェーダーに定数バッファを設定
    {
        ID3D11Buffer* pConstantBuffers[] = {
            m_ConstantBuffer.Get()
        };
        m_Context->PSSetConstantBuffers(0, _countof(pConstantBuffers), pConstantBuffers);
    }

    // ピクセルシェーダーにサンプラーステートを設定
    {
        ID3D11SamplerState* pSamplerStates[] = {
            m_SamplerState.Get()
        };
        m_Context->PSSetSamplers(0, _countof(pSamplerStates), pSamplerStates);
    }

    // ラスタライザーステートを設定
    m_Context->RSSetState(m_RasterizerState.Get());

    // ブレンドステートを設定
    m_Context->OMSetBlendState(m_BlendState.Get(), nullptr, 0xFFFFFFFF);

    // テクスチャをバインド
    {
        if (m_DiffuseTexture)
        {
            m_DiffuseTexture->Bind(0);
        }

        if (m_AmbientTexture)
        {
            m_AmbientTexture->Bind(1);
        }

        if (m_SpecularTexture)
        {
            m_SpecularTexture->Bind(2);
        }

        if (m_EmissiveTexture)
        {
            m_EmissiveTexture->Bind(3);
        }
    }

    m_Context->DrawIndexed(m_IndexCount, 0, 0);
}

void Shape::Initialize(
    const std::vector<Vertex>& vertices,
    const std::vector<u16>& indices,
    const std::string& vertexShader,
    const std::string& pixelShader
)
{
    // 頂点バッファ
    Util::CreateBuffer(
        m_Device,
        vertices.data(),
        static_cast<UINT>(vertices.size()) * sizeof(Vertex),
        D3D11_BIND_VERTEX_BUFFER,
        sizeof(Vertex),
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

    // 定数バッファ
    {
        Util::CreateBuffer(
            m_Device,
            &m_ConstantBufferData,
            sizeof(m_ConstantBufferData),
            D3D11_BIND_CONSTANT_BUFFER,
            sizeof(m_ConstantBufferData),
            m_ConstantBuffer
        );

        // 定数バッファを更新
        m_Context->UpdateSubresource(
            m_ConstantBuffer.Get(),
            0,
            nullptr,
            &m_ConstantBufferData,
            0,
            0
        );
    }

    // 頂点シェーダー&入力レイアウト
    Util::CreateVertexShaderAndInputLayout(
        m_Device,
        vertexShader,
        Vertex::pInputElementDescs,
        Vertex::InputElementCount,
        m_VertexShader,
        m_InputLayout
    );

    // ピクセルシェーダー
    Util::CreatePixelShader(
        m_Device,
        pixelShader,
        m_PixelShader
    );

    // ラスタライザーステート
    Util::CreateRasterizerState(
        m_Device,
        D3D11_CULL_BACK,
        FALSE,
        m_RasterizerState
    );

    // ブレンドステート
    Util::CreateBlendState(
        m_Device,
        m_BlendState
    );

    // サンプラーステート
    Util::CreateSamplerState(
        m_Device,
        D3D11_FILTER_ANISOTROPIC,
        D3D11_TEXTURE_ADDRESS_WRAP,
        D3D11_TEXTURE_ADDRESS_WRAP,
        D3D11_TEXTURE_ADDRESS_WRAP,
        m_SamplerState
    );

    // テクスチャ
    {
        m_DiffuseTexture = std::make_unique<Texture>(
            m_Device,
            m_Context
            );
        m_DiffuseTexture->Initialize("..\\Assets\\Image\\metal.png");

        //m_AmbientTexture = std::make_unique<Texture>(
        //    m_Device,
        //    m_Context
        //    );
        //m_AmbientTexture->Initialize("..\\Assets\\Image\\icon.png");

        m_SpecularTexture = std::make_unique<Texture>(
            m_Device,
            m_Context
            );
        m_SpecularTexture->Initialize("..\\Assets\\Image\\metal_specular.png");

        //m_EmissiveTexture = std::make_unique<Texture>(
        //    m_Device,
        //    m_Context
        //    );
        //m_EmissiveTexture->Initialize("..\\Assets\\Image\\icon.png");
    }

    m_ViewMatrix = glm::mat4x4(1);
    m_ProjectionMatrix = glm::mat4x4(1);
    m_ModelMatrix = glm::mat4x4(1);

    m_bUpdateContantBuffer = true;
}

