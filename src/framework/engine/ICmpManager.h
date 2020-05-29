/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2019 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#pragma once

namespace Framework
{
    class ICmpManager
    {
    public:
        virtual ~ICmpManager() { }
        virtual const char* GetName() const = 0; //Return with the name of the cmpmanager like renderablecmp manager, tranformcmp manager...etc
        virtual int GetCmpID() = 0;
        virtual size_t Size() const = 0;  //Return with the size of the cmps
        virtual shared_ptr<BaseCmp> AddCmp() = 0;
        virtual void RemoveCmp(BaseCmp* aCmp) = 0;
        virtual void Update() { }
        virtual void Serialize(Json::Value& aSerializer) const {}
        virtual void Deserialize(Json::Value& aSerializer) const {}
    };
}
