
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
    void InitializeAsTorus(u16 row, u16 column, f32 irad, f32 orad, const glm::vec4* pColor = nullptr);

    // 球体として初期化
    void InitializeAsSphere(u16 row, u16 column, f32 rad, const glm::vec4* pColor = nullptr);

    void Update(
        const glm::vec3* pTranslate = nullptr,
        const glm::vec3* pRotate = nullptr,
        const glm::vec3* pScale = nullptr
    );

    void Draw();

    //------------------------------
    // 各種設定関数
    //------------------------------

    void SetAmbientColor(const glm::vec4& ambient) { m_ConstantBufferData.AmbientColor = ambient; }

    void SetLightDirection(const glm::vec4& light) { m_LightDirection = light; }

    void SetEyeDirection(const glm::vec4& eye) { m_EyeDirection = eye; }

    void SetViewMatrix(const glm::mat4x4& view) { m_ViewMatrix = view; }

    void SetProjectionMatrix(const glm::mat4x4& projection) { m_ProjectionMatrix = projection; }

protected:
    void Initialize(
        const std::vector<Vertex>& vertices,
        const std::vector<u16>& indices,
        const std::string& vertexShader,
        const std::string& pixelShader
    );

protected:
    glm::vec4   m_LightDirection;
    glm::vec4   m_EyeDirection;
    glm::mat4x4 m_ModelMatrix;
    glm::mat4x4 m_ViewMatrix;
    glm::mat4x4 m_ProjectionMatrix;

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


