/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*   Brief      : Triangle procedural generation
*******************************************************************************/

#include "precompiled.h"
#include "graphic/procedural/triangle.h"
#include "core/graphic/assettransform.h"

namespace Framework
{
    namespace Procedural
    {
        Triangle::Triangle(const glm::vec3 &aVertex0, const glm::vec3 &aVertex1, const glm::vec3 &aVertex2, const glm::vec3 &aColor)
            : Asset3D("#triangle", "")
            , mColor(aColor)
        {
            glm::vec3 lNormal = glm::normalize(glm::cross(aVertex2 - aVertex1, aVertex0 - aVertex1));

            mVertexData.resize(3);
            mVertexData[0].mVertex = aVertex0;
            mVertexData[0].mNormal = lNormal;
            mVertexData[1].mVertex = aVertex1;
            mVertexData[1].mNormal = lNormal;
            mVertexData[2].mVertex = aVertex2;
            mVertexData[2].mNormal = lNormal;

            mVertexIndices.resize(3);

            mVertexIndices[0] = 0;
            mVertexIndices[1] = 1;
            mVertexIndices[2] = 2;

            AssetTransform::SetUniqueMaterialFromColor(*this, mColor);
        }
    }
}
