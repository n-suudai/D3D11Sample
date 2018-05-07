#include "SampleApp.hpp"


// DXGI & D3D11 のライブラリをリンク
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")


using Vertex = Vertex_PositionColorTexture;


// 定数バッファデータ
struct ConstantBufferData
{
    glm::mat4x4 World;
    glm::mat4x4 View;
    glm::mat4x4 Projection;
};
ConstantBufferData g_ConstantBufferData;


// 頂点バッファデータ
const Vertex g_vertices[] = {
    { { -1.0f,  1.0f, -1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
    { {  1.0f,  1.0f, -1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
    { { -1.0f, -1.0f, -1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
    { { -1.0f, -1.0f, -1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
    { {  1.0f,  1.0f, -1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
    { {  1.0f, -1.0f, -1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },

    { {  1.0f,  1.0f, -1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
    { {  1.0f,  1.0f,  1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
    { {  1.0f, -1.0f, -1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
    { {  1.0f, -1.0f, -1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
    { {  1.0f,  1.0f,  1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
    { {  1.0f, -1.0f,  1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },

    { {  1.0f,  1.0f,  1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
    { { -1.0f,  1.0f,  1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
    { {  1.0f, -1.0f,  1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
    { {  1.0f, -1.0f,  1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
    { { -1.0f,  1.0f,  1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
    { { -1.0f, -1.0f,  1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },

    { { -1.0f,  1.0f,  1.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
    { { -1.0f,  1.0f, -1.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
    { { -1.0f, -1.0f,  1.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
    { { -1.0f, -1.0f,  1.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
    { { -1.0f,  1.0f, -1.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
    { { -1.0f, -1.0f, -1.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },

    { { -1.0f,  1.0f,  1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
    { {  1.0f,  1.0f,  1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
    { { -1.0f,  1.0f, -1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
    { { -1.0f,  1.0f, -1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
    { {  1.0f,  1.0f,  1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
    { {  1.0f,  1.0f, -1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },

    { { -1.0f, -1.0f, -1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
    { {  1.0f, -1.0f, -1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
    { { -1.0f, -1.0f,  1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
    { { -1.0f, -1.0f,  1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
    { {  1.0f, -1.0f, -1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
    { {  1.0f, -1.0f,  1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },
};


// インデックスバッファデータ
const u16 g_indices[] = {
     0,  1,  2,
     3,  4,  5,
     6,  7,  8,
     9, 10, 11,
    12, 13, 14,
    15, 16, 17,
    18, 19, 20,
    21, 22, 23,
    24, 25, 26,
    27, 28, 29,
    30, 31, 32,
    33, 34, 35,
};


SampleApp::SampleApp(IApp* pApp)
    : m_pApp(pApp)
    , m_BufferCount(2)
    , m_BufferFormat(DXGI_FORMAT_R8G8B8A8_UNORM)
    , m_FeatureLevel(D3D_FEATURE_LEVEL_11_1)
{

}

SampleApp::~SampleApp()
{
    Term();
}

// 初期化
bool SampleApp::Init()
{
    ResultUtil result;

    // ウィンドウハンドルを取得
    HWND hWnd = reinterpret_cast<HWND>(m_pApp->GetWindowHandle());

    // 描画領域のサイズを取得
    const Size2D& clientSize = m_pApp->GetClientSize();

    // ファクトリーを生成
    result = CreateDXGIFactory(IID_PPV_ARGS(&m_Factory));
    if (!result)
    {
        ShowErrorMessage(result, "CreateDXGIFactory");
        return false;
    }

    // デバイス&コンテキストを生成
    {
        // BGRAサポートを有効化
        uint32_t createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(DEBUG) || defined(_DEBUG)
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        D3D_FEATURE_LEVEL featureLevels[] = {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
        };

        result = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE, // ハードウェア ドライバー を使用
            nullptr,
            createDeviceFlags,
            featureLevels,
            _countof(featureLevels),
            D3D11_SDK_VERSION,
            &m_Device,
            &m_FeatureLevel,
            &m_Context
        );
        if (!result)
        {
            ShowErrorMessage(result, "D3D11CreateDevice");
            return false;
        }
    }

    // スワップチェインを生成
    {
        // 使用可能なMSAAを取得
        ZeroMemory(&m_SampleDesc, sizeof(m_SampleDesc));
        for (int i = 1; i <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; i <<= 1)
        {
            UINT Quality;
            if (SUCCEEDED(m_Device->CheckMultisampleQualityLevels(DXGI_FORMAT_D24_UNORM_S8_UINT, i, &Quality)))
            {
                if (0 < Quality)
                {
                    m_SampleDesc.Count = i;
                    m_SampleDesc.Quality = Quality - 1;
                }
            }
        }

        // スワップチェインを生成
        DXGI_SWAP_CHAIN_DESC swapChainDesc;
        ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
        swapChainDesc.BufferDesc.Width = static_cast<UINT>(clientSize.width);
        swapChainDesc.BufferDesc.Height = static_cast<UINT>(clientSize.height);
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
        swapChainDesc.BufferDesc.Format = m_BufferFormat;
        swapChainDesc.SampleDesc = m_SampleDesc;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
        swapChainDesc.BufferCount = m_BufferCount;
        swapChainDesc.Windowed = TRUE;
        swapChainDesc.OutputWindow = hWnd;
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        result = m_Factory->CreateSwapChain(
            m_Device.Get(),
            &swapChainDesc,
            &m_SwapChain
        );
        if (!result)
        {
            ShowErrorMessage(result, "m_Factory->CreateSwapChain");
            return false;
        }
    }

    // バックバッファを生成
    if (!CreateBackBuffer(clientSize))
    {
        return false;
    }

    // 頂点バッファを生成
    if (!CreateVertexBuffer(g_vertices, sizeof(g_vertices), sizeof(g_vertices[0])))
    {
        return false;
    }

    // インデックスバッファを生成
    if (!CreateIndexBuffer(g_indices, sizeof(g_indices), sizeof(g_indices[0])))
    {
        return false;
    }

    // 定数バッファを生成
    if (!CreateConstantBuffer(&g_ConstantBufferData, sizeof(g_ConstantBufferData)))
    {
        return false;
    }

    // シェーダーを生成
    if (!CreateShader("Resource\\Shader\\VertexShader.cso", "Resource\\Shader\\PixelShader.cso"))
    {
        return false;
    }

    // ラスタライザーステートを生成
    if (!CreateRasterizerState())
    {
        return false;
    }

    // サンプラーステートを生成
    if (!CreateSamplerState())
    {
        return false;
    }

    if (!CreateTextureFromFile("..\\Assets\\Image\\icon.png"))
    {
        return false;
    }

    // ビットマップフォントを生成
    {
        glm::mat4x4 projection = glm::orthoLH(
            0.0f,
            static_cast<f32>(clientSize.width),
            static_cast<f32>(clientSize.height),
            0.0f,
            0.0f,
            1.0f
        );
        projection = glm::transpose(projection);

        m_BitmapFont = std::make_unique<BitmapFont>(
                m_Device,
                m_Context,
                projection
            );
    }

    m_BitmapFont->Initialize("Meiryo");

    return true;
}

// 解放
void SampleApp::Term()
{

}

// 更新処理
void SampleApp::Update()
{
    m_BitmapFont->Put(glm::vec2(0.5f, 0.5f), "テキスト");

    // ワールド変換行列を設定
    {
        static f32 rot = 0.0f;
        //rot += 0.0001f;

        g_ConstantBufferData.World = glm::rotate(
            glm::mat4x4(1.0f),
            rot,
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        g_ConstantBufferData.World = glm::transpose(g_ConstantBufferData.World);
    }

    // ビュー変換行列を設定
    {
        g_ConstantBufferData.View = glm::lookAtLH(
            glm::vec3(0.0f,  2.8f, 6.0f),
            glm::vec3(0.0f, -0.1f, 0.0f),
            glm::vec3(0.0f,  1.0f, 0.0f)
        );

        g_ConstantBufferData.View = glm::transpose(g_ConstantBufferData.View);
    }

    // プロジェクション行列を設定
    {
        const Size2D& clientSize = m_pApp->GetClientSize();

        g_ConstantBufferData.Projection = glm::perspectiveFovLH(
            glm::radians(45.0f),
            static_cast<f32>(clientSize.width),
            static_cast<f32>(clientSize.height),
            0.01f,
            100.0f
        );

        g_ConstantBufferData.Projection = glm::transpose(g_ConstantBufferData.Projection);
    }
}

// 描画処理
void SampleApp::Render()
{
    // 指定色でクリア
    {
        // red, green, blue, alpha
        FLOAT clearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
        m_Context->ClearRenderTargetView(m_RenderTargetView.Get(), clearColor);
    }

    // 深度ステンシルビューをクリア
    m_Context->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    // 入力レイアウトを設定
    m_Context->IASetInputLayout(m_InputLayout.Get());

    // 入力レイアウトに頂点バッファを設定
    {
        UINT stride = sizeof(g_vertices[0]);
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

    // 入力レイアウトにインデックスバッファを設定
    m_Context->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

    // プリミティブトポロジーの設定
    m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // 頂点シェーダーを設定
    m_Context->VSSetShader(m_VertexShader.Get(), nullptr, 0);

    // 頂点シェーダーに定数バッファを設定
    {
        m_Context->UpdateSubresource(m_ConstantBuffer.Get(), 0, nullptr, &g_ConstantBufferData, 0, 0);

        ID3D11Buffer* pConstantBuffers[] = {
            m_ConstantBuffer.Get()
        };
        m_Context->VSSetConstantBuffers(0, _countof(pConstantBuffers), pConstantBuffers);
    }

    // ピクセルシェーダーを設定
    m_Context->PSSetShader(m_PixelShader.Get(), nullptr, 0);

    // ピクセルシェーダーにサンプラーステートを設定
    {
        ID3D11SamplerState* pSamplers[] = {
            m_SamplerState.Get()
        };
        m_Context->PSSetSamplers(0, _countof(pSamplers), pSamplers);
    }

    // ピクセルシェーダーにシェーダーリソースビューを設定
    {
        ID3D11ShaderResourceView* pShaderResourceViews[] = {
            m_ShaderResourceView.Get()
        };
        m_Context->PSSetShaderResources(0, _countof(pShaderResourceViews), pShaderResourceViews);
    }

    // ラスタライザーステートを設定
    m_Context->RSSetState(m_RasterizerState.Get());

    // ブレンドステートを設定
    m_Context->OMSetBlendState(nullptr, nullptr, 0);

    // 深度ステンシルステートを設定
    m_Context->OMSetDepthStencilState(m_DepthStencilState.Get(), 0);

    // 描画
    m_Context->DrawIndexed(_countof(g_indices), 0, 0);


    // ビットマップフォント描画
    m_BitmapFont->Flush();


    // 結果をウインドウに反映
    ResultUtil result = m_SwapChain->Present(0, 0);
    if (!result)
    {
        ShowErrorMessage(result, "m_SwapChain->Present");
    }
}

// リサイズ
void SampleApp::OnResize(const Size2D& newSize)
{
    // バックバッファを再生成
    CreateBackBuffer(newSize);
}

// キー
void SampleApp::OnKey(KEY_CODE key, bool isDown)
{
    if (key == KEY_CODE_ESCAPE && isDown)
    {
        m_pApp->PostQuit();
    }
}

// マウスボタン
void SampleApp::OnMouse(const Position2D& position, MOUSE_BUTTON button, bool isDown)
{
    position; button; isDown;
}

// マウスホイール
void SampleApp::OnMouseWheel(const Position2D& position, s32 wheelDelta)
{
    position; wheelDelta;
}

// バックバッファを作成
bool SampleApp::CreateBackBuffer(const Size2D& newSize)
{
    ResultUtil result;

    // バックバッファを破棄
    m_Context->OMSetRenderTargets(0, nullptr, nullptr);
    m_RenderTargetView.Reset();

    // バッファのサイズを変更
    result = m_SwapChain->ResizeBuffers(
        m_BufferCount,
        static_cast<UINT>(newSize.width),
        static_cast<UINT>(newSize.height),
        m_BufferFormat,
        DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
    );
    if (!result)
    {
        ShowErrorMessage(result, "m_SwapChain->ResizeBuffers");
        return false;
    }

    // レンダーターゲットを生成
    {
        ComPtr<ID3D11Texture2D> backBuffer;

        result = m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
        if (!result)
        {
            ShowErrorMessage(result, "m_SwapChain->GetBuffer");
            return false;
        }

        result = m_Device->CreateRenderTargetView(
            backBuffer.Get(),
            nullptr,
            &m_RenderTargetView
        );
        if (!result)
        {
            ShowErrorMessage(result, "m_Device->CreateRenderTargetView");
            return false;
        }
    }

    // 深度ステンシルビューを生成
    {
        DXGI_FORMAT depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
        DXGI_FORMAT textureFormat = depthStencilFormat;
        DXGI_FORMAT resourceFormat = depthStencilFormat;

        // テクスチャとシェーダリソースビューのフォーマットを適切なものに変更.
        switch (depthStencilFormat)
        {
        case DXGI_FORMAT_D16_UNORM:
        {
            textureFormat = DXGI_FORMAT_R16_TYPELESS;
            resourceFormat = DXGI_FORMAT_R16_UNORM;
        }
        break;

        case DXGI_FORMAT_D24_UNORM_S8_UINT:
        {
            textureFormat = DXGI_FORMAT_R24G8_TYPELESS;
            resourceFormat = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
        }
        break;

        case DXGI_FORMAT_D32_FLOAT:
        {
            textureFormat = DXGI_FORMAT_R32_TYPELESS;
            resourceFormat = DXGI_FORMAT_R32_FLOAT;
        }
        break;

        case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
        {
            textureFormat = DXGI_FORMAT_R32G8X24_TYPELESS;
            resourceFormat = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
        }
        break;
        }

        // 深度ステンシルテクスチャを生成
        {
            CD3D11_TEXTURE2D_DESC texture2DDesc(
                textureFormat,
                static_cast<UINT>(newSize.width),
                static_cast<UINT>(newSize.height),
                1,
                1,
                D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE,
                D3D11_USAGE_DEFAULT,
                0,
                m_SampleDesc.Count,
                m_SampleDesc.Quality,
                0
            );

            result = m_Device->CreateTexture2D(
                &texture2DDesc,
                nullptr,
                &m_DepthStencilTexture
            );
            if (!result)
            {
                ShowErrorMessage(result, "m_Device->CreateTexture2D");
                return false;
            }
        }

        // 深度ステンシルビューを生成
        {
            D3D11_DSV_DIMENSION dimension = D3D11_DSV_DIMENSION_TEXTURE2D;
            if (m_SampleDesc.Count > 0)
            {
                dimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
            }

            CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(
                dimension,
                depthStencilFormat
            );

            result = m_Device->CreateDepthStencilView(
                m_DepthStencilTexture.Get(),
                &depthStencilViewDesc,
                &m_DepthStencilView
            );
            if (!result)
            {
                ShowErrorMessage(result, "m_Device->CreateDepthStencilView");
                return false;
            }
        }
    }

    // レンダーターゲットを設定
    ID3D11RenderTargetView* pRenderTargetViews[] = {
        m_RenderTargetView.Get()
    };
    m_Context->OMSetRenderTargets(_countof(pRenderTargetViews), pRenderTargetViews, m_DepthStencilView.Get());

    // ビューポートを設定
    D3D11_VIEWPORT viewport;
    viewport.Width = static_cast<FLOAT>(newSize.width);
    viewport.Height = static_cast<FLOAT>(newSize.height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    m_Context->RSSetViewports(1, &viewport);

    return true;
}

// シェーダーを作成
bool SampleApp::CreateShader(const std::string& vertexShader, const std::string& pixelShader)
{
    bool result = Util::CreateVertexShaderAndInputLayout(
        m_Device,
        vertexShader,
        Vertex::pInputElementDescs,
        Vertex::InputElementCount,
        m_VertexShader,
        m_InputLayout
    );
    if (!result) { return false; }

    return Util::CreatePixelShader(
        m_Device,
        pixelShader,
        m_PixelShader
    );
}

// 頂点バッファを作成
bool SampleApp::CreateVertexBuffer(const void* pVertices, UINT byteWidth, UINT stride)
{
    return Util::CreateBuffer(
        m_Device,
        pVertices,
        byteWidth,
        D3D11_BIND_VERTEX_BUFFER,
        stride,
        m_VertexBuffer
    );
}

// インデックスバッファを作成
bool SampleApp::CreateIndexBuffer(const void* pIndices, UINT byteWidth, UINT stride)
{
    return Util::CreateBuffer(
        m_Device,
        pIndices,
        byteWidth,
        D3D11_BIND_INDEX_BUFFER,
        stride,
        m_IndexBuffer
    );
}

// 定数バッファを作成
bool SampleApp::CreateConstantBuffer(const void* pInitData, UINT byteWidth)
{
    return Util::CreateBuffer(
        m_Device,
        pInitData,
        byteWidth,
        D3D11_BIND_CONSTANT_BUFFER,
        byteWidth,
        m_ConstantBuffer
    );
}

// ラスタライザーステートを作成
bool SampleApp::CreateRasterizerState()
{
    return Util::CreateRasterizerState(
        m_Device,
        D3D11_CULL_BACK,
        TRUE,
        m_RasterizerState
    );
}

// 深度ステンシルステートを作成
bool SampleApp::CreateDepthStencilState()
{
    // 深度ステンシルステートを作成する
    CD3D11_DEPTH_STENCIL_DESC depthStencilDesc(D3D11_DEFAULT);
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;

    ResultUtil result = m_Device->CreateDepthStencilState(
        &depthStencilDesc,
        &m_DepthStencilState
    );
    if (!result)
    {
        ShowErrorMessage(result, "m_Device->CreateDepthStencilState");
        return false;
    }
    return true;
}

// サンプラーステートを作成
bool SampleApp::CreateSamplerState()
{
    return Util::CreateSamplerState(
        m_Device,
        D3D11_FILTER_ANISOTROPIC,
        D3D11_TEXTURE_ADDRESS_WRAP,
        D3D11_TEXTURE_ADDRESS_WRAP,
        D3D11_TEXTURE_ADDRESS_WRAP,
        m_SamplerState
    );
}

// テクスチャを生成
bool SampleApp::CreateTextureFromFile(const std::string& fileName)
{
    ResultUtil result = Util::CreateTextureFromFile(
        m_Device,
        fileName,
        m_Texture2D,
        m_ShaderResourceView
    );
    if (!result)
    {
        ShowErrorMessage(result, "Util::CreateTextureFromFile");
        return false;
    }

    return true;
}

// ファイルの読み込み（バイナリ）
bool SampleApp::ReadFile(const std::string& fileName, std::vector<BYTE>& out)
{
    out.clear();

    ResultUtil result = Util::ReadFile(fileName, out);

    if (!result)
    {
        ShowErrorMessage(result, "Util::ReadFile");
        return false;
    }

    return true;
}

// PNG ファイルの読み込み
bool SampleApp::LoadPng(const std::string& fileName, std::vector<BYTE>& image, DXGI_FORMAT& format, Size2D& size)
{
    ResultUtil result = Util::LoadPng(fileName, image, format, size);
    if (!result)
    {
        ShowErrorMessage(result, "Util::LoadPng");
        return false;
    }

    return true;
}

// エラーメッセージ表示
void SampleApp::ShowErrorMessage(const ResultUtil& result, const std::string& text)
{
    m_pApp->ShowMessageBox(
        result.GetText() + "\n\n" + text,
        "エラー"
    );
}

