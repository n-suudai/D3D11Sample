#pragma once


class SampleApp
{
public:
    SampleApp(IApp* pApp);

    ~SampleApp();

    // 初期化
    bool Init();

    // 解放
    void Term();

    // 更新処理
    void Update();

    // 描画処理
    void Render();

    // リサイズ
    void OnResize(const Size2D& newSize);

    // キー
    void OnKey(KEY_CODE key, bool isDown);

    // マウスボタン
    void OnMouse(const Position2D& position, MOUSE_BUTTON button, bool isDown);

    // マウスホイール
    void OnMouseWheel(const Position2D& position, s32 wheelDelta);


private:
    // バックバッファを作成
    bool CreateBackBuffer(const Size2D& newSize);

    // 深度ステンシルステートを作成
    bool CreateDepthStencilState();

    // エラーメッセージ表示
    void ShowErrorMessage(const ResultUtil& result, const std::string& text);

private:
    IApp * m_pApp;
    UINT m_BufferCount;

    DXGI_FORMAT       m_BufferFormat;
    D3D_FEATURE_LEVEL m_FeatureLevel;
    DXGI_SAMPLE_DESC  m_SampleDesc;

    ComPtr<IDXGIFactory>   m_Factory;
    ComPtr<IDXGISwapChain> m_SwapChain;

    ComPtr<ID3D11Device>           m_Device;
    ComPtr<ID3D11DeviceContext>    m_Context;
    ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
    ComPtr<ID3D11DepthStencilView> m_DepthStencilView;

    ComPtr<ID3D11Texture2D> m_DepthStencilTexture;
    ComPtr<ID3D11DepthStencilState> m_DepthStencilState;


    std::unique_ptr<BitmapFont> m_BitmapFont;
    std::unique_ptr<Shape>      m_Torus;
    std::unique_ptr<Shape>      m_Sphere;
};


