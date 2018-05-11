
#pragma once





class Shape
{
public:
    struct ConstantBuffer
    {
        glm::mat4x4 ModelViewProjection;    // 転送行列
        glm::vec4   LightDirection;         // 平行光      ※逆行列で変換済み
        glm::vec4   EyeDirection;           // 視線ベクトル ※逆行列で変換済み
        glm::vec4   AmbientColor;           // 環境光の色
    };

    using Vertex = Vertex_PositionColorNormal;

public:
    Shape(
        const ComPtr<ID3D11Device>& device,
        const ComPtr<ID3D11DeviceContext> context
    );

    ~Shape();

    // トーラスとして初期化
    void InitializeAsTorus(u16 row, u16 column, f32 irad, f32 orad);

    ConstantBuffer& GetConstantBuffer() { return m_ConstantBufferData; }

    void Draw();

protected:
    void Initialize(
        const std::vector<Vertex>& vertices,
        const std::vector<u16>& indices,
        const std::string& vertexShader,
        const std::string& pixelShader
    );

protected:
    UINT                                m_IndexCount;           // インデックス数
    ConstantBuffer                      m_ConstantBufferData;   // 定数バッファ用データ
    ComPtr<ID3D11Device>                m_Device;               // デバイス
    ComPtr<ID3D11DeviceContext>         m_Context;              // デバイスコンテキスト
    ComPtr<ID3D11Buffer>                m_VertexBuffer;         // 頂点バッファ
    ComPtr<ID3D11Buffer>                m_IndexBuffer;          // インデックスバッファ
    ComPtr<ID3D11Buffer>                m_ConstantBuffer;       // 定数バッファ
    ComPtr<ID3D11VertexShader>          m_VertexShader;         // 頂点シェーダー
    ComPtr<ID3D11PixelShader>           m_PixelShader;          // ピクセルシェーダー
    ComPtr<ID3D11InputLayout>           m_InputLayout;          // 入力レイアウト
    ComPtr<ID3D11RasterizerState>       m_RasterizerState;      // ラスタライザーステート
    ComPtr<ID3D11BlendState>            m_BlendState;           // ブレンドステート
};


