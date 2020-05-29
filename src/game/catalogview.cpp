/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/
#include "precompiled.h"

#include "engine.h"
#include "core/utils.h"
#include "graphic/texture.h"
#include "graphic/gui/alignment.h"
#include "graphic/gui/imagepanel.h"
#include "graphic/gui/layout.h"
#include "graphic/gui/orientation.h"
#include "graphic/gui/toolbutton.h"
#include "graphic/gui/entypo.h"
#include "graphic/gui/theme.h"
#include "graphic/gui/dragdropdata.h"

#include "catalogitem.h"
#include "catalogview.h"
#include "catalogmanager.h"
#include "catalogfilter.h"

using namespace Framework;
using namespace std::placeholders;

namespace Game
{
    UICatalogView::UICatalogView()
        : UIWidget(nullptr)
        , mItemViewer(new UIImagePanel(nullptr))
        , mFilterViewer(new UICatalogFilter(nullptr))
        , mActiveFilter(CatalogGroup::eINVALID)
        , mMinimizedSize(glm::ivec2(1, 1)) // Setting size to (1, 1) because (0,0) is not used by NanoGUI
        , mMaximizedSize(glm::ivec2(1, 1))
    {

    }

    UICatalogView::~UICatalogView()
    {

    }

    void UICatalogView::setTheme(UITheme *aTheme)
    {
        mFilterViewer->setTheme(aTheme);
        mItemViewer->setTheme(aTheme);
        mTheme = aTheme;
    }

    void UICatalogView::setCollapse(bool aCollapse)
    {
        if (aCollapse)
            setFixedSize(mMinimizedSize); // Setting size to (1, 1) because (0,0) is not used by NanoGUI
        else
            setFixedSize(mMaximizedSize);
    }

    void UICatalogView::performLayout(NVGcontext* aCtx)
    {
        int lHeaderHeight = mFilterViewer->preferredSize(aCtx).y;
        int lMargin = mTheme->mTabInnerMargin;
        mFilterViewer->setPosition({ 0, 0 });
        mFilterViewer->setSize({ mSize.x, lHeaderHeight });
        mFilterViewer->performLayout(aCtx);

        mItemViewer->setPosition({ lMargin, lHeaderHeight + lMargin });
        mItemViewer->setSize({ max(mSize.x - 2 * lMargin, 0), max(mSize.y - 2 * lMargin - lHeaderHeight, 0) });
        mItemViewer->performLayout(aCtx);
    }

    glm::ivec2 UICatalogView::preferredSize(NVGcontext* aCtx) const
    {
        glm::ivec2 lContentSize = mItemViewer->preferredSize(aCtx);
        glm::ivec2 lHeaderSize = mFilterViewer->preferredSize(aCtx);
        int lMargin = 0; // mTheme->mTabInnerMargin;
        glm::ivec2 lBorderSize = glm::ivec2(2 * lMargin, 2 * lMargin);
        glm::ivec2 lPreferredSize = lContentSize + lBorderSize + glm::ivec2(0, lHeaderSize.y);

        glm::ivec2 lTargetSize(
            mFixedSize[0] ? mFixedSize[0] : lPreferredSize[0],
            mFixedSize[1] ? mFixedSize[1] : lPreferredSize[1]
        );
        return lTargetSize;
    }

    void UICatalogView::draw(NVGcontext* aCtx)
    {
        mFilterViewer->draw(aCtx);
        mItemViewer->draw(aCtx);
    }

    bool UICatalogView::dragEvent(const glm::ivec2 &p, int aButton, int aModifiers, UIDragDropData* aData)
    {
        if( mItemViewer->contains(p - mPos) &&
            mItemViewer->mouseButtonEvent(p, aButton, true, aModifiers) &&
            mItemViewer->currentIndex() != -1/*not a click 'between' icons*/ )
        {
            //Get the catalog manager
            string lManagerName = Utils::Demangling(typeid(CatalogManager).name());
            CatalogManager& lCatalogManager = static_cast<CatalogManager&>(Engine::Instance()->Manager(lManagerName));

            vector<CatalogItem*> lCatalogItems = lCatalogManager.GetCatalogItems(mActiveFilter);
            int i = mItemViewer->currentIndex();
            ASSERT(0 <= i && i < (int)lCatalogItems.size());
            INFO(LogLevel::eLEVEL2, "Dragged item: Idx:%d Name: %s\n", mItemViewer->currentIndex(), lCatalogItems[i]->GetName().c_str());
            aData->SetData<CatalogItem*>(lCatalogItems[i]);
        }
        return true;
    }

    bool UICatalogView::mouseButtonEvent(const glm::ivec2 &p, int button, bool down, int modifiers)
    {
        if( mItemViewer->contains(p - mPos) && mItemViewer->mouseButtonEvent(p, button, down, modifiers) )
            return true;
        else if( mFilterViewer->contains(p - mPos) && mFilterViewer->mouseButtonEvent(p, button, down, modifiers))
            return true;

        if (button == GLFW_MOUSE_BUTTON_1 && down && !mFocused)
            requestFocus();
        return false;
    }

    void UICatalogView::Serialize(Json::Value& aSerializer) const
    {
        //TODO implement it
        ASSERT(false);
    }

    void UICatalogView::Deserialize(const Json::Value& aSerializer)
    {

        UIWidget::Deserialize(aSerializer);
        setLayout(new UIBoxLayout(Orientation::eVertical, Alignment::eMiddle, 0, 6));

        //---------------------------------------------------------
        // Filter deserialization
        //---------------------------------------------------------
        mFilterViewer->addTab(ENTYPO_ICON_EDIT);
        mFilterViewer->addTab(ENTYPO_ICON_HOME);
        mFilterViewer->addTab(ENTYPO_ICON_GRID);
        mFilterViewer->addTab(ENTYPO_ICON_TREE);

        auto lCallbackFunc = std::bind(&UICatalogView::ChangedCatalogGroup, this, _1);
        mFilterViewer->setCallback(lCallbackFunc);


        //---------------------------------------------------------
        // Item viewer deserialization
        //---------------------------------------------------------
        if (aSerializer.isMember("fixedWidth"))
        {
            int lFixedWidth = aSerializer.get("fixedWidth", 1).asInt();
            mItemViewer->setFixedWidth(lFixedWidth);
            mItemViewer->setWidth(lFixedWidth);

        }
        mItemViewer->setThumbSize( aSerializer.get("itemsThumbSize", mItemViewer->thumbSize()).asInt());
        mItemViewer->setSpacing(aSerializer.get("itemsSpacing", mItemViewer->spacing()).asInt());
        mItemViewer->setMargin(aSerializer.get("itemsMargin", mItemViewer->margin()).asInt());

        //Activate the first filter
        ChangedCatalogGroup(0);
        mFilterViewer->setActiveTab(0);
    }


    void UICatalogView::ChangedCatalogGroup(int aCatalogGroupIndex)
    {
        CatalogGroup aCatalogGroup = static_cast<CatalogGroup>(aCatalogGroupIndex);

        //Get the catalog manager
        string lManagerName = Utils::Demangling(typeid(CatalogManager).name());
        CatalogManager& lCatalogManager = static_cast<CatalogManager&>(Engine::Instance()->Manager(lManagerName));
        vector<CatalogItem*> lCatalogItems = lCatalogManager.GetCatalogItems(aCatalogGroup);

        //Load the images
        UIImagePanel::Images lImages;
        for (auto lItemIter : lCatalogItems)
        {
            pair<int, string> lImage = std::make_pair(lItemIter->GetIcon(), lItemIter->GetName());
            lImages.push_back(lImage);
        }

        mItemViewer->setImages(lImages);
        mActiveFilter = aCatalogGroup;
        mMaximizedSize = mFixedSize;
    }
}
