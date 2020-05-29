/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief	    : Motion for pan and zoom (pseudo 2D) movement.
 *
*******************************************************************************/
#pragma once

#include <glm/glm.hpp>
#include "graphic/motion.h"


namespace Framework
{
    class TransformCmp;

    class PanAndZoomMotion : public Motion
    {
    public:
        PanAndZoomMotion()
        {
            mPosition.z = (mZRangeMax - mZRangeMin)/2.0f;
        }

        bool SetZRange(float aZRangeMin, float aZRangeMax)
        {
            if ( 0.0f < aZRangeMin && aZRangeMin < aZRangeMax )
            {
                mZRangeMin = aZRangeMin;
                mZRangeMax = aZRangeMax;
                return true;
            }
            CRASH("PanAndZoomMotion::SetRange - Invalid parameter(s)");
            return false;
        }

        inline float GetZRangeMin() const { return mZRangeMin; }
        inline float GetZRangeMax() const { return mZRangeMax; }
            
        void ApplyTo(Object3D &obj) override;
        void ApplyTo(std::shared_ptr<TransformCmp> aTransformCmp) override;
        void Reset() override;

        void Forward(float amount) override;
        void Right(float amount) override;
        void Up(float amount) override;

    private:
         float mZRangeMin = 1.0e-6f, mZRangeMax = 1000.0f;           
    };
}
