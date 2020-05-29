/*
    nanogui/combobox.h -- simple combo box widget based on a popup button

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/**
 * \file nanogui/combobox.h
 *
 * \brief Simple combo box widget based on a popup button.
 */

#pragma once

#include <vector>
#include "graphic/gui/popupbutton.h"

namespace Framework
{
    namespace GUI
    {
        /**
        * \class ComboBox combobox.h nanogui/combobox.h
        *
        * \brief Simple combo box widget based on a popup button.
        */
        class UIComboBox : public UIPopupButton 
        {
        public:
            /// Create an empty combo box
            UIComboBox();

            /// Create an empty combo box
            UIComboBox(UIWidget *aParent);

            /// Create a new combo box with the given items
            UIComboBox(UIWidget *aParent, const std::vector<std::string> &aItems);

            /**
            * \brief Create a new combo box with the given items, providing both short and
            * long descriptive labels for each item
            */
            UIComboBox(UIWidget *aParent, const std::vector<std::string> &aItems, const std::vector<std::string> &aItemsShort);

            /// The callback to execute for this ComboBox.
            std::function<void(int)> callback() const { return mCallback; }

            /// Sets the callback to execute for this ComboBox.
            void setCallback(const std::function<void(int)> &callback) { mCallback = callback; }

            /// Sets the callback to execute for this ComboBox.
            void setRemoveCallback(const std::function<void(int)> &callback) { mRemoveCallback = callback; }

            /// The current index this ComboBox has selected.
            int selectedIndex() const { return mSelectedIndex; }

            /// Sets the current index this ComboBox has selected.
            void setSelectedIndex(int idx);

            /// Sets the items for this ComboBox, providing both short and long descriptive lables and icons for each item.
            void setItems(const std::vector<std::string> &items, const std::vector<std::string> &itemsShort, std::vector<int> itemIcons = vector<int>(), std::vector<bool> aRemovables = vector<bool>());

            /// Sets the items for this ComboBox.
            void setItems(const std::vector<std::string> &items) { setItems(items, items); }

            /// The items associated with this ComboBox.
            const std::vector<std::string> &items() const { return mItems; }

            /// The short descriptions associated with this ComboBox.
            const std::vector<std::string> &itemsShort() const { return mItemsShort; }

            /// Handles mouse scrolling events for this ComboBox.
            virtual bool scrollEvent(const glm::ivec2 &p, const glm::vec2 &rel) override;

            /// Saves the state of this ComboBox to the specified Serializer.
            virtual void Serialize(Json::Value& aSerializer) const override;

            /// Sets the state of this ComboBox from the specified Serializer.
            virtual void Deserialize(const Json::Value& aSerializer) override;

            virtual void performLayout(NVGcontext *ctx) override;

            void ignoreParentPosition(bool aIgnore) { mIgnoreParentPosition = aIgnore; }

            const std::string getSelectedValue() { return mItems.at(mSelectedIndex); }
            void        setSelectedValue(std::string aValue);

        protected: 
            std::vector<std::string>    mItems;         /// The items associated with this ComboBox.
            std::vector<std::string>    mItemsShort;    /// The short descriptions of items associated with this ComboBox.
            std::function<void(int)>    mCallback;      /// The callback for this ComboBox.
            std::function<void(int)>    mRemoveCallback;/// The callback for remove buttons next to removable items.
            int                         mSelectedIndex; /// The current index this ComboBox has selected.
            bool                        mIgnoreParentPosition; ///  Set this variable to true for ComboBoxes that are direct children of main window.

        //public:
        //    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        };
    }
}
