
#pragma once


struct Float4
{
    float x, y, z, w;
};

struct Float2
{
    float x, y;
};


struct Vertex_PositionColor
{
    Float4 Position;
    Float4 Color;

    static constexpr UINT PositionOffset = 0;
    static constexpr UINT PositionSize = sizeof(Float4);

    static constexpr UINT ColorOffset = PositionOffset + PositionSize;
    static constexpr UINT ColorSize = sizeof(Float4);

    static constexpr D3D11_INPUT_ELEMENT_DESC pInputElementDescs[] = {
        { "POSITION", 0,    DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  PositionOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0,    DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  ColorOffset,    D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    static constexpr UINT InputElementCount = _countof(pInputElementDescs);
};


struct Vertex_PositionColorTexture
{
    Float4 Position;
    Float4 Color;
    Float2 Texture;

    static constexpr UINT PositionOffset = 0;
    static constexpr UINT PositionSize = sizeof(Float4);

    static constexpr UINT ColorOffset = PositionOffset + PositionSize;
    static constexpr UINT ColorSize = sizeof(Float4);

    static constexpr UINT TextureOffset = ColorOffset + ColorSize;
    static constexpr UINT TextureSize = sizeof(Float2);

    static constexpr D3D11_INPUT_ELEMENT_DESC pInputElementDescs[] = {
        { "POSITION", 0,    DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  PositionOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0,    DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  ColorOffset,    D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0,    DXGI_FORMAT_R32G32_FLOAT,       0,  TextureOffset,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    static constexpr UINT InputElementCount = _countof(pInputElementDescs);
};


struct Vertex_PositionColorNormalTexture
{
    Float4 Position;
    Float4 Color;
    Float4 Normal;
    Float2 Texture;

    static constexpr UINT PositionOffset = 0;
    static constexpr UINT PositionSize = sizeof(Float4);

    static constexpr UINT ColorOffset = PositionOffset + PositionSize;
    static constexpr UINT ColorSize = sizeof(Float4);

    static constexpr UINT NormalOffset = ColorOffset + ColorSize;
    static constexpr UINT NormalSize = sizeof(Float4);

    static constexpr UINT TextureOffset = NormalOffset + NormalSize;
    static constexpr UINT TextureSize = sizeof(Float2);

    static constexpr D3D11_INPUT_ELEMENT_DESC pInputElementDescs[] = {
        { "POSITION", 0,    DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  PositionOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0,    DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  ColorOffset,    D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0,    DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  NormalOffset,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0,    DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  TextureOffset,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    static constexpr UINT InputElementCount = _countof(pInputElementDescs);
};


struct Vertex_PositionColorNormal
{
    Float4 Position;
    Float4 Color;
    Float4 Normal;

    static constexpr UINT PositionOffset = 0;
    static constexpr UINT PositionSize = sizeof(Float4);

    static constexpr UINT ColorOffset = PositionOffset + PositionSize;
    static constexpr UINT ColorSize = sizeof(Float4);

    static constexpr UINT NormalOffset = ColorOffset + ColorSize;
    static constexpr UINT NormalSize = sizeof(Float4);

    static constexpr D3D11_INPUT_ELEMENT_DESC pInputElementDescs[] = {
        { "POSITION", 0,    DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  PositionOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0,    DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  ColorOffset,    D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0,    DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  NormalOffset,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    static constexpr UINT InputElementCount = _countof(pInputElementDescs);
};


