/*
    src/popupbutton.cpp -- Button which launches a popup widget

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include "precompiled.h"

#include "json/json.h"
#include "graphic/gui/popupbutton.h"
#include "graphic/gui/buttontype.h"
#include "graphic/gui/theme.h"
#include "graphic/gui/entypo.h"
#include "graphic/gui/screen.h"

namespace Framework
{
    namespace GUI
    {

        UIPopupButton::UIPopupButton()
            : UIButton(nullptr, "@UNTITLED", 0)
            , mPopup(nullptr)
            , mChevronIcon(ENTYPO_ICON_CHEVRON_SMALL_RIGHT)
            , mPopupSize(glm::ivec2(320, 320))
        {
            setFlags(ButtonTypeToInt(ButtonType::ePopupButton));
        }


        UIPopupButton::UIPopupButton(UIWidget *aParent, const std::string &aCaption, int aButtonIcon)
            : UIButton(aParent, aCaption, aButtonIcon)
            , mPopup(nullptr)
            , mChevronIcon(ENTYPO_ICON_CHEVRON_SMALL_RIGHT)
            , mPopupSize(glm::ivec2(320, 320))
        {
            setFlags(ButtonTypeToInt(ButtonType::ePopupButton));
        }


        void UIPopupButton::createPopup()
        {
            if(mPopup)
                return;
            mPopup = new UIPopup(window(), this);
            mPopup->setSize(mPopupSize);
            mPopup->setVisible(false);
            mPopup->setAppearance(UIPopup::Appearance::Down | UIPopup::Appearance::Menu);
        }


        UIPopupButton::~UIPopupButton()
        {
            //  Since ComboBox widgets are directly added to the first UIWindow in the heirarchy,
            //  we should call removeChild on the parent UIWindow instead of deleting it in the 
            //  UIPopupButton destructor.
            window()->removeChild(mPopup);
            //delete mPopup;
        }


        glm::ivec2 UIPopupButton::preferredSize(NVGcontext *ctx) const 
        {
            return UIButton::preferredSize(ctx) + glm::ivec2(15, 0);
        }

        void UIPopupButton::draw(NVGcontext* ctx) 
        {
            if (!mEnabled && mPushed)
                mPushed = false;

            UIButton::draw(ctx);

            if(mPopup)
                mPopup->setVisible(mPushed);

            if (mChevronIcon) 
            {
                auto icon = utf8(mChevronIcon);
                NVGcolor textColor =
                    mTextColor.a() == 0 ? mTheme->mTextColor : mTextColor;

                nvgFontSize(ctx, (mFontSize < 0 ? mTheme->mButtonFontSize : mFontSize) * 1.5f);
                nvgFontFace(ctx, "icons");
                nvgFillColor(ctx, mEnabled ? textColor : mTheme->mDisabledTextColor);
                nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);

                float iw = nvgTextBounds(ctx, 0, 0, icon.data(), nullptr, nullptr);
                glm::vec2 iconPos(0, mPos.y + mSize.y * 0.5f - 1);

                if(mPopup)
                {
                    if(mPopup->appearance() & UIPopup::Left)
                        iconPos.x = mPos.x + 8;
                    else if(mPopup->appearance() & UIPopup::Right)
                        iconPos.x = mPos.x + mSize.x - iw - 8;
                    else
                        ASSERT(0 && "Popup appearance case not implemented");
                }

                nvgText(ctx, iconPos.x, iconPos.y, icon.data(), nullptr);
            }
        }

        void UIPopupButton::performLayout(NVGcontext *ctx)
        {
            UIButton::performLayout(ctx);
            if ( !mPopup )
                return;

            const UIWindow *lParentWindow = window();
            int lPosY = absolutePosition().y - lParentWindow->position().y + mSize.y / 2;
            switch ( mPopup->appearance() )
            {
                case UIPopup::Left:
                {
                    mPopup->setAnchorPos(glm::ivec2(0 - 15, lPosY));
                    break;
                }

                case UIPopup::Right:
                {
                    const auto lAbsPos = absolutePosition(); // fishy...
                    mPopup->setAnchorPos(glm::ivec2(lAbsPos.x + mSize.x, lAbsPos.y));
                    break;
                }

                case UIPopup::Down:
                case UIPopup::Up:
                {
                    ASSERT(0 && "Popup appearance case not implemented");
                    break;
                }
                
                case (UIPopup::Right | UIPopup::Menu): 
                {
                    const auto lAbsPos = absolutePosition(); // fishy...
                    mPopup->setAnchorPos(glm::ivec2(lAbsPos.x + mSize.x, lAbsPos.y));
                    break; 
                }

                case (UIPopup::Down | UIPopup::Menu):
                {
                    const auto lAbsPos = absolutePosition(); // fishy...
                    mPopup->setAnchorPos(glm::ivec2(lAbsPos.x, lAbsPos.y + mSize.y));
                break; 
                }

                case (UIPopup::Left | UIPopup::Menu):
                case (UIPopup::Up | UIPopup::Menu):
                    ASSERT(0 && "Popup appearance case not implemented");
                break;
            }
            
            mPopup->performLayout(ctx);
        }


        void UIPopupButton::CollapseChildPopups(UIWidget* aItem)
        {
            UIButton* lButton = dynamic_cast<UIButton*>(aItem);
            if( !lButton )
                return;
            // else could be a separator
            lButton->setPushed(false);
            UIPopupButton* lPopupButton = dynamic_cast<UIPopupButton*>(lButton);
            if( lPopupButton && lPopupButton->popup() )
            {
                lPopupButton->popup()->setVisible(false);
                for(auto& lItem : lPopupButton->popup()->children())
                    CollapseChildPopups(lItem);
            }
        }

 
        bool UIPopupButton::mouseButtonEvent(const glm::ivec2 &p, int button, bool down, int modifiers)
        {
            bool res = UIButton::mouseButtonEvent(p, button, down, modifiers);

            UIPopup* lParentPopup = dynamic_cast<UIPopup*>(mParent);
            if(!lParentPopup)
            {
                UIScreen::sActivePopupButton = this;
                return res;
            }

            if( mPopup &&
                (mPopup->appearance() & UIPopup::Appearance::Menu) )
            {
                if ( mPushed )
                {
                    // collapse all other popups on this level
                    for ( auto& lItem : lParentPopup->children() )
                        if ( lItem != this )
                            CollapseChildPopups(lItem);
                    // this mPopup will be shown
                }
                // else this mPopup will be hidden
            }

            else
            { // find a "root" popupbutton and collapse all popups
                UIPopupButton* lRootButton = nullptr;
                do
                {
                    lRootButton = lParentPopup->parentButton();
                    ASSERT(lRootButton);
                    lParentPopup = dynamic_cast<UIPopup*>(lRootButton->parent());
                } while ( lParentPopup );
                CollapseChildPopups(lRootButton);
            }

            return res;
        }

            
        void UIPopupButton::setAppearance(uint32_t appearance)
        {
            if ( !mPopup )
                return;

            if(mCaption.empty() && !mIcon)
            {
                if ((mPopup->appearance() & UIPopup::Left) && mChevronIcon == ENTYPO_ICON_CHEVRON_SMALL_LEFT)
                    mChevronIcon = ENTYPO_ICON_CHEVRON_SMALL_RIGHT;
                else if ((mPopup->appearance() & UIPopup::Right) && mChevronIcon == ENTYPO_ICON_CHEVRON_SMALL_RIGHT)
                    mChevronIcon = ENTYPO_ICON_CHEVRON_SMALL_LEFT;
                else if ((mPopup->appearance() & UIPopup::Down) && mChevronIcon == ENTYPO_ICON_CHEVRON_SMALL_DOWN)
                    mChevronIcon = ENTYPO_ICON_CHEVRON_SMALL_DOWN;
                else if ((mPopup->appearance() & UIPopup::Up) && mChevronIcon == ENTYPO_ICON_CHEVRON_SMALL_UP)
                    mChevronIcon = ENTYPO_ICON_CHEVRON_SMALL_UP;
            }
            else
                mChevronIcon = 0;
            mPopup->setAppearance(appearance);
        }

        void UIPopupButton::Serialize(Json::Value& aSerializer) const
        {
            //UIButton::Serialize(aSerializer);
            //s.set("chevronIcon", mChevronIcon);
        }

        static void DeserializePopupItem(const Json::Value& aSerializer, UIWidget* aParent)
        {
            string lPopupItemType = aSerializer["type"].asString();
            UIWidget* lItemWidget = nullptr;
            // Child is Button
            if( lPopupItemType.compare(Framework::Utils::Demangling(typeid(UIButton).name()) ) == 0 )
                lItemWidget = new UIButton;
            //Child is PopupButton
            else if (lPopupItemType.compare(Framework::Utils::Demangling(typeid(UIPopupButton).name()) ) == 0 )
                lItemWidget = new UIPopupButton;

            // Invalid type of popup item 
            if ( !lItemWidget )
                CRASH("Object of type '%s' is not allowed as a popup item!", lPopupItemType.c_str());

            lItemWidget->setParent(aParent); //Set parent
            aParent->addChild(lItemWidget);  //Set child

            lItemWidget->Deserialize(aSerializer);

            if (aSerializer.isMember("items"))
            {
                for ( const auto& lItem : aSerializer["items"] )
                    DeserializePopupItem(lItem, lItemWidget);
            }
        }

        void UIPopupButton::Deserialize(const Json::Value& aSerializer)
        {
            ASSERT(!mPopup);
            UIButton::Deserialize(aSerializer);
            if(mCaption.empty())
                mChevronIcon = aSerializer.get("chevronIcon", mChevronIcon).asInt();
            else
                mChevronIcon = 0;

            UIPopup* lParentPopup = dynamic_cast<UIPopup*>(this->mParent);
            if(!hasFontSize())
            {
                if(lParentPopup)
                    setFontSize(lParentPopup->fontSize());
                //else
                //    CRASH("Font size for PopupButton '%s' is not specified", id().c_str());
            }

            if (aSerializer.isMember("popup"))
            {
                createPopup();
                setAppearance(UIPopup::Appearance::Right);

                const auto& lPopupChild = aSerializer["popup"];
                mPopup->Deserialize(lPopupChild);
                if(!mPopup->layout())
                {
                    if(lParentPopup && lParentPopup->layout())
                        mPopup->setLayout(lParentPopup->layout());
                    else
                        CRASH("Layout for Popup '%s' is not specified", mPopup->id().c_str());
                }
                if(!mPopup->hasFontSize())
                {
                    if(hasFontSize())
                        mPopup->setFontSize(fontSize());
                    //else
                    //    CRASH("Font size for Popup '%s' is not specified", mPopup->id().c_str());
                   
                }
                ASSERT(mPopup->layout() /*&& mPopup->hasFontSize()*/);

                if (lPopupChild.isMember("items"))
                    for (const auto& lItem : lPopupChild["items"])
                        DeserializePopupItem(lItem, mPopup);
            }
        }
    }
}
