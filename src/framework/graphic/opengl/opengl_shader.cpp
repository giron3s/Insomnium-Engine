/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential

 * Brief	    : OpenGL shader implementation
*******************************************************************************/

#include "precompiled.h"
#include "graphic/opengl/opengl.h"
#include "graphic/opengl/opengl_shader.h"
#include "graphic/opengl/opengl_shadermaterial.h"
#include "engine.h"

namespace Framework
{
    OpenGLShader::OpenGLShader(const std::string& aName) 
        : Shader(aName)
        , mProgramID(GL_NONE)
    {}

    OpenGLShader::~OpenGLShader()
    {
        for ( auto lShaderID : mShadersIDs )
            __(glDeleteShader(lShaderID));
        if ( mProgramID )
            __(glDeleteProgram(mProgramID));
    }

    uint32_t OpenGLShader::GetProgramID() const
    {
        return mProgramID;
    }

    bool OpenGLShader::Load(const string& aPath, string& aError)
    {
        const string lShaderPath = Engine::Instance()->Config().GetDataDirectory() + string("/resources/shaders/") + aPath;

        string vertex = lShaderPath + string(".vert");
        string geometry = lShaderPath + string(".geo");
        string fragment = lShaderPath + string(".frag");

        if ( LoadVertexShader(vertex, aError) == false ) {
            return false;
        }
        if ( LoadGeometryShader(geometry, aError) == false ) {
            return false;
        }

        if ( LoadFragmentShader(fragment, aError) == false ) {
            return false;
        }
        return LinkProgram(aError);
    }

    bool OpenGLShader::LoadVertexShader(const string& aFilename, string& aError)
    {
        GLuint shaderObjectID;

        __(shaderObjectID = glCreateShader(GL_VERTEX_SHADER));

        return LoadShader(shaderObjectID, aFilename, aError);
    }

    bool OpenGLShader::LoadGeometryShader(const string& aFilename, string& aError)
    {
        GLuint shaderObjectID;

        __(shaderObjectID = glCreateShader(GL_GEOMETRY_SHADER));

        return LoadShader(shaderObjectID, aFilename, aError, true);
    }

    bool OpenGLShader::LoadFragmentShader(const string& aFilename, string& aError)
    {
        GLuint shaderObjectID;

        __(shaderObjectID = glCreateShader(GL_FRAGMENT_SHADER));

        return LoadShader(shaderObjectID, aFilename, aError);
    }

    bool OpenGLShader::LoadShader(uint32_t shaderObjectID, const string& aFilename, string& aError, bool aOptional)
    {
        GLint result = GL_FALSE;

        /* Open file and read it */
        FILE* shader = fopen(aFilename.c_str(), "rt");

        if ( shader == NULL ) {
            if ( aOptional ) {
                return true;
            }
            aError = string("File ") + aFilename + string(" couldn't be opened");
            return false;
        }

        fseek(shader, 0, SEEK_END);
        long size = ftell(shader);
        fseek(shader, 0, SEEK_SET);

        if ( size == -1 ) {
            aError = string("Failed when getting the size of ") + aFilename;
            fclose(shader);
            return false;
        }
        std::string shaderText(size + 1, '\0');
        fread((void*)shaderText.data(), sizeof(char), size, shader);
        fclose(shader);

        char* lVersion = (char*)strstr(shaderText.c_str(), "#version ");
        if ( !lVersion )
            CRASH("Shader \"%s\" does not have #verstion directive !", aFilename.c_str());
        static const char* sLatestVersion = nullptr; // glGetString(GL_SHADING_LANGUAGE_VERSION) is bullshitting us!
        if ( !sLatestVersion )
        {
            result = 0;
            glGetIntegerv(GL_NUM_SHADING_LANGUAGE_VERSIONS, &result);
            if ( result )
            {
                sLatestVersion = (const char*)glGetStringi(GL_SHADING_LANGUAGE_VERSION, 0);
                ASSERT(sLatestVersion);
                ASSERT('3' <= sLatestVersion[0] && sLatestVersion[0] <= '9' &&
                    '3' <= sLatestVersion[1] && sLatestVersion[1] <= '9' &&
                    '0' <= sLatestVersion[2] && sLatestVersion[3] <= '9');
            }
            else // no supported profiles reported. use default min required version
                sLatestVersion = "330 core";
        }
        // sLatestVersion should be in format: "abc core" where a - major version, bc - minor version
        lVersion += 9;
        while ( *lVersion == ' ' ) // skip any extra spaces
            ++lVersion;
        lVersion[0] = sLatestVersion[0];
        lVersion[1] = sLatestVersion[1];
        lVersion[2] = sLatestVersion[2];

        const char* lShaderText = shaderText.c_str();
        __(glShaderSource(shaderObjectID, 1, &lShaderText, NULL));
        __(glCompileShader(shaderObjectID));

        /* Get compilation result */
        result = 0;
        __(glGetShaderiv(shaderObjectID, GL_INFO_LOG_LENGTH, &result));
        aError.resize(result + 1, '\0');
        __(glGetShaderInfoLog(shaderObjectID, result, NULL, const_cast<GLchar*>(aError.c_str())));
        __(glGetShaderiv(shaderObjectID, GL_COMPILE_STATUS, &result));
        if ( result == GL_FALSE )
        {
            WARNING("failed to compile shader [%s]\n%s\n", aFilename.c_str(), aError.c_str());
            return false;
        }
        if (aError[0])
        {
            WARNING("compile shader [%s] log:\n%s\n", aFilename.c_str(), aError.c_str());
            aError.clear();
        }
        INFO(LogLevel::eLEVEL3, "compiled shader [%s]", aFilename.c_str());

        mShadersIDs.push_back(shaderObjectID);
        return true;
    }

    bool OpenGLShader::LinkProgram(string& aError)
    {
        GLint result = GL_FALSE;

        __(mProgramID = glCreateProgram());
        ASSERT(mProgramID);

        for ( auto lShaderID : mShadersIDs )
            __(glAttachShader(mProgramID, lShaderID));

        /* Link the program */
        __(glLinkProgram(mProgramID));

        /* Delete shader components */
        for ( auto lShaderID : mShadersIDs )
            __(glDeleteShader(lShaderID));
        mShadersIDs.clear();

        /* Get linking result */
        result = 0;
        __(glGetProgramiv(mProgramID, GL_INFO_LOG_LENGTH, &result));
        aError.resize(result + 1, '\0');
        __(glGetProgramInfoLog(mProgramID, result, NULL, const_cast<GLchar*>(aError.c_str())));
        __(glGetProgramiv(mProgramID, GL_LINK_STATUS, &result));
        if ( result == GL_FALSE )
        {
            WARNING("failed to link program [%s]\n%s\n", mName.c_str(), aError.c_str());
            return false;
        }
        if ( aError[0] ) // a warning
            WARNING("link program [%s] log:\n%s\n", mName.c_str(), aError.c_str());
        aError.clear();
        INFO(LogLevel::eLEVEL3,"linked program [%s]", mName.c_str());

        BuildUniformsMap();

        return true;
    }

    bool OpenGLShader::Attach(void) const
    {
        __(glUseProgram(mProgramID));
        return true;
    }

    bool OpenGLShader::Detach(void) const
    {
        __(glUseProgram(GL_NONE));
        return true;
    }

    const map<string, uint32_t>& OpenGLShader::GetUniforms(void) const
    {
        return mUniformNames;
    }

    const bool OpenGLShader::GetUniformID(const string& aName, uint32_t* aId) const
    {
        auto lIter = mUniformNames.find(aName);

        if ( lIter == mUniformNames.end() )
        {
            WARNING("Not found '%s' in the uniform name list", aName.c_str());
            return false;
        }

        *aId = lIter->second;
        return true;
    }

    const bool OpenGLShader::GetAttributeID(const string& aName, uint32_t* aId) const
    {
        __(*aId = glGetAttribLocation(mProgramID, aName.c_str()));
        return *aId != -1 ? true : false;
    }

    bool OpenGLShader::SetUniformMat4(const string& aName, const glm::mat4 aValue[], uint32_t aNumItems) const
    {
        auto lIter = mUniformNames.find(aName);

        if ( lIter == mUniformNames.end() )
        {
            WARNING("Not found '%s' in the uniform name list", aName.c_str());
            return false;
        }

        __(glUniformMatrix4fv(lIter->second, aNumItems, GL_FALSE, (GLfloat*)aValue));
        return true;
    }

    bool OpenGLShader::SetUniformMat3(const string& aName, const glm::mat3 aValue[], uint32_t aNumItems) const
    {
        auto lIter = mUniformNames.find(aName);

        if ( lIter == mUniformNames.end() )
        {
            WARNING("Not found '%s' in the uniform name list", aName.c_str());
            return false;
        }

        __(glUniformMatrix3fv(lIter->second, aNumItems, GL_FALSE, (GLfloat*)aValue));
        return true;
    }

    bool OpenGLShader::SetUniformTexture2D(const string& aName, uint32_t aUnitID) const
    {
        auto lIter = mUniformNames.find(aName);

        if ( lIter == mUniformNames.end() )
        {
            WARNING("Not found '%s' in the uniform name list", aName.c_str());
            return false;
        }

        __(glUniform1i(lIter->second, aUnitID));
        return true;
    }

    bool OpenGLShader::SetUniformTexture2DArray(const string& aName, uint32_t aUnitIDs[], uint32_t aNumItems) const
    {
        auto lIter = mUniformNames.find(aName);

        if ( lIter == mUniformNames.end() )
        {
            WARNING("Not found '%s' in the uniform name list", aName.c_str());
            return false;
        }

        __(glUniform1iv(lIter->second, aNumItems, (GLint*)aUnitIDs));
        return true;
    }

    bool OpenGLShader::SetUniformFloat(const string& aName, float aValue) const
    {
        auto lIter = mUniformNames.find(aName);

        if ( lIter == mUniformNames.end() )
        {
            WARNING("Not found '%s' in the uniform name list", aName.c_str());
            return false;
        }

        __(glUniform1f(lIter->second, aValue));
        return true;
    }

    bool OpenGLShader::SetUniformInt(const string& aName, int32_t aValue) const
    {
        auto lIter = mUniformNames.find(aName);

        if ( lIter == mUniformNames.end() )
        {
            WARNING("Not found '%s' in the uniform name list", aName.c_str());
            return false;
        }

        __(glUniform1i(lIter->second, aValue));
        return true;
    }

    bool OpenGLShader::SetUniformBool(const string& aName, bool aValue) const
    {
        auto lIter = mUniformNames.find(aName);

        if ( lIter == mUniformNames.end() )
        {
            WARNING("Not found '%s' in the uniform name list", aName.c_str());
            return false;
        }

        __(glUniform1i(lIter->second, aValue));
        return true;
    }

    bool OpenGLShader::SetUniformVec4(const string& aName, const glm::vec4& aValue) const
    {
        auto lIter = mUniformNames.find(aName);

        if ( lIter == mUniformNames.end() )
        {
            WARNING("Not found '%s' in the uniform name list", aName.c_str());
            return false;
        }

        __(glUniform4fv(lIter->second, 1, &aValue[0]));
        return true;
    }

    bool OpenGLShader::SetUniformVec3(const string& aName, const glm::vec3& aValue) const
    {
        auto lIter = mUniformNames.find(aName);

        if ( lIter == mUniformNames.end() )
        {
            WARNING("Not found '%s' in the uniform name list", aName.c_str());
            return false;
        }

        __(glUniform3fv(lIter->second, 1, &aValue[0]));
        return true;
    }

    bool OpenGLShader::SetUniformVec2(const string& aName, const glm::vec2& aValue) const
    {
        auto lIter = mUniformNames.find(aName);

        if ( lIter == mUniformNames.end() )
        {
            WARNING("Not found '%s' in the uniform name list", aName.c_str());
            return false;
        }

        __(glUniform2fv(lIter->second, 1, &aValue[0]));
        return true;
    }

    void OpenGLShader::SetCustomParams() const { /* Empty on purpose */ }

    void OpenGLShader::BuildUniformsMap(void)
    {
        mUniformNames.clear();

        int32_t lCount, i;
        __(glGetProgramiv(mProgramID, GL_ACTIVE_UNIFORMS, &lCount));

        for ( i = 0; i < lCount; ++i )
        {
            char lUniformName[128];
            GLint lSize = 0;
            GLenum lType = 0;

            /* Get uniform name */
            *lUniformName = '\0';
            GLsizei lUniformNameLength = 0;
            __(glGetActiveUniform(mProgramID, i, sizeof lUniformName, &lUniformNameLength, &lSize, &lType, lUniformName));
            ASSERT(lUniformNameLength < sizeof lUniformName);
            ASSERT(lSize);
            ASSERT(lType);
            lUniformName[lUniformNameLength] = '\0';

            /* Get location */
            uint32_t uniformID;

            __(uniformID = glGetUniformLocation(mProgramID, lUniformName));

            /* Save in map */
            mUniformNames[lUniformName] = uniformID;
        }
    }

}
