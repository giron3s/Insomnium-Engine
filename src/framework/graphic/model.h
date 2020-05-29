/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#pragma once

namespace Framework
{
    class Shader;
    class Entity;

    class Model
    {
        public:

           /**
            * Constructors
            */
            Model()
            {
                static uint32_t sId = 0xFF000000; // ARGB
                static const std::vector<ReservedId> sReservedIds
                {
                    ReservedId::Reserved,
                    ReservedId::ClearColor,
                    ReservedId::GizmoAxisX,
                    ReservedId::GizmoAxisY,
                    ReservedId::GizmoAxisZ,
                    ReservedId::GizmoAxisAll,
                    ReservedId::ModelMask
                };
                do
                    sId += 167; // about 100400 different selectable objects should be really enough
                while ( std::find(sReservedIds.cbegin(), sReservedIds.cend(), static_cast<ReservedId>(sId)) != sReservedIds.cend() );
                mId = sId;
            }

            /**
             * Destructors
             */
            virtual ~Model() = default;

            /**
             * Returned Id is guaranteed to be different than any Reserved Ids
             */
            uint32_t GetId() const { return mId; }

            void    SetOwnerEntity(Entity* aOwnerEntity) { mOwnerEntity = aOwnerEntity; }
            Entity* GetOwnerEntity() const              { return mOwnerEntity; }

            void          SetShader(const Shader *aShader) { mShader = aShader; }       
            const Shader* GetShader() const { return mShader; }

            enum class ReservedId : uint32_t
            {
                Reserved     = 0x00FFFFFF,
                ClearColor   = 0xFF000000,
                GizmoAxisX   = 0xFF0000FF, // : {255, 0, 0, 255}
                GizmoAxisY   = 0xFF00FF00, // : {0, 255, 0, 255}
                GizmoAxisZ   = 0xFFFF0000, // : {0, 0, 255, 255}
                GizmoAxisAll = 0xFFDEDEDE, // : {222, 222, 222, 255}
                ModelMask    = 0xFFFFFFFF
            };

            mutable uint32_t mFlags = 0;

            enum ModelFlags
            {
                eDISABLE_TRASFORM_2D = 1,
                eDISABLE_TRASFORM_3D = 2,
                eFIRST_CUSTOM_FLAG   = 65536 // a placeholder that can be replaced
            };

            Entity* mOwnerEntity = nullptr;

        protected:
            uint32_t mId = 0; /**< Used for model selection in color picking algorithm */
            const Shader* mShader = nullptr; /**< A shader used to render this model */
    };
}
