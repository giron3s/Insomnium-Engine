/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential

 * Brief        : OpenGL shader implementation
*******************************************************************************/

#pragma once

#include <map>
#include <string>
#include <vector>
#include "graphic/shader.h"
#include "graphic/opengl/opengl_shadermaterial.h"

namespace Framework
{
    class OpenGLShader : public virtual Shader
    {
    public:
                     OpenGLShader(const std::string& aName);
                     ~OpenGLShader() override;

        bool         Init() override { return true; }
        uint32_t     GetProgramID() const override;
        bool         Load(const std::string &aPath, std::string &aError) override;

    protected:
        bool         LoadVertexShader(const std::string &aFileName, std::string &aError) override;
        bool         LoadGeometryShader(const std::string &aFileName, std::string &aError) override;
        bool         LoadFragmentShader(const std::string &aFileName, std::string &aError) override;

        bool         LinkProgram(std::string &aError) override;

    public:
        bool         Attach(void) const override;
        bool         Detach(void) const override;

        const std::map<std::string, uint32_t> &GetUniforms(void) const override;
        const bool   GetUniformID(const std::string &aName, uint32_t *id) const override;
        const bool   GetAttributeID(const std::string &aName, uint32_t *id) const override;

        bool         SetUniformMat4(const std::string &aName, const glm::mat4 aValue[], uint32_t aNumItems = 1) const override;
        bool         SetUniformMat3(const std::string &aName, const glm::mat3 aValue[], uint32_t aNumItems = 1) const override;
        bool         SetUniformTexture2D(const std::string &aName, uint32_t aUnitID) const override;
        bool         SetUniformTexture2DArray(const std::string &aName, uint32_t aUnitID[], uint32_t aNumItems) const override;
        bool         SetUniformFloat(const std::string &aName, float aValue) const override;
        bool         SetUniformInt(const std::string &aName, int32_t aValue) const override;
        bool         SetUniformBool(const std::string &aName, bool aValue) const override;
        bool         SetUniformVec4(const std::string &aName, const glm::vec4 &aValue) const override;
        bool         SetUniformVec3(const std::string &aName, const glm::vec3 &aValue) const override;
        bool         SetUniformVec2(const std::string &aName, const glm::vec2 &aValue) const override;
        void         SetCustomParams(void) const override;
        //std::string  GetShaderFilename() const override;

    private:
        bool         LoadShader(uint32_t shaderObjectID, const std::string &aFileName, std::string &aError, bool optional = false);
        void         BuildUniformsMap(void);

    protected:
        std::vector<uint32_t>           mShadersIDs; // temp
        std::map<std::string, uint32_t> mUniformNames;
        uint32_t                        mProgramID;
        //std::string                     mFilename;
    };
}
