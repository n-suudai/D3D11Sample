#include "SampleApp.hpp"


// DXGI & D3D11 のライブラリをリンク
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "dxguid.lib")



SampleApp::SampleApp(IApp* pApp)
    : m_pApp(pApp)
    , m_BufferCount(2)
    , m_BufferFormat(DXGI_FORMAT_R8G8B8A8_UNORM)
    , m_FeatureLevel(D3D_FEATURE_LEVEL_11_1)
    , m_ImGuiActive(true)
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

    // imgui 初期化
    if (!InitializeImGui())
    {
        return false;
    }

    m_Scene = std::make_unique<Scene>();
    result = m_Scene->Load("..\\Assets\\Model\\scene\\scene.obj");
    if (!result)
    {
        ShowErrorMessage(result, "SceneLoading");
        return false;
    }

    m_Visualizer = std::make_unique<Visualizer>(
        m_Device,
        m_Context
        );

    m_ClientSize = clientSize;
    m_Framebuffer.Clear(m_ClientSize);

    return true;
}

// 解放
void SampleApp::Term()
{
    // imgui 解放
    TerminateImGui();
}


struct VaryingVertex
{
    glm::vec4 position;
    glm::vec3 normal;
};
using VertexShaderFunc = std::function<VaryingVertex(const Scene::Vertex& v)>;
using FragmentShaderFunc = std::function<glm::vec4(const glm::vec3& n, const glm::vec3& p_ndc)>;

void Rasterize(
    const Scene* pScene,
    Framebuffer& framebuffer,
    bool wireframe,
    bool cullbackface,
    const VertexShaderFunc& vertexShader,
    const FragmentShaderFunc& fragmentShader
)
{
    // Bresenham's line algorithm
    const auto rasterLine = [&](const glm::vec2& p1, const glm::vec2& p2) {
        int x1 = int(p1.x);
        int y1 = int(p1.y);
        int x2 = int(p2.x);
        int y2 = int(p2.y);
        bool trans = false;
        if (abs(x2 - x1) < abs(y2 - y1)) {
            std::swap(x1, y1);
            std::swap(x2, y2);
            trans = true;
        }
        if (x1 > x2) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
        const int dx = x2 - x1;
        const int dy = y2 - y1;
        const int delta = abs(dy) * 2;
        const int yd = dy > 0 ? 1 : -1;
        int error = 0;
        int y = y1;
        for (int x = x1; x <= x2; x++) {
            framebuffer.SetPixel(trans ? y : x, trans ? x : y, glm::vec4(1));
            error += delta;
            if (error > dx) {
                y += yd;
                error -= dx * 2;
            }
        }
    };

    // ビューポート変換
    const auto viewportTrans = [&](const glm::vec3& p) -> glm::vec2 {
        return glm::vec2(
            (p.x + 1.f) * .5f * framebuffer.Size.width,
            (p.y + 1.f) * .5f * framebuffer.Size.height
        );
    };

    // Rasterize a triangle
    const auto rasterTriangle = [&](const VaryingVertex& v1, const VaryingVertex& v2, const VaryingVertex& v3) {
        // Clip space -> NDC -> Screen space
        const auto p1_ndc = glm::vec3(v1.position) / v1.position.w;
        const auto p2_ndc = glm::vec3(v2.position) / v2.position.w;
        const auto p3_ndc = glm::vec3(v3.position) / v3.position.w;
        const auto p1 = viewportTrans(p1_ndc);
        const auto p2 = viewportTrans(p2_ndc);
        const auto p3 = viewportTrans(p3_ndc);

        // Wireframe?
        if (wireframe) {
            rasterLine(p1, p2);
            rasterLine(p2, p3);
            rasterLine(p3, p1);
            return;
        }

        // Bounding box in screen coordinates
        glm::vec2 min(Inf);
        glm::vec2 max(-Inf);
        const auto merge = [&](const glm::vec2& p) {
            min = glm::min(min, p);
            max = glm::max(max, p);
        };
        merge(p1);
        merge(p2);
        merge(p3);
        min = glm::max(min, glm::vec2(0));
        max = glm::min(max, glm::vec2(framebuffer.Size.width - 1, framebuffer.Size.height - 1));

        // Edge function (CCW)
        const auto edgeFunc = [](const glm::vec2& a, const glm::vec2& b, const glm::vec2& c) {
            const auto d1 = b - a;
            const auto d2 = c - a;
            return d1.x*d2.y - d1.y*d2.x;
        };

        // Check inside/outside tests for each pixel
        const auto denom = edgeFunc(p1, p2, p3);
        const bool back = denom < 0;
        if (back && cullbackface) {
            return;
        }
        for (int y = int(min.y); y <= int(max.y); y++) {
            for (int x = int(min.x); x <= int(max.x); x++) {
                const auto p = glm::vec2(x, y) + 0.5f;
                auto b1 = edgeFunc(p2, p3, p);
                auto b2 = edgeFunc(p3, p1, p);
                auto b3 = edgeFunc(p1, p2, p);
                const bool inside = (b1 > 0 && b2 > 0 && b3 > 0) || (b1 < 0 && b2 < 0 && b3 < 0);
                if (!inside) {
                    continue;
                }
                b1 /= denom;
                b2 /= denom;
                b3 /= denom;
                const auto p_ndc = b1 * p1_ndc + b2 * p2_ndc + b3 * p3_ndc;
                if (framebuffer.DepthBuffer[y*framebuffer.Size.width + x] < p_ndc.z) {
                    continue;
                }
                framebuffer.DepthBuffer[y*framebuffer.Size.width + x] = p_ndc.z;
                const auto n = glm::normalize(b1 / v1.position.w*v1.normal + b2 / v2.position.w*v2.normal + b3 / v3.position.w*v3.normal);
                framebuffer.SetPixel(x, y, fragmentShader(back ? -n : n, p_ndc));
            }
        }
    };

    // 三角形のクリッピング処理を行う（クリップ座標系）
    // デバイス座標系でクリッピングを行うとカメラ後方の投影が残ってしまう
    // Sutherland-Hodgeman
    // Clip triangle
    const auto clipTriangle = [&](
        const VaryingVertex& p1,
        const VaryingVertex& p2,
        const VaryingVertex& p3,
        const std::function<void(
            const VaryingVertex& v1_clipped,
            const VaryingVertex& v2_clipped,
            const VaryingVertex& v3_clipped)>& processClippedTriangle
        ) {
            // Polygon as a vector of vertices in CCW order
            static std::vector<VaryingVertex> poly;
            poly.clear();
            poly.insert(poly.end(), { p1, p2, p3 });

            // Perform clipping
            const glm::vec4 clip_plane_ns[] = {
                glm::vec4(1, 0, 0, 1),    // w=x
                glm::vec4(-1, 0, 0, 1),    // w=-x
                glm::vec4(0, 1, 0, 1),    // w=y
                glm::vec4(0,-1, 0, 1),    // w=-y
                glm::vec4(0, 0, 1, 1),    // w=z
                glm::vec4(0, 0,-1, 1),    // w=-z
            };
            for (const auto& clip_plane_n : clip_plane_ns) {
                static std::vector<VaryingVertex> outpoly;
                outpoly.clear();
                for (int i = 0; i < int(poly.size()); i++) {
                    // Current edge
                    const auto& v1 = poly[i];
                    const auto& v2 = poly[(i + 1) % poly.size()];

                    // Signed distance
                    const auto d1 = glm::dot(v1.position, clip_plane_n);
                    const auto d2 = glm::dot(v2.position, clip_plane_n);

                    // Calculate intersection between a segment and a clip plane
                    const auto intersect = [&]() -> VaryingVertex {
                        const auto a = d1 / (d1 - d2);
                        return {
                            (1.f - a)*v1.position + a * v2.position,
                            glm::normalize((1.f - a)*v1.normal + a * v2.normal)
                        };
                    };
                    if (d1 > 0) {
                        if (d2 > 0) {
                            // Both inside
                            outpoly.push_back(v2);
                        }
                        else {
                            // p1: indide, p2: outside
                            outpoly.push_back(intersect());
                        }
                    }
                    else if (d2 > 0) {
                        // p1: outside, p2: inside
                        outpoly.push_back(intersect());
                        outpoly.push_back(v2);
                    }
                }

                poly = outpoly;
            }

            // Triangulate the polygon
            if (poly.empty()) {
                return;
            }
            const auto& vt1 = poly[0];
            for (int i = 1; i < int(poly.size()) - 1; i++) {
                const auto& vt2 = poly[i];
                const auto& vt3 = poly[(i + 1) % poly.size()];
                processClippedTriangle(vt1, vt2, vt3);
            }
    };

    pScene->ForeachTriangles(
        [&](const Scene::Triangle& triangle)
        {
            const auto p1 = vertexShader(triangle.Vertex1);
            const auto p2 = vertexShader(triangle.Vertex2);
            const auto p3 = vertexShader(triangle.Vertex3);

            clipTriangle(
                p1, p2, p3,
                rasterTriangle
            );
        }
    );
}




// 更新処理
void SampleApp::Update()
{
    static glm::vec3 translate = glm::vec3(0.0f);
    static glm::vec3 rotate = glm::vec3(0.0f);
    static glm::vec3 scale = glm::vec3(0.3f);

    static glm::vec3 lightdir = glm::normalize(glm::vec3(0.5, 0.8, 1));

    enum RenderMode {
        eWireframe,
        eNormal,
        eShaded,
    };

    static float fov = 35.0f;
    static float znear = 0.1f;
    static float zfar = 10.0f;
    static int mode = eShaded;
    static bool cullbackface = false;
    static bool animate = false;


    if (m_ImGuiActive)
    {
        ImGui::SetNextWindowSize(ImVec2(320, 120), ImGuiSetCond_Once);
        ImGui::Begin("SoftwareRastarizing");

        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("Framebuffer size: (%d, %d)", m_Framebuffer.Size.width, m_Framebuffer.Size.height);

        ImGui::Separator();

        ImGui::Text("Translate");
        ImGui::PushItemWidth(60.0f);
        ImGui::PushID("Translate");
        ImGui::DragFloat("X", &translate.x, 0.1f); ImGui::SameLine();
        ImGui::DragFloat("Y", &translate.y, 0.1f); ImGui::SameLine();
        ImGui::DragFloat("Z", &translate.z, 0.1f);
        ImGui::PopID();
        ImGui::PopItemWidth();

        ImGui::Text("Rotate");
        ImGui::PushItemWidth(60.0f);
        ImGui::PushID("Rotate");
        ImGui::DragFloat("X", &rotate.x, 0.1f); ImGui::SameLine();
        ImGui::DragFloat("Y", &rotate.y, 0.1f); ImGui::SameLine();
        ImGui::DragFloat("Z", &rotate.z, 0.1f);
        ImGui::PopID();
        ImGui::PopItemWidth();

        ImGui::Text("Scale");
        ImGui::PushItemWidth(60.0f);
        ImGui::PushID("Scale");
        ImGui::DragFloat("X", &scale.x, 0.1f); ImGui::SameLine();
        ImGui::DragFloat("Y", &scale.y, 0.1f); ImGui::SameLine();
        ImGui::DragFloat("Z", &scale.z, 0.1f);
        ImGui::PopID();
        ImGui::PopItemWidth();

        ImGui::Separator();

        ImGui::RadioButton("Wireframe", &mode, eWireframe); ImGui::SameLine();
        ImGui::RadioButton("Normal",    &mode, eNormal);    ImGui::SameLine();
        ImGui::RadioButton("Shaded",    &mode, eShaded);

        ImGui::Separator();

        ImGui::Checkbox("Enable animation", &animate);

        ImGui::Separator();

        ImGui::Checkbox("Enable backface culling", &cullbackface);

        ImGui::Separator();

        ImGui::DragFloat("Fov", &fov, 0.1f);
        ImGui::DragFloat("ZNear", &znear, 0.1f);
        ImGui::DragFloat("ZFar", &zfar, 0.1f);

        ImGui::End();
    }

    const static glm::vec3 init_eye = glm::vec3(0.0f, 2.5f, -4.5f);
    const static glm::vec3 init_center = glm::vec3(0.0f, 1.0f, 0.0f);
    const static glm::vec3 init_d = glm::normalize(init_center - init_eye);

    glm::mat4x4 viewMatrix = [&]() {
        const glm::vec3 up(0.0f, 1.0f, 0.0f);

        // Camera rotation
        const auto forward = [&]() -> glm::vec3 {
            static float pitch = glm::degrees(asin(init_d.y));
            static float yaw = glm::degrees(atan2(init_d.z, init_d.x));
            static auto prevMousePos = ImGui::GetMousePos();
            const auto mousePos = ImGui::GetMousePos();
            const bool rotating = !ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDown(0);
            if (rotating)
            {
                const float sensitivity = 0.1f;
                const float dx = float(prevMousePos.x - mousePos.x) * sensitivity;
                const float dy = float(prevMousePos.y - mousePos.y) * sensitivity;
                yaw -= dx;
                pitch = glm::clamp(pitch - dy, -89.0f, 89.0f);
            }
            prevMousePos = mousePos;
            return glm::vec3(
                cos(glm::radians(pitch)) * cos(glm::radians(yaw)),
                sin(glm::radians(pitch)),
                cos(glm::radians(pitch)) * sin(glm::radians(yaw)));
        }();

        // Camera position
        const auto p = [&]() -> glm::vec3 {
            static auto p = init_eye;
            const auto w = -forward;
            const auto u = glm::normalize(glm::cross(up, w));
            const auto v = glm::cross(w, u);
            const float factor = ImGui::GetIO().KeyShift ? 10.0f : 1.0f;
            const float speed = ImGui::GetIO().DeltaTime * factor;
            if (ImGui::IsKeyDown('W')) { p += forward * speed; }
            if (ImGui::IsKeyDown('S')) { p -= forward * speed; }
            if (ImGui::IsKeyDown('A')) { p -= u * speed; }
            if (ImGui::IsKeyDown('D')) { p += u * speed; }
            return p;
        }();

        return glm::lookAtLH(p, p + forward, up);
    }();

    m_Framebuffer.Clear(m_ClientSize);

    if (animate)
    {
        rotate.y = ImGui::GetTime() * 0.5f;
    }

    // Transformation matrix
    glm::mat4x4 modelMatrix = glm::mat4x4(1.0f);
    modelMatrix = glm::translate(modelMatrix, translate);
    modelMatrix = glm::rotate(modelMatrix, rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, rotate.y, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, rotate.z, glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, scale);

    glm::mat4x4 projectionMatrix = glm::perspectiveLH(glm::radians(fov), float(m_Framebuffer.Size.width) / m_Framebuffer.Size.height, znear, zfar);
    glm::mat4x4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
    glm::mat3 transNormalMatrix = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));

    Rasterize(
        m_Scene.get(),
        m_Framebuffer,
        mode == eWireframe,
        cullbackface,
        [&](const Scene::Vertex& v) -> VaryingVertex
        {
            return {
                mvpMatrix * v.Position, transNormalMatrix * v.Normal
            };
        },
        [&](const glm::vec3& n, const glm::vec3& p_ndc) -> glm::vec4
        {
            p_ndc;
            if (mode == eNormal)
            {
                return glm::abs(glm::vec4(n, 1.0f));
            }
            else if (mode == eShaded)
            {
                return glm::vec4(glm::vec3(0.2f + 0.8f*glm::max(0.f, glm::dot(n, lightdir))), 1.0f);
            }
            return glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
        }
    );

    m_Visualizer->Update(
        m_Framebuffer.Size,
        m_Framebuffer.Buffer.data(),
        static_cast<u32>(m_Framebuffer.Buffer.size())
    );

    if (m_ImGuiActive)
    {
        // imgui 描画
        ImGui::Render();
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

    // ブレンドステートを設定
    //m_Context->OMSetBlendState(nullptr, nullptr, 0);

    // 深度ステンシルステートを設定
    m_Context->OMSetDepthStencilState(m_DepthStencilState.Get(), 0);

    // ラスタライザーステートを設定
    m_Context->RSSetState(m_RasterizerState.Get());

    m_Visualizer->Render();

    // imgui 描画
    RenderImGui();

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
    m_ClientSize = newSize;
}

// キー
void SampleApp::OnKey(KEY_CODE key, bool isDown)
{
    if (key == KEY_CODE_ESCAPE && isDown)
    {
        m_pApp->PostQuit();
    }

    if (key == KEY_CODE_I && isDown)
    {
        m_ImGuiActive = !m_ImGuiActive;
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

    return Util::CreateRasterizerState(
        m_Device,
        D3D11_CULL_BACK,
        FALSE,
        m_RasterizerState
    );
}

// 深度ステンシルステートを作成
bool SampleApp::CreateDepthStencilState()
{
    // 深度ステンシルステートを作成する
    CD3D11_DEPTH_STENCIL_DESC depthStencilDesc(D3D11_DEFAULT);
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

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

// エラーメッセージ表示
void SampleApp::ShowErrorMessage(const ResultUtil& result, const std::string& text)
{
    m_pApp->ShowMessageBox(
        result.GetText() + "\n\n" + text,
        "エラー"
    );
}

// imgui 初期化
bool SampleApp::InitializeImGui()
{
    ImGui::CreateContext();

    ResultUtil result =  ImGui_ImplDX11_Init(
        m_pApp->GetWindowHandle(),
        m_Device.Get(),
        m_Context.Get()
    );
    if (!result)
    {
        ShowErrorMessage(result, "ImGui_ImplDX11_Init");
        return false;
    }

    ImGui_ImplDX11_NewFrame();
    return true;
}

// imgui 解放
void SampleApp::TerminateImGui()
{
    // imgui 解放
    ImGui_ImplDX11_Shutdown();

    ImGui::DestroyContext();
}

// imgui 描画
void SampleApp::RenderImGui()
{
    if (m_ImGuiActive)
    {
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        ImGui_ImplDX11_NewFrame();
    }
}

