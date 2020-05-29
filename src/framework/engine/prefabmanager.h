/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#pragma once

#ifdef _WIN32
    #include "json/json-forwards.h"
#endif
#include <map>

namespace Framework
{
	class Config;

	class PrefabManager
	{
	public:
		Json::Value GetPrefab(const std::string& prefabName);
		void AddPrefab(const std::string prefabFile);

	private:
		friend class Engine;

		void GetPrefabsFromConfig(const Config& config);
		PrefabManager() = default;
		~PrefabManager() = default;

		std::map<const std::string, Json::Value> m_Database;

	};
}
