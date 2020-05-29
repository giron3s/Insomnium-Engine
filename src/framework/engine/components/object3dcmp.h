/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*  Brief       : Object3D Component
*******************************************************************************/

#pragma once

#include "engine/components/basecmp.h"

namespace Framework
{
    class Object3D;

    class Object3DCmp : public BaseCmp
    {
    public:
                          Object3DCmp() { }
        virtual           ~Object3DCmp() { }

        virtual Object3D* GetObject3D() const = 0;
    };
}
