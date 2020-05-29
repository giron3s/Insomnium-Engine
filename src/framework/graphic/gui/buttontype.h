/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#pragma once

namespace Framework
{
    namespace GUI
    {
        // Flags to specify the button behavior (can be combined with binary OR)
        // NormalButton = (1 << 0) < A normal Button.
        // RadioButton  = (1 << 1) < A radio Button.
        // ToggleButton = (1 << 2) < A toggle Button.
        // PopupButton  = (1 << 3) < A popup Button.
        #define ButtonTypes    \
            X( eNormalButton,  1 ) \
            X( eRadioButton,   2 ) \
            X( eToggleButton,  4 ) \
            X( ePopupButton,   8 ) \

        enum ButtonType
        {
            eINVALID = 0
            #define X(ID, VALUE) , ID = VALUE
                ButtonTypes
            #undef X
        };

        //Enum to string
        static const char *ButtonTypeToString(ButtonType aButtonType)
        {
            switch (aButtonType)
            {
        #define X(ID, VALUE) case ButtonType::ID: return #ID;
            ButtonTypes
        #undef X
            };
            return nullptr;
        }

        //Enum to int
        static const int ButtonTypeToInt(ButtonType aButtonType)
        {
        #define X(ID, VALUE) if(strcmp(#ID, ButtonTypeToString(aButtonType)) == 0) return ID;
            ButtonTypes
        #undef X
            return -1;
        }

        //String to enum
        static ButtonType StringToButtonType(const char *aButtonTypeName)
        {
        #define X(ID, VALUE) if(strcmp(aButtonTypeName, #ID) == 0) return ButtonType::ID;
            ButtonTypes
        #undef X
            return ButtonType::eINVALID;
        }
    }
}
