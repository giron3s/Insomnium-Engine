/*
    nanogui/tabwidget.h -- A wrapper around the widgets TabHeader and StackedWidget
    which hooks the two classes together.

    The tab widget was contributed by Stefan Ivanov.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/** \file */

#pragma once

#include "graphic/gui/widget.h"
#include <functional>

namespace Framework
{
    namespace GUI
    {
        /**
         * \class TabWidget tabwidget.h nanogui/tabwidget.h
         *
         * \brief A wrapper around the widgets TabHeader and StackedWidget which hooks
         *        the two classes together.
         *
         * \rst
         *
         * .. warning::
         *
         *    Unlike other widgets, children may **not** be added *directly* to a
         *    TabWidget.  For example, the following code will raise an exception:
         *
         *    .. code-block:: cpp
         *
         *       // `this` might be say a nanogui::Screen instance
         *       Window *window = new Window(this, "Window Title");
         *       TabWidget *tabWidget = window->add<TabWidget>();
         *       // this label would be a direct child of tabWidget,
         *       // which is forbidden, so an exception will be raised
         *       new Label(tabWidget, "Some Label");
         *
         *    Instead, you are expected to be creating tabs and adding widgets to those.
         *
         *    .. code-block:: cpp
         *
         *       // `this` might be say a nanogui::Screen instance
         *       Window *window = new Window(this, "Window Title");
         *       TabWidget *tabWidget = window->add<TabWidget>();
         *       // Create a tab first
         *       auto *layer = tabWidget->createTab("Tab Name");
         *       // Add children to the created tabs
         *       layer->setLayout(new GroupLayout());
         *       new Label(layer, "Some Label");
         *
         *    A slightly more involved example of creating a TabWidget can also be found
         *    in :ref:`nanogui_example_1` (search for ``tabWidget`` in the file).
         *
         * \endrst
         */
        class UITabWidget : public UIWidget {
        public:

            UITabWidget();

            UITabWidget(UIWidget *aParent);

            /**
             * \brief Forcibly prevent mis-use of the class by throwing an exception.
             *        Children are not to be added directly to the TabWidget, see
             *        the class level documentation (\ref TabWidget) for an example.
             *
             * \throws std::runtime_error
             *     An exception is always thrown, as children are not allowed to be
             *     added directly to this Widget.
             */
            virtual void addChild(int index, UIWidget *widget) override;

            void setActiveTab(int tabIndex);
            int activeTab() const;
            int tabCount() const;

            /**
             * Sets the callable objects which is invoked when a tab is changed.
             * The argument provided to the callback is the index of the new active tab.
             */
            void setCallback(const std::function<void(int)> &callback) { mCallback = callback; };
            const std::function<void(int)> &callback() const { return mCallback; }

            /// Creates a new tab with the specified name and returns a pointer to the layer.
            UIWidget* createTab(const std::string &label);
            UIWidget* createTab(int index, const std::string &label);

            /// Inserts a tab at the end of the tabs collection and associates it with the provided widget.
            void addTab(const std::string &label, UIWidget* aTab);

            /// Inserts a tab into the tabs collection at the specified index and associates it with the provided widget.
            void addTab(int index, const std::string &label, UIWidget* aTab);

            /**
             * Removes the tab with the specified label and returns the index of the label.
             * Returns whether the removal was successful.
             */
            bool removeTab(const std::string &label);

            /// Removes the tab with the specified index.
            void removeTab(int index);

            /// Retrieves the label of the tab at a specific index.
            const std::string &tabLabelAt(int index) const;

            /**
             * Retrieves the index of a specific tab using its tab label.
             * Returns -1 if there is no such tab.
             */
            int tabLabelIndex(const std::string &label);

            /**
             * Retrieves the index of a specific tab using a widget pointer.
             * Returns -1 if there is no such tab.
             */
            int tabIndex(UIWidget* tab);

            /**
             * This function can be invoked to ensure that the tab with the provided
             * index the is visible, i.e to track the given tab. Forwards to the tab
             * header widget. This function should be used whenever the client wishes
             * to make the tab header follow a newly added tab, as the content of the
             * new tab is made visible but the tab header does not track it by default.
             */
            void ensureTabVisible(int index);

            /**
             * \brief Returns a ``const`` pointer to the Widget associated with the
             *        specified label.
             *
             * \param label
             *     The label used to create the tab.
             *
             * \return
             *     The Widget associated with this label, or ``nullptr`` if not found.
             */
            const UIWidget *tab(const std::string &label) const;

            /**
             * \brief Returns a pointer to the Widget associated with the specified label.
             *
             * \param label
             *     The label used to create the tab.
             *
             * \return
             *     The Widget associated with this label, or ``nullptr`` if not found.
             */
            UIWidget *tab(const std::string &label);

            /**
             * \brief Returns a ``const`` pointer to the Widget associated with the
             *        specified index.
             *
             * \param index
             *     The current index of the desired Widget.
             *
             * \return
             *     The Widget at the specified index, or ``nullptr`` if ``index`` is not
             *     a valid index.
             */
            const UIWidget *tab(int index) const;

            /**
             * \brief Returns a pointer to the Widget associated with the specified index.
             *
             * \param index
             *     The current index of the desired Widget.
             *
             * \return
             *     The Widget at the specified index, or ``nullptr`` if ``index`` is not
             *     a valid index.
             */
            UIWidget *tab(int index);

            virtual void performLayout(NVGcontext* ctx) override;
            virtual glm::ivec2 preferredSize(NVGcontext* ctx) const override;
            virtual void draw(NVGcontext* ctx) override;

            /// Save the state of the tabwidget into the given \ref Serializer instance
            virtual void Serialize(Json::Value& aSerializer) const override;

            /// Restore the state of the tabwidget from the given \ref Serializer instance
            virtual void Deserialize(const Json::Value& aSerializer) override;
        private:
            UITabHeader*             mHeader;
            UIStackedWidget*         mContent;
            std::function<void(int)> mCallback;
            //public:
            //    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        };
    }
}
