#include "TextModel.h"


TextModelOpenGLData::TextModelOpenGLData()
{
    FT_Library lib;

    if (FT_Init_FreeType(&lib))
        std::cerr << "Failed to initialize FreeType library\n";

    FT_Face face;

    if (FT_New_Face(lib, font_path.c_str(), 0, &face))
        std::cerr << "Failed to load font " << font_path << '\n';

    FT_Set_Pixel_Sizes(face, 0, 50);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; ++c)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cerr << "Failed to load Glyph N" << static_cast<int>(c) << '\n';
            continue;
        }
        
        auto texture = std::make_unique<Texture>();
        texture->generate();
        texture->bind();

        int width = face->glyph->bitmap.width;
        int rows = face->glyph->bitmap.rows;

        texture->texImage2D(GL_RED, width, rows, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int left = face->glyph->bitmap_left;
        int top = face->glyph->bitmap_top;
        int advance = face->glyph->advance.x;

        characters[c] = { std::move(texture), { width, rows }, { left, top }, advance };
    }

    Shader vertex(GL_VERTEX_SHADER, vertex_path);
    Shader fragment(GL_FRAGMENT_SHADER, fragment_path);
    shdProgram = std::move(ShaderProgram(vertex, fragment));

    Texture::unbind();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    FT_Done_Face(face);
    FT_Done_FreeType(lib);

    textVAO.generate();
    textVBO.generate();

    textVAO.bind();
    textVBO.bind();

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    textVAO.enableAttribute(0);
    textVAO.setAttribPointer(0, 4, false, 4, 0);

    VAO::unbind();
}

const std::string & TextModel::getText() const
{
    return text;
}

void TextModel::setText(const std::string & text)
{
    this->text = text;
    auto& textData = TextModelOpenGLData::instance();

    std::vector<float> linesW{ 0.0f };
    float max_h = 0.0f;

    lines = 1;

    for (const auto& c : text)
    {
        if (c == '\n')
        {
            ++lines;
            linesW.push_back(0.0f);
            continue;
        }

        auto& character = textData.characters[c];

        float h = character.size.y * scale;
        max_h = std::max(max_h, h);

        linesW.back() += (character.advance >> 6) * scale;
    }

    size.x = *std::max_element(linesW.begin(), linesW.end());
    size.y = max_h * lines + lineDelim * (lines - 1);
}
