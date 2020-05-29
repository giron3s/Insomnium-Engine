/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/
#pragma once

#include "graphic/object.h"


namespace Framework
{
    class Object3D;

    class Object2D : public Object
    {
      public:

        Object3D* mBuddy = nullptr;

        /**
         * Constructor
         */
        Object2D(size_t aEntityID)
            : Object(aEntityID)
        {}
    };
}

