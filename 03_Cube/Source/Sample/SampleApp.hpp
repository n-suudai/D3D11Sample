﻿#pragma once


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

    // シェーダーを作成
    bool CreateShader(const std::string& vertexShader, const std::string& pixelShader);

    // 頂点バッファを作成
    bool CreateVertexBuffer(const void* pVertices, UINT byteWidth);

    // インデックスバッファを作成
    bool CreateIndexBuffer(const void* pIndices, UINT byteWidth);

    // 定数バッファを作成
    bool CreateConstantBuffer(const void* pInitData, UINT byteWidth);

    // ラスタライザーステートを作成
    bool CreateRasterizerState();

    // 深度ステンシルステートを作成
    bool CreateDepthStencilState();

    // ファイルの読み込み（バイナリ）
    bool ReadFile(const std::string& fileName, std::vector<BYTE>& out);

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

    ComPtr<ID3D11Buffer>       m_VertexBuffer;
    ComPtr<ID3D11Buffer>       m_IndexBuffer;
    ComPtr<ID3D11Buffer>       m_ConstantBuffer;
    ComPtr<ID3D11InputLayout>  m_InputLayout;
    ComPtr<ID3D11VertexShader> m_VertexShader;
    ComPtr<ID3D11PixelShader>  m_PixelShader;

    ComPtr<ID3D11RasterizerState>   m_RasterizerState;
    ComPtr<ID3D11DepthStencilState> m_DepthStencilState;
};


