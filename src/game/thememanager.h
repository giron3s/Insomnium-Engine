/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2019 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#pragma once

#include "core/utils.h"
#include "engine/basemanager.h"
#include "graphic/gui/theme.h"

namespace Game
{
    class ThemeManager : public Framework::BaseManager
    {
    public:
                        ThemeManager();
                        ~ThemeManager() override;

        void            Initialize() override;
        void            DeInitialize() override;

        void            SelectElement(std::string aElement);
        void            SetValue(std::string aValue);

        void            ApplyTheme();
        void            SaveTheme();
        void            LoadTheme();

    private:

        UITheme*        mCustomTheme;
        std::string     mSelectedElement;
    };
}
