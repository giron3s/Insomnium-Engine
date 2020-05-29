/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#pragma once

#include "core/macros.h"

#ifdef _WIN32
    #include "json/json-forwards.h"
#elif __linux__
    #include "json/allocator.h"
    #include "json/assertions.h"
    #include "json/autolink.h"
    #include "json/config.h"
    #include "json/features.h"
    #include "json/forwards.h"
    #include "json/json.h"
    #include "json/reader.h"
    #include "json/value.h"
    #include "json/version.h"
#endif

namespace Framework
{
	class SerializableObject
	{
	public:
		SerializableObject() = default;
		virtual ~SerializableObject() = default;

		virtual void Serialize(Json::Value& aSerializer) const = 0;
		virtual void Deserialize(const Json::Value& aSerializer) = 0;
	};

    template<typename glm_vec_t>
    void SerializeVec(Json::Value& aSerializer, __in const glm_vec_t& aVec)
    {
        const int lVecLength = aVec.length();
        aSerializer = Json::Value(Json::ValueType::arrayValue);
        aSerializer.resize(lVecLength);
        for ( int i = 0; i < lVecLength; ++i )
            aSerializer[i] = aVec[i];
    }

    template<typename glm_vec_t>
    void DeserializeVec(const Json::Value& aSerializer, __out glm_vec_t& aVec)
    {
        const int lVecLength = aVec.length();
        assert(aSerializer.isArray() && aSerializer.size() == lVecLength);
        if ( std::is_floating_point< decltype(aVec.x) >::value )
            for ( int i = 0; i < lVecLength; ++ i )
                aVec[i] = aSerializer[i].asFloat();
        else if ( std::is_integral< decltype(aVec.x) >::value )
            for ( int i = 0; i < lVecLength; ++ i )
                aVec[i] = static_cast<decltype(aVec.x)>(aSerializer[i].asInt());
        else
            assert(0); // not supported by this template
    }
}
