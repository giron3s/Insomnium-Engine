/*
src/textbox.cpp -- Fancy text box with builtin regular
expression-based validation

The text box widget was contributed by Christian Schueller.

NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
The widget drawing code is based on the NanoVG demo application
by Mikko Mononen.

All rights reserved. Use of this source code is governed by a
BSD-style license that can be found in the LICENSE.txt file.
*/

#include "precompiled.h"
#include "json/json.h"
#include "graphic/gui/alignment.h"
#include "graphic/gui/window.h"
#include "graphic/gui/screen.h"
#include "graphic/gui/textbox.h"
#include "graphic/gui/theme.h"
#include "graphic/gui/entypo.h"
#include <regex>
#include <iostream>

#include "engine.h"

namespace Framework
{
    namespace GUI
    {
        UITextBox::UITextBox()
            : UIWidget(nullptr),
            mEditable(false),
            mSpinnable(false),
            mCommitted(true),
            mPasswordField (false),
            mResetOnClick (false),
            mValue("Undefined"),
            mDefaultValue(""),
            mAlignment(Alignment::eCenter),
            mUnits(""),
            mFormat(""),
            mUnitsImage(-1),
            mValidFormat(true),
            mValueTemp("Undefined"),
            mCursorPos(-1),
            mSelectionPos(-1),
            mMousePos(glm::ivec2(-1, -1)),
            mMouseDownPos(glm::ivec2(-1, -1)),
            mMouseDragPos(glm::ivec2(-1, -1)),
            mMouseDownModifier(0),
            mTextOffset(0),
            mLastClick(0) {
            if (mTheme) mFontSize = mTheme->mTextBoxFontSize;
        }

        UITextBox::UITextBox(UIWidget *parent, const string &value)
            : UIWidget(parent),
            mEditable(false),
            mSpinnable(false),
            mCommitted(true),
            mPasswordField(false),
            mResetOnClick(false),
            mValue(value),
            mDefaultValue(""),
            mAlignment(Alignment::eCenter),
            mUnits(""),
            mFormat(""),
            mUnitsImage(-1),
            mValidFormat(true),
            mValueTemp(value),
            mCursorPos(-1),
            mSelectionPos(-1),
            mMousePos(glm::ivec2(-1, -1)),
            mMouseDownPos(glm::ivec2(-1, -1)),
            mMouseDragPos(glm::ivec2(-1, -1)),
            mMouseDownModifier(0),
            mTextOffset(0),
            mLastClick(0) {
            if (mTheme) mFontSize = mTheme->mTextBoxFontSize;
        }

        void UITextBox::setEditable(bool editable) {
            mEditable = editable;
            setCursor(editable ? Cursor::eIBeam : Cursor::eArrow);
        }

        void UITextBox::setTheme(UITheme *theme)
        {
            UIWidget::setTheme(theme);
            if (mTheme)
                mFontSize = mTheme->mTextBoxFontSize;
        }

        glm::ivec2 UITextBox::preferredSize(NVGcontext *ctx) const
        {
            glm::ivec2 size(0, fontSize() * 1.4f);

            float uw = 0;
            if (mUnitsImage > 0) {
                int w, h;
                nvgImageSize(ctx, mUnitsImage, &w, &h);
                float uh = size.y * 0.4f;
                uw = w * uh / h;
            }
            else if (!mUnits.empty()) {
                uw = nvgTextBounds(ctx, 0, 0, mUnits.c_str(), nullptr, nullptr);
            }
            float sw = 0;
            if (mSpinnable) {
                sw = 14.f;
            }

            float ts = nvgTextBounds(ctx, 0, 0, mValue.c_str(), nullptr, nullptr);
            size.x = size.y + ts + uw + sw;
            return size;
        }

        void UITextBox::draw(NVGcontext* ctx)
        {
            UIWidget::draw(ctx);

            NVGpaint bg = nvgBoxGradient(ctx,
                mPos.x + 1, mPos.y + 1 + 1.0f, mSize.x - 2, mSize.y - 2,
                3, 4, Color(255, 32), Color(32, 32));
            NVGpaint fg1 = nvgBoxGradient(ctx,
                mPos.x + 1, mPos.y + 1 + 1.0f, mSize.x - 2, mSize.y - 2,
                3, 4, Color(150, 32), Color(32, 32));
            NVGpaint fg2 = nvgBoxGradient(ctx,
                mPos.x + 1, mPos.y + 1 + 1.0f, mSize.x - 2, mSize.y - 2,
                3, 4, nvgRGBA(255, 0, 0, 100), nvgRGBA(255, 0, 0, 50));

            nvgBeginPath(ctx);
            nvgRoundedRect(ctx, mPos.x + 1, mPos.y + 1 + 1.0f, mSize.x - 2,
                mSize.y - 2, 3);

            if (mEditable && focused())
                mValidFormat ? nvgFillPaint(ctx, fg1) : nvgFillPaint(ctx, fg2);
            else if (mSpinnable && mMouseDownPos.x != -1)
                nvgFillPaint(ctx, fg1);
            else
                nvgFillPaint(ctx, bg);

            nvgFill(ctx);

            nvgBeginPath(ctx);
            nvgRoundedRect(ctx, mPos.x + 0.5f, mPos.y + 0.5f, mSize.x - 1,
                mSize.y - 1, 2.5f);
            nvgStrokeColor(ctx, Color(0, 48));
            nvgStroke(ctx);

            nvgFontSize(ctx, fontSize());
            nvgFontFace(ctx, mTheme->mTextboxFont.c_str());
            glm::ivec2 drawPos(mPos.x, mPos.y + mSize.y * 0.5f + 1);

            float xSpacing = mSize.y * 0.3f;

            float unitWidth = 0;

            if (mUnitsImage > 0) {
                int w, h;
                nvgImageSize(ctx, mUnitsImage, &w, &h);
                float unitHeight = mSize.y * 0.4f;
                unitWidth = w * unitHeight / h;
                NVGpaint imgPaint = nvgImagePattern(
                    ctx, mPos.x + mSize.x - xSpacing - unitWidth,
                    drawPos.y - unitHeight * 0.5f, unitWidth, unitHeight, 0,
                    mUnitsImage, mEnabled ? 0.7f : 0.35f);
                nvgBeginPath(ctx);
                nvgRect(ctx, mPos.x + mSize.x - xSpacing - unitWidth,
                    drawPos.y - unitHeight * 0.5f, unitWidth, unitHeight);
                nvgFillPaint(ctx, imgPaint);
                nvgFill(ctx);
                unitWidth += 2;
            }
            else if (!mUnits.empty()) {
                unitWidth = nvgTextBounds(ctx, 0, 0, mUnits.c_str(), nullptr, nullptr);
                nvgFillColor(ctx, Color(255, mEnabled ? 64 : 32));
                nvgTextAlign(ctx, NVG_ALIGN_RIGHT | NVG_ALIGN_MIDDLE);
                nvgText(ctx, mPos.x + mSize.x - xSpacing, drawPos.y,
                    mUnits.c_str(), nullptr);
                unitWidth += 2;
            }

            float spinArrowsWidth = 0.f;

            if (mSpinnable && !focused()) {
                spinArrowsWidth = 14.f;

                nvgFontFace(ctx, "icons");
                nvgFontSize(ctx, ((mFontSize < 0) ? mTheme->mButtonFontSize : mFontSize) * 1.2f);

                bool spinning = mMouseDownPos.x != -1;

                /* up button */ {
                    bool hover = mMouseFocus && spinArea(mMousePos) == SpinArea::Top;
                    nvgFillColor(ctx, (mEnabled && (hover || spinning)) ? mTheme->mTextColor : mTheme->mDisabledTextColor);
                    auto icon = utf8(ENTYPO_ICON_CHEVRON_UP);
                    nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
                    glm::vec2 iconPos(mPos.x + 4.f,
                        mPos.y + mSize.y / 2.f - xSpacing / 2.f);
                    nvgText(ctx, iconPos.x, iconPos.y, icon.data(), nullptr);
                }

                /* down button */ {
                    bool hover = mMouseFocus && spinArea(mMousePos) == SpinArea::Bottom;
                    nvgFillColor(ctx, (mEnabled && (hover || spinning)) ? mTheme->mTextColor : mTheme->mDisabledTextColor);
                    auto icon = utf8(ENTYPO_ICON_CHEVRON_DOWN);
                    nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
                    glm::vec2 iconPos(mPos.x + 4.f,
                        mPos.y + mSize.y / 2.f + xSpacing / 2.f + 1.5f);
                    nvgText(ctx, iconPos.x, iconPos.y, icon.data(), nullptr);
                }

                nvgFontSize(ctx, fontSize());
                nvgFontFace(ctx, mTheme->mTextboxFont.c_str());
            }

            switch (mAlignment)
            {
            case Alignment::eLeft:
                nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
                drawPos.x += xSpacing + spinArrowsWidth;
                break;
            case Alignment::eRight:
                nvgTextAlign(ctx, NVG_ALIGN_RIGHT | NVG_ALIGN_MIDDLE);
                drawPos.x += mSize.x - unitWidth - xSpacing;
                break;
            case Alignment::eCenter:
                nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
                drawPos.x += mSize.x * 0.5f;
                break;
            }

            nvgFontSize(ctx, fontSize());
            nvgFillColor(ctx, mEnabled && (!mCommitted || !mValue.empty()) ? mTheme->mTextColor : mTheme->mDisabledTextColor);

            // clip visible text area
            float clipX = mPos.x + xSpacing + spinArrowsWidth - 1.0f;
            float clipY = mPos.y + 1.0f;
            float clipWidth = mSize.x - unitWidth - spinArrowsWidth - 2 * xSpacing + 2.0f;
            float clipHeight = mSize.y - 3.0f;

            nvgSave(ctx);
            nvgIntersectScissor(ctx, clipX, clipY, clipWidth, clipHeight);

            glm::ivec2 oldDrawPos(drawPos);
            drawPos.x += mTextOffset;

            if (mCommitted)
            {
                if(mPasswordField)
                    nvgText(ctx, drawPos.x, drawPos.y, mValue.empty() ? getPasswordValue(mPlaceholder).c_str() : getPasswordValue(mValue).c_str(), nullptr);
                else
                    nvgText(ctx, drawPos.x, drawPos.y, mValue.empty() ? mPlaceholder.c_str() : mValue.c_str(), nullptr);
            }
            else 
            {
                std::string lTempValue = mValueTemp;
                if (mPasswordField)
                    mValueTemp = getPasswordValue(mValueTemp);

                const int maxGlyphs = 1024;
                NVGglyphPosition glyphs[maxGlyphs];
                float textBound[4];
                nvgTextBounds(ctx, drawPos.x, drawPos.y, mValueTemp.c_str(),
                    nullptr, textBound);
                float lineh = textBound[3] - textBound[1];

                // find cursor positions
                int nglyphs =
                    nvgTextGlyphPositions(ctx, drawPos.x, drawPos.y,
                        mValueTemp.c_str(), nullptr, glyphs, maxGlyphs);
                updateCursor(ctx, textBound[2], glyphs, nglyphs);

                // compute text offset
                int prevCPos = mCursorPos > 0 ? mCursorPos - 1 : 0;
                int nextCPos = mCursorPos < nglyphs ? mCursorPos + 1 : nglyphs;
                float prevCX = cursorIndex2Position(prevCPos, textBound[2], glyphs, nglyphs);
                float nextCX = cursorIndex2Position(nextCPos, textBound[2], glyphs, nglyphs);

                if (nextCX > clipX + clipWidth)
                    mTextOffset -= nextCX - (clipX + clipWidth) + 1;
                if (prevCX < clipX)
                    mTextOffset += clipX - prevCX + 1;

                drawPos.x = oldDrawPos.x + mTextOffset;

                // draw text with offset
                nvgText(ctx, drawPos.x, drawPos.y, mValueTemp.c_str(), nullptr);
                nvgTextBounds(ctx, drawPos.x, drawPos.y, mValueTemp.c_str(),
                    nullptr, textBound);

                // recompute cursor positions
                nglyphs = nvgTextGlyphPositions(ctx, drawPos.x, drawPos.y,
                    mValueTemp.c_str(), nullptr, glyphs, maxGlyphs);

                if (mCursorPos > -1) {
                    if (mSelectionPos > -1) {
                        float caretx = cursorIndex2Position(mCursorPos, textBound[2],
                            glyphs, nglyphs);
                        float selx = cursorIndex2Position(mSelectionPos, textBound[2],
                            glyphs, nglyphs);

                        if (caretx > selx)
                            swap(caretx, selx);

                        // draw selection
                        nvgBeginPath(ctx);
                        nvgFillColor(ctx, nvgRGBA(255, 255, 255, 80));
                        nvgRect(ctx, caretx, drawPos.y - lineh * 0.5f, selx - caretx,
                            lineh);
                        nvgFill(ctx);
                    }

                    float caretx = cursorIndex2Position(mCursorPos, textBound[2], glyphs, nglyphs);

                    // draw cursor
                    nvgBeginPath(ctx);
                    nvgMoveTo(ctx, caretx, drawPos.y - lineh * 0.5f);
                    nvgLineTo(ctx, caretx, drawPos.y + lineh * 0.5f);
                    nvgStrokeColor(ctx, nvgRGBA(255, 192, 0, 255));
                    nvgStrokeWidth(ctx, 1.0f);
                    nvgStroke(ctx);
                }

                mValueTemp = lTempValue;
            }
            nvgRestore(ctx);
        }

        bool UITextBox::mouseButtonEvent(const glm::ivec2 &p, int button, bool down, int modifiers) 
        {

            if (button == GLFW_MOUSE_BUTTON_1 && down && !mFocused) 
            {
                if (!mSpinnable || spinArea(p) == SpinArea::None) /* not on scrolling arrows */
                    requestFocus();
            }

            if (mEditable && focused()) 
            {
                if (down) 
                {
                    mMouseDownPos = p;
                    mMouseDownModifier = modifiers;

                    if(mResetOnClick)
                        mValueTemp = "";

                    double time = glfwGetTime();
                    if (time - mLastClick < 0.25) 
                    {
                        /* Double-click: select all text */
                        mSelectionPos = 0;
                        mCursorPos = (int)mValueTemp.size();
                        mMouseDownPos = glm::ivec2(-1, -1);
                    }

                    mLastClick = time;
                }
                else 
                {
                    mMouseDownPos = glm::ivec2(-1, -1);
                    mMouseDragPos = glm::ivec2(-1, -1);
                }
                return true;
            }
            else if (mSpinnable && !focused()) 
            {
                if (down) 
                {
                    if (spinArea(p) == SpinArea::None) 
                    {
                        mMouseDownPos = p;
                        mMouseDownModifier = modifiers;

                        double time = glfwGetTime();
                        if (time - mLastClick < 0.25) {
                            /* Double-click: reset to default value */
                            mValue = mDefaultValue;
                            if (mCallback)
                                mCallback(mValue);

                            mMouseDownPos = glm::ivec2(-1, -1);
                        }
                        mLastClick = time;
                    }
                    else 
                    {
                        mMouseDownPos = glm::ivec2(-1, -1);
                        mMouseDragPos = glm::ivec2(-1, -1);
                    }
                }
                else 
                {
                    mMouseDownPos = glm::ivec2(-1, -1);
                    mMouseDragPos = glm::ivec2(-1, -1);
                }

                return true;
            }

            return false;
        }

        bool UITextBox::mouseMotionEvent(const glm::ivec2 &p, const glm::ivec2 & /* rel */,
            int /* button */, int /* modifiers */) {
            mMousePos = p;

            if (!mEditable)
                setCursor(Cursor::eArrow);
            else if (mSpinnable && !focused() && spinArea(mMousePos) != SpinArea::None) /* scrolling arrows */
                setCursor(Cursor::eHand);
            else
                setCursor(Cursor::eIBeam);

            if (mEditable && focused()) {
                return true;
            }
            return false;
        }

        bool UITextBox::mouseDragEvent(const glm::ivec2 &p, const glm::ivec2 &/* rel */,
            int /* button */, int /* modifiers */) {
            mMousePos = p;
            mMouseDragPos = p;

            if (mEditable && focused()) {
                return true;
            }
            return false;
        }

        bool UITextBox::focusEvent(bool focused)
        {
            UIWidget::focusEvent(focused);

            string backup = mValue;

            if (mEditable) {
                if (focused) {
                    mValueTemp = mValue;
                    mCommitted = false;
                    mCursorPos = 0;
                }
                else {
                    if (mValidFormat) {
                        if (mValueTemp == "")
                            mValue = mDefaultValue;
                        else
                            mValue = mValueTemp;
                    }

                    if (mCallback && !mCallback(mValue))
                        mValue = backup;

                    mValidFormat = true;
                    mCommitted = true;
                    mCursorPos = -1;
                    mSelectionPos = -1;
                    mTextOffset = 0;
                }

                mValidFormat = (mValueTemp == "") || checkFormat(mValueTemp, mFormat);
            }

            if(focused == false)
                Engine::Instance()->StateMachine().ExecuteAction("OnTextChanged");

            return true;
        }

        bool UITextBox::keyboardEvent(int key, int /* scancode */, int action, int modifiers) {
            if (mEditable && focused()) {
                if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                    if (key == GLFW_KEY_LEFT) {
                        if (modifiers == GLFW_MOD_SHIFT) {
                            if (mSelectionPos == -1)
                                mSelectionPos = mCursorPos;
                        }
                        else {
                            mSelectionPos = -1;
                        }

                        if (mCursorPos > 0)
                            mCursorPos--;
                    }
                    else if (key == GLFW_KEY_RIGHT) {
                        if (modifiers == GLFW_MOD_SHIFT) {
                            if (mSelectionPos == -1)
                                mSelectionPos = mCursorPos;
                        }
                        else {
                            mSelectionPos = -1;
                        }

                        if (mCursorPos < (int)mValueTemp.length())
                            mCursorPos++;
                    }
                    else if (key == GLFW_KEY_HOME) {
                        if (modifiers == GLFW_MOD_SHIFT) {
                            if (mSelectionPos == -1)
                                mSelectionPos = mCursorPos;
                        }
                        else {
                            mSelectionPos = -1;
                        }

                        mCursorPos = 0;
                    }
                    else if (key == GLFW_KEY_END) {
                        if (modifiers == GLFW_MOD_SHIFT) {
                            if (mSelectionPos == -1)
                                mSelectionPos = mCursorPos;
                        }
                        else {
                            mSelectionPos = -1;
                        }

                        mCursorPos = (int)mValueTemp.size();
                    }
                    else if (key == GLFW_KEY_BACKSPACE) {
                        if (!deleteSelection()) {
                            if (mCursorPos > 0) {
                                mValueTemp.erase(mValueTemp.begin() + mCursorPos - 1);
                                mCursorPos--;
                            }
                        }
                    }
                    else if (key == GLFW_KEY_DELETE) {
                        if (!deleteSelection()) {
                            if (mCursorPos < (int)mValueTemp.length())
                                mValueTemp.erase(mValueTemp.begin() + mCursorPos);
                        }
                    }
                    else if (key == GLFW_KEY_ENTER) {
                        if (!mCommitted)
                            focusEvent(false);
                    }
                    else if (key == GLFW_KEY_A && modifiers == SYSTEM_COMMAND_MOD) {
                        mCursorPos = (int)mValueTemp.length();
                        mSelectionPos = 0;
                    }
                    else if (key == GLFW_KEY_X && modifiers == SYSTEM_COMMAND_MOD) {
                        copySelection();
                        deleteSelection();
                    }
                    else if (key == GLFW_KEY_C && modifiers == SYSTEM_COMMAND_MOD) {
                        copySelection();
                    }
                    else if (key == GLFW_KEY_V && modifiers == SYSTEM_COMMAND_MOD) {
                        deleteSelection();
                        pasteFromClipboard();
                    }

                    mValidFormat =
                        (mValueTemp == "") || checkFormat(mValueTemp, mFormat);
                }

                return true;
            }

            return false;
        }

        bool UITextBox::keyboardCharacterEvent(unsigned int codepoint) {
            if (mEditable && focused()) {
                ostringstream convert;
                convert << (char)codepoint;

                deleteSelection();
                mValueTemp.insert(mCursorPos, convert.str());
                mCursorPos++;

                mValidFormat = (mValueTemp == "") || checkFormat(mValueTemp, mFormat);

                return true;
            }

            return false;
        }

        bool UITextBox::checkFormat(const string &input, const string &format)
        {
            if (format.empty())
                return true;
            try {
                regex regex(format);
                return regex_match(input, regex);
            }
            catch (const regex_error &) {
#if __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 9)
                cerr << "Warning: cannot validate text field due to lacking regular expression support. please compile with GCC >= 4.9" << endl;
                return true;
#else
                throw;
#endif
            }
        }

        bool UITextBox::copySelection() {
            if (mSelectionPos > -1) {
                UIScreen *sc = dynamic_cast<UIScreen *>(this->window()->parent());
                if (!sc)
                    return false;

                int begin = mCursorPos;
                int end = mSelectionPos;

                if (begin > end)
                    swap(begin, end);

                glfwSetClipboardString(sc->glfwWindow(),
                    mValueTemp.substr(begin, end).c_str());
                return true;
            }

            return false;
        }

        void UITextBox::pasteFromClipboard() {
            UIScreen *sc = dynamic_cast<UIScreen *>(this->window()->parent());
            if (!sc)
                return;
            const char* cbstr = glfwGetClipboardString(sc->glfwWindow());
            if (cbstr)
                mValueTemp.insert(mCursorPos, string(cbstr));
        }

        bool UITextBox::deleteSelection() {
            if (mSelectionPos > -1) {
                int begin = mCursorPos;
                int end = mSelectionPos;

                if (begin > end)
                    swap(begin, end);

                if (begin == end - 1)
                    mValueTemp.erase(mValueTemp.begin() + begin);
                else
                    mValueTemp.erase(mValueTemp.begin() + begin,
                        mValueTemp.begin() + end);

                mCursorPos = begin;
                mSelectionPos = -1;
                return true;
            }

            return false;
        }

        void UITextBox::updateCursor(NVGcontext *, float lastx,
            const NVGglyphPosition *glyphs, int size) {
            // handle mouse cursor events
            if (mMouseDownPos.x != -1) {
                if (mMouseDownModifier == GLFW_MOD_SHIFT) {
                    if (mSelectionPos == -1)
                        mSelectionPos = mCursorPos;
                }
                else
                    mSelectionPos = -1;

                mCursorPos =
                    position2CursorIndex(mMouseDownPos.x, lastx, glyphs, size);

                mMouseDownPos = glm::ivec2(-1, -1);
            }
            else if (mMouseDragPos.x != -1) {
                if (mSelectionPos == -1)
                    mSelectionPos = mCursorPos;

                mCursorPos =
                    position2CursorIndex(mMouseDragPos.x, lastx, glyphs, size);
            }
            else {
                // set cursor to last character
                if (mCursorPos == -2)
                    mCursorPos = size;
            }

            if (mCursorPos == mSelectionPos)
                mSelectionPos = -1;
        }

        float UITextBox::cursorIndex2Position(int index, float lastx,
            const NVGglyphPosition *glyphs, int size) {
            float pos = 0;
            if (index == size)
                pos = lastx; // last character
            else
                pos = glyphs[index].x;

            return pos;
        }

        int UITextBox::position2CursorIndex(float posx, float lastx,
            const NVGglyphPosition *glyphs, int size) {
            int mCursorId = 0;
            float caretx = glyphs[mCursorId].x;
            for (int j = 1; j < size; j++) {
                if (abs(caretx - posx) > abs(glyphs[j].x - posx)) {
                    mCursorId = j;
                    caretx = glyphs[mCursorId].x;
                }
            }
            if (abs(caretx - posx) > abs(lastx - posx))
                mCursorId = size;

            return mCursorId;
        }

        UITextBox::SpinArea UITextBox::spinArea(const glm::ivec2 & pos) {
            if (0 <= pos.x - mPos.x && pos.x - mPos.x < 14.f) { /* on scrolling arrows */
                if (mSize.y >= pos.y - mPos.y && pos.y - mPos.y <= mSize.y / 2.f) { /* top part */
                    return SpinArea::Top;
                }
                else if (0.f <= pos.y - mPos.y && pos.y - mPos.y > mSize.y / 2.f) { /* bottom part */
                    return SpinArea::Bottom;
                }
            }
            return SpinArea::None;
        }

        void UITextBox::Serialize(Json::Value& aSerializer) const
        {
            //UIWidget::save(s);
            //s.set("editable", mEditable);
            //s.set("spinnable", mSpinnable);
            //s.set("committed", mCommitted);
            //s.set("value", mValue);
            //s.set("defaultValue", mDefaultValue);
            //s.set("alignment", (int) mAlignment);
            //s.set("units", mUnits);
            //s.set("format", mFormat);
            //s.set("unitsImage", mUnitsImage);
            //s.set("validFormat", mValidFormat);
            //s.set("valueTemp", mValueTemp);
            //s.set("cursorPos", mCursorPos);
            //s.set("selectionPos", mSelectionPos);
        }

        void UITextBox::Deserialize(const Json::Value& aSerializer)
        {
            UIWidget::Deserialize(aSerializer);
            mEditable = aSerializer.get("editable", mEditable).asBool();
            mSpinnable = aSerializer.get("spinnable", mSpinnable).asBool();
            mCommitted = aSerializer.get("committed", mCommitted).asBool();
            mPasswordField = aSerializer.get("passwordField", mPasswordField).asBool();
            mResetOnClick = aSerializer.get("resetOnClick", mResetOnClick).asBool();
            mValue = aSerializer.get("value", mValue).asString();
            mDefaultValue = aSerializer.get("defaultValue", mDefaultValue).asString();
            string lAlignmentStr = aSerializer.get("alignment", AlignmentToString(mAlignment)).asString();
            mAlignment = StringToAlignment(lAlignmentStr.c_str());
            aSerializer.get("units", mUnits).asString();
            mFormat = aSerializer.get("format", mFormat).asString();
            mUnitsImage = aSerializer.get("unitsImage", mUnitsImage).asInt();
            mValidFormat = aSerializer.get("validFormat", mValidFormat).asBool();
            mValueTemp = aSerializer.get("valueTemp", mValueTemp).asString();
            mCursorPos = aSerializer.get("cursorPos", mCursorPos).asInt();
            mSelectionPos = aSerializer.get("selectionPos", mSelectionPos).asInt();
            mMousePos = mMouseDownPos = mMouseDragPos = glm::ivec2(-1);
            mMouseDownModifier = mTextOffset = 0;
        }

        std::string UITextBox::getPasswordValue(string aValue)
        {
            if (aValue.compare("password") == 0)
                return aValue;

            return std::string(aValue.size(), '*');
        }
    }
}
