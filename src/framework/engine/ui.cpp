/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#include <graphic/display.h>
#include "precompiled.h"
#include "engine.h"
#include "engine/ui.h"
#include "graphic/gui/uiobject.h"
#include "graphic/gui/window.h"
#include "graphic/gui/widget.h"
#include "graphic/gui/screen.h"

#include "json/json.h"
#include "core/serialization/jsoncpputils.h"

using namespace Framework::GUI;

namespace Framework
{
    UI::UI(const string& aFileName)
        :  mFileName(aFileName)
    {
        mUIWidgets.reserve(64);
    }

    UI::~UI()
    {

    }

    void UI::Initialize()
    {
    }   

    void UI::DeInitialize()
    {
        UIScreen& lUIScreen = Engine::Instance()->Display();
        shared_ptr<GUI::UIWindow > lUIRootWindow = static_pointer_cast<GUI::UIWindow>(mUIRootWidget);
        ASSERT(lUIRootWindow != nullptr);
        lUIScreen.disposeWindow(lUIRootWindow.get());
    }
	
    //**********************************************************************************************
    //   Functions handle the UIWidgets
    //**********************************************************************************************
    void UI::DestroyUIWidget( weak_ptr<UIWidget> aUIWidget )
    {
        shared_ptr<UIWidget> lUIWidget = aUIWidget.lock();
        auto lResult = find( mUIWidgets.begin(), mUIWidgets.end(), lUIWidget );
        if ( lResult == mUIWidgets.end() )
        {
            CRASH("Can not destroy the '%s' UIWidget", aUIWidget.lock()->id().c_str());
            return;
        }
        RemoveUIWidget( *lResult );
    }

    void UI::AddUIWidget( shared_ptr<UIWidget> aUIWidget )
    {
        mUIWidgets.push_back( aUIWidget );
        aUIWidget->setOwner(this);
    }

    void UI::RemoveUIWidget( shared_ptr<UIWidget> aUIWidget )
    {
        vector<shared_ptr<UIWidget>>::iterator it = remove( mUIWidgets.begin(), mUIWidgets.end(), aUIWidget );
        if ( it != mUIWidgets.end() )
        {
            mUIWidgets.erase( it );
        }
    }

    shared_ptr<GUI::UIWidget> UI::CreateUIWidgetFromData(const Json::Value& aSerializer, UIWidget* aParent)
    {
        string lUIObjectType = aSerializer["type"].asString();
        shared_ptr<SerializableObject> lObject = Engine::Instance()->ObjectFactory().Create(lUIObjectType);
        if (lObject.get() == nullptr)
            CRASH("Can not create the '%s' object!", lUIObjectType.c_str());

        shared_ptr<UIWidget> lWidget(dynamic_pointer_cast<UIWidget>(lObject));

        AddUIWidget(lWidget);              //Register the widget at the .UI
        lWidget->setParent(aParent);       //Set parent
        aParent->addChild(lWidget.get());  //Set child

        lWidget->Deserialize(aSerializer);

        if ( aSerializer.isMember("child") )
        {
            for (const auto& lChild : aSerializer["child"])
                CreateUIWidgetFromData(lChild, lWidget.get());
        }
        return lWidget;
    }

    weak_ptr<UIWidget> UI::GetUIWidgetByName( const string& aName ) const
    {
       for ( shared_ptr<UIWidget> mUIWidget : mUIWidgets )
        {
            if ( mUIWidget->id() ==  aName )
            {
                return weak_ptr<UIWidget>( mUIWidget );
            }
        }
        return weak_ptr<UIWidget>();
    }

    void UI::GetUIWidgetsByName( const string& aName, vector<weak_ptr<UIWidget>>& aUIWidgets ) const
    {
        for ( shared_ptr<UIWidget> mUIWidget : mUIWidgets )
        {
            if ( mUIWidget->id() ==  aName )
            {
                aUIWidgets.push_back( weak_ptr<UIWidget>( mUIWidget ) );
            }
        }
    }

    weak_ptr<UIWidget> UI::GetUIWidgetByNameSubString( const string& aName ) const
    {
        for ( shared_ptr<UIWidget> mUIWidget : mUIWidgets )
        {
            if ( mUIWidget->id().find(aName) != string::npos )
            {
                return weak_ptr<UIWidget>( mUIWidget );
            }
        }
        return weak_ptr<UIWidget>();
    }

    void UI::GetUIWidgetsByNameSubString( const string& aName, vector<weak_ptr<UIWidget>>& aUIWidgets ) const
    {
        for ( shared_ptr<UIWidget> mUIWidget : mUIWidgets )
        {
            if ( mUIWidget->id().find(aName)!= string::npos )
            {
                aUIWidgets.push_back( weak_ptr<UIWidget>( mUIWidget ) );
            }
        }
    }

	void UI::Serialize(Json::Value& serializer) const
	{

	}

	void UI::Deserialize(const Json::Value& aSerializer)
    {
        //Parse the UI objects
        UIScreen& lUIScreen = Engine::Instance()->Display();

        for(auto lWidgetRes : aSerializer)
            mUIRootWidget = CreateUIWidgetFromData(lWidgetRes, &lUIScreen);

        //Root widget has to be UIWindow
        if (static_pointer_cast<GUI::UIWindow>(mUIRootWidget) == nullptr)
            CRASH("The UI's root widget has to be UIWindow type!");

        //Update the focus for new UI
        lUIScreen.performLayout();
        lUIScreen.setVisible(true);
        lUIScreen.updateFocus(mUIRootWidget.get());
    }
}
