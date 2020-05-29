/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*******************************************************************************/

#pragma once

#include "lua.hpp"
#include "LuaIntf.h"

#include <memory>
#include <vector>
#include <map>

namespace LuaIntf
{
	LUA_USING_SHARED_PTR_TYPE(std::shared_ptr);
	LUA_USING_LIST_TYPE(std::vector);
	LUA_USING_MAP_TYPE(std::map);
}
