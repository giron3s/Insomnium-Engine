/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief       : Transform Component
 *******************************************************************************/

#include "precompiled.h"
#include "engine.h"
#include "engine/components/cameracmp.h"
#include "engine/components/entity.h"
#include "engine/components/renderablecmp.h"
#include "engine/components/transformcmp.h"


namespace Framework
{
   TransformCmp::TransformCmp() : mPosition(0.f), mScaling(1.f), mTransformChanged(true)
    {
    }

    TransformCmp::~TransformCmp()
    {
    }

    void TransformCmp::Update()
    {
       if (mTransformChanged)
       {
          Object3D* lObj3D = mObject3DCmp.lock()->GetObject3D();
          glm::mat4 lOrientation = glm::toMat4(mOrientation);
          lObj3D->SetPosition(mPosition);
          lObj3D->SetOrientation(lOrientation);
          lObj3D->SetScaleFactor(mScaling);
          mTransformChanged = false;
       }
    }

    void TransformCmp::SetPosition(const glm::vec3& aPosition)
    {
       mPosition = aPosition;
       mTransformChanged = true;
    }

    void TransformCmp::SetOrientation(const glm::quat& aOrientation)
    {
       mOrientation = aOrientation;
       mTransformChanged = true;
    }

    void TransformCmp::SetScale(const glm::vec3& aScale)
    {
       mScaling = aScale;
       mTransformChanged = true;
    }

    glm::vec3 TransformCmp::GetPosition()
    {
        return mPosition;
    }

    glm::quat TransformCmp::GetOrientation()
    {
        return mOrientation;
    }

    glm::vec3 TransformCmp::GetScale()
    {
        return mScaling;
    }

    void TransformCmp::Serialize(Json::Value& aSerializer) const
    {
        if ( mPosition.x || mPosition.y || mPosition.z )
        {
            SerializeVec<glm::vec3>(aSerializer["position"], mPosition);
        }

        if ( mScaling.x != 1.0f || mScaling.y != 1.0f || mScaling.z != 1.0f )
        {
            SerializeVec<glm::vec3>(aSerializer["scale"], mScaling);
        }

        // "rotation" assumed to be defined in form (direction,angle)
        // where direction is a vector about which rotation shall be performed
        glm::quat q(mOrientation);
        if ( q.x || q.y || q.z )
        {
            glm::vec4 v(q.x, q.y, q.z, q.w);
            float f = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
            v.x /= f; v.y /= f; v.z /= f; // v.xyz = rotation axis (normalized)
            v.w = acosf(v.w) * 360.0f / PI; // v.w = angle in degrees
            SerializeVec<glm::vec4>(aSerializer["rotation"], v);
        }
    }

    void TransformCmp::Deserialize(const Json::Value& aSerializer)
    {
        if (aSerializer.isMember("position"))
            DeserializeVec<glm::vec3>(aSerializer["position"], mPosition);
        if (aSerializer.isMember("scale"))
            DeserializeVec<glm::vec3>(aSerializer["scale"], mScaling);

        glm::mat4 lOrientationMatrix(1.0f);

        if (aSerializer.isMember("direction"))
        { // good only for lights since Up vector is chosen randomly
            glm::vec3 lDirection;
            DeserializeVec<glm::vec3>(aSerializer["direction"], lDirection);
            lDirection = glm::normalize(lDirection);
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
            if ( glm::length(glm::cross(lDirection, up)) < 1.0e-6f )
                up = glm::vec3(-1.0f, 0.0f, 0.0f); // is that correct?
            
            lOrientationMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), lDirection, up);
            
            mOrientation = glm::quat(lOrientationMatrix);
        }
        else if (aSerializer.isMember("rotation"))
        {
            /* "rotation" assumed to be defined in form (direction,angle)
            where direction is a vector about which rotation shall be performed */
            glm::vec4 lRotation;
            DeserializeVec<glm::vec4>(aSerializer["rotation"], lRotation);
            float f = lRotation.x * lRotation.x +
                      lRotation.y * lRotation.y +
                      lRotation.z * lRotation.z;
            if ( f < 1.0e-6 )
              CRASH("Invalid rotation axis definition");
            f = sqrtf(f);
            lRotation.x /= f; lRotation.y /= f; lRotation.z /= f;
            // lRotation.xyz = rotation axis (normalized)
            // lRotation.w = angle in degrees
            lRotation.w *= (PI / 360.0f); // lRotation.w = half-angle in radians

            if ( lRotation.w < 1.0e-6 )
                CRASH("Ignore the rotation, because it is under the threshold")

            f = sinf(lRotation.w);
            mOrientation.x = lRotation.x * f;
            mOrientation.y = lRotation.y * f;
            mOrientation.z = lRotation.z * f;
            mOrientation.w = cosf(lRotation.w);
            
            lOrientationMatrix = glm::toMat4(mOrientation);
        }

        // for this component to work, the entity has to contain a component with an Object3D (a TrsBaseCmp), so let's search for it
        Entity* lEntity = GetOwner();
        if ( !lEntity->HasComponent<Object3DCmp>() )
            CRASH("Entity containing a TransformCmp must also contain an Object3D!");

        mObject3DCmp = lEntity->GetComponent<Object3DCmp>();
        Object3D* lObj3D = mObject3DCmp.lock()->GetObject3D();

        lObj3D->SetPosition(mPosition);
        lObj3D->SetOrientation(lOrientationMatrix);
        lObj3D->SetScaleFactor(mScaling);

        if ( lObj3D->mBuddy )
        {
            Object2D* lBuddy = lObj3D->mBuddy;
            lBuddy->SetPosition(mPosition);
            lBuddy->SetOrientation(lOrientationMatrix);
            lBuddy->SetScaleFactor(mScaling);
        }
    }

    uint32_t TransformCmp::GetUpdatePriority() const
    {
       // TransformCmp should have lower update priority than any component that modifies it (like MotionCmp)
       return 1;
    }
}
