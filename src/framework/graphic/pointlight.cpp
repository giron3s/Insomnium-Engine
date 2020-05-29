#include "precompiled.h"
#include "pointlight.h"


namespace Framework
{
    void PointLight::UpdateViewProjectionMatrix() const
    {
        // NOTE: Shadows are for ground plane only!
        const auto lLightViewMatrix = glm::lookAt(GetPosition(), GetPosition() + glm::vec3(0.0f, -1.0f, 0.0f)/*down*/, glm::vec3(1.0f, 0.0f, 0.0f));
        const auto lLightProjMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, GetCutoff());
        mViewProjectionMatrix = lLightProjMatrix * lLightViewMatrix;
    }
}
