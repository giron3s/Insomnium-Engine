/*
    nanogui/messagedialog.h -- Simple "OK" or "Yes/No"-style modal dialogs

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/** \file */

#pragma once

#include "graphic/gui/window.h"

namespace Framework
{
    namespace GUI
    {
        /**
        * \class MessageDialog messagedialog.h nanogui/messagedialog.h
        *
        * \brief Simple "OK" or "Yes/No"-style modal dialogs.
        */
        class UIMessageDialog : public UIWindow 
        {
        public:
            /// Classification of the type of message this MessageDialog represents.
            enum class Type
            {
                eInformation,
                eQuestion,
                eWarning,
                eMaxMessageDialogType
            };

            UIMessageDialog();

            UIMessageDialog(UIWidget *aParent, 
                            Type aType,
                            const std::string &aTitle = "@UNTITLED",
                            const std::string &aMessage = "Message",
                            const std::string &aButtonText = "OK",
                            const std::string &aAltButtonText = "Cancel", 
                            bool aAltButton = false);

            UILabel *messageLabel() { return mMessageLabel; }
            const UILabel *messageLabel() const { return mMessageLabel; }

            std::function<void(int)> callback() const { return mCallback; }
            void setCallback(const std::function<void(int)> &callback) { mCallback = callback; }

            /// Save the state of the messageDialog into the given \ref Serializer instance
            virtual void Serialize(Json::Value& aSerializer) const override;

            /// Restore the state of the messageDialog from the given \ref Serializer instance
            virtual void Deserialize(const Json::Value& aSerializer) override;

        private:
            void createMessageDialog();

            Type strToMessageDialogType(const std::string& aType);

        protected:
            std::function<void(int)> mCallback;
            UILabel*                 mMessageLabel;

            Type                     mDialogType;

            std::string              mMessage;
            std::string              mButtonText;
            std::string              mAltButtonText;

            bool                     mAltButton;

        //public:
        //    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        };
    }
}
