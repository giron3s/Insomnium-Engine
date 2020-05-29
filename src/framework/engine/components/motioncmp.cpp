/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief       : Motion Component
 *******************************************************************************/

#include "precompiled.h"
#include "engine.h"
#include "engine/components/entity.h"
#include "engine/components/cameracmp.h"
#include "engine/components/motioncmp.h"
#include "engine/components/transformcmp.h"
#include "graphic/camera.h"
#include "graphic/flymotion.h"
#include "graphic/PanAndZoomMotion.h"
#include <functional>


namespace Framework
{
    MotionCmp::MotionCmp() 
        : mShiftKeyHeld(false)
        , mLeftMouseBtnHeld(false)
        , mMotionType(MOTION_WALKING)
        , mKeyboardSensibility(Engine::Instance()->Config().GetKeyboardSensibility()) 
        , mMouseSensibility(Engine::Instance()->Config().GetMouseSensibility())
        , mMouseZoomSensibility(Engine::Instance()->Config().GetMouseZoomSensibility())
        , mUpdatable(false)
        , mCameraMotion(nullptr)
    {
       function<void(const InputEventType aEventType, const MouseButtonEventData& aMouseButtonEvent)> lMouseButtonEventHandler;
       function<void(const InputEventType aEventType, const MouseMotionEventData& aMouseMotionEvent)> lMouseMotionEventHandler;
       function<void(const InputEventType aEventType, const ScrollEventData& aMouseScrollEvent)> lMouseWheelHandler;
       function<void(const InputEventType aEventType, const KeyboardEventData& aKeyboardEventData)> lKeyboardEventHandler;

       lMouseButtonEventHandler = [this](const InputEventType aEventType, const MouseButtonEventData& aMouseButtonEvent) { OnMouseButtonEvent(aEventType, aMouseButtonEvent); };
       lMouseMotionEventHandler = [this](const InputEventType aEventType, const MouseMotionEventData& aMouseMotionEvent) { OnMouseMotionEvent(aEventType, aMouseMotionEvent); };
       lMouseWheelHandler       = [this](const InputEventType aEventType, const ScrollEventData& aMouseScrollEvent) { OnMouseScrollEvent(aEventType, aMouseScrollEvent); };
       lKeyboardEventHandler    = [this](const InputEventType aEventType, const KeyboardEventData& aKeyboardEventData) { OnKeyboardEvent(aEventType, aKeyboardEventData); };

       mMouseButtonEventHandler = Engine::Instance()->Input().MouseButtonEvent.Connect(lMouseButtonEventHandler);
       mMouseMotionEventHandler = Engine::Instance()->Input().MouseMotionEvent.Connect(lMouseMotionEventHandler);
       mMouseWheelHandler = Engine::Instance()->Input().ScrollEvent.Connect(lMouseWheelHandler);
       mKeyboardEventHandler = Engine::Instance()->Input().KeyboardEvent.Connect(lKeyboardEventHandler);
    }

    MotionCmp::~MotionCmp()
    {
        Engine::Instance()->Input().MouseButtonEvent.Disconnect(mMouseButtonEventHandler);
        Engine::Instance()->Input().MouseMotionEvent.Disconnect(mMouseMotionEventHandler);
        Engine::Instance()->Input().ScrollEvent.Disconnect(mMouseWheelHandler);
        Engine::Instance()->Input().KeyboardEvent.Disconnect(mKeyboardEventHandler);
    }

    void MotionCmp::Update()
    {
        if (mUpdatable)
        {
            if (mTransformCmp.expired())
            {
                WARNING("Can not apply the motion, because the %s entity hasn't got transform component", GetOwner()->GetName().c_str());
            }
            
            /* Apply the motion to the 3d object */
            mCameraMotion->ApplyTo(mTransformCmp.lock());
            /* Invoke update callback if present */
            if (mUpdateCallback)
                mUpdateCallback(mCameraMotion->GetPosition());
        }
    }

    void MotionCmp::OnMouseMotionEvent(const InputEventType aEventType, const MouseMotionEventData& aMouseMotionEvent)
    {
       if (!mUpdatable) 
       {
           return; 
       }

       if (mLeftMouseBtnHeld)
       {
          FlyMotion* lFlyMotion = dynamic_cast<FlyMotion*>(mCameraMotion.get());
          if (lFlyMotion)
          {
            // LEFT_MOUSE_CLICK + MOUSE - rotate the scene
            lFlyMotion->RotateYaw((float)(mMouseSensibility * PI * aMouseMotionEvent.dx / 400));
            lFlyMotion->RotatePitch((float)(mMouseSensibility * PI * aMouseMotionEvent.dy / 300));
          }
       }
       else if (mShiftKeyHeld)
       {
          // SHIFT + MOUSE - pan the camera
          mCameraMotion->Right((float)(mMouseSensibility * PI * aMouseMotionEvent.dx / 400));
          mCameraMotion->Forward(-(float)(mMouseSensibility * PI * aMouseMotionEvent.dy / 300));
       }
    }

    void MotionCmp::OnMouseScrollEvent(const InputEventType aEventType, const ScrollEventData& aMouseScrollEvent)
    {
        if (!mUpdatable)
        {
            return;
        }

       float lMouseWheelDelta = aMouseScrollEvent.yoffset;
       mCameraMotion->Forward(mMouseZoomSensibility * lMouseWheelDelta);
    }
    
    void MotionCmp::OnMouseButtonEvent(const InputEventType aEventType, const MouseButtonEventData& aMouseButtonEvent)
    {
        if (!mUpdatable)
        {
            return;
        }

       switch (aMouseButtonEvent.action)
       {
           case GLFW_RELEASE:
           {
              if (aMouseButtonEvent.button == GLFW_MOUSE_BUTTON_LEFT)
                 mLeftMouseBtnHeld = false;
              break;
           }

           case GLFW_PRESS:
           {
              if (aMouseButtonEvent.button == GLFW_MOUSE_BUTTON_LEFT)
                 mLeftMouseBtnHeld = true;
              break;
           }
        }
    }

    void MotionCmp::OnKeyboardEvent(const InputEventType aEventType, const KeyboardEventData& aKeyboardEventData)
    {
       if (!mUpdatable) { return; }

       double elapsedMs = 0.1;

        if (aKeyboardEventData.action == GLFW_PRESS &&
            (aKeyboardEventData.keyboardKey == GLFW_KEY_LEFT_SHIFT || aKeyboardEventData.keyboardKey == GLFW_KEY_RIGHT_SHIFT) )
        {
            mShiftKeyHeld = true;
        }
        else
        {
            mShiftKeyHeld = false;
        }

        if (aKeyboardEventData.keyboardKey == GLFW_KEY_W)
        {
            mCameraMotion->Forward(mKeyboardSensibility * (float)elapsedMs);
        }
        if (aKeyboardEventData.keyboardKey == GLFW_KEY_S)
        {
            mCameraMotion->Forward(-mKeyboardSensibility * (float)elapsedMs);
        }
        if (aKeyboardEventData.keyboardKey == GLFW_KEY_A ||
            aKeyboardEventData.keyboardKey == GLFW_KEY_LEFT)
        {
            mCameraMotion->Right(-mKeyboardSensibility * (float)elapsedMs);
        }
        if (aKeyboardEventData.keyboardKey == GLFW_KEY_D ||
            aKeyboardEventData.keyboardKey == GLFW_KEY_RIGHT)
        {
            mCameraMotion->Right(mKeyboardSensibility * (float)elapsedMs);
        }
        if (aKeyboardEventData.keyboardKey == GLFW_KEY_F ||
            aKeyboardEventData.keyboardKey == GLFW_KEY_DOWN)
        {
            mCameraMotion->Up(-mKeyboardSensibility * (float)elapsedMs);
        }
        if (aKeyboardEventData.keyboardKey == GLFW_KEY_R ||
            aKeyboardEventData.keyboardKey == GLFW_KEY_UP)
        {
            mCameraMotion->Up(mKeyboardSensibility * (float)elapsedMs);
        }
    }

    void MotionCmp::Serialize(Json::Value& aSerializer) const
    {
        aSerializer["motion_type"] = mMotionType;
        if (mMotionType == MOTION_PAN_AND_ZOOM)
        {
            PanAndZoomMotion* lPanAndZoomMotion = dynamic_cast<PanAndZoomMotion*>(mCameraMotion.get());
            assert(lPanAndZoomMotion);
            glm::vec2 lRange(lPanAndZoomMotion->GetZRangeMin(), lPanAndZoomMotion->GetZRangeMax());
            SerializeVec<glm::vec2>(aSerializer["z_range"], lRange);
        }
        else
        {
            FlyMotion* lFlyMotion = dynamic_cast<FlyMotion*>(mCameraMotion.get());
            assert(lFlyMotion);
            aSerializer["pitch"] = lFlyMotion->GetPitch();
            aSerializer["yaw"] = lFlyMotion->GetYaw();
        }
        aSerializer["keyboard_sensibility"] = mKeyboardSensibility;
        aSerializer["mouse_sensibility"] = mMouseSensibility;
        aSerializer["mouse_zoom_sensibility"] = mMouseZoomSensibility;
    }

    void MotionCmp::Deserialize(const Json::Value& aSerializer)
    {
        mMotionType = aSerializer.get("motion_type", mMotionType).asString();

        if (mMotionType != MOTION_WALKING
            && mMotionType != MOTION_FREEFLY
            && mMotionType != MOTION_FLY
            && mMotionType != MOTION_PAN_AND_ZOOM)
        {
           CRASH("motion_type must be one of: %s, %s, %s, %s!", MOTION_WALKING.c_str(), MOTION_FREEFLY.c_str(), MOTION_FLY.c_str(), MOTION_PAN_AND_ZOOM.c_str());
        }

        mKeyboardSensibility  = aSerializer.get("keyboard_sensibility",   mKeyboardSensibility).asFloat();
        mMouseSensibility     = aSerializer.get("mouse_sensibility",       mMouseSensibility).asFloat();
        mMouseZoomSensibility = aSerializer.get("mouse_zoom_sensibility", mMouseZoomSensibility).asFloat();
        
        // for this component to work, the entity has to contain a TransformCmp and Object3DCmp
        Entity* lEntity = GetOwner();
        if (!lEntity->HasComponent<TransformCmp>() || !lEntity->HasComponent<Object3DCmp>())
        {
           CRASH("The %s's MotionCmp must contain a TransformCmp and Object3DCmp!", lEntity->GetName().c_str());
        }
        mTransformCmp = lEntity->GetComponent<TransformCmp>();
        Object3D* lObj3D = lEntity->GetComponent<Object3DCmp>().lock()->GetObject3D();
        if (mMotionType == MOTION_PAN_AND_ZOOM)
        {
            mCameraMotion = std::make_unique<PanAndZoomMotion>();
            PanAndZoomMotion* lPanAndZoomMotion = dynamic_cast<PanAndZoomMotion*>(mCameraMotion.get());
            assert(lPanAndZoomMotion);
            if (aSerializer.isMember("z_range"))
            {
                glm::vec2 lRange(lPanAndZoomMotion->GetZRangeMin(), lPanAndZoomMotion->GetZRangeMax());
                DeserializeVec<glm::vec2>(aSerializer["z_range"], lRange);
                lPanAndZoomMotion->SetZRange(lRange.x, lRange.y);
            }
        }
        else
        {
            mCameraMotion = std::make_unique<FlyMotion>();
            FlyMotion* lFlyMotion = static_cast<FlyMotion*>(mCameraMotion.get());
            float lPitch = aSerializer.get("pitch", -45.0f).asFloat(),
                  lYaw   = aSerializer.get("yaw", 0.0f).asFloat();
            lFlyMotion->RotatePitch(lPitch);
            lFlyMotion->RotateYaw(lYaw);
        }
        mCameraMotion->SetPosition(lObj3D->GetPosition());

        /* Apply the motion to the 3d object */
        mCameraMotion->ApplyTo(*lObj3D);
        mCameraMotion->ApplyTo(mTransformCmp.lock());
    }


    uint32_t MotionCmp::GetUpdatePriority() const
    {
       // MotionCmp modifies the TransformCmp of an entity, so it must run before it in the update loop
       return 2;
    }

}
