/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#include "precompiled.h"

#include <fstream>

#include "jsoncpputils.h"

#include "core/logger.h"

#ifdef  _WIN32
    #include "jsoncpp.cpp"
#endif

using namespace Framework;

void Json::JsonUtils::Merge(Json::Value& a, const Json::Value& b)
{
	if (!a.isObject() || !b.isObject()) return;

	for (const auto& key : b.getMemberNames())
	{
		if (a[key].isObject()) 
		{
			Merge(a[key], b[key]);
		}
		else 
		{
			a[key] = b[key];
		}
	}
}

Json::Value::ArrayIndex Json::JsonUtils::GetComponentIdxByType(const Json::Value& components, const string& componentType)
{
    Json::Value::ArrayIndex idx = 0;
	for (const auto& component : components)
	{
		if (component["type"].asString() == componentType)
		{
			return idx;
		}
		++idx;
	}

	return -1;
}

 void Json::JsonUtils::OverrideEntity(Json::Value& a, const Json::Value& b)
 {
	Json::Value componentsCopy = a["components"];
	Json::Value componentsToAdd;

 	for (const auto& key : b.getMemberNames())
 	{
		a[key] = b[key];
 	}

	a["components"] = componentsCopy;

	for (const auto& componentB : b["components"])
	{
        Json::Value::ArrayIndex componentAIdx = GetComponentIdxByType(a["components"], componentB["type"].asString());
		
		if (componentAIdx == -1)
		{
			componentsToAdd.append(componentB);
		}
		else
		{
			Json::Value& componentA = a["components"][componentAIdx];

			for (const auto& key : componentB.getMemberNames())
			{
				componentA[key] = componentB[key];
			}
		}
	}

	if (!componentsToAdd.empty())
	{
		for (const auto& component : componentsToAdd)
		{
			a["components"].append(component);
		}
	}
 }


 bool Json::JsonUtils::OpenAndParseJsonFromFile(Json::Value& out, string filename)
 {
	 Json::CharReaderBuilder builder;
	 string errs;
	 bool lResult = false;

	 ifstream sceneFile(filename, ifstream::binary);
	 lResult = Json::parseFromStream(builder, sceneFile, &out, &errs);

     if (!lResult)
     {
         WARNING("Error parsing %s\n %s\n", filename.c_str(), errs.c_str());
     }

     return lResult;
 }
