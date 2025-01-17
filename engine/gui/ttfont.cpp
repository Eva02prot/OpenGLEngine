//
//  ttfont.cpp
//  OpenGLEngine
//
//  Created by 彭怀亮 on 5/28/19.
//  Copyright © 2019 彭怀亮. All rights reserved.
//

#include "ttfont.hpp"
#include "../std/asset.hpp"
#ifdef _GLES_
#include "FilePath.h"
#endif

namespace engine
{
    TTFont TTFont::instance;
    
    TTFont::~TTFont()
    {
        delete shader;
        Characters.clear();
    }

    float TTFont::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, const glm::vec3 color)
    {
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        shader->use();
        shader->setVec3("textColor", color);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);
        
        GLfloat start = x;
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = Characters[*c];
            GLfloat xpos = x + ch.Bearing.x * scale;
            GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
            GLfloat w = ch.Size.x * scale;
            GLfloat h = ch.Size.y * scale;
            
            GLfloat vertices[6][4] = {
                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos,     ypos,       0.0, 1.0 },
                { xpos + w, ypos,       1.0, 1.0 },
                
                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos + w, ypos,       1.0, 1.0 },
                { xpos + w, ypos + h,   1.0, 0.0 }
            };
            
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
            {
                glDrawArrays(DRAW_MODE, 0, 6);
            }
            x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        return x - start;
    }

    int TTFont::initial()
    {
        shader =new Shader("ttfont.vs","ttfrag.fs");
        mat4 proj = ortho(0.0f, (float)UI_WIDTH, 0.0f, (float)UI_HEIGHT);
        shader->use();
        shader->setMat4("projection", proj);
        shader->setInt("text", 0);
        
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        
        FT_Face face;
        std::string path = getResPath("fonts/arial.ttf");
        if (FT_New_Face(ft, path.c_str(), 0, &face)) std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        
        FT_Set_Pixel_Sizes(face, 0, FONT_SIZE);
        
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        
        FT_GlyphSlot glyph = face->glyph;
        // Load first 128 characters of ASCII set
        for (GLubyte c = 0; c < 128; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
#ifdef _GLES_
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, glyph->bitmap.width, glyph->bitmap.rows, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);
#else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, glyph->bitmap.width, glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);
#endif
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            Character character = {texture, ivec2(glyph->bitmap.width, glyph->bitmap.rows), ivec2(glyph->bitmap_left, glyph->bitmap_top), (GLuint)glyph->advance.x};
            Characters.insert(std::pair<GLchar, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
        return 0;
    }

}
