/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#pragma once

#include <vector>
#include <memory>
#include "core/serialization/serializableobject.h"

namespace Framework
{
    namespace GUI
    {
        class UIWidget;
    }
}

namespace Framework
{
    class UI : public SerializableObject,
               public std::enable_shared_from_this<UI>
    {

    public:
                                       UI(const std::string& aFileName);
        virtual                        ~UI();
                                       
        const std::string&             GetFileName() const { return mFileName; }
                                       
        void                           Initialize();
        void                           DeInitialize();
                                       
        virtual void                   Serialize(Json::Value& serializer) const override;
        virtual void                   Deserialize(const Json::Value& serializer) override;
                                       
        void                           DestroyUIWidget( std::weak_ptr<GUI::UIWidget> aUIWidget );
        std::shared_ptr<GUI::UIWidget> CreateUIWidgetFromData(const Json::Value& aSerializer, GUI::UIWidget* aParent);

    private:
        void                           AddUIWidget( std::shared_ptr<GUI::UIWidget> aUIWidget );
        void                           RemoveUIWidget( std::shared_ptr<GUI::UIWidget> aUIWidget );
                                       
    public:                            
        std::weak_ptr<GUI::UIWidget>   GetUIWidgetByName( const std::string& aName ) const;
        void                           GetUIWidgetsByName( const std::string& aName, std::vector<std::weak_ptr<GUI::UIWidget>>& aUIWidgets ) const;
        std::weak_ptr<GUI::UIWidget>   GetUIWidgetByNameSubString( const std::string& aName ) const;
        void                           GetUIWidgetsByNameSubString( const std::string& aName, std::vector<std::weak_ptr<GUI::UIWidget>>& aUIWidgets ) const;

    private:
        std::string                                   mFileName;
        std::shared_ptr<GUI::UIWidget>                mUIRootWidget;
        std::vector<std::shared_ptr<GUI::UIWidget>>   mUIWidgets;
        std::vector<std::shared_ptr<GUI::UIWidget>>   mDestroyedUIWidgets;
    };
};
