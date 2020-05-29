/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief       : Transform Component
 *******************************************************************************/

#pragma once

#include "basecmp.h"
#include <glm/gtc/quaternion.hpp>


namespace Framework
{
    class Object3DCmp;


    class TransformCmp : public BaseCmp
    {
    public:
                 TransformCmp();
                 ~TransformCmp();
                         
        void     SetPosition(const glm::vec3& aPosition);
        void     SetOrientation(const glm::quat& aOrientation);
        void     SetScale(const glm::vec3& aScale);
          
        glm::vec3     GetPosition();
        glm::quat     GetOrientation();
        glm::vec3     GetScale();

        void     Update() override;
        uint32_t GetUpdatePriority() const override;
        void     Serialize(Json::Value& aSerializer) const override;
        void     Deserialize(const Json::Value& aSerializer) override;

    protected:
       std::weak_ptr<Object3DCmp> mObject3DCmp;       ///The object applied the tranformation
    
       glm::vec3                  mPosition;          /// translation
       glm::quat                  mOrientation;       /// rotation
       glm::vec3                  mScaling;           /// scaling
       
       bool                       mTransformChanged;  /// if true, mObject3D's trs will be updated
    };
}

