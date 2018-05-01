#pragma once



struct Padding
{
    uint32_t up;
    uint32_t down;
    uint32_t right;
    uint32_t left;
};


struct Spacing
{
    uint32_t horizontal;
    uint32_t vertical;
};


// This tag holds information on how the font was generated.
struct FontInfo
{
    std::string face;       // This is the name of the true type font.
    uint16_t    size;       // The size of the true type font.
    uint8_t     bold;       // The font is bold.
    uint8_t     italic;     // The font is italic.
    std::string charSet;    // The name of the OEM charset used (when not unicode).
    uint8_t     unicode;    // Set to 1 if it is the unicode charset.
    uint16_t    stretchH;   // The font height stretch in percentage. 100% means no stretch.
    uint8_t     smooth;     // Set to 1 if smoothing was turned on.
    uint8_t     aa;         // The supersampling level used. 1 means no supersampling was used.
    Padding     padding;    // The padding for each character (up, right, down, left).
    Spacing     spacing;    // The spacing for each character (horizontal, vertical).
    uint16_t    outline;    // The outline thickness for the characters.
};


// This tag holds information common to all characters.
struct FontCommonInfo
{
    uint16_t lineHeight;    // This is the distance in pixels between each line of text.
    uint16_t base;          // The number of pixels from the absolute top of the line to the base of the characters.
    uint16_t scaleW;        // The width of the texture, normally used to scale the x pos of the character image.
    uint16_t scaleH;        // The height of the texture, normally used to scale the y pos of the character image.
    uint16_t pages;         // The number of texture pages included in the font.
    uint8_t  packed;        // Set to 1 if the monochrome characters have been packed into each of the texture channels. In this case alphaChnl describes what is stored in each channel.
    uint8_t  alphaChnl;     // Set to 0 if the channel holds the glyph data, 1 if it holds the outline, 2 if it holds the glyph and the outline, 3 if its set to zero, and 4 if its set to one.
    uint8_t  redChnl;       // Set to 0 if the channel holds the glyph data, 1 if it holds the outline, 2 if it holds the glyph and the outline, 3 if its set to zero, and 4 if its set to one.
    uint8_t  greenChnl;     // Set to 0 if the channel holds the glyph data, 1 if it holds the outline, 2 if it holds the glyph and the outline, 3 if its set to zero, and 4 if its set to one.
    uint8_t  blueChnl;      // Set to 0 if the channel holds the glyph data, 1 if it holds the outline, 2 if it holds the glyph and the outline, 3 if its set to zero, and 4 if its set to one.
};


// This tag gives the name of a texture file. There is one for each page in the font.
struct FontPage
{
    uint16_t    id;     // The page id.
    std::string file;   // The texture file name.
};


// This tag describes on character in the font. There is one for each included character in the font.
struct Char
{
    uint16_t id;        // The character id.
    uint16_t x;         // The left position of the character image in the texture.
    uint16_t y;         // The top position of the character image in the texture.
    uint16_t width;     // The width of the character image in the texture.
    uint16_t height;    // The height of the character image in the texture.
    uint16_t xoffset;   // How much the current position should be offset when copying the image from the texture to the screen.
    uint16_t yoffset;   // How much the current position should be offset when copying the image from the texture to the screen.
    uint16_t xadvance;  // How much the current position should be advanced after drawing the character.
    uint16_t page;      // The texture page where the character image is found.
    uint8_t  chnl;      // The texture channel where the character image is found (1 = blue, 2 = green, 4 = red, 8 = alpha, 15 = all channels).
};


// The kerning information is used to adjust the distance between certain characters,
// e.g. some characters should be placed closer to each other than others.
struct Kerning
{
    uint16_t first;     // The first character id.
    uint16_t second;    // The second character id.
    uint16_t amount;    // How much the x position should be adjusted when drawing the second character immediately following the first.
};


struct FontData
{
    FontInfo              info;
    FontCommonInfo        common;
    std::vector<FontPage> pages;
    std::vector<Char>     chars;
    std::vector<Kerning>  kernings;
};


struct Font
{
    uint16_t indexes[65535];
    uint16_t indexes[65535];
};


class BitmapFont
{
public:
    BitmapFont();

    ~BitmapFont();

    void Put(const char* pMsg);

    void Flush();

protected:
    FontData m_Data;

};


