/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#include "precompiled.h"
#include "engine.h"
#include "engine/script.h"

#include "objecteditormanager.h"



using namespace Framework;
using namespace LuaIntf;

namespace Game
{
    ObjectEditorManager::ObjectEditorManager()
        : BaseManager(Utils::Demangling(typeid(ObjectEditorManager).name()))
        , mSelectedEntity(nullptr)
        , mHaveValuesChanged(false)
        , mOriginalName("Undefined")
        , mOriginalType("Undefined")
        , mOriginalImage("Undefined")
        , mOriginalPositionX(0.0f)
        , mOriginalPositionY(0.0f)
        , mOriginalPositionZ(0.0f)
        , mOriginalOrientationX(0.0f)
        , mOriginalOrientationY(0.0f)
        , mOriginalOrientationZ(0.0f)
        , mOriginalScaleX(0.0f)
        , mOriginalScaleY(0.0f)
        , mOriginalScaleZ(0.0f)
    {
        
    }

    ObjectEditorManager::~ObjectEditorManager()
    {

    }

    void ObjectEditorManager::Initialize()
    {
        LuaContext& lLuaContext = Engine::Instance()->Script().GetLuaContext();
        LuaState lLuaState = lLuaContext.state();

        LuaBinding(lLuaState).addFunction("GetObjectEditorManager", [this]() -> ObjectEditorManager&
        {
            string lManagerName = Utils::Demangling(typeid(ObjectEditorManager).name());
            return static_cast<ObjectEditorManager&>(Engine::Instance()->Manager(lManagerName));
        });

        LuaBinding(lLuaState).beginClass<ObjectEditorManager>("ObjectEditorManager")
            .addFunction("LoadObjectProperties", &ObjectEditorManager::LoadObjectProperties, LUA_ARGS(size_t))
            .addFunction("UnloadObjectProperties", &ObjectEditorManager::UnloadObjectProperties)
            .addFunction("HaveValuesChanged", &ObjectEditorManager::HaveValuesChanged)
            .addFunction("UndoValuesChanged", &ObjectEditorManager::UndoValuesChanged)
            .addFunction("GetObjectName", &ObjectEditorManager::GetObjectName)
            .addFunction("GetObjectType", &ObjectEditorManager::GetObjectType)
            .addFunction("GetObjectImage", &ObjectEditorManager::GetObjectImage)
            .addFunction("GetObjectPositionX", &ObjectEditorManager::GetObjectPositionX)
            .addFunction("GetObjectPositionY", &ObjectEditorManager::GetObjectPositionY)
            .addFunction("GetObjectPositionZ", &ObjectEditorManager::GetObjectPositionZ)
            .addFunction("GetObjectOrientationX", &ObjectEditorManager::GetObjectOrientationX)
            .addFunction("GetObjectOrientationY", &ObjectEditorManager::GetObjectOrientationY)
            .addFunction("GetObjectOrientationZ", &ObjectEditorManager::GetObjectOrientationZ)
            .addFunction("GetObjectScaleX", &ObjectEditorManager::GetObjectScaleX)
            .addFunction("GetObjectScaleY", &ObjectEditorManager::GetObjectScaleY)
            .addFunction("GetObjectScaleZ", &ObjectEditorManager::GetObjectScaleZ)
            .addFunction("SetObjectName", &ObjectEditorManager::SetObjectName, LUA_ARGS(std::string))
            .addFunction("SetObjectType", &ObjectEditorManager::SetObjectType, LUA_ARGS(std::string))
            .addFunction("SetObjectImage", &ObjectEditorManager::SetObjectImage, LUA_ARGS(std::string))
            .addFunction("SetObjectPositionX", &ObjectEditorManager::SetObjectPositionX, LUA_ARGS(float))
            .addFunction("SetObjectPositionY", &ObjectEditorManager::SetObjectPositionY, LUA_ARGS(float))
            .addFunction("SetObjectPositionZ", &ObjectEditorManager::SetObjectPositionZ, LUA_ARGS(float))
            .addFunction("SetObjectOrientationX", &ObjectEditorManager::SetObjectOrientationX, LUA_ARGS(float))
            .addFunction("SetObjectOrientationY", &ObjectEditorManager::SetObjectOrientationY, LUA_ARGS(float))
            .addFunction("SetObjectOrientationZ", &ObjectEditorManager::SetObjectOrientationZ, LUA_ARGS(float))
            .addFunction("SetObjectScaleX", &ObjectEditorManager::SetObjectScaleX, LUA_ARGS(float))
            .addFunction("SetObjectScaleY", &ObjectEditorManager::SetObjectScaleY, LUA_ARGS(float))
            .addFunction("SetObjectScaleZ", &ObjectEditorManager::SetObjectScaleZ, LUA_ARGS(float))
            .endClass();
    }

    void ObjectEditorManager::DeInitialize()
    {
        
    }

    bool ObjectEditorManager::LoadObjectProperties(size_t aEntityId)
    {
        mSelectedEntity = Engine::Instance()->EntityManager().GetEntityByID(aEntityId);

        if (mSelectedEntity == nullptr)
        {
            WARNING("Not found any entity with '%d id", (int)aEntityId);
            return false;
        }

        mHaveValuesChanged = false;

        mOriginalName = GetObjectName();
        mOriginalType = GetObjectType();
        mOriginalImage = GetObjectImage();

        mOriginalPositionX = GetObjectPositionX();
        mOriginalPositionY = GetObjectPositionY();
        mOriginalPositionZ = GetObjectPositionZ();

        mOriginalOrientationX = GetObjectOrientationX();
        mOriginalOrientationY = GetObjectOrientationY();
        mOriginalOrientationZ = GetObjectOrientationZ();

        mOriginalScaleX = GetObjectScaleX();
        mOriginalScaleY = GetObjectScaleY();
        mOriginalScaleZ = GetObjectScaleZ();

        return true;
    }

    void ObjectEditorManager::UnloadObjectProperties()
    {
        Framework::GUI::UIWidget* lCanvasGL = Engine::Instance()->Display().findWidget("canvasGL");
        if (lCanvasGL != nullptr)
            lCanvasGL->requestFocus();
    }

    bool ObjectEditorManager::HaveValuesChanged()
    {
        return mHaveValuesChanged;
    }

    void ObjectEditorManager::UndoValuesChanged()
    {
        mHaveValuesChanged = false;

        SetObjectName(mOriginalName);
        SetObjectType(mOriginalType);
        SetObjectImage(mOriginalImage);

        SetObjectPositionX(mOriginalPositionX);
        SetObjectPositionY(mOriginalPositionY);
        SetObjectPositionZ(mOriginalPositionZ);

        SetObjectOrientationX(mOriginalOrientationX);
        SetObjectOrientationY(mOriginalOrientationY);
        SetObjectOrientationZ(mOriginalOrientationZ);

        SetObjectScaleX(mOriginalScaleX);
        SetObjectScaleY(mOriginalScaleY);
        SetObjectScaleZ(mOriginalScaleZ);
    }

    std::shared_ptr<TransformCmp> ObjectEditorManager::GetObjectTransform()
    {

        if (mSelectedEntity == nullptr)
        {
            WARNING("The selected entity is NULL!");
            return nullptr;
        }

        shared_ptr<TransformCmp> lTransformCmp(mSelectedEntity->GetComponent<TransformCmp>());
        return lTransformCmp;
    }

    const std::string ObjectEditorManager::GetObjectName()
    {
        if (mSelectedEntity == nullptr)
        {
            WARNING("The selected entity is NULL!");
            return nullptr;
        }

        weak_ptr<CatalogCmp> lCatalogCmp = mSelectedEntity->GetComponent<CatalogCmp>();
        if (lCatalogCmp.expired())
        {
            WARNING("The selected entity '%s' hasn't got any catalog component!", mSelectedEntity->GetName().c_str());
            return nullptr;
        }
        return lCatalogCmp.lock()->GetName();
    }

    const std::string ObjectEditorManager::GetObjectType()
    {
        if (mSelectedEntity == nullptr)
        {
            WARNING("The selected entity is NULL!");
            return string();
        }

        weak_ptr<CatalogCmp> lCatalogCmp = mSelectedEntity->GetComponent<CatalogCmp>();
        if (lCatalogCmp.expired())
        {
            WARNING("The selected entity '%s' hasn't got any catalog component!", mSelectedEntity->GetName().c_str());
            return string();
        }

        return lCatalogCmp.lock()->GetGroup();
    }

    const int ObjectEditorManager::GetObjectImage()
    {
        if (mSelectedEntity == nullptr)
        {
            WARNING("The selected entity is NULL!");
            return -1;
        }

        weak_ptr<CatalogCmp> lCatalogCmp = mSelectedEntity->GetComponent<CatalogCmp>();
        if (lCatalogCmp.expired())
        {
            WARNING("The selected entity '%s' hasn't got any catalog component!", mSelectedEntity->GetName().c_str());
            return -1;
        }

        return Engine::Instance()->ResourceManager().FindImage(lCatalogCmp.lock()->GetIcon());
    }

    float ObjectEditorManager::GetObjectPositionX() 
    { 
        if (GetObjectTransform()) 
            return GetObjectTransform()->GetPosition().x; 
        else 
            return 0.0f; 
    }

    float ObjectEditorManager::GetObjectPositionY()
    {
        if (GetObjectTransform())
            return GetObjectTransform()->GetPosition().y;
        else
            return 0.0f;
    }

    float ObjectEditorManager::GetObjectPositionZ()
    {
        if (GetObjectTransform())
            return GetObjectTransform()->GetPosition().z;
        else
            return 0.0f;
    }

    float ObjectEditorManager::GetObjectOrientationX()
    {
        if (GetObjectTransform())
            return glm::eulerAngles(GetObjectTransform()->GetOrientation()).x;
        else
            return 0.0f;
    }

    float ObjectEditorManager::GetObjectOrientationY()
    {
        if (GetObjectTransform())
            return glm::eulerAngles(GetObjectTransform()->GetOrientation()).y;
        else
            return 0.0f;
    }

    float ObjectEditorManager::GetObjectOrientationZ()
    {
        if (GetObjectTransform())
            return glm::eulerAngles(GetObjectTransform()->GetOrientation()).z;
        else
            return 0.0f;
    }
    
    float ObjectEditorManager::GetObjectScaleX()
    {
        if (GetObjectTransform())
            return GetObjectTransform()->GetScale().x;
        else
            return 0.0f;
    }

    float ObjectEditorManager::GetObjectScaleY()
    {
        if (GetObjectTransform())
            return GetObjectTransform()->GetScale().y;
        else
            return 0.0f;
    }

    float ObjectEditorManager::GetObjectScaleZ()
    {
        if (GetObjectTransform())
            return GetObjectTransform()->GetScale().z;
        else
            return 0.0f;
    }

    void ObjectEditorManager::SetObjectName(const std::string aName)
    {
        if(mSelectedEntity != nullptr) 
            mSelectedEntity->SetName(aName);

        shared_ptr<CatalogCmp> lCatalogCmp(mSelectedEntity->GetComponent<CatalogCmp>());
        if (lCatalogCmp != nullptr)
            lCatalogCmp->SetName(aName);

        if (aName != mOriginalName)
            mHaveValuesChanged = true;
    }

    void ObjectEditorManager::SetObjectType(const std::string aType)
    {
        if (mSelectedEntity == nullptr)
            return;

        shared_ptr<CatalogCmp> lCatalogCmp(mSelectedEntity->GetComponent<CatalogCmp>());
        if (lCatalogCmp != nullptr)
            lCatalogCmp->SetGroup(aType);

        if (aType != mOriginalType)
            mHaveValuesChanged = true;
    }

    void ObjectEditorManager::SetObjectImage(const std::string aImage)
    {
        if (mSelectedEntity == nullptr)
            return;

        shared_ptr<CatalogCmp> lCatalogCmp(mSelectedEntity->GetComponent<CatalogCmp>());
        if (lCatalogCmp != nullptr)
            lCatalogCmp->SetIcon(aImage);

        if (aImage != mOriginalImage)
            mHaveValuesChanged = true;
    }

    void ObjectEditorManager::SetObjectPositionX(float aValue)
    {
        if (GetObjectTransform())
        {
            glm::vec3 lPos = GetObjectTransform()->GetPosition();
            lPos.x = aValue;
            GetObjectTransform()->SetPosition(lPos);
        }

        if (aValue != mOriginalPositionX)
            mHaveValuesChanged = true;
    }

    void ObjectEditorManager::SetObjectPositionY(float aValue)
    {
        if (GetObjectTransform())
        {
            glm::vec3 lPos = GetObjectTransform()->GetPosition();
            lPos.y = aValue;
            GetObjectTransform()->SetPosition(lPos);
        }

        if (aValue != mOriginalPositionY)
            mHaveValuesChanged = true;
    }

    void ObjectEditorManager::SetObjectPositionZ(float aValue)
    {
        if (GetObjectTransform())
        {
            glm::vec3 lPos = GetObjectTransform()->GetPosition();
            lPos.z = aValue;
            GetObjectTransform()->SetPosition(lPos);
        }

        if (aValue != mOriginalPositionZ)
            mHaveValuesChanged = true;
    }

    void ObjectEditorManager::SetObjectOrientationX(float aValue)
    {
        //  TODO - The code in this function needs further testing to ensure it is working correctly.
        if (GetObjectTransform())
        {
            glm::highp_vec3 lOrientation = glm::eulerAngles(GetObjectTransform()->GetOrientation());
            lOrientation.x = aValue;
            GetObjectTransform()->SetOrientation(glm::quat(lOrientation));
        }

        if (aValue != mOriginalOrientationX)
            mHaveValuesChanged = true;
    }

    void ObjectEditorManager::SetObjectOrientationY(float aValue)
    {
        //  This function works properly because we are only rotating objects around the Y-Axis
        //  If we need to rotate the object on two or three axis at the same time, this code will
        //  not work.
        if (GetObjectTransform())
        {
            GetObjectTransform()->SetOrientation(glm::quat(glm::highp_vec3(0.0f, aValue/ 57.2958f, 0.0f)));
        }

        if (aValue != mOriginalOrientationY)
            mHaveValuesChanged = true;
    }

    void ObjectEditorManager::SetObjectOrientationZ(float aValue)
    {
        //  TODO - The code in this function needs further testing to ensure it is working correctly.
        if (GetObjectTransform())
        {
            glm::highp_vec3 lOrientation = glm::eulerAngles(GetObjectTransform()->GetOrientation());
            lOrientation.z = aValue;
            GetObjectTransform()->SetOrientation(glm::quat(lOrientation));
        }

        if (aValue != mOriginalOrientationZ)
            mHaveValuesChanged = true;
    }

    void ObjectEditorManager::SetObjectScaleX(float aValue)
    {
        if (GetObjectTransform())
        {
            glm::vec3 lScale = GetObjectTransform()->GetScale();
            lScale.x = aValue;
            GetObjectTransform()->SetScale(lScale);
        }

        if (aValue != mOriginalScaleX)
            mHaveValuesChanged = true;
    }

    void ObjectEditorManager::SetObjectScaleY(float aValue)
    {
        if (GetObjectTransform())
        {
            glm::vec3 lScale = GetObjectTransform()->GetScale();
            lScale.y = aValue;
            GetObjectTransform()->SetScale(lScale);
        }

        if (aValue != mOriginalScaleY)
            mHaveValuesChanged = true;
    }

    void ObjectEditorManager::SetObjectScaleZ(float aValue)
    {
        if (GetObjectTransform())
        {
            glm::vec3 lScale = GetObjectTransform()->GetScale();
            lScale.z = aValue;
            GetObjectTransform()->SetScale(lScale);
        }

        if (aValue != mOriginalScaleZ)
            mHaveValuesChanged = true;
    }
}
