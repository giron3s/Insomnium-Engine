/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
*   Brief      : Procedural objects generation library
*******************************************************************************/

#include "precompiled.h"
#include "graphic/asset3d.h"
#include "graphic/procedural/proceduralutils.h"
#include <glm/gtx/quaternion.hpp>

namespace Framework
{
    namespace Procedural
    {
        #define PI 3.14159265358979323846

        void AppendBentPlane(Asset3D &aAsset, float aWidth, float aHeight, float aAngleWidth, float aAngleHeight, float aAngleRadius,
                             uint32_t aNumVertsWidth, uint32_t aNumVertsHeight)
        {
            float lRadiusWidth, lOffsetWidth, lAngleIncrementWidth;
            float lRadiusHeight, lOffsetHeight, lAngleIncrementHeight;
            float lAngleIncrementRadius;

            glm::vec3 lCenter(0.0f, 0.0f, 0.0f);

            if (aNumVertsWidth < 2)
            {
                aNumVertsWidth = 2;
            }
            if (aNumVertsHeight < 2)
            {
                aNumVertsHeight = 2;
            }

            float lNumEdgesWidth = (float)(aNumVertsWidth - 1);
            float lNumEdgesHeight = (float)(aNumVertsHeight - 1);

            float lHalfWidth = aWidth / 2.0f;
            float lHalfHeight = aHeight / 2.0f;

            /*
             * For each row of vertices of the plane two indices are needed
             * per column, plus one at the beggining and other at the end
             * for degenerate triangles. That is number of 2*columns + 2.
             * Then multiplied by the number of rows minus one, as the last
             * row does have to generate more triangles
             */
            aAsset.mVertexData.resize(aNumVertsWidth * aNumVertsHeight);
            aAsset.mVertexIndices.resize((size_t)(2 * 3 * lNumEdgesWidth * lNumEdgesHeight)); /** 2 triangles, 3 vertices each */

            Asset3D::VertexData *lData = &aAsset.mVertexData[0];

            /* Set parameters for generation along the z-axis */
            if (aAngleWidth != 0.0f) 
            {
                lRadiusWidth = aWidth / aAngleWidth;
                lOffsetWidth = (float)(lRadiusWidth * glm::sin((PI + aAngleWidth) / 2.0f));
                lCenter = glm::vec3(0.0f, -lOffsetWidth, 0.0f);
            }
            else 
            {
                lRadiusWidth = lHalfWidth;
            }
            lAngleIncrementWidth = aAngleWidth / lNumEdgesWidth;

            /* Set parameters for generation along the x-axis */
            if (aAngleHeight != 0.0f)
            {
                lRadiusHeight = aHeight / aAngleHeight;
                lOffsetHeight = (float)(lRadiusHeight * glm::sin((PI + aAngleHeight) / 2.0f));
                lCenter += glm::vec3(0.0f, -lOffsetHeight, 0.0f);
            }
            else
            {
                lRadiusHeight = lHalfHeight;
            }
            lAngleIncrementHeight = aAngleHeight / lNumEdgesHeight;

            lAngleIncrementRadius = aAngleRadius / 2.0f / lNumEdgesHeight;

            float lVertexAngleHeight = (float)((PI - aAngleHeight) / 2.0f);
            float lVertexAngleRadius = (float)((2.0f * PI - aAngleRadius) / 4.0f);

            for (unsigned int i = 0, count = 0; i < aNumVertsHeight;
                 ++i, lVertexAngleHeight += lAngleIncrementHeight, lVertexAngleRadius += lAngleIncrementRadius)
            {
                glm::vec3 lOffset;
                glm::mat4 lRotation(1.0f);

                if (aAngleWidth != 0.0f && aAngleHeight != 0.0f)
                {
                    lRotation = glm::toMat4(glm::quat(glm::vec3(lVertexAngleHeight - PI / 2.0f, 0.0f, 0.0f)));
                }

                if (aAngleHeight != 0.0f) 
                {
                    lOffset = glm::vec3(0.0f, glm::sin(lVertexAngleHeight), -glm::cos(lVertexAngleHeight));
                }
                else if (aAngleRadius != 0.0f)
                {
                    lOffset = glm::vec3(0.0f, 0.0f, -glm::cos(lVertexAngleRadius));
                }
                else
                {
                    lOffset = glm::vec3(0.0f, 0.0f, -1.0f + 2.0f * i / (float)lNumEdgesHeight);
                }

                float lRadius = lRadiusWidth * sin(lVertexAngleRadius);

                float lVertexAngleWidth = (float)((PI + aAngleWidth) / 2.0f);
                for (unsigned int j = 0; j < aNumVertsWidth; ++j, lVertexAngleWidth -= lAngleIncrementWidth)
                {
                    glm::vec3 lUnitVertex, lNormal;

                    if (aAngleWidth != 0.0f)
                    {
                        lUnitVertex = glm::vec3(cos(lVertexAngleWidth), sin(lVertexAngleWidth), 0.0f);
                        lNormal = lUnitVertex;
                    } else {
                        lUnitVertex = glm::vec3(-1.0 + 2 * j / (float)lNumEdgesWidth, 0.0f, 0.0f);
                        lNormal = glm::vec3(0.0f, 1.0f, 0.0f);
                    }

                    lUnitVertex = glm::vec3(lRotation * glm::vec4(lUnitVertex, 1.0f)) * lRadius + lOffset * lRadiusHeight;
                    lNormal = glm::vec3(lRotation * glm::vec4(lNormal, 1.0f));

                    lData[count].mVertex = lUnitVertex + lCenter;
                    if (aAngleRadius != 0.0f) 
                    {
                        lData[count].mNormal = glm::normalize(lUnitVertex);
                    }
                    else
                    {
                        lData[count].mNormal = lNormal;
                    }
                    count++;
                }
            }

            /* Generate the indices */
            uint32_t *lIndex = &aAsset.mVertexIndices[0];
            for (unsigned int i = 0, count = 0; i < lNumEdgesHeight; ++i)
            {
                for (unsigned int j = 0; j < lNumEdgesWidth; ++j)
                {
                    uint32_t span = i * aNumVertsWidth;
                    lIndex[count++] = j + span;
                    lIndex[count++] = j + span + aNumVertsWidth;
                    lIndex[count++] = j + span + 1;

                    lIndex[count++] = j + span + 1;
                    lIndex[count++] = j + span + aNumVertsWidth;
                    lIndex[count++] = j + span + aNumVertsWidth + 1;
                }
            }
        }
    }
}
