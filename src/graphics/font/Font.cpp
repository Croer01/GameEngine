//
// Created by adria on 03/11/2018.
//

#include <vector>
#include <array>
#include "Font.hpp"
#include "../../utils.hpp"
#include "Text.hpp"

Font::Font(const FT_Face &face, unsigned int pixelsSize) {
    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, pixelsSize);

    int pixelStoreValue;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &pixelStoreValue);
    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Load first 128 characters of ASCII set
    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
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
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        CheckGlError();

        // Now store character for later use
        FontCharacter character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<GLuint>(face->glyph->advance.x)
        };
        characters_.insert(std::pair<GLchar, FontCharacter>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, pixelStoreValue);
    CheckGlError();
}

std::shared_ptr<Text> Font::createText(const std::string &text) {
    TextDef textDef;
    std::vector<std::array<float, 20>> vertices;
    std::vector<GLuint> textureIds;

    // Iterate through all characters
    std::string::const_iterator c;
    float x =0.f;
    float y =0.f;
    for (c = text.begin(); c != text.end(); c++)
    {
        FontCharacter ch = characters_[*c];

        GLfloat xpos = x + ch.Bearing.x;
        GLfloat ypos = y - ch.Bearing.y + ch.Size.y;

        GLfloat w = ch.Size.x;
        GLfloat h = ch.Size.y;

        //The order of the vertices are fliped to from "n" to "u" way to deal with the inverted y axis
        // Update VBO for each character. vertex(3) | uv(2)
        std::array<float, 20> quadVertices = {
                xpos, ypos + h, 0.0f,        0.f, 1.f,
                xpos, ypos, 0.0f,            0.f, 0.f,
                xpos + w, ypos, 0.0f,        1.f, 0.f,
                xpos + w, ypos + h, 0.0f,    1.f, 1.f
        };
        vertices.push_back(quadVertices);

        textureIds.push_back(ch.TextureID);
        //FREE_TYPE: Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }

    textDef.verticesByCharacter = vertices;
    textDef.textureIds = textureIds;

    return std::make_shared<Text>(textDef);
}

Font::~Font() {
    for (auto character : characters_)
        glDeleteTextures(0, &character.second.TextureID);

}
