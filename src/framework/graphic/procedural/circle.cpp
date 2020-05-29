/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*   Brief      : Circle procedural generation
*******************************************************************************/

#include "precompiled.h"
#include "graphic/procedural/circle.h"
#include "core/graphic/assettransform.h"


namespace Framework
{
    namespace Procedural
    {

        #define PI 3.14159265358979323846

        Circle::Circle(float aRadius, const glm::vec3 &aColor, uint32_t aNumVertsSide) 
            : Asset3D("#circle", "")
            , mRadius(aRadius)
            , mColor(aColor)
            , mNumVertsSide(aNumVertsSide)
        {
            glm::vec3 lNormal(0.0f, 1.0f, 0.0f);
            uint32_t lCount = 0;

            /* A circle has a center vertex and '_numVertsSide' border vertices */
            mVertexData.resize(1 + mNumVertsSide);
            Asset3D::VertexData *lData = &mVertexData[0];

            /* Vertices */
            lData[lCount].mVertex = glm::vec3(0.0f, 0.0f, 0.0f);
            lData[lCount].mNormal = lNormal;
            lCount++;

            for (unsigned int i = 0; i < mNumVertsSide; ++i) 
            {
                float lAngle = (float)(2.0f * PI * i / mNumVertsSide);

                lData[lCount].mVertex = glm::vec3(mRadius * glm::cos(lAngle), 0.0f, mRadius * glm::sin(lAngle));
                lData[lCount].mNormal = lNormal;
                lCount++;
            }

            /* Faces */

            /* A cap has '_numVertsSide' triangles */
            mVertexIndices.resize(mNumVertsSide * 3);
            uint32_t *lIndex = &mVertexIndices[0];

            /* Bottom lid */
            lCount = 0;
            for (unsigned int i = 0; i < mNumVertsSide - 1; ++i)
            {
                lIndex[lCount++] = 0;
                lIndex[lCount++] = 2 + i;
                lIndex[lCount++] = 1 + i;
            }

            /* Bottom lid last triangle */
            lIndex[lCount++] = 0;
            lIndex[lCount++] = 1;
            lIndex[lCount++] = mNumVertsSide;

            AssetTransform::SetUniqueMaterialFromColor(*this, mColor);
        }
    }
}
