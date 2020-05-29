/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#include "precompiled.h"
#include "engine.h"
#include "engine/components/renderablecmp.h"
#include "graphic/model3d.h"
//#include "graphic/blinnphongshader.h"
#include "graphic/procedural/plane.h"
#include "graphic/procedural/grid.h"
#include "graphic/opengl/opengl_shader.h"

namespace Framework
{
    RenderableCmp::RenderableCmp()
        : mModel3D(NULL)
        , mModel2D(NULL)
    {
        
    }

    RenderableCmp::~RenderableCmp()
    {
        delete mModel3D;
        delete mModel2D;
    }

    void RenderableCmp::Serialize(Json::Value& aSerializer) const
    {
        //Serialize the 2D object
        if ( mModel2D )
        {
            // check for a special procedural model name w/o an asset
            if (mModel2D->GetName().compare("#grid") == 0)
            {
                aSerializer["model2d"] = "#grid";
                Procedural::Grid* lGrid = dynamic_cast<Procedural::Grid*>(mModel2D);
                ASSERT(lGrid);
                aSerializer["minorStep2D"] = lGrid->GetMinorStep2D();
                aSerializer["majorStep2D"] = lGrid->GetMajorStep2D();
                aSerializer["minorStep3D"] = lGrid->GetMinorStep3D();
                aSerializer["majorStep3D"] = lGrid->GetMajorStep3D();
                aSerializer["rescaleDistance"] = lGrid->GetRescaleDistance();
                aSerializer["minorLineWidth"] = lGrid->GetMinorLineWidth();
                aSerializer["majorLineWidth"] = lGrid->GetMajorLineWidth();
                SerializeVec<glm::vec3>(aSerializer["color"], lGrid->GetColor());
            }
        }

        //Serialize the 3D object
        if ( mModel3D )
        {
            aSerializer["model3d"] = mModel3D->GetAsset3D()->GetName();
            if( mModel3D->IsShadowCaster() )
                aSerializer["shadowcaster"] = true;
        }

        if ( mModel2D && !mModel2D->mText.empty() )
            aSerializer["text"] = mModel2D->mText;
    }

    void RenderableCmp::Deserialize(const Json::Value& aSerializer)
    {
        ASSERT(mModel3D == NULL && mModel2D == NULL);
        Entity* lEntity = GetOwner();
        size_t lEntityID = lEntity->GetEntityID();

        string lModelName;
        bool lAssignShaderForModel3D = false;
        bool lAssignShaderForModel2D = false;

        // Load values for model (3D)
        if (aSerializer.isMember("model3d"))
        {
            lModelName = aSerializer.get("model3d", "").asString();

            // check for procedural model names
            if (lModelName.compare("#plane") == 0)
            {
                auto lPlaneAsset = std::make_shared<Procedural::Plane>();
                auto lRenderer = Engine::Instance()->Display().GetRenderer();
                if (!lRenderer->PrepareForRendering(*lPlaneAsset))
                    CRASH("Failed to prepare model %s\n", lPlaneAsset->GetName().c_str());
                mModel3D = new Model3D(std::move(lPlaneAsset));
            }
            // else create model from asset3d
            else
            {
                mModel3D = Engine::Instance()->ResourceManager().CreateModel3D(lModelName);
                if (!mModel3D)
                    return;
                // create a buddy model2d for non-procedural model3d
                auto lModel3DSize = mModel3D->GetOOBB().GetMax() - mModel3D->GetOOBB().GetMin();
                mModel2D = Engine::Instance()->ResourceManager().CreateModel2D(lModelName, lModel3DSize.x, lModel3DSize.z);
                ASSERT(mModel2D);
                if ( mModel2D )
                {
                    mModel2D->SetEntityID(lEntityID);
                    mModel2D->SetName(mModel3D->GetName());
                    mModel2D->SetOwnerEntity(lEntity);
                    lAssignShaderForModel2D = true;
                }
            }
            mModel3D->SetEntityID(lEntityID);
            mModel3D->SetName(std::to_string(lEntityID) + "_" + mOwner->GetName());
            mModel3D->SetOwnerEntity(lEntity);
            mModel3D->mFlags = aSerializer.get("flags", 0).asUInt();
            mModel3D->SetShadowCaster(aSerializer.get("shadowcaster", false).asBool());
            lAssignShaderForModel3D = true;
        }

        // Load values for model (2D)
        if ( aSerializer.isMember("model2d") )
        {
            if ( mModel2D )
                CRASH("'model2d' has been created already as a buddy for 'model3d'");
            
            lModelName = aSerializer.get("model2d", "").asString();

            // check for procedural model names
            if ( lModelName.compare("#grid") == 0 )
            {
                float lMinorStep2D      = aSerializer.get("minorStep2D", 1.0f).asFloat();
                float lMajorStep2D      = aSerializer.get("majorStep2D", 1.0f).asFloat();
                float lMinorStep3D      = aSerializer.get("minorStep3D", 1.0f).asFloat();
                float lMajorStep3D      = aSerializer.get("majorStep3D", 1.0f).asFloat();
                float lSize3D           = aSerializer.get("size3D", 1.0f).asFloat();
                float lRescaleDistance  = aSerializer.get("rescaleDistance", 1.0f).asFloat();
                float lMinorLineWidth   = aSerializer.get("minorLineWidth", 1.0f).asFloat();
                float lMajorLineWidth   = aSerializer.get("majorLineWidth", 1.0f).asFloat();

                glm::vec3 lColor(1.0f, 1.0f, 1.0f);
                if ( aSerializer.isMember("color") )
                    DeserializeVec<glm::vec3>(aSerializer["color"], lColor);
                mModel2D = new Procedural::Grid( lMinorStep2D, lMajorStep2D, lMinorStep3D, lMajorStep3D, lSize3D, lRescaleDistance, lMinorLineWidth, lMajorLineWidth, lColor);
            }
            // else create model from asset2d
            else
            {
                ASSERT(!mModel3D);
                mModel2D = Engine::Instance()->ResourceManager().CreateModel2D(lModelName/*use default size?*/);
                if (!mModel2D)
                    return;
                mModel2D->SetName(std::to_string(lEntityID) + "_" + mOwner->GetName());
                lAssignShaderForModel2D = true;
            }
            mModel2D->SetEntityID(lEntityID);
            mModel2D->SetOwnerEntity(lEntity);
            mModel2D->mFlags = aSerializer.get("flags", 0).asUInt();
        }

        if ( lModelName.empty() )
            CRASH("Neither 'model2d' nor 'model3d' attribute is defined in RenderableCmp!");

        if ( mModel3D && mModel2D )
        {
            mModel3D->mBuddy = mModel2D;
            mModel2D->mBuddy = mModel3D;
        }

        // assign shaders
        /*if ( lAssignShaderForModel3D )
        {
            const Shader* lShader = Engine::Instance()->ResourceManager().FindShader("Blinn-Phong lighting");
            if (!lShader)
                CRASH("ERROR 'Blinn-Phong lighting' shader not found\n");
            mModel3D->SetShader(lShader);
        }*/
        if ( lAssignShaderForModel2D )
        {
            const Shader* lShader = Engine::Instance()->ResourceManager().FindShader("No lighting");
            if (!lShader)
                CRASH("ERROR 'No lighting' shader not found\n");
            mModel2D->SetShader(lShader);
        }

        if ( mModel2D && aSerializer.isMember("text") )
            mModel2D->mText = aSerializer["text"].asString();
    }

    Object3D* RenderableCmp::GetObject3D() const
    {
       return mModel3D;
    }
}
