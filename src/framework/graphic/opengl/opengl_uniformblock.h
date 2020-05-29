/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential

 * Brief        : Manages the access to a uniform block in a shader. It takes care
 *                of finding out the block size and offsets, then allows to set the
 *                values in the block and bind it
*******************************************************************************/
#pragma once

#include <string.h>
#include <map>
#include <string>
#include <vector>
#include "graphic/opengl/opengl.h"

namespace Framework
{
    class OpenGLUniformBlock
    {
      public:
        OpenGLUniformBlock();
        ~OpenGLUniformBlock();

        void SetBlockName(const std::string &aBlockName) { mBlockName = aBlockName; }
        void SetBlockArrayIndex(uint32_t aBlockArrayIndex) { mBlockArrayIndex = aBlockArrayIndex; }
        void SetBindingPoint(uint32_t aBindingPoint) { mBindingPoint = aBindingPoint; }
        void AddParamName(const std::string &aName);

        bool PrepareForShader(GLuint aProgramID);

        template <typename T>
        bool SetParamValue(const std::string &aName, const T &aValue)
        {
            ASSERT( mParamsOffsets[aName] + sizeof(aValue) <= mParamsBuffer.size() );
            memcpy(mParamsBuffer.data() + mParamsOffsets[aName], &aValue, sizeof aValue);
            return true;
        }

        void Bind();

      private:
        bool                         mLinkedToShader;

        std::string                  mBlockName;
        int32_t                      mBlockArrayIndex;
        GLuint                       mBindingPoint;
        std::map<std::string, GLint> mParamsOffsets;
        std::vector<std::string>     mParamsFullName;

        GLuint                       mProgramID;
        GLuint                       mBlockIndex;
        GLint                        mBlockSize;
        std::vector<GLubyte>         mParamsBuffer;
        GLuint                       mUniformBufferObj;
    };
}
