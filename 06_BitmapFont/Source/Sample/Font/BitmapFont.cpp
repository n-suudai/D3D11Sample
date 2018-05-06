

std::string GetValueStr(const std::string& line, const std::string& name)
{
    size_t pos = line.find(name);
    size_t size = name.size();

    // 見つからなかった
    if (pos == std::string::npos) { return ""; }

    size_t valueFirst = line.find('=', pos + size);
    size_t valueEnd = line.find(' ', valueFirst);

    return line.substr(valueFirst, valueEnd - valueFirst);
}

template<class T>
T GetValue(const std::string& valueStr, T notFound = T())
{
    if (valueStr == "") { return notFound; }

    std::istringstream iss = valueStr;

    T value;

    iss >> value;

    return value;
}

template<class T>
T GetValue(const std::string& line, const std::string& name, T notFound = T())
{
    return GetValue<T>(GetValueStr(line, name), notFound);
}


FontPadding GetPadding(const std::string& line, const std::string& name)
{
    std::string valueStr = GetValueStr(line, name);

    FontPadding padding = { 0 };

    size_t first = 0;
    size_t last = 0;

    // up
    last = valueStr.find_first_not_of(',', first);
    padding.up = GetValue<u16>(valueStr.substr(first, last - first));
    first = last + 1;

    // down
    last = valueStr.find_first_not_of(',', first);
    padding.down = GetValue<u16>(valueStr.substr(first, last - first));
    first = last + 1;

    // right
    last = valueStr.find_first_not_of(',', first);
    padding.right = GetValue<u16>(valueStr.substr(first, last - first));
    first = last + 1;

    // left
    padding.left = GetValue<u16>(valueStr.substr(first, valueStr.size() - first));

    return padding;
}


FontSpacing GetSpacing(const std::string& line, const std::string& name)
{
    std::string valueStr = GetValueStr(line, name);

    FontSpacing spacing = { 0 };

    size_t first = 0;
    size_t last = 0;

    // horizontal
    last = valueStr.find_first_not_of(',', first);
    spacing.horizontal = GetValue<u16>(valueStr.substr(first, last - first));
    first = last + 1;

    // vertical
    spacing.vertical = GetValue<u16>(valueStr.substr(first, valueStr.size() - first));

    return spacing;
}


BitmapFont::BitmapFont(
    const ComPtr<ID3D11Device>& device,
    const ComPtr<ID3D11DeviceContext> context
)
    : m_Device(device)
    , m_Context(context)
{

}

BitmapFont::~BitmapFont()
{

}

void BitmapFont::FileLoad(const std::string& fileName, FontData& out)
{
    std::ifstream file(fileName);

    file.unsetf(std::ios::skipws);  // スペースを読み飛ばさない
    file.seekg(std::ios::beg);  // 先頭にセット

    std::string lineStr = "";

    // info
    {
        std::getline(file, lineStr);
        out.info.face = GetValue<std::string>(lineStr, "face", "");
        out.info.size = GetValue<u16>(lineStr, "size", 0);
        out.info.bold = GetValue<u8>(lineStr, "bold", 0);
        out.info.italic = GetValue<u8>(lineStr, "italic", 0);
        out.info.charSet = GetValue<std::string>(lineStr, "charSet", "");
        out.info.unicode = GetValue<u8>(lineStr, "unicode", 0);
        out.info.stretchH = GetValue<u16>(lineStr, "streachH", 0);
        out.info.smooth = GetValue<u8>(lineStr, "smooth", 0);
        out.info.aa = GetValue<u8>(lineStr, "aa", 0);
        out.info.padding = GetPadding(lineStr, "padding");
        out.info.spacing = GetSpacing(lineStr, "spacing");
        out.info.outline = GetValue<u16>(lineStr, "outline", 0);
    }

    // common
    {
        lineStr = "";
        std::getline(file, lineStr);
        out.common.lineHeight = GetValue<u16>(lineStr, "lineHeight", 0);
        out.common.base = GetValue<u16>(lineStr, "base", 0);
        out.common.scaleW = GetValue<u16>(lineStr, "scaleW", 0);
        out.common.scaleH = GetValue<u16>(lineStr, "scaleH", 0);
        out.common.pages = GetValue<u16>(lineStr, "pages", 0);
        out.common.packed = GetValue<u8>(lineStr, "packed", 0);
        out.common.alphaChnl = GetValue<u8>(lineStr, "alphaChnl", 0);
        out.common.redChnl = GetValue<u8>(lineStr, "redChnl", 0);
        out.common.greenChnl = GetValue<u8>(lineStr, "greenChnl", 0);
        out.common.blueChnl = GetValue<u8>(lineStr, "blueChnl", 0);
    }

    // pages
    {
        FontPage page;
        for (u16 i = 0; i < out.common.pages; ++i)
        {
            lineStr = "";
            std::getline(file, lineStr);

            page.id = GetValue<u16>(lineStr, "id", 0);
            page.file = GetValue<std::string>(lineStr, "file", "");
            out.pages.push_back(page);
        }
    }

    // chars
    {
        lineStr = "";
        std::getline(file, lineStr);

        u32 charsCount = GetValue<u32>(lineStr, "count");
        u16 id = 0;

        for (u32 i = 0; i < charsCount; ++i)
        {
            lineStr = "";
            std::getline(file, lineStr);

            id = GetValue<u16>(lineStr, "id", 0);
            out.chars[id].id = id;
            out.chars[id].x = GetValue<u16>(lineStr, "x", 0);
            out.chars[id].y = GetValue<u16>(lineStr, "y", 0);
            out.chars[id].width = GetValue<u16>(lineStr, "width", 0);
            out.chars[id].height = GetValue<u16>(lineStr, "height", 0);
            out.chars[id].xoffset = GetValue<u16>(lineStr, "xoffset", 0);
            out.chars[id].yoffset = GetValue<u16>(lineStr, "yoffset", 0);
            out.chars[id].xadvance = GetValue<u16>(lineStr, "xadvance", 0);
            out.chars[id].page = GetValue<u16>(lineStr, "page", 0);
            out.chars[id].chnl = GetValue<u8>(lineStr, "chnl", 0);
        }
    }

    // kerning
    {
        lineStr = "";
        std::getline(file, lineStr);

        u32 kerningsCount = GetValue<u32>(lineStr, "count", 0);

        FontKerning kerning = { 0 };

        for (u32 i = 0; i < kerningsCount; ++i)
        {
            lineStr = "";
            std::getline(file, lineStr);

            kerning.first = GetValue<u32>(lineStr, "first", 0);
            kerning.second = GetValue<u32>(lineStr, "second", 0);
            kerning.amount = GetValue<u32>(lineStr, "amount", 0);

            out.kernings.push_back(kerning);
        }
    }
}

void BitmapFont::Initialize(const std::string& fileName)
{
    // ファイル読み込み
    FileLoad(fileName, m_Data);
    
    // テクスチャ作成
    {
        Texture texture;
        for (auto& page : m_Data.pages)
        {
            Util::CreateTextureFromFile(
                m_Device,
                "Resource\\Font\\" + page.file,
                texture.Texture2D,
                texture.ShaderResourceView
            );

            m_Textures.push_back(texture);
        }
    }

    // 頂点バッファ作成
    {
        m_BufferSize = 4096;
        m_VertexStream.reset(new Vertex_BitmapFont[m_BufferSize * 4]);

        ZeroMemory(m_VertexStream.get(), sizeof(Vertex_BitmapFont) * m_BufferSize * 4);

        Util::CreateBuffer(
            m_Device,
            m_VertexStream.get(),
            sizeof(Vertex_BitmapFont) * m_BufferSize * 4,
            D3D11_BIND_VERTEX_BUFFER,
            m_VertexBuffer
        );
    }

    // インデックスバッファ作成
    {
        std::unique_ptr<u16> indexStream(new u16[m_BufferSize * 6]);
        u16* pIndex = indexStream.get();
        for (u32 ii = 0; ii < m_BufferSize; ++ii)
        {
            pIndex[ii * 6 + 0] = ii * 4 + 0;
            pIndex[ii * 6 + 1] = ii * 4 + 1;
            pIndex[ii * 6 + 2] = ii * 4 + 2;
            pIndex[ii * 6 + 3] = ii * 4 + 1;
            pIndex[ii * 6 + 4] = ii * 4 + 3;
            pIndex[ii * 6 + 5] = ii * 4 + 2;
        }

        Util::CreateBuffer(
            m_Device,
            pIndex,
            sizeof(u16) * m_BufferSize * 6,
            D3D11_BIND_INDEX_BUFFER,
            m_IndexBuffer
        );
    }

    // ラスタライザーステート作成
    {
        Util::CreateRasterizerState(
            m_Device,
            D3D11_CULL_BACK,
            m_RasterizerState
        );
    }

    // サンプラーステート作成
    {
        Util::CreateSamplerState(
            m_Device,
            D3D11_FILTER_ANISOTROPIC,
            D3D11_TEXTURE_ADDRESS_WRAP,
            D3D11_TEXTURE_ADDRESS_WRAP,
            D3D11_TEXTURE_ADDRESS_WRAP,
            m_SamplerState
        );
    }

    // 頂点シェーダーを作成
    {
        Util::CreateVertexShaderAndInputLayout(
            m_Device,
            "Resource\\Shader\\FontVS.cso",
            Vertex_BitmapFont::pInputElementDescs,
            Vertex_BitmapFont::InputElementCount,
            m_VertexShader,
            m_InputLayout
        );
    }

    // ピクセルシェーダーを作成
    {
        Util::CreatePixelShader(
            m_Device,
            "Resource\\Shader\\FontPS.cso",
            m_PixelShader
        );
    }

    // フォント描画縦サイズを取得
    m_FontHeight = m_Data.chars[static_cast<int>('A')].height + 2.0f;

    m_Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    m_CurrentColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

//void BitmapFont::PutFormat(const glm::vec2& position, const char* pFormat, ...)
//{
//
//}
//
//void BitmapFont::PutFormat(const glm::vec2& position, const glm::vec4& color, const char* pFormat, ...)
//{
//
//}

void BitmapFont::Put(const glm::vec2& position, const glm::vec4& color, const char* pUtf8)
{
    std::wstring widen = Util::Widen(pUtf8);
    Put(position, color, widen.c_str());
}

void BitmapFont::Put(const glm::vec2& position, const char* pUtf8)
{
    std::wstring widen = Util::Widen(pUtf8);
    Put(position, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), widen.c_str());
}

void BitmapFont::Put(const glm::vec2& position, const glm::vec4& color, const wchar_t* pUtf16)
{

}

void BitmapFont::Put(const glm::vec2& position, const wchar_t* pUtf16);

void BitmapFont::Draw();

void BitmapFont::Clear();

void BitmapFont::Flush();

