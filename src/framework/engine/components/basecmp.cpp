/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#include "precompiled.h"

#include <engine/components/basecmp.h>
#include <climits>

namespace Framework
{
    BaseCmp::BaseCmp() : mOwner(nullptr)
    { 

    }

    BaseCmp::~BaseCmp()
    {     

    }
    
    void BaseCmp::Update()
    {     
    }

	void BaseCmp::Serialize(Json::Value& serializer) const
	{
	}

	void BaseCmp::Deserialize(const Json::Value& serializer)
	{
	}

   uint32_t BaseCmp::GetUpdatePriority() const
   {
      // lowest update priority
      return 0;
   }
}
