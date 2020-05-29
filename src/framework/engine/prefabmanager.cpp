/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#include "precompiled.h"

#include <fstream>

#include "json/json.h"

#include "core/config.h"

#include "engine/prefabmanager.h"


namespace Framework
{

Json::Value PrefabManager::GetPrefab(const std::string& aPrefabName)
{
    std::map<const std::string, Json::Value>::iterator lResult = m_Database.find(aPrefabName);
    if (lResult == m_Database.end())
    {
        CRASH("Not find any '%s' in the prefabManager", aPrefabName.c_str());
    }
	return m_Database[aPrefabName];
}

void PrefabManager::AddPrefab(const std::string prefabFileName)
{
	Json::Value prefabJson;
	Json::Reader reader;

	std::ifstream prefabFile(prefabFileName, std::ifstream::binary);
	if (!reader.parse(prefabFile, prefabJson, false))
	{
		WARNING( "Error parsing %s\n", prefabFileName.c_str() );
        WARNING( "Error message: %s\n", reader.getFormattedErrorMessages().c_str() );
		ASSERT(false);
	}

	Json::Value prefabdefinition = prefabJson["prefabdefinition"];

	ASSERT(!prefabdefinition.empty());

	std::string name = prefabdefinition["name"].asString();

	ASSERT(m_Database.find(name) == m_Database.end());

	m_Database[name] = prefabdefinition;
}

void PrefabManager::GetPrefabsFromConfig(const Config& config)
{
	std::vector<std::string> files;
	config.GetPrefabFiles(files);

	for (const auto& file : files)
	{
		AddPrefab(file);
	}
}

}
