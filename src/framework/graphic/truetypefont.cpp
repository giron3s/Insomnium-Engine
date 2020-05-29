/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *
 *  Brief       : Allows to load and render to a bitmap a true type font
 *                It also implements a cache of pre-renderered letters for later use. The cache is empty upon construction
 ********************************************************************************/
#include "precompiled.h"
#include "graphic/truetypefont.h"
#include "graphic/freetypefont.h"

namespace Framework
{
    TrueTypeFont *TrueTypeFont::New(void) { return new FreeTypeFont(); }
    void TrueTypeFont::Delete(TrueTypeFont *aFont) { delete aFont; }
}
