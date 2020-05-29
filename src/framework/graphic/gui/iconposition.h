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

        enum class IconPosition
        {
            // The available icon positions
            // Left          < Button icon on the far left.
            // LeftCentered  < Button icon on the left, centered (depends on caption text length).
            // RightCentered < Button icon on the right, centered (depends on caption text length).
            // Right         < Button icon on the far right.

            #define IconPositions \
                X( eLeft,          0 ) \
                X( eLeftCentered,  1 ) \
                X( eRightCentered, 2 ) \
                X( eRight,         3 ) \

            #define X(ID, VALUE) ID = VALUE,
                IconPositions
                eINVALID
            #undef X
        };

        //Enum to string
        static const char *IconPositionToString(IconPosition aIconPosition)
        {
            switch (aIconPosition)
            {
        #define X(ID, VALUE) case IconPosition::ID: return #ID;
            IconPositions
        #undef X
            };

            return nullptr;
        }

        //String to enum
        static IconPosition StringToIconPosition(const char *aIconPositionName)
        {
        #define X(ID, VALUE) if(strcmp(aIconPositionName, #ID) == 0) return IconPosition::ID;
            IconPositions
        #undef X

            return IconPosition::eINVALID;
        }
    }
}
