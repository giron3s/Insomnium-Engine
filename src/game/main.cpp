/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#include "precompiled.h"
#include "engine.h"
#include "floorplanner3d.h"
#include "core/logger.h"

#include "appmanager.h"
#include "catalogmanager.h"
#include "catalogview.h"
#include "objecteditormanager.h"
#include "ftuemanager.h"
#include "userconfig.h"
#include "engine/languagemanager.h"
#include "thememanager.h"

using namespace Framework;
using namespace Game;

#if _WIN32
    int WINAPI WinMain( HINSTANCE aInstance, HINSTANCE aPrevInstance, LPSTR apCmdLine,	int aShowCmd)
#elif __linux__
    int main(int argc, char *argv[])
#endif
{
    //Init engine
    Engine lEngine("data/config/default.config");
        
    //Register containers
    lEngine.ContainerFactory().Register<Game::UserConfig>();
	
	//Create managers - All managers needs to register before run the engine
    lEngine.ManagerFactory().Register<Framework::LanguageManager>();
	lEngine.ManagerFactory().Register<Game::CatalogManager>();
	lEngine.ManagerFactory().Register<Game::AppManager>();
	lEngine.ManagerFactory().Register<Game::ObjectEditorManager>();
	lEngine.ManagerFactory().Register<Game::FTUEManager>();
    lEngine.ManagerFactory().Register<Game::ThemeManager>();

    //Register objectFactory
    lEngine.ObjectFactory().Register<Game::UIFloorPlanner3D>();
    lEngine.ObjectFactory().Register<Game::UICatalogView>();

    //Run engine
    lEngine.Run();

    return true;       
}
