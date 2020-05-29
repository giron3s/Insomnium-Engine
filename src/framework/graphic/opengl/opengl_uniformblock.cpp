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
#include "precompiled.h"
#include "graphic/opengl/opengl_uniformblock.h"

namespace Framework
{
    OpenGLUniformBlock::OpenGLUniformBlock()
        : mProgramID(0)
        , mBindingPoint(0)
        , mLinkedToShader(false)
        , mBlockArrayIndex(-1)
        , mBlockIndex(0)
        , mBlockSize(0)
        , mUniformBufferObj(0)
    {
        //static GLuint sNextBindingPoint = 0;
        //mBindingPoint = sNextBindingPoint++;
    }

    OpenGLUniformBlock::~OpenGLUniformBlock()
    {
        if (mLinkedToShader)
        {
            glDeleteBuffers(1, &mUniformBufferObj);
        }
    }

    void OpenGLUniformBlock::AddParamName(const string &aName)
    {
        mParamsOffsets[aName] = -1;
        mParamsFullName.push_back(mBlockName + string(".") + aName);
    }

    bool OpenGLUniformBlock::PrepareForShader(GLuint aProgramID)
    {
        /* Prepare array of lNames pointers */
        std::vector<const GLchar*> lNames(mParamsFullName.size(), nullptr);
        for (size_t i = 0; i < mParamsFullName.size(); ++i)
        {
            lNames[i] = mParamsFullName[i].c_str();
        }

        string lAccessName;
        if (mBlockArrayIndex == -1) {
            lAccessName = mBlockName;
        } else {
            lAccessName = mBlockName + string("[") + to_string(mBlockArrayIndex) + string("]");
        }

        __(mBlockIndex = glGetUniformBlockIndex(aProgramID, lAccessName.c_str()));
        if (mBlockIndex == GL_INVALID_INDEX)
        {
            CRASH("ERROR OpenGLUniformBlock bad block index for block: %s\n", lAccessName.c_str());
            return false;
        }

        ASSERT(mBlockSize == 0);
        __(glGetActiveUniformBlockiv(aProgramID, mBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &mBlockSize));
        ASSERT(0 < mBlockSize && mBlockSize < 256);

        std::vector<GLuint> lIndices(mParamsFullName.size(), GL_INVALID_INDEX);
        __(glGetUniformIndices(aProgramID, lNames.size(), lNames.data(), lIndices.data()));
        for (size_t i = 0; i < lIndices.size(); ++i)
        {
            if (lIndices[i] == GL_INVALID_INDEX)
            {
                CRASH("ERROR OpenGLUniformBlock could not get all indices\n");
                return false;
            }
        }

        /* Allocate data buffer */
        mParamsBuffer.resize(mBlockSize, 0);

        /* Retrieve the lOffsets*/
        std::vector<GLint> lOffsets(mParamsFullName.size(), -1);
        __(glGetActiveUniformsiv(aProgramID, lIndices.size(), lIndices.data(), GL_UNIFORM_OFFSET, lOffsets.data()));
        for (size_t i = 0; i < lOffsets.size(); ++i)
        {
            if (lOffsets[i] == -1)
            {
                CRASH("ERROR OpenGLUniformBlock  - invalid offset(s)\n");
                return false;
            }
        }

        /* Associate them with their lNames */
        for (size_t i = 0; i < mParamsFullName.size(); ++i)
        {
            string baseName = mParamsFullName[i].substr(mBlockName.length() + 1);
            mParamsOffsets[baseName] = lOffsets[i];
        }

        __(glGenBuffers(1, &mUniformBufferObj));
        __(glBindBuffer(GL_UNIFORM_BUFFER, mUniformBufferObj));
        __(glBufferData(GL_UNIFORM_BUFFER, mBlockSize, mParamsBuffer.data(), GL_DYNAMIC_DRAW)); // set all to 0 and provide usage flag
        // fishy, fishy...
        __(glUniformBlockBinding(aProgramID, mBlockIndex, mBindingPoint));
        __(glBindBufferBase(GL_UNIFORM_BUFFER, mBindingPoint, mUniformBufferObj));
        __(glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE));

        mLinkedToShader = true;
        mProgramID = aProgramID;

        return true;
    }

    void OpenGLUniformBlock::Bind()
    {
        __(glBindBuffer(GL_UNIFORM_BUFFER, mUniformBufferObj));
        __(glBufferSubData(GL_UNIFORM_BUFFER, 0, mBlockSize, mParamsBuffer.data()));
        __(glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE));
    }
}
