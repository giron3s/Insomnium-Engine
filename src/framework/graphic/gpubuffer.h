/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#pragma once

#include "precompiled.h"
#include <vector>

namespace Framework
{
    class GPUBufferBase
    {
    public:
        virtual ~GPUBufferBase() {}

        virtual void Bind(GLenum aTarget) const = 0;
    };


    template<typename bufferDataType>
    class GPUBuffer : public GPUBufferBase
    {
    protected:
        GLuint    mID;             //OpenGL handle
        bool      mAutoBind;
        GLenum    mTarget;         //Target to bind

    public:
        GPUBuffer(GLenum aTarget = GL_ARRAY_BUFFER, bool aAutoBind = true )
        {
            mID       = 0;
            mAutoBind = aAutoBind;
            mTarget   = aTarget;

            glGenBuffers(1, &mID);  // 1 for 1 buffer
        }

        ~GPUBuffer()
        {
            glDeleteBuffers(1, &mID);
        }

        // Copy constructor, makes a new OpenGL buffer. Unbinds copy buffers!
        GPUBuffer(const GPUBuffer& aOther)
        {
            // For newer OpenGL versions: int oldCopyWriteBuffer = glGetIntegerv(GL_COPY_WRITE_BUFFER_BINDING);
            // int oldCopyReadBuffer = glGetIntegerv(GL_COPY_READ_BUFFER_BINDING);

            mAutoBind = aOther.mAutoBind;
            SetTarget(aOther.mTarget);

            glGenBuffers(1, &mID);

            // Code to make sure the data and the flags are the same
            GLint lIsImmutable;
            aOther.Bind();
            glGetBufferParameteriv(aOther.mTarget, GL_BUFFER_IMMUTABLE_STORAGE, &lIsImmutable);

            if(lIsImmutable)
            {
                GLint immutableFlags;
                aOther.Bind();
                glGetBufferParameteriv(aOther.mTarget, GL_BUFFER_STORAGE_FLAGS, &immutableFlags);

                SetImmutableData(aOther.Read(), immutableFlags);
            }
            else
            {
                GLint usage;
                aOther.Bind();
                glGetBufferParameteriv(aOther.mTarget, GL_BUFFER_USAGE, &usage);

                SetMutableData(aOther.Read(), usage);
            }
        }

        GLuint GetID()
        {
            return mID;
        }

        GLenum GetTarget()
        {
            return mTarget;
        }

        void SetTarget(GLenum aTarget)
        {
            mTarget = aTarget;
        }

        void Bind(GLenum aTarget) const
        {
            if (mAutoBind)
                glBindBuffer(aTarget, mID);
        }

        void Bind() const
        {
            Bind(mTarget);
        }

        //Returns the buffer's size, in bytes
        size_t GetSize() const
        {
            GLint lGLintBufferSize;
            Bind();
            glGetBufferParameteriv(mTarget, GL_BUFFER_SIZE, &lGLintBufferSize);
            return static_cast<size_t>(lGLintBufferSize);
        }

        //Get the amount of element in the buffer
        int GetLength() const
        {
            int lBufferSize = GetSize();
            return lBufferSize / sizeof(bufferDataType);
        }

        void SetMutableData(const vector<bufferDataType>& aData, GLenum aUsage)
        {
            Bind();
            glBufferData(mTarget, sizeof(bufferDataType) * aData.size(), aData.data(), aUsage); //Vector.size() returns the amount of elements
        }

        void SetMutableData(const vector<bufferDataType>& aData)
        {
            SetMutableData(aData, GL_DYNAMIC_DRAW);
        }

        // Allocate memory only
        void SetMutableData(std::size_t aSize, GLenum aUsage = GL_DYNAMIC_DRAW)
        {
            Bind();
            glBufferData(mTarget, aSize, nullptr, aUsage);
        }

        void SetImmutableData(const std::vector<bufferDataType>& aData, GLenum aImmutableFlags) // immutableFlags being a bitwise operation
        {
            Bind();
            glBufferStorage(mTarget, sizeof(bufferDataType) * aData.size(), aData.data(), aImmutableFlags);
        }

        // Easier Lua binding, see setMutableData(data)
        void SetImmutableData(const std::vector<bufferDataType>& aData)
        {
            // Binding is done in there:
            SetImmutableData(aData, GL_DYNAMIC_STORAGE_BIT); // Best flag I could find?
        }

        // Allocate memory only
        void SetImmutableData(std::size_t aSize, GLenum aImmutableFlags = GL_DYNAMIC_STORAGE_BIT)
        {
            Bind();
            glBufferStorage(mTarget, aSize, nullptr, aImmutableFlags);
        }

        std::vector<bufferDataType> Read(GLintptr aOffset, GLsizeiptr aSize) const
        {
            Bind();
            std::vector<bufferDataType> lData(aSize / sizeof(bufferDataType)); // Allocate
            glGetBufferSubData(mTarget, aOffset, aSize, lData.data());

            return lData;
        }

        // Read all of the data
        std::vector<bufferDataType> Read() const
        {
            return Read(0, GetSize());
        }

        // If the buffer is immutable, make sure you gave the right immutableFlags to make it changeable
        // Will replace the bytes starting at offset
        void Modify(GLintptr aOffset, const std::vector<bufferDataType>& aData)
        {
            Bind();
            glBufferSubData(mTarget, aOffset, sizeof(bufferDataType) * aData.size(), aData.data());
        }
    };
}
