/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#pragma once

#include "json/json.h"

namespace Json
{
	namespace JsonUtils
	{
		void Merge(Json::Value& a, const Json::Value& b);
        Json::Value::ArrayIndex GetComponentIdxByType(const Json::Value& components, const std::string& componentType);
		void OverrideEntity(Json::Value& a, const Json::Value& b);

		bool OpenAndParseJsonFromFile(Json::Value& out, std::string filename);
	}
}
