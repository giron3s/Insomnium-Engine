/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#pragma once
#include "core/utils.h"
#include "core/serialization/jsoncpputils.h"

namespace Framework
{
    class Asset3D;
    class Model3D;
    class Asset2D;
    class Model2D;
    class Shader;
    class Project;

    class ResourceManager final
    {

    public:
                                 ResourceManager();
                                 ~ResourceManager();

        void                     Initialize(const std::vector<std::string>& aResourceFiles);
        void                     DeInitialize();

        // Shaders
        const Shader*            FindShader(const std::string& aName) const;
        void                     ClearShaders();

        // Assets/Models 3D
        bool                     AddAsset3D(const Json::Value& aSerializer);
        std::shared_ptr<const Asset3D> FindAsset3D(const std::string& aAssetName) const;
        void                     ClearAssets3D();
        Model3D*                 CreateModel3D(const std::string& aAssetName) const;
        
        // Assets/Models 2D
        bool                     AddAsset2D(const Json::Value& aSerializer);
        std::shared_ptr<const Asset2D> FindAsset2D(const std::string& aAssetName) const;
        void                     ClearAssets2D();
        Model2D*                 CreateModel2D(const std::string& aAssetName, float aWidth = 1.0f, float aHeight = 1.0f) const;

        // Images
        bool                     AddImage(const Json::Value& aSerializer);
        int                      FindImage(const string& aImageName);
        void                     ClearImages();

        // Projects
        bool                        AddProject(const string& aProjectFile);
        std::shared_ptr<Project>    GetProjectByFile(const string& aProjectFile);
        std::vector< std::string>   GetProjectList();
        void                        ClearProjectList();

    protected:

        std::vector<Shader*>                    mShaders;
        std::vector< std::shared_ptr<Asset3D> > mAssets3D;
        std::vector< std::shared_ptr<Asset2D> > mAssets2D;
        using ImageRecord = std::pair< std::string/*name*/, int/*id*/ >;
        std::vector<ImageRecord>                mImages;
        std::vector< std::shared_ptr<Project> > mProjects;
    };
}



