#include "precompiled.h"
#include "directlight.h"


namespace Framework
{
    void DirectLight::UpdateViewProjectionMatrix() const
    {
        glm::vec3 pos = normalize(GetPosition());
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        if ( glm::length(glm::cross(pos, up)) < 1.0e-6f )
            up = glm::vec3(1.0f/*or -1 ?*/, 0.0f, 0.0f);
        pos *= (mProjectionVolume.z / 2.0f); // TODO: auto calculate it!
        const auto lLightViewMatrix = glm::lookAt(pos, glm::vec3(0.0f, 0.0f, 0.0f), up);
        const auto lLightProjMatrix = glm::ortho(-mProjectionVolume.x, mProjectionVolume.x, -mProjectionVolume.y, mProjectionVolume.y, 1.0f, mProjectionVolume.z);
        mViewProjectionMatrix = lLightProjMatrix * lLightViewMatrix;
    }
}