#include "precompiled.h"
#include "light.h"
#include "json/json.h"


namespace Framework
{
    bool Light::SetShadowMap(uint32_t aSizeX, uint32_t aSizeY)
    {
        if ( mShadowMap )
            return mShadowMap->SetSize(aSizeX, aSizeY);
        mShadowMap.reset(ShadowMapRenderTarget::New());
        RenderTarget::Attachments lAttachments;
        lAttachments[GL_DEPTH_ATTACHMENT] = { GL_TEXTURE_2D, GL_DEPTH_COMPONENT32F, DEFAULT_DEPTH_CLEAR_VALUE };
        return mShadowMap->Init(aSizeX, aSizeY, &lAttachments);
    }

    void Light::ClearShadowMap()
    {
        mShadowMap.reset();
    }


    void Light::Serialize(Json::Value& aSerializer) const
    {
        SerializeVec<glm::vec3>(aSerializer["ambient_color"], mAmbient);
        SerializeVec<glm::vec3>(aSerializer["diffuse_color"], mDiffuse);
        SerializeVec<glm::vec3>(aSerializer["specular_color"], mSpecular);
        if ( mShadowMap )
            SerializeVec<glm::vec2>(aSerializer["shadow_map_size"], mShadowMap->GetSize());
    }


    void Light::Deserialize(const Json::Value& aSerializer)
    {
        if ( !aSerializer.isMember("ambient_color") || !aSerializer.isMember("diffuse_color") || !aSerializer.isMember("specular_color") )
            CRASH("'ambient_color', 'diffuse_color' or 'specular_color' is not defined for light component!");

        DeserializeVec<glm::vec3>(aSerializer["ambient_color"], mAmbient);
        DeserializeVec<glm::vec3>(aSerializer["diffuse_color"], mDiffuse);
        DeserializeVec<glm::vec3>(aSerializer["specular_color"], mSpecular);

        if ( aSerializer.isMember("shadow_map_size") )
        {
            glm::uvec2 lShadowMapSize;
            DeserializeVec<glm::uvec2>(aSerializer["shadow_map_size"], lShadowMapSize);
            if ( !SetShadowMap(lShadowMapSize.x, lShadowMapSize.y) )
                CRASH("Light::SetShadowMap failed");
        }
    }

}
