/*******************************************************************************
*  Author      : giron3s
*  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
*                Unauthorized copying of this file, via any medium is strictly prohibited
*                Proprietary and confidential
*
   Brief       : Class to abstract shader functionality
*
*******************************************************************************/
#pragma once

#include <stdint.h>
#include "glm/glm.hpp"
#include <map>
#include <string>

namespace Framework
{
    class Shader
    {
    protected:
        const std::string mName;

    public:

        inline const std::string& GetName() const { return mName; }

        static Shader* New(const std::string& aName);

        Shader(const std::string& aName) : mName(aName)
        {}

        virtual ~Shader() = default;

        virtual bool Init() = 0;

        /**
        * Return OpenGL program's id
        */
        virtual uint32_t GetProgramID() const = 0;

        /**
         * Loads a new shader code and compiles it. Path contains the subdirectory
         * inside data/shaders/ and the name of the shader files without the extension.
         *
         * i.e.: anti-aliasing/fxaa will load data/shaders/anti-aliasing/fxaa.vert
         *       and data/shaders/anti-aliasing/fxaa/frag
         *
         * @param		path  Path to the vertex/fragment shader pair
         * @param[out]	error If compilation fails error will contain
         *					  a description of the error
         *
         * @return true or false
         */
        virtual bool Load(const std::string& path, std::string& error) = 0;

    protected:
        /**
         * Loads a new vertex shader code and compiles it
         *
         * @param		filename  File name of the vertex shader to be compiled
         * @param[out]	error	  If compilation fails error will contain
         *						  a description of the error
         *
         * @return true or false
         */
        virtual bool LoadVertexShader(const std::string& filename, std::string& error) = 0;

        /**
         * Loads a new geometry shader code and compiles it
         *
         * @param		filename  File name of the geometry shader to be compiled
         * @param[out]	error	  If compilation fails error will contain
         *						  a description of the error
         *
         * @return true or false
         */
        virtual bool LoadGeometryShader(const std::string& filename, std::string& error) = 0;

        /**
         * Loads a new fragment shader code and compiles it
         *
         * @param		filename  File name of the fragment shader to be compiled
         * @param[out]	error	  If compilation fails error will contain
         *						  a description of the error
         *
         * @return true or false
         */
        virtual bool LoadFragmentShader(const std::string& filename, std::string& error) = 0;

        /**
         * Links the program together and prepares the shader to be used
         *
         * @param[out] error	If linking fails error will contain a
         *						description of the error
         *
         * @return true or false
         */
        virtual bool LinkProgram(std::string& error) = 0;

    public:
        /**
         * Makes the shader active
         */
        virtual bool Attach(void) const = 0;

        /**
         * Makes the shader inactive
         */
        virtual bool Detach(void) const = 0;

        /**
         * Gets a list of all the shader uniforms
         *
         * @return A vector with all the available uniform names
         */
        virtual const std::map<std::string, uint32_t>& GetUniforms(void) const = 0;

        /**
         * Retrieves the ID of a specific uniform
         *
         * @param name  Name of the uniform
         * @param id    Output ID
         *
         */
        virtual const bool GetUniformID(const std::string& aName, uint32_t* id) const = 0;
        virtual const bool GetAttributeID(const std::string& aName, uint32_t* id) const = 0;

        /**
         * Sets the aValue of a shader uniform as a mat4x4
         *
         * @param name  Name of the shader uniform
         * @param aValue Value of the uniform to be set
         *
         * @return true if the aValue was set or false if the
         *         attribute cannot be found
         */

        virtual bool SetUniformMat4(const std::string& aName, const glm::mat4 aValue[], uint32_t aNumItems = 1) const = 0;
        virtual bool SetUniformMat3(const std::string& aName, const glm::mat3 aValue[], uint32_t aNumItems = 1) const = 0;
        virtual bool SetUniformTexture2D(const std::string& aName, uint32_t aUnitID) const = 0;
        virtual bool SetUniformTexture2DArray(const std::string& aName, uint32_t aUnitIDs[], uint32_t aNumItems) const = 0;
        virtual bool SetUniformFloat(const std::string& aName, float aValue) const = 0;
        virtual bool SetUniformInt(const std::string& aName, int32_t aValue) const = 0;
        virtual bool SetUniformBool(const std::string& aName, bool aValue) const = 0;
        virtual bool SetUniformVec4(const std::string& aName, const glm::vec4& aValue) const = 0;
        virtual bool SetUniformVec3(const std::string& aName, const glm::vec3& aValue) const = 0;
        virtual bool SetUniformVec2(const std::string& aName, const glm::vec2& aValue) const = 0;

        /**
         * Sets custom parameters only known by the implementer class
         */
        virtual void SetCustomParams(void) const = 0;
    };
}
