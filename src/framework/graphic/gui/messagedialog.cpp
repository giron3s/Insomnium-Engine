/*
    src/messagedialog.cpp -- Simple "OK" or "Yes/No"-style modal dialogs

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include "precompiled.h"

#include "json/json.h"
#include "graphic/gui/messagedialog.h"
#include "graphic/gui/layout.h"
#include "graphic/gui/button.h"
#include "graphic/gui/label.h"
#include "graphic/gui/entypo.h"
#include "graphic/gui/theme.h"

namespace Framework
{
    namespace GUI
    {

        UIMessageDialog::UIMessageDialog()
            : UIWindow()
            , mDialogType(Type::eMaxMessageDialogType)
            , mMessage("")
            , mButtonText("")
            , mAltButtonText("")
            , mAltButton(false)
        {

        }

        UIMessageDialog::UIMessageDialog( UIWidget *aParent, 
                                          Type aDialogType,
                                          const string &aTitle,
                                          const string &aMessage,
                                          const string &aButtonText,
                                          const string &aAltButtonText, 
                                          bool aAltButton) 
            : UIWindow(aParent, aTitle)
            , mDialogType(aDialogType)
            , mMessage(aMessage)
            , mButtonText(aButtonText)
            , mAltButtonText(aAltButtonText)
            , mAltButton(aAltButton)
        {
            createMessageDialog();
        }

        void UIMessageDialog::createMessageDialog()
        {
            setLayout(new UIBoxLayout(Orientation::eVertical, Alignment::eMiddle, 10, 10));
            setModal(true);

            UIWidget *panel1 = new UIWidget(this);
            panel1->setLayout(new UIBoxLayout(Orientation::eHorizontal, Alignment::eMiddle, 10, 15));
            int icon = 0;
            switch (mDialogType)
            {
                case Type::eInformation: icon = mTheme->mMessageInformationIcon; break;
                case Type::eQuestion: icon = mTheme->mMessageQuestionIcon; break;
                case Type::eWarning: icon = mTheme->mMessageWarningIcon; break;
                case Type::eMaxMessageDialogType: WARNING("Invalid message dialog type!"); break;
            }

            UILabel *iconLabel = new UILabel(panel1, string(utf8(icon).data()), "icons");
            iconLabel->setFontSize(50);
            mMessageLabel = new UILabel(panel1, mMessage);
            mMessageLabel->setFixedWidth(200);
            UIWidget *panel2 = new UIWidget(this);
            panel2->setLayout(new UIBoxLayout(Orientation::eHorizontal, Alignment::eMiddle, 0, 15));

            if (mAltButton) {
                UIButton *button = new UIButton(panel2, mAltButtonText, mTheme->mMessageAltButtonIcon);
                button->setCallback([&] { if (mCallback) mCallback(1); dispose(); });
            }
            UIButton *button = new UIButton(panel2, mButtonText, mTheme->mMessagePrimaryButtonIcon);
            button->setCallback([&] { if (mCallback) mCallback(0); dispose(); });
            center();
            requestFocus();
        }


        UIMessageDialog::Type UIMessageDialog::strToMessageDialogType(const std::string& aType)
        {
            Type lType;

            if (aType.compare("information") == 0)
            {
                lType = Type::eInformation;
            }
            else if (aType.compare("question") == 0)
            {
                lType = Type::eQuestion;
            }
            else if (aType.compare("warning") == 0)
            {
                lType = Type::eWarning;
            }
            else
            {
                lType = Type::eMaxMessageDialogType;
            }
            return lType;
        }


        void UIMessageDialog::Serialize(Json::Value& aSerializer) const
        {
        
        }

        void UIMessageDialog::Deserialize(const Json::Value& aSerializer)
        {
            UIWindow::Deserialize(aSerializer);

            mTitle = aSerializer.get("title", mTitle).asString();
            mMessage = aSerializer.get("message", mMessage).asString();
            mButtonText = aSerializer.get("buttonText", mButtonText).asString();
            mDialogType = strToMessageDialogType(aSerializer.get("dialogType", "unknow").asString());

            if (aSerializer.isMember("altButton") && aSerializer["altButton"].asBool())
            {
                mAltButton = true;
                mAltButtonText = aSerializer.get("altButtonText", mAltButtonText).asString();
            }

            createMessageDialog();
        }
    }
}
