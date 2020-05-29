/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2019 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#pragma once

#include "core/utils.h"
#include "core/serialization/jsoncpputils.h"

namespace Framework
{
    class Container
    {
        NON_COPYABLE_CLASS(Container);

    public:

        Container(const std::string& aName) : mName(aName) { }

        virtual ~Container() = default;

        virtual void Initialize() = 0;
        virtual void DeInitialize() = 0;

        const std::string& GetName() const { return mName; }

    protected:

        std::string mName;
    };
}

