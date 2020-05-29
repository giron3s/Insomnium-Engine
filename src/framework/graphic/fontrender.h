/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 * Brief        : Uses a TrueType font to render text on a RenderTarget. This is the
 *                high level API for the font renderer which is then specialized
 *                depending on the rendering API
 *
 *                It allows to set a specific TrueType font for rendering and then
 *                render the desired text onto a render target with the given color
 *
 *                This class does NOT support formatting like printf. For that functionality
 *                instead use TextConsole class
 *******************************************************************************/
#pragma once

#include <string>
#include <stdint.h>
#include <glm/glm.hpp>

#include "graphic/rendertarget.h"
#include "graphic/truetypefont.h"

namespace Framework
{
    class FontRenderer
    {
      public:
        /**
         * Allocates a new FontRenderer of the specific underlaying API
         *
         * To free the returned memory FontRenderer::Delete() must be used
         *
         * @return A pointer to the newly allocated FontRenderer
         */
        static FontRenderer *New();

        /**
         * Frees the shader previously allocated by FontRenderer::New()
         *
         * @param target  Pointer to the allocated FontRenderer
         */
        static void Delete(FontRenderer *fontRenderer);

        /**
         * Destructor
         */
        virtual ~FontRenderer() = default;
        /**
         * Sets the new font to be used for rendering
         *
         * @param font  The new font to be used for rendering
         *
         * @return true if the font was set correctly or false otherwise
         */
        virtual bool SetFont(std::shared_ptr<const TrueTypeFont> font) = 0;

        inline const TrueTypeFont* GetFont() const { return mFont.get(); }

        /**
         * @param text   Text for evaluation
         * @param scale  Applied font scale
         * @param vertical_spacing  Verical spacing between text lines
         *
         * @return       Bounding rectangle
         */
        virtual glm::uvec2 EvaluateText(const char* text, float scale, uint32_t vertical_spacing) const = 0;

        /**
         * Renders the given text onto the given render target with the indicated color
         * at the indicated (x, y) position on the render target
         *
         * @param text   Text to be rendered
         * @param scale  Applied font scale
         * @param layout (x,y) = starting position for text rendering (in pixels), z = vertical spacing between lines (in pixels)
         * @param color  Color to used for the rendering
         * @param target Render target to render the text on
         */
        virtual void RenderText(const char* text, float scale, const glm::uvec3& layout,
                                const glm::vec4& color, RenderTarget& target) const = 0;

    protected:
       std::shared_ptr<const TrueTypeFont> mFont;
    };
}
