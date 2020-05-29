/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#include <precompiled.h>

#include "engine.h"
#include "engine/resourcemanager.h"
#include "engine/project.h"

#include "graphic/renderer.h"
#include "graphic/model3d.h"
#include "graphic/model2d.h"
#include "graphic/opengl/opengl_shader.h"
#include "graphic/procedural/texturedquad.h"

namespace Framework
{
    ResourceManager::ResourceManager()
    {}


    ResourceManager::~ResourceManager()
    {
        ClearAssets3D();
        ClearAssets2D();
        ClearImages();
        ClearShaders();
    }


    void ResourceManager::Initialize(const vector<string>& aResourceFiles)
    {
        // first, add all shaders that are in use

        // No lighting shader:
        Shader* lShader = new OpenGLShader("No lighting");
        string err;
        if ( !lShader->Load("lighting/no_lighting", err) )
            CRASH("Failed to load 'No lighting' shader: %s", err.c_str());        
        mShaders.push_back(lShader);

        // Shaders for deferred shading technique:
        lShader = new OpenGLShader("DeferredShading_GeometryPass");
        if ( !lShader->Load("lighting/deferred/geometry_pass", err) )
            CRASH("Failed to load 'Deferred shading - geometry pass' shader: %s", err.c_str());        
        mShaders.push_back(lShader);

        lShader = new OpenGLShader("DeferredShading_AmbientPass");
        if ( !lShader->Load("lighting/deferred/ambient_pass", err) )
            CRASH("Failed to load 'Deferred shading - ambient pass' shader: %s", err.c_str());        
        mShaders.push_back(lShader);

        lShader = new OpenGLShader("DeferredShading_DirectionalLightPass");
        if ( !lShader->Load("lighting/deferred/directional_light_pass", err) )
            CRASH("Failed to load 'Deferred shading - directional light pass' shader: %s", err.c_str());        
        mShaders.push_back(lShader);

        lShader = new OpenGLShader("DeferredShading_PointLightPass");
        if ( !lShader->Load("lighting/deferred/point_light_pass", err) )
            CRASH("Failed to load 'Deferred shading - point light pass' shader: %s", err.c_str());        
        mShaders.push_back(lShader);

        lShader = new OpenGLShader("DeferredShading_SpotLightPass");
        if ( !lShader->Load("lighting/deferred/spot_light_pass", err) )
            CRASH("Failed to load 'Deferred shading - spot light pass' shader: %s", err.c_str());        
        mShaders.push_back(lShader);

        // TODO: remove that later
        /*lShader = BlinnPhongShader::New(); // "Blinn-Phong lighting";
        if (lShader->Init() == false)
            CRASH("ERROR initializing Blinn-Phong lighting shader\n");
        mShaders.push_back(lShader);*/

        // Shadow map:
        lShader = OpenGLShader::New("Depth");
        if ( !lShader->Load("utils/depth", err) )
            CRASH("Failed to load 'utils/depth': %s\n", err.c_str());
        mShaders.push_back(lShader);

        // Gizmo drawing:
        lShader = new OpenGLShader("Gizmo drawing");
        if ( lShader->Load("utils/gizmos", err) )
            mShaders.push_back(lShader);
        else
        {
            WARNING("Failed to load Gizmo drawing shader: %s", err.c_str());
            delete lShader;
        }

        // Grid drawing 2D:
        lShader = new OpenGLShader("Grid drawing 2D");
        if ( lShader->Load("utils/grid2d", err) )
           mShaders.push_back(lShader);
        else
        {
            WARNING("Failed to load Grid drawing shader: %s", err.c_str());
            delete lShader;
        }

        // Grid drawing 3D:
        lShader = new OpenGLShader("Grid drawing 3D");
        if (lShader->Load("utils/grid3d", err))
            mShaders.push_back(lShader);
        else
        {
            WARNING("Failed to load Grid drawing 3D shader: %s", err.c_str());
            delete lShader;
        }

        // Now load models, images, scenes
        for ( const auto& lResourceFile : aResourceFiles )
        {
            Json::Value lRes;
            Json::JsonUtils::OpenAndParseJsonFromFile(lRes, lResourceFile);
            for ( auto lResModel : lRes["models"] )
            {
                if ( lResModel.isMember("asset3d") )
                    AddAsset3D(lResModel);
                if ( lResModel.isMember("asset2d") )
                    AddAsset2D(lResModel);
                if ( lResModel.isMember("icon") )
                    AddImage(lResModel);
            }
            for ( auto lImage : lRes["images"] )
                AddImage(lImage);
            for ( auto lProject : lRes["projects"] )
                AddProject(lProject.asString());
        }
    }


    void ResourceManager::DeInitialize()
    {
        ClearAssets3D();
        ClearAssets2D();
        ClearImages();
        ClearProjectList();
    }

    // Note: there is no AddShader

    const Shader* ResourceManager::FindShader(const string& aName) const
    {
        for (auto lShader : mShaders )
            if ( lShader->GetName() == aName )
                return lShader;
        return nullptr;
    }

    void ResourceManager::ClearShaders()
    {
        for ( auto lShader : mShaders )
            delete lShader;
        mShaders.clear();
    }


    bool ResourceManager::AddAsset3D(const Json::Value& aSerializer)
    {
        if ( !aSerializer.isMember("name") )
            CRASH("Object has no 'name' attribute!");

        if ( !aSerializer.isMember("asset3d") )
            CRASH("Object has no 'asset3d' attribute!");


        const string lName = aSerializer["name"].asString();
        const string lResourceName = aSerializer["asset3d"].asString();
        // check that we won't add duplicates :
        for ( auto lAsset : mAssets3D )
            if ( lAsset->GetName() == lName ||
                lAsset->GetResourceName() == lResourceName )
            {
                WARNING("Asset3D '%s'(%s) already exists and cannot be added again!",
                    lName.c_str(), lResourceName.c_str());
                return false;
            }
        mAssets3D.push_back(std::make_shared<Asset3D>(lName, lResourceName));
        return true;
    }


    bool ResourceManager::AddAsset2D(const Json::Value& aSerializer)
    {
        if ( !aSerializer.isMember("name") )
            CRASH("Object has no 'name' attribute!");

        if ( !aSerializer.isMember("asset2d") )
            CRASH("Object has no 'asset2d' attribute!");

        const string lName = aSerializer["name"].asString();
        const string lResourceName = aSerializer["asset2d"].asString();
        // check that we won't add duplicates :
        for ( auto lAsset : mAssets2D )
            if ( lAsset->GetName() == lName /*|| // ignore duplicate resource names for now
                lAsset->GetResourceName() == lResourceName*/ )
            {
                WARNING("Asset2D '%s'(%s) already exists and cannot be added again!",
                    lName.c_str(), lResourceName.c_str());
                return false;
            }
        mAssets2D.push_back(std::make_shared<Asset2D>(lName, lResourceName));
        return true;
    }

		
	Model3D* ResourceManager::CreateModel3D(const string& aAssetName) const
    {
        auto lAsset = FindAsset3D(aAssetName);
        if ( !lAsset )
        {
            WARNING("Asset3D '%s' does not exist in the resource manager!", aAssetName.c_str());
            return nullptr;
        }
        if ( !lAsset->mRendererResources )
        {
            Asset3D* lAsset_ = const_cast<Asset3D*>(lAsset.get());
            if ( !lAsset->GetResourceName().empty() )
                if ( !lAsset_->Load(lAsset->GetResourceName()) )
                {
                    WARNING("Failed to load asset %s(%s)", lAsset->GetName().c_str(), lAsset->GetResourceName().c_str());
                    return nullptr;
                }
            auto lRenderer = Engine::Instance()->Display().GetRenderer();
            if ( !lRenderer->PrepareForRendering(*lAsset_) )
            {
                WARNING("Failed to prepare asset %s", lAsset->GetName().c_str());
                return nullptr;
            }
        }
        return new Model3D(std::move(lAsset));
    }


    std::shared_ptr<const Asset3D> ResourceManager::FindAsset3D(const string& aAssetName) const
    {
        for (auto& lAsset : mAssets3D )
            if ( lAsset->GetName() == aAssetName )
                return lAsset;
        return nullptr;
    }


    Model2D* ResourceManager::CreateModel2D(const string& aAssetName, float aWidth, float aHeight) const
    {
        auto lAsset = FindAsset2D(aAssetName);
        if ( !lAsset )
        {
            WARNING("Asset2D '%s' does not exist in the resource manager!", aAssetName.c_str());
            return nullptr;
        }
        if ( !lAsset->mRendererResources )
        {
            Asset2D* lAsset_ = const_cast<Asset2D*>(lAsset.get());
            Texture lTexture;
            ASSERT(!lAsset->GetResourceName().empty());
            if ( !lTexture.Load(lAsset->GetResourceName()) )
            {
                WARNING("Failed to load asset %s(%s)", lAsset->GetName().c_str(), lAsset->GetResourceName().c_str());
                // create a stub texture instead of returning nullptr
                lTexture.Clear();
                lTexture.mName.assign("#stub_pink");
                lTexture.mWidth = lTexture.mHeight = 8;
                lTexture.mBytesPerPixel = 3;
                lTexture.mFormat = GL_RGB;
                lTexture.mType = GL_UNSIGNED_BYTE;
                lTexture.mPixels.resize(lTexture.mWidth * lTexture.mHeight * lTexture.mBytesPerPixel, 0);
                for ( size_t i = 0; i < lTexture.mPixels.size(); i += 3 )
                    lTexture.mPixels[i] = lTexture.mPixels[i + 2] = 0xff;
            }
            Procedural::TexturedQuad lTexQuadAsset(aWidth, aHeight, std::move(lTexture));
            const_cast<string&>(lTexQuadAsset.GetName()) = lAsset_->GetName(); // hack!
            *lAsset_ = std::move(lTexQuadAsset);
            auto lRenderer = Engine::Instance()->Display().GetRenderer();
            if ( !lRenderer->PrepareForRendering(*lAsset_) )
            {
                WARNING("Failed to prepare asset %s", lAsset->GetName().c_str());
                return nullptr;
            }
        }
        return new Model2D(std::move(lAsset));
    }


    std::shared_ptr<const Asset2D> ResourceManager::FindAsset2D(const string& aAssetName) const
    {
        for (auto& lAsset : mAssets2D )
            if ( lAsset->GetName() == aAssetName )
                return lAsset;
        return nullptr;
    }


    void ResourceManager::ClearAssets3D()
    {
        mAssets3D.clear();
    }


    void ResourceManager::ClearAssets2D()
    {
        mAssets2D.clear();
    }

    bool ResourceManager::AddImage(const Json::Value& aSerializer)
    {
        if(!aSerializer.isMember("name"))
            CRASH("Image has no 'name' attribute!");
        const std::string lName = aSerializer["name"].asString();
        std::string lResourceFileName;
        if ( aSerializer.isMember("file") )
            lResourceFileName = aSerializer["file"].asString();
        if ( aSerializer.isMember("icon") && lResourceFileName.empty() )
            lResourceFileName = aSerializer["icon"].asString();
        if( lResourceFileName.empty() )
            CRASH("Image has no 'file' / 'icon' attribute or has both!");
        if ( FindImage(lName) > 0 )
        {
            WARNING("Image '%s'(%s) already exists and cannot be added again!", lName.c_str(), lResourceFileName.c_str());
            return false;
        }

        /* note: here we do not check for adding the same image resource
        with different names */
        UIScreen& lUIScreen = Engine::Instance()->Display();
        int lImageId = nvgCreateImage(lUIScreen.nvgContext(), lResourceFileName.c_str(), 0);
        if (lImageId > 0)
            mImages.emplace_back( std::move(lName), lImageId);
        else
            WARNING("Could not load image \"%s\"", lName.c_str());
        return true;
    }

    int ResourceManager::FindImage(const string& aName)
    {
        for ( auto& lImage : mImages )
            if ( lImage.first == aName )
                return lImage.second;
        return -1;
    }

    void ResourceManager::ClearImages()
    {
        UIScreen& lUIScreen = Engine::Instance()->Display();
        for (auto& lImage : mImages)
            nvgDeleteImage(lUIScreen.nvgContext(), lImage.second);
        mImages.clear();
    }

    bool ResourceManager::AddProject(const string& aProjectFile)
    {
        if (GetProjectByFile(aProjectFile))
        {
            WARNING("Project '%s already exists and cannot be added again!", aProjectFile.c_str());
            return false;
        }

        auto lProject = std::make_shared<Project>(aProjectFile);
        mProjects.push_back(lProject);
        return true;
    }

    std::shared_ptr<Project> ResourceManager::GetProjectByFile(const string& aProjectFile)
    {
        for ( auto& lProject : mProjects )
            if ( lProject->GetFileName().compare(aProjectFile) == 0 )
                return lProject;
        return std::shared_ptr<Project>();
    }

    std::vector<std::string> ResourceManager::GetProjectList()
    {
        std::vector<string> lProjectList;
        for (auto& lProject : mProjects)
        {
            lProjectList.push_back(lProject->GetName());
        }

        return lProjectList;
    }

    void ResourceManager::ClearProjectList()
    {
        mProjects.clear();
    }
}
