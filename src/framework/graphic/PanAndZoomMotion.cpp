/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *   Brief      : PanAndZoomMotion object
 *******************************************************************************/

#include "precompiled.h"
#include "graphic/PanAndZoomMotion.h"
#include "engine/components/transformcmp.h"

namespace Framework
{

    void PanAndZoomMotion::ApplyTo(Object3D &aObj)
    {
        aObj.SetPosition(mPosition);
    }

    void PanAndZoomMotion::ApplyTo(std::shared_ptr<TransformCmp> aTransformCmp)
    {
       aTransformCmp->SetPosition(mPosition);
    }

    void PanAndZoomMotion::Reset()
    {
    }

    void PanAndZoomMotion::Forward(float aAmount)
    {
        bool lClipped = false;
        mPosition.z -= aAmount;
        if(mPosition.z < mZRangeMin)
        {
            mPosition.z = mZRangeMin;
            lClipped = true;
        }
        else if(mPosition.z > mZRangeMax)
        {
            mPosition.z = mZRangeMax;
            lClipped = true;
        }

        if(lClipped)
            INFO(LogLevel::eLEVEL2, "Object position has been clipped by PanAndZoomMotion constraints.");
    }
    
    void PanAndZoomMotion::Right(float aAmount)
    { mPosition.x += aAmount; }
    
    void PanAndZoomMotion::Up(float aAmount)
    { mPosition.y += aAmount; }
}
