/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*  Brief       : Light object
*******************************************************************************/
#pragma once

#include "engine/components/entity.h"
#include "graphic/object3d.h"
#include "graphic/shadowmaprendertarget.h"


namespace Framework
{
    class Light : public Object3D
    {
    public:
        /* TODO: add comments */
        Light(const glm::vec3& aAmbient = glm::vec3(0.0f, 0.0f, 0.0f),
            const glm::vec3& aDiffuse = glm::vec3(0.0f, 0.0f, 0.0f),
            const glm::vec3& aSpecular = glm::vec3(0.0f, 0.0f, 0.0f),
            const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f),
            size_t aEntityID = Entity::GetDefaultID())
            : Object3D(aEntityID)
            , mAmbient(aAmbient)
            , mDiffuse(aDiffuse)
            , mSpecular(aSpecular)
        {
            SetPosition(aPosition);
        }

        void SetAmbient(const glm::vec3& aAmbient) { mAmbient = aAmbient; }
        void SetDiffuse(const glm::vec3& aDiffuse) { mDiffuse = aDiffuse; }
        void SetSpecular(const glm::vec3& aSpecular) { mSpecular = aSpecular; }

        inline const glm::vec3& GetAmbient() const { return mAmbient; }
        inline const glm::vec3& GetDiffuse() const { return mDiffuse; }
        inline const glm::vec3& GetSpecular() const { return mSpecular; }
        inline RenderTarget*    GetShadowMap() const { return mShadowMap.get(); }
        
        mutable bool DoUpdateViewProjectionMatrix = true; /* NOTE: it
        must be set EXPLICITLY to true if any light parameter affecting
        shadow volume changes (position, direction, ...). */

        const glm::mat4& GetViewProjectionMatrix() const
        {
            if ( DoUpdateViewProjectionMatrix && mShadowMap )
            { // tip: if there is no shadow map - no point to deal with ViewProjectionMatrix
                UpdateViewProjectionMatrix();
                DoUpdateViewProjectionMatrix = false;
            }
            return mViewProjectionMatrix;
        }

        bool SetShadowMap(uint32_t aSizeX, uint32_t aSizeY);
        void ClearShadowMap();

        /**
         * Debug information
         */
        void SetRenderMarker(bool aFlag) { mRenderMarker = aFlag; }
        bool GetRenderMarker(void) const { return mRenderMarker; }

        void Serialize(Json::Value& aSerializer) const;
        void Deserialize(const Json::Value& aSerializer);

    protected:
        glm::vec3              mAmbient;
        glm::vec3              mDiffuse;
        glm::vec3              mSpecular;
        std::unique_ptr<ShadowMapRenderTarget> mShadowMap;
        mutable glm::mat4 mViewProjectionMatrix = glm::mat4(0.0f);

        bool                   mRenderMarker = false;

        virtual void UpdateViewProjectionMatrix() const = 0;
    };
}
