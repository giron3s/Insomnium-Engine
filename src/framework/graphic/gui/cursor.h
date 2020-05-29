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

        enum class Cursor
        {
            /// Cursor shapes available to use in GLFW.
            #define Cursors    \
                X( eArrow,     0 ) \
                X( eIBeam,     1 ) \
                X( eCrosshair, 2 ) \
                X( eHand,      3 ) \
                X( eHResize,   4 ) \
                X( eVResize,   5 ) \


            #define X(ID, VALUE) ID = VALUE,
                Cursors
                eCursorCount
            #undef X
        };

        //Enum to string
        static const char *CursorToString(Cursor aCursor)
        {
            switch (aCursor)
            {
        #define X(ID, VALUE) case Cursor::ID: return #ID;
            Cursors
        #undef X
            };

            return nullptr;
        }

        //String to enum
        static Cursor StringToCursor(const char *aCursorName)
        {
        #define X(ID, VALUE) if(strcmp(aCursorName, #ID) == 0) return Cursor::ID;
            Cursors
        #undef X

            return Cursor::eCursorCount;
        }
    }
}
