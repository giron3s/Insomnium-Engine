/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2018 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#pragma once

#include "graphic/gui/widget.h"
#include "cataloggroup.h"

namespace Game
{
    class UICatalogFilter;
}

namespace Framework
{
    namespace GUI
    {
        class UIImagePanel;
        class UIButton;
        class UIDragDropData;
        class UITheme;
    }
}

namespace Game
{
    class UICatalogView : public Framework::GUI::UIWidget
    {
    public:
                            UICatalogView();
                            ~UICatalogView();

        virtual void        performLayout(NVGcontext* aCtx) override;
        virtual glm::ivec2  preferredSize(NVGcontext *aCtx) const override;
        void                draw(NVGcontext* aCtx) override;
        
        void                setTheme(UITheme *aTheme);

        void                setCollapse(bool aCollapse);

        bool                dragEvent(const glm::ivec2 &p, int aButton, int aModifiers, UIDragDropData* aData) override;
        bool                mouseButtonEvent(const glm::ivec2 &p, int aButton, bool aDown, int aModifiers) override;

        virtual void        Serialize(Json::Value& aSerializer) const override;
        virtual void        Deserialize(const Json::Value& aSerializer) override;

    private:
        void                ChangedCatalogGroup(int aCatalogGroupIndex);

    protected:
        glm::ivec2                              mMinimizedSize; // Setting size to (1, 1) because (0,0) is not used by NanoGUI
        glm::ivec2                              mMaximizedSize;

        Framework::GUI::UIImagePanel*           mItemViewer;
        UICatalogFilter*                        mFilterViewer;
        CatalogGroup                            mActiveFilter;
    };
}
