/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#pragma once

#include <map>
#include <vector>
#include "json/json.h"

namespace Framework
{
    class Config
    {
        using DataFilesContainerType = std::map<std::string, std::string>;

        NON_COPYABLE_CLASS( Config );
    public:
                                        Config(const std::string& aConfigFileName);
        virtual                         ~Config() = default;

        const glm::uvec2&               GetResolution() const { return mResolution; }
        bool                            GetFullScreen() const { return mFullscreen; }
        bool                            GetResizable() const { return mResizable; }
        float                           GetSelectionLineWidth() const { return mSelectionLineWidth; }
        const glm::vec4&                GetSelectionLineColor() const { return mSelectionLineColor; }
        float                           GetFocusLineWidth() const { return mFocusLineWidth; }
        const glm::vec4&                GetFocusLineColor() const { return mFocusLineColor; }
                                        
        struct FONT_INFO 
        {
            std::string path = "";
            int         size = 0;
            glm::vec4   color = glm::vec4(0,0,0,1);
        };
        const FONT_INFO& GetFontInfo() const { return mFontInfo; }

        float                           GetKeyboardSensibility() const { return mKeyboardSensibility; }
        float                           GetMouseSensibility() const { return mMouseSensibility; }
        float                           GetMouseZoomSensibility() const { return mMouseZoomSensibility; }

        const std::string&              GetGameName() const { return mGameName; }
        const std::string&              GetDataDirectory() const { return mDataDirectory; }

        const std::string&              GetPrefabFile(const std::string& aName);
        void                            GetPrefabFiles(std::vector<std::string>& aFilesOut) const;
        const std::vector<std::string>& GetResourceFiles() const { return mResourceFiles; }
        const std::string&              GetCatalogFile(const std::string& aName);
        const std::string&              GetInitialStateFile() const { return mInitialStateFile;  }
        const std::string&              GetLanguageFile() const { return mLanguageFile; }
        const std::string&              GetThemeFile() const { return mUIThemeFile; }

    protected:
        void                            LoadFromJson(const Json::Value& aSerializer);
        void                            LoadInputFromJson(const Json::Value& aSerializer);
        void                            LoadGraphicsFromJson(const Json::Value& aSerializer);
        void                            LoadResourcesFromJson(const Json::Value& aSerializer);
        void                            LoadPrefabsFromJson(const Json::Value& aSerializer);
        void                            LoadCatalogsFromJson(const Json::Value& aSerializer);

    private:
        Config() = default;

        std::string                mGameName;
        std::string                mInitialStateFile;
        std::string                mLanguageFile;
        std::string                mDataDirectory;
        std::string                mUIThemeFile;
        std::string                mLogFile;
        LogLevel                   mLogLevel;

        glm::uvec2                 mResolution;
        bool                       mFullscreen;
        bool                       mResizable;
        float                      mSelectionLineWidth = 1.0f;
        glm::vec4                  mSelectionLineColor = { 1.0f, 1.0f, 1.0f, 1.0f };
        float                      mFocusLineWidth = 1.0f;
        glm::vec4                  mFocusLineColor = { 1.0f, 1.0f, 1.0f, 1.0f };

        float                      mKeyboardSensibility;
        float                      mMouseSensibility;
        float                      mMouseZoomSensibility;

		FONT_INFO                  mFontInfo;

        std::vector< std::string > mResourceFiles;
        DataFilesContainerType     mCatalogFiles;
        DataFilesContainerType     mPrefabFiles;
};
}
