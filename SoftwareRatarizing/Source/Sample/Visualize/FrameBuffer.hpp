#pragma once


struct Framebuffer
{
    Size2D          Size;
    std::vector<u8> Buffer;

    void Clear(const Size2D& size)
    {
        if (Size.width != size.width || Size.height != size.height)
        {
            Size = size;
        }

        Buffer.assign(Size.width * Size.height * 4, 0);
    }

    void SetPixel(u32 x, u32 y, const glm::vec4& color)
    {
        if (Size.width <= x || Size.height <= y) { return; }

        const u32 i = 4 * (y * Size.width + x);
        Buffer[i] = (u8)glm::clamp(int(color.r * 255), 0, 255);
        Buffer[i + 1] = (u8)glm::clamp(int(color.g * 255), 0, 255);
        Buffer[i + 2] = (u8)glm::clamp(int(color.b * 255), 0, 255);
        Buffer[i + 3] = (u8)glm::clamp(int(color.a * 255), 0, 255);
    };
};

