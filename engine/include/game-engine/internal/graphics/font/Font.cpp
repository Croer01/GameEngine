//
// Created by adria on 03/11/2018.
//

#include <vector>
#include <array>
#include <utf8.h>
#include "Font.hpp"
#include <game-engine/internal/utils.hpp>
#include "Text.hpp"
#include <zlib.h>
#include "ProggyClean.hpp"

#include <iostream>
#include <fstream>

namespace GameEngine {
namespace Internal {

namespace {
/* Get from cinderlib Unicode header */
std::u32string toUtf32( const std::string &utf8Str )
{
    std::u32string result;
    utf8::utf8to32( utf8Str.begin(), utf8Str.end(), back_inserter( result ));
    return result;
}
}

Font::Font(const FT_Library &ftLibrary, const std::string &fontPath, unsigned int pixelsSize)
{
    FT_Error ftError = FT_New_Face(ftLibrary, fontPath.c_str(), 0, &face_);
    CheckFreeTypeError(ftError);
    initialize(pixelsSize);
}

// Modified version zlib's uncompress method to support gzip
int uncompressGzip(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen)
{
    z_stream stream;
    int err;

    stream.next_in = (z_const Bytef *)source;
    stream.avail_in = (uInt)sourceLen;
    /* Check for source > 64K on 16-bit machine: */
    if ((uLong)stream.avail_in != sourceLen) return Z_BUF_ERROR;

    stream.next_out = dest;
    stream.avail_out = (uInt)*destLen;
    if ((uLong)stream.avail_out != *destLen) return Z_BUF_ERROR;

    stream.zalloc = (alloc_func)0;
    stream.zfree = (free_func)0;

    err = inflateInit2(&stream, 16 + MAX_WBITS);
    if (err != Z_OK) return err;

    err = inflate(&stream, Z_FINISH);
    if (err != Z_STREAM_END) {
        inflateEnd(&stream);
        if (err == Z_NEED_DICT || (err == Z_BUF_ERROR && stream.avail_in == 0))
            return Z_DATA_ERROR;
        return err;
    }
    *destLen = stream.total_out;

    err = inflateEnd(&stream);
    return err;
}

Font::Font(const FT_Library &ftLibrary, unsigned int pixelsSize)
{
    // Original file size
    unsigned long fontDataSize = 41208;
    inMemoryFont_.resize(fontDataSize);
    fontDataSize = 41208 * sizeof(unsigned char);
    int result = uncompressGzip(&inMemoryFont_[0], &fontDataSize, ProggyClean_ttf, ProggyClean_ttf_len);

    if(fontDataSize != inMemoryFont_.size())
    {
        throw std::runtime_error("Unexpected error during default font loading. Error: expected size " +
                                  std::to_string(inMemoryFont_.size()) +
                                  " but get " + std::to_string(fontDataSize));
    }

    if (result == Z_OK)
    {
        FT_Error ftError = FT_New_Memory_Face(ftLibrary, &inMemoryFont_[0], inMemoryFont_.size() * sizeof(unsigned char), 0 , &face_);
        CheckFreeTypeError(ftError);
        initialize(pixelsSize);
    }
    else
    {
        throw std::runtime_error("Unexpected error during default font loading. Error: " + std::to_string(result));
    }
}

void Font::initialize(unsigned int pixelsSize)
{
    //based on "Learn OpenGl": https://learnopengl.com/In-Practice/Text-Rendering
    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face_, 0, pixelsSize);

    pixelHeight_ = pixelsSize;
    //FreeType scale the metrics 1/64: https://www.freetype.org/freetype2/docs/tutorial/step2.html
    lineSpacing_ = face_->size->metrics.height / 64;

    int pixelStoreValue;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &pixelStoreValue);
    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Load first 128 characters of ASCII set
    for (GLubyte c = 0; c < 255; c++) {
        registerCharacter(FT_Get_Char_Index(face_, c));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, pixelStoreValue);
    CheckGlError();
}

void Font::registerCharacter(FT_UInt glyph)
{
    // Load character glyph
    if (FT_Load_Glyph(face_, glyph, FT_LOAD_RENDER)) {
        throw std::runtime_error("ERROR::FREETYTPE: Failed to load Glyph");
    }
    // Generate texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        face_->glyph->bitmap.width,
        face_->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        face_->glyph->bitmap.buffer
    );
    // Set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    CheckGlError();

    // Now store character for later use
    FontCharacter fCharacter = {
        texture,
        glm::ivec2(face_->glyph->bitmap.width, face_->glyph->bitmap.rows),
        glm::ivec2(face_->glyph->bitmap_left, face_->glyph->bitmap_top),
        static_cast<GLuint>(face_->glyph->advance.x)
    };
    characters_.insert(std::pair<GLchar, FontCharacter>(glyph, fCharacter));
}

std::shared_ptr<Text> Font::createText(const std::string &text) {
    return std::make_shared<Text>(shared_from_this(), createTextDef(text));
}

    Font::~Font() {
        for (auto character : characters_)
            glDeleteTextures(1, &character.second.TextureID);

        FT_Done_Face(face_);
    }

    const TextDef Font::createTextDef(const std::string &text) {
        TextDef textDef;
        std::vector<std::array<float, 20>> vertices;
        std::vector<GLuint> textureIds;
        int maxWidth = 0;

        // Iterate through all characters
        std::u32string::const_iterator c;
        float x = 0.f;
        float y = 0.f;
        auto u32Text = toUtf32(text);
        for (c = u32Text.begin(); c != u32Text.end(); c++)
        {
            if (*c == '\n')
            {
                if (x > maxWidth)
                    maxWidth = std::ceil(x);

                y += lineSpacing_;
                x = 0;

                // all the vertex are in the same position
                std::array<float, 20> quadVertices = {
                    x, y, 0.0f, 0.f, 1.f,
                    x, y, 0.0f, 0.f, 0.f,
                    x, y, 0.0f, 1.f, 0.f,
                    x, y, 0.0f, 1.f, 1.f
                };
                textDef.characters.emplace_back(true, quadVertices, -1);
                continue;
            }

            FT_UInt glyph = FT_Get_Char_Index(face_, *c);
            auto it = characters_.find(glyph);

            FontCharacter ch;

            if (it == characters_.end())
            {
                registerCharacter(glyph);
                ch = characters_[glyph];
            }
            else
                ch = it->second;

            GLfloat xpos = x + ch.Bearing.x;
            GLfloat ypos = y - ch.Bearing.y + pixelHeight_;

            GLfloat w = ch.Size.x;
            GLfloat h = ch.Size.y;

            if(*c == ' ')
                w += ch.Advance >> 6;

            //The order of the vertices are flipped from "n" to "u" way to deal with the inverted y axis
            // Update VBO for each character. vertex(3) | uv(2)
            std::array<float, 20> quadVertices = {
                    xpos, ypos + h, 0.0f, 0.f, 1.f,
                    xpos, ypos, 0.0f, 0.f, 0.f,
                    xpos + w, ypos, 0.0f, 1.f, 0.f,
                    xpos + w, ypos + h, 0.0f, 1.f, 1.f
            };
            //FREE_TYPE: Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance
                    >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64 (divide initialAmount of 1/64th pixels by 64 to get initialAmount of pixels))

            textDef.characters.emplace_back(false, quadVertices,ch.TextureID);
        }

        // compare the last width
        if(x > maxWidth)
            maxWidth = x;

        // the first line never calculate the maxWidth
        textDef.width = y == 0? x : maxWidth;
        // the first line is not included in "y"
        textDef.height = std::ceil(lineSpacing_ + y);
        textDef.lineHeight = lineSpacing_;
        return textDef;
    }
}
}