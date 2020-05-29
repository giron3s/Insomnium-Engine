/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#pragma once

#include "engine/components/object3dcmp.h"

namespace Framework
{
    class Model3D;
    class Model2D;


    class RenderableCmp : public Object3DCmp
    {

    public:
                  RenderableCmp();
                  ~RenderableCmp();

        void      Serialize(Json::Value& aSerializer) const override;
        void      Deserialize(const Json::Value& aSerializer) override;

        Object3D* GetObject3D() const override;
        Model3D*  GetModel3D() const { return mModel3D; }
        Model2D*  GetModel2D() const { return mModel2D; }

    protected:
        Model3D*  mModel3D;
        Model2D*  mModel2D;
    };
}

