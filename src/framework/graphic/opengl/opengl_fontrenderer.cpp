/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 * Brief        : Uses a TrueType font to render text on a RenderTarget
 ********************************************************************************/
#include "precompiled.h"
#include "graphic/opengl/opengl_fontrenderer.h"
#include "graphic/opengl/opengl.h"
#include "graphic/renderer.h"
#include "glm/gtx/transform.hpp"


namespace Framework
{

    OpenGLFontRenderer::OpenGLFontRenderer()
    {
        memset(mGlyphTextures, 0, sizeof(mGlyphTextures));
        memset(mGlyphVAOs, 0, sizeof(mGlyphVAOs));
        memset(mGlyphBuffers, 0, sizeof(mGlyphBuffers));
    }

    OpenGLFontRenderer::~OpenGLFontRenderer()
    {
        if ( glIsTexture(mGlyphTextures[0]) )
            glDeleteTextures(GL_FONT_RENDERER_NUM_GLYPHS, mGlyphTextures);
        if ( glIsBuffer(mGlyphBuffers[0]) )
            glDeleteBuffers(GL_FONT_RENDERER_NUM_GLYPHS, mGlyphBuffers);
        if ( glIsVertexArray(mGlyphVAOs[0]) )
            glDeleteVertexArrays(GL_FONT_RENDERER_NUM_GLYPHS, mGlyphVAOs);
    }

    bool OpenGLFontRenderer::SetFont(std::shared_ptr<const TrueTypeFont> font)
    {
        if ( !mShader )
        { // one-time resource initialization
            mShader = Shader::New("Glyph renderer");
            std::string lError;
            if ( !mShader->Load("text/glyph", lError) )
                CRASH("ERROR compiling shader text/glyph: %s\n", lError.c_str());
            __(glGenTextures(GL_FONT_RENDERER_NUM_GLYPHS, mGlyphTextures));
            __(glGenVertexArrays(GL_FONT_RENDERER_NUM_GLYPHS, mGlyphVAOs));
            __(glGenBuffers(GL_FONT_RENDERER_NUM_GLYPHS, mGlyphBuffers));
        }

        mFont = std::move(font);

        /* Generate all textures and vertex arrays for the glyphs */
        __(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
        for (uint32_t i = 0; i < GL_FONT_RENDERER_NUM_GLYPHS; ++i)
        {
            uint32_t width = 0, height = 0, advance = 0;
            int32_t offsetLeft = 0, offsetTop = 0;
            const uint8_t* buffer = mFont->GetBitmap(i, width, height, offsetLeft, offsetTop, advance);
            ASSERT(advance);
            constexpr uint32_t dummy_size = 4;
            uint8_t dummy_buffer[dummy_size * dummy_size];
            if ( !buffer )
            { // an 'empty' glyph?
                ASSERT(!width && !height);
                width = height = dummy_size;
                buffer = dummy_buffer;
                memset(dummy_buffer, 0, sizeof(dummy_buffer));
            }
            __(glBindTexture(GL_TEXTURE_2D, mGlyphTextures[i]));
            {
                /* Swizzle mask to emulate GL_LUMINANCE_ALPHA behaviour,
                 * just for fun */
                //GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_RED};
                //__(glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask));
                __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
                __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
                __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
                __(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
                __(glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, buffer));
            }
            __(glBindTexture(GL_TEXTURE_2D, GL_NONE));

            /* Bind the vertex array for this glyph */
            __(glBindVertexArray(mGlyphVAOs[i]));
            {
                __(glBindBuffer(GL_ARRAY_BUFFER, mGlyphBuffers[i]));
                {
                    GLfloat verticesData[] = {
                        // position                       // textcoord
                        0, 0,                             0, 0, // left-bottom
                        0, (GLfloat)height,               0, 1, // left-top
                        (GLfloat)width, 0,                1, 0, // right-bottom
                        (GLfloat)width, (GLfloat)height,  1, 1 // right-top
                    };

                    __(glBufferData(GL_ARRAY_BUFFER, sizeof verticesData, verticesData, GL_STATIC_DRAW));

                    __(glEnableVertexAttribArray(0));
                    __(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0));

                    __(glEnableVertexAttribArray(1));
                    __(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), reinterpret_cast<void *>(2 * sizeof(GLfloat))));
                }
                __(glBindBuffer(GL_ARRAY_BUFFER, GL_NONE));
            }
            __(glBindVertexArray(GL_NONE));
        }

        return true;
    }


    glm::uvec2 OpenGLFontRenderer::EvaluateText(const char* aText, float aScale, uint32_t aVerticalSpacing) const
    {
        glm::uvec2 lBox(0,0), lLineSpan(0,0);
        for ( char c = *aText; c; c = *++aText )
        {
            uint32_t glyphWidth = 0, glyphHeight = 0, advance = 0;
            int32_t offsetLeft = 0, offsetTop = 0;
            mFont->GetBitmap(c, glyphWidth, glyphHeight, offsetLeft, offsetTop, advance);
            lLineSpan.x += advance;
            if ( glyphHeight > lLineSpan.y )
                lLineSpan.y = glyphHeight;
            if ( c == '\n' )
            {
                if ( lLineSpan.x > lBox.x)
                    lBox.x = lLineSpan.x;
                lBox.y += (lLineSpan.y + aVerticalSpacing);
                lLineSpan.x = lLineSpan.y = 0;
            }
        }
        if ( !lBox.x ) // one line only
            lBox.x = lLineSpan.x;
        lBox.y += lLineSpan.y;
        lBox.x *= aScale;
        lBox.y *= aScale;
        return lBox;
    }


    void OpenGLFontRenderer::RenderText(const char* aText, float aScale, const glm::uvec3& aPosition, const glm::vec4 &aColor, RenderTarget &aTarget) const
    {
        //aTarget.BindForDrawning();
        mShader->Attach();
        __(glEnable(GL_BLEND));
        __(glDisable(GL_DEPTH_TEST));
        __(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        __(glActiveTexture(GL_TEXTURE0));

        /* The glyphs are created in window coordinates so we don't
         * need the glyphs width and height here. This matrix scales
         * to (0.0, 1.0) on x and y, then transforms into NDC */
        glm::mat4 glyphTransform(
            2.0f / aTarget.GetSize().x, 0.0f, 0.0f, 0.0f,
            0.0f, -2.0f / aTarget.GetSize().y, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f, 1.0f);
        mShader->SetUniformMat4("glyphTransform", &glyphTransform);
        mShader->SetUniformFloat("scale", aScale);
        mShader->SetUniformVec4("color", aColor);
        mShader->SetUniformTexture2D("glyph", 0);

        glm::vec2 lPos(aPosition.x, aPosition.y);
        uint32_t lMaxGlyphHeight = 0;
        for ( char c = *aText; c; c = *++aText )
        {
            if ( c == '\n' )
            { // a newline character
                lPos.x = aPosition.x; // reset x position
                lPos.y += (lMaxGlyphHeight + aPosition.z/*vertical spacing*/) * aScale; // shift y position by scaled (max glyph height from current line + vertical spacing)
                lMaxGlyphHeight = 0;
                continue;
            }
            ASSERT(c >= ' ' && c <= '~');

            /* Get the size of the glyph */
            uint32_t glyphWidth = 0, glyphHeight = 0, advance = 0;
            int32_t offsetLeft = 0, offsetTop = 0;

            mFont->GetBitmap(c, glyphWidth, glyphHeight, offsetLeft, offsetTop, advance);
            if ( glyphHeight > lMaxGlyphHeight )
                lMaxGlyphHeight = glyphHeight;

            advance *= aScale;
            glyphHeight *= aScale;

            /* Adjust the coordinates to take into account the bearings */
            glm::vec2 glyphPos = glm::vec2(lPos.x + static_cast<float>(offsetLeft) * aScale, lPos.y + static_cast<float>(offsetTop) * aScale );
            mShader->SetUniformVec2("glyphPos", glyphPos);

            /* Bind the target texture */
            __(glBindTexture(GL_TEXTURE_2D, mGlyphTextures[c]));
            __(glBindVertexArray(mGlyphVAOs[c]));
            __(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));

            lPos.x += advance;
        }

        __(glEnable(GL_DEPTH_TEST));
        __(glDisable(GL_BLEND));
        mShader->Detach();
        //aTarget.Unbind();
    }
}
