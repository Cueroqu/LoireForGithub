//
//  DialogLayer.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-23.
//
//

#ifndef __Loire__DialogLayer__
#define __Loire__DialogLayer__

#include <iostream>
#include "cocos2d.h"
#include "CustomMessageBox.h"
USING_NS_CC;

class DialogLayer : public CustomMessageBox {
public:
    static const int MENUITEM_MARGIN = 50;
    
    DialogLayer();
    ~DialogLayer();
    
    virtual bool init();
    void initiate_dialog();
    CREATE_FUNC(DialogLayer);
    
//    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 & transform, uint32_t flags) override;
    
    virtual void okMenuItemCallback(Ref *pSender);
    virtual void cancelMenuItemCallback(Ref *pSender);
    
    std::function<void(Ref *pSender)> onOkMenuItemPressed;
    std::function<void(Ref *pSender)> onCancelMenuItemPressed;
    
//protected:
//    void onDraw(const cocos2d::Mat4 &transform, uint32_t flags);
};

#endif /* defined(__Loire__DialogLayer__) */
