/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#include "precompiled.h"
#include "graphic/asset2d.h"


namespace Framework
{

    void Asset2D::RenderReady()
    {
        if( mVertexData.empty() )
            return;
        //glm::vec2& lMin = const_cast<glm::vec2&>(mBoundingBox.GetMin());
        //glm::vec2& lMax = const_cast<glm::vec2&>(mBoundingBox.GetMax());
        mMaxLengthVertex = glm::vec2(0,0);
        float maxLength = 0.0f;
        for ( const auto& lElement : mVertexData )
        {
            // Calculate maximum lLength
            float lLength = glm::length(lElement.mVertex);
            if ( lLength > maxLength ) {
                maxLength = lLength;
                mMaxLengthVertex = lElement.mVertex;
            }
            //// Calculate the maximum and minimum for each axis
            //if ( lElement.mVertex.x < lMin.x )
            //    lMin.x = lElement.mVertex.x;
            //else if ( lElement.mVertex.x > lMax.x )
            //    lMax.x = lElement.mVertex.x;
            //if ( lElement.mVertex.y < lMin.y )
            //    lMin.y = lElement.mVertex.y;
            //else if ( lElement.mVertex.y > lMax.y )
            //    lMax.y = lElement.mVertex.y;
            //if ( lElement.mVertex.z < lMin.z )
            //    lMin.z = lElement.mVertex.z;
            //else if ( lElement.mVertex.z > lMax.z )
            //    lMax.z = lElement.mVertex.z;
        }
        mResourceName.clear();
        mVertexData.clear();
        mTextures.clear();
    }

}
