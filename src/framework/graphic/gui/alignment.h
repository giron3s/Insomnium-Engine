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

        enum class Alignment
        {
            /// The different kinds of alignments a layout can perform.
            #define Alignments    \
                X( eMinimum,  0 ) /*Take only as much space as is required*/ \
                X( eMiddle,   1 ) /*Center align*/                           \
                X( eMaximum,  2 ) /*Take as much space as is allowed.*/      \
                X( eFill,     3 ) /*Fill according to preferred sizes.*/     \
                X( eLeft,     4 ) \
                X( eCenter,   5 ) \
                X( eRight,    6 ) \


            #define X(ID, VALUE) ID = VALUE,
                Alignments
                eINVALID
            #undef X
        };

        //Enum to string
        static const char *AlignmentToString(Alignment aAlignment)
        {
            switch (aAlignment)
            {
        #define X(ID, VALUE) case Alignment::ID: return #ID;
            Alignments
        #undef X
            };

            return nullptr;
        }

        //String to enum
        static Alignment StringToAlignment(const char *aAlignmentName)
        {
        #define X(ID, VALUE) if(strcmp(aAlignmentName, #ID) == 0) return Alignment::ID;
            Alignments
        #undef X

            return Alignment::eINVALID;
        }
    }
}
