/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#pragma once

#include "core/utils.h"
#include "engine/basemanager.h"
#include "graphic/scene.h"
#include "engine/components/transformcmp.h"
#include "engine/components/catalogcmp.h"

namespace Game
{
    class ObjectEditorManager : public Framework::BaseManager
    {
    public:
                                  ObjectEditorManager();
                                  ~ObjectEditorManager() override;

        void                      Initialize() override;
        void                      DeInitialize() override;

        bool                      LoadObjectProperties(size_t aEntityId);
        void                      UnloadObjectProperties();

        bool                      HaveValuesChanged();
        void                      UndoValuesChanged();

        const std::string         GetObjectName();
        const std::string         GetObjectType();
        const int                 GetObjectImage();

        float                     GetObjectPositionX();
        float                     GetObjectPositionY();
        float                     GetObjectPositionZ();

        float                     GetObjectOrientationX();
        float                     GetObjectOrientationY();
        float                     GetObjectOrientationZ();

        float                     GetObjectScaleX();
        float                     GetObjectScaleY();
        float                     GetObjectScaleZ();

        void                      SetObjectName(std::string aName);
        void                      SetObjectType(std::string aType);
        void                      SetObjectImage(std::string aImage);

        void                      SetObjectPositionX(float aValue);
        void                      SetObjectPositionY(float aValue);
        void                      SetObjectPositionZ(float aValue);

        void                      SetObjectOrientationX(float aValue);
        void                      SetObjectOrientationY(float aValue);
        void                      SetObjectOrientationZ(float aValue);

        void                      SetObjectScaleX(float aValue);
        void                      SetObjectScaleY(float aValue);
        void                      SetObjectScaleZ(float aValue);

    private:
        std::shared_ptr<Framework::TransformCmp> GetObjectTransform();

    protected:
        std::shared_ptr<Framework::Entity> mSelectedEntity;

        bool                               mHaveValuesChanged;
                                           
        std::string                        mOriginalName;
        std::string                        mOriginalType;
        std::string                        mOriginalImage;
                                           
        float                              mOriginalPositionX;
        float                              mOriginalPositionY;
        float                              mOriginalPositionZ;
                                           
        float                              mOriginalOrientationX;
        float                              mOriginalOrientationY;
        float                              mOriginalOrientationZ;
                                           
        float                              mOriginalScaleX;
        float                              mOriginalScaleY;
        float                              mOriginalScaleZ;
    };
}
