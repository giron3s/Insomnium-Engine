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

    enum class Orientation
    {
        /// The different kinds of alignments a layout can perform.
        #define Orientations     \
            X( eHorizontal,  0 ) /* Layout expands on horizontal axis.*/ \
            X( eVertical,    1 ) /* Layout expands on vertical axis.*/ \

        #define X(ID, VALUE) ID = VALUE,
            Orientations
            eINVALID
        #undef X
        };

        //Enum to string
        static const char *OrientationToString(Orientation aOrientation)
        {
            switch (aOrientation)
            {
        #define X(ID, VALUE) case Orientation::ID: return #ID;
            Orientations
        #undef X
            };

            return nullptr;
        }

        //String to enum
        static Orientation StringToOrientation(const char *aOrientationName)
        {
        #define X(ID, VALUE) if(strcmp(aOrientationName, #ID) == 0) return Orientation::ID;
            Orientations
        #undef X

            return Orientation::eINVALID;
        }
    }
}
