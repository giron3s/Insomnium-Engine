/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2017 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#include "precompiled.h"

#include "engine/objectfactory.h"
#include "engine/components/entity.h"
#include "engine/components/renderablecmp.h"
#include "engine/components/cameracmp.h"
#include "engine/components/motioncmp.h"
#include "engine/components/transformcmp.h"
#include "engine/components/directlightcmp.h"
#include "engine/components/pointlightcmp.h"
#include "engine/components/spotlightcmp.h"
#include "engine/components/catalogcmp.h"

#include "graphic/gui/button.h"
#include "graphic/gui/canvasgl.h"
#include "graphic/gui/checkbox.h"
#include "graphic/gui/colorpicker.h"
#include "graphic/gui/colorwheel.h"
#include "graphic/gui/combobox.h"
#include "graphic/gui/drawrect.h"
#include "graphic/gui/imagepanel.h"
#include "graphic/gui/imageview.h"
#include "graphic/gui/label.h"
#include "graphic/gui/layout.h"
#include "graphic/gui/messagedialog.h"
#include "graphic/gui/popup.h"
#include "graphic/gui/popupbutton.h"
#include "graphic/gui/progressbar.h"
#include "graphic/gui/screen.h"
#include "graphic/gui/slider.h"
#include "graphic/gui/tabwidget.h"
#include "graphic/gui/textbox.h"
#include "graphic/gui/toolbutton.h"
#include "graphic/gui/vscrollpanel.h"
#include "graphic/gui/widget.h"
#include "graphic/gui/window.h"
#include "graphic/gui/imagelabel.h"

namespace Framework
{
    ObjectFactory::ObjectFactory()
    {
        /*Entities and Components*/
        Register<Framework::Entity>();
        Register<Framework::CameraCmp>();
        Register<Framework::MotionCmp>();
        Register<Framework::TransformCmp>();
        Register<Framework::RenderableCmp>();
        Register<Framework::DirectLightCmp>();
        Register<Framework::PointLightCmp>();
        Register<Framework::SpotLightCmp>();
        Register<Framework::CatalogCmp>();

        /* UIObjects*/
        Register<Framework::GUI::UIButton>();
        Register<Framework::GUI::UICanvasGL>();
        Register<Framework::GUI::UICheckBox>();
        Register<Framework::GUI::UIColorPicker>();
        Register<Framework::GUI::UIColorWheel>();
        Register<Framework::GUI::UIComboBox>();
        Register<Framework::GUI::UIDrawRect>();
        Register<Framework::GUI::UIImagePanel>();
        Register<Framework::GUI::UIImageView>();
        Register<Framework::GUI::UILabel>();
        Register<Framework::GUI::UIMessageDialog>();
        Register<Framework::GUI::UIPopup>();
        Register<Framework::GUI::UIPopupButton>();
        Register<Framework::GUI::UIProgressBar>();
        Register<Framework::GUI::UIScreen>();
        Register<Framework::GUI::UISlider>();
        Register<Framework::GUI::UITabWidget>();
        Register<Framework::GUI::UITextBox>();
        Register<Framework::GUI::UIToolButton>();
        Register<Framework::GUI::UIVScrollPanel>();
        Register<Framework::GUI::UIWidget>();
        Register<Framework::GUI::UIWindow>();
        Register<Framework::GUI::UIImageLabel>();
    }
}
