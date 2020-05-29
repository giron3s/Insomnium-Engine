/*
    src/combobox.cpp -- simple combo box widget based on a popup button

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include "precompiled.h"

#include "json/json.h"
#include "graphic/gui/combobox.h"
#include "graphic/gui/layout.h"
#include "graphic/gui/buttontype.h"
#include "graphic/gui/entypo.h"

namespace Framework
{
    namespace GUI
    {
        UIComboBox::UIComboBox()
            : UIPopupButton()
            , mSelectedIndex(0)
            , mIgnoreParentPosition(false)
        {
        }

        UIComboBox::UIComboBox(UIWidget *aParent) 
            : UIPopupButton(aParent)
            , mSelectedIndex(0)
            , mIgnoreParentPosition(false)
        {
            createPopup();
        }

        UIComboBox::UIComboBox(UIWidget *aParent, const vector<string> &aItems)
            : UIPopupButton(aParent)
            , mSelectedIndex(0)
            , mIgnoreParentPosition(false)
        {
            setItems(aItems);
        }

        UIComboBox::UIComboBox(UIWidget *aParent, const vector<string> &aItems, const vector<string> &aItemsShort)
            : UIPopupButton(aParent)
            , mSelectedIndex(0)
            , mIgnoreParentPosition(false)
        {
            setItems(aItems, aItemsShort);
        }

        void UIComboBox::performLayout(NVGcontext *ctx)
        {
            UIPopupButton::performLayout(ctx);

            if (!mPopup)
                return;

            const UIWindow *parentWindow = window();
            int posY = absolutePosition().y - parentWindow->position().y + mSize.y / 2;
            switch (mPopup->appearance())
            {
            case (UIPopup::Right | UIPopup::Menu): {
                const auto lAbsPos = absolutePosition(); // fishy...
                mPopup->setAnchorPos(glm::ivec2(lAbsPos.x + mSize.x, lAbsPos.y));
                break; }

            case (UIPopup::Down | UIPopup::Menu): {
                const auto lAbsPos = absolutePosition(); // fishy...
                if(mIgnoreParentPosition)
                    mPopup->setAnchorPos(glm::ivec2(mPos.x, mPos.y + mSize.y));
                else
                    mPopup->setAnchorPos(glm::ivec2(lAbsPos.x - parentWindow->position().x, lAbsPos.y - parentWindow->position().y + mSize.y));
                break; }

            case (UIPopup::Up | UIPopup::Menu): {
                const auto lAbsPos = absolutePosition(); // fishy...
                if(mIgnoreParentPosition)
                    mPopup->setAnchorPos(glm::ivec2(mPos.x, mPos.y));
                else
                    mPopup->setAnchorPos(glm::ivec2(lAbsPos.x - parentWindow->position().x, lAbsPos.y - parentWindow->position().y));
                break; }
            }

            mPopup->performLayout(ctx);
            mPopup->setFixedWidth(mFixedSize.x);
        }

        void UIComboBox::setSelectedIndex(int idx) 
        {
            if (mItemsShort.empty())
                return;
            const vector<UIWidget *> &children = popup()->children();
            ((UIButton *)children[mSelectedIndex])->setPushed(false);
            ((UIButton *)children[idx])->setPushed(false);
            mSelectedIndex = idx;
            setCaptionUnlocalized(mItemsShort[idx]);
        }

        void UIComboBox::setItems(const vector<string> &items, const vector<string> &itemsShort, vector<int> itemIcons, vector<bool> aRemovables)
        {
            ASSERT(items.size() == itemsShort.size());
            ASSERT(itemIcons.size() == 0 || items.size() == itemIcons.size());

            createPopup();

            mItems = items;
            mItemsShort = itemsShort;
            if (mSelectedIndex < 0 || mSelectedIndex >= (int)items.size())
                mSelectedIndex = 0;

            while (mPopup->childCount() != 0)
                mPopup->removeChild(mPopup->childCount() - 1);

            if(aRemovables.size() > 0)
                mPopup->setLayout(new UIGridLayout());
            else
                mPopup->setLayout(new UIGroupLayout());

            int index = 0;
            for (const auto &lItem : items)
            {
                UIButton* lButton = nullptr;

                //Combobox without icon
                if (itemIcons.size() == 0)
                    lButton = new UIButton(mPopup, "");
                //Combobox with icon
                else
                    lButton = new UIButton(mPopup, "", itemIcons.at(index));

                lButton->setTheme(mTheme);
                lButton->setFlags(ButtonTypeToInt(ButtonType::eNormalButton));
                lButton->setFixedSize(mFixedSize);
                lButton->setIconPosition(Framework::GUI::IconPosition::eLeft);
                lButton->setCaptionAlignment(Alignment::eLeft, 50);
                lButton->setCaptionUnlocalized(lItem);

                lButton->setCallback([&, index] {
                    mSelectedIndex = index;
                    setCaptionUnlocalized(mItemsShort[index]);
                    setPushed(false);
                    popup()->setVisible(false);
                    if (mCallback)
                        mCallback(index);
                });

                if (aRemovables.size() > 0 && aRemovables.at(index) == true)
                {
                    lButton = new UIButton(mPopup, "", ENTYPO_ICON_CROSS);
                    lButton->setFlags(ButtonTypeToInt(ButtonType::eNormalButton));
                    lButton->setFixedSize(glm::ivec2(mFixedSize.x * 0.4f, mFixedSize.y));
                    lButton->setIconPosition(Framework::GUI::IconPosition::eLeft);
                    lButton->setCaptionAlignment(Alignment::eLeft, 0);

                    lButton->setCallback([&, index] {
                        popup()->setVisible(false);
                        if (mRemoveCallback)
                            mRemoveCallback(index);
                    });
                }
                
                index++;
            }

            setSelectedIndex(mSelectedIndex);
            mPopup->setAnchorPos(glm::vec2(0, 0));
            mPopup->setAnchorHeight(0);
        }

        bool UIComboBox::scrollEvent(const glm::ivec2 &p, const glm::vec2 &rel) 
        {
            if (rel.y < 0) {
                setSelectedIndex(min(mSelectedIndex + 1, (int)(items().size() - 1)));
                if (mCallback)
                    mCallback(mSelectedIndex);
                return true;
            }
            else if (rel.y > 0) {
                setSelectedIndex(max(mSelectedIndex - 1, 0));
                if (mCallback)
                    mCallback(mSelectedIndex);
                return true;
            }
            return UIWidget::scrollEvent(p, rel);
        }

        void UIComboBox::Serialize(Json::Value& aSerializer) const
        {
            //Widget::save(s);
            //s.set("items", mItems);
            //s.set("itemsShort", mItemsShort);
            //s.set("selectedIndex", mSelectedIndex);
        }

        void UIComboBox::Deserialize(const Json::Value& aSerializer)
        {
            UIPopupButton::Deserialize(aSerializer);

            vector<string> lItems;
            vector<string> lItemsShort;

            if (aSerializer.isMember("items") )
            { 
                const Json::Value lResItems = aSerializer["items"];
                for (size_t i = 0, lSize = lResItems.size(); i < lSize; ++i)
                    lItems.push_back(lResItems[static_cast<int>(i)].asString());
            }
            if (aSerializer.isMember("itemsShort"))
            {
                const Json::Value lResItemsShort = aSerializer["itemsShort"];
                for (size_t i = 0, lSize = lResItemsShort.size(); i < lSize; ++i)
                    lItemsShort.push_back(lResItemsShort[static_cast<int>(i)].asString());
            }
            mSelectedIndex = aSerializer.get("selectedIndex", mSelectedIndex).asInt();


            if (mSelectedIndex < 0 || mSelectedIndex >= lItems.size())
            {
                CRASH("Invalid value of the selectedIndex!");
                return;
            }

            if (lItems.size() == 0)
            {
                CRASH("The items attribute is empty!");
                return;
            }

            else if(lItemsShort.size() != 0 && lItems.size() != lItemsShort.size())
            {
                CRASH("Different size of items and itemsShort! (%zu - %zu)", mItems.size(), mItemsShort.size())
                return;
            }

            //Set the items
            if(lItemsShort.empty())
                setItems(lItems);
            else
                setItems(lItems, lItemsShort);
        }

        void UIComboBox::setSelectedValue(string aValue)
        {
            vector<string>::iterator lIter = std::find(mItems.begin(), mItems.end(), aValue);
            if (lIter == mItems.end())
            {
                WARNING("UIComboBox::setSelectedValue - '%s' value does not exist", aValue.c_str());
                return;
            }

            setSelectedIndex(std::distance(mItems.begin(), lIter));
        }
    } 
}
