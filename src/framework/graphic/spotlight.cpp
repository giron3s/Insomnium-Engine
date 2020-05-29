#include "precompiled.h"
#include "spotlight.h"


namespace Framework
{
    void SpotLight::UpdateViewProjectionMatrix() const
    {
        ASSERT_vec3_is_normalized(GetDirection());
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        if ( glm::length(glm::cross(GetDirection(), up)) < 1.0e-6f )
            up = glm::vec3(1.0f/*or -1 ?*/, 0.0f, 0.0f);
        const auto lLightViewMatrix = glm::lookAt(GetPosition(), GetPosition() + GetDirection(), up);
        const auto lLightProjMatrix = glm::perspective(glm::radians(GetConeAngle()), 1.0f, 0.1f, GetCutoff());
        mViewProjectionMatrix = lLightProjMatrix * lLightViewMatrix;
    }
}