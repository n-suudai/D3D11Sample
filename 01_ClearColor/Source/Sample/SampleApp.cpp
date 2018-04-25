#include "SampleApp.hpp"


// DXGI & D3D11 のライブラリをリンク
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")



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
        DXGI_SAMPLE_DESC sampleDesc;
        ZeroMemory(&sampleDesc, sizeof(sampleDesc));
        for (int i = 1; i <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; i <<= 1)
        {
            UINT Quality;
            if (SUCCEEDED(m_Device->CheckMultisampleQualityLevels(DXGI_FORMAT_D24_UNORM_S8_UINT, i, &Quality)))
            {
                if (0 < Quality)
                {
                    sampleDesc.Count = i;
                    sampleDesc.Quality = Quality - 1;
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
        swapChainDesc.SampleDesc = sampleDesc;
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
    return CreateBackBuffer(clientSize);
}

// 解放
void SampleApp::Term()
{

}

// 更新処理
void SampleApp::Update()
{

}

// 描画処理
void SampleApp::Render()
{
    // 指定色でクリア
    FLOAT clearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
    m_Context->ClearRenderTargetView(m_RenderTargetView.Get(), clearColor);

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

    // レンダーターゲットを設定
    ID3D11RenderTargetView* pRenderTargetViews[] = {
        m_RenderTargetView.Get()
    };
    m_Context->OMSetRenderTargets(_countof(pRenderTargetViews), pRenderTargetViews, nullptr);

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


// エラーメッセージ表示
void SampleApp::ShowErrorMessage(const ResultUtil& result, const std::string& text)
{
    m_pApp->ShowMessageBox(
        result.GetText() + "\n\n" + text,
        "エラー"
    );
}

