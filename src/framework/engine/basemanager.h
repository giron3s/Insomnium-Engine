/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief       : Base manager class
 *******************************************************************************/

#pragma once
#include <map>
#include "core/utils.h"
#include "core/serialization/jsoncpputils.h"

namespace Framework
{
    class BaseManager
    {
        NON_COPYABLE_CLASS(BaseManager);
    public:

        BaseManager(const std::string& aName) : mName(aName) { }

        virtual ~BaseManager() = default;

        virtual void Initialize() = 0;
        virtual void DeInitialize() = 0;
        //virtual void Update() { }

        const std::string& GetName() const { return mName; }

    protected:
        std::string mName;
    };
};

