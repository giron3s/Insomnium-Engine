/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 * Brief        : Uses a TrueType font to render text on a RenderTarget
 ********************************************************************************/
#pragma once

#include <stdint.h>
#include <glm/glm.hpp>
#include <string>
#include "graphic/fontrender.h"
#include "graphic/shader.h"
#include "graphic/opengl/opengl.h"

namespace Framework
{
    class OpenGLFontRenderer : public FontRenderer
    {
    #define GL_FONT_RENDERER_NUM_GLYPHS 128
      public:
        OpenGLFontRenderer();
        ~OpenGLFontRenderer() override;

        bool SetFont(std::shared_ptr<const TrueTypeFont> aFont) override;
        glm::uvec2 EvaluateText(const char* aText, float aScale, uint32_t aVerticalSpacing) const override;
        void RenderText(const char* aText, float aScale, const glm::uvec3& aPosition, const glm::vec4& aColor, RenderTarget& aTarget) const override;

      private:
        GLuint        mGlyphTextures[GL_FONT_RENDERER_NUM_GLYPHS];
        GLuint        mGlyphVAOs[GL_FONT_RENDERER_NUM_GLYPHS];
        GLuint        mGlyphBuffers[GL_FONT_RENDERER_NUM_GLYPHS];
        Shader*       mShader = nullptr;
    };
}
