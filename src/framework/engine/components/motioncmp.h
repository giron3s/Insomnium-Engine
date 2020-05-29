/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief       : Motion Component
 *******************************************************************************/

#pragma once

#include "engine/components/basecmp.h"

namespace Framework
{
    class TransformCmp;
    class Motion;
}

namespace Framework
{
    class MotionCmp : public BaseCmp
    {

    public:
                    MotionCmp();
                    ~MotionCmp();

        void        Serialize(Json::Value& aSerializer) const override;
        void        Deserialize(const Json::Value& aSerializer) override;
        void        Update() override;
        uint32_t    GetUpdatePriority() const override;

        inline void SetUpdatable(bool aUpdatable)
        { mUpdatable = aUpdatable; }

        inline void SetUpdateCallback(const std::function<void(const glm::vec3& aNewPosition)> &callback)
        { mUpdateCallback = callback; }

        inline Motion* GetCameraMotion() const
        { return mCameraMotion.get(); }

    private:
        void        OnMouseMotionEvent(const InputEventType aEventType, const MouseMotionEventData& aMouseMotionEvent);
        void        OnMouseScrollEvent(const InputEventType aEventType, const ScrollEventData& aMouseScrollEvent);
        void        OnMouseButtonEvent(const InputEventType aEventType, const MouseButtonEventData& aMouseButtonEvent);
        void        OnKeyboardEvent(const InputEventType aEventType, const KeyboardEventData& aKeyboardEventData);

    protected:
        std::weak_ptr<TransformCmp> mTransformCmp;
        std::unique_ptr<Motion>     mCameraMotion;

        EventHandler                mMouseButtonEventHandler;
        EventHandler                mMouseMotionEventHandler;
        EventHandler                mMouseWheelHandler;
        EventHandler                mKeyboardEventHandler;

        bool                        mShiftKeyHeld;
        bool                        mLeftMouseBtnHeld;

        std::string                 mMotionType;

        float                       mKeyboardSensibility;
        float                       mMouseSensibility;
        float                       mMouseZoomSensibility;

        bool                        mUpdatable;
        std::function<void(const glm::vec3& aNewPosition)> mUpdateCallback = nullptr;
    };
}

