//
//  CustomMessageBox.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-8-15.
//
//

#ifndef __Loire__CustomMessageBox__
#define __Loire__CustomMessageBox__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

class CustomMessageBox : public LayerColor {
public:
    static constexpr float LABEL_MENU_MARGIN = 100;
    static constexpr float LABEL_BOTTOM_MARGIN = 120;
    static constexpr float LABEL_TOP_MARGIN = 50;
    
    CustomMessageBox();
    ~CustomMessageBox();
    
    virtual bool init();
    void initiate_dialog();
    CREATE_FUNC(CustomMessageBox);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void draw(cocos2d::Renderer * renderer, const cocos2d::Mat4 & transform, uint32_t flags) override;
    
    virtual bool onTouchBegan(Touch * touch, Event * event) override;
    virtual void onTouchMoved(Touch * touch, Event * event) override;
    virtual void onTouchEnded(Touch * touch, Event * event) override;
    virtual void onTouchCancelled(Touch * touch, Event * event) override;
    virtual void okMenuItemCallback(Ref * pSender);
    virtual void setString(const std::string & text);
    
    std::function<void(Ref * pSender)> onOkMenuItemPressed;
protected:
    Label * _label;
    Menu * m_pMenu;
    bool m_bTouchedMenu;
    virtual void onDraw(const cocos2d::Mat4 & transform, uint32_t flags);
    CustomCommand _customCommand;
    EventListenerTouchOneByOne * listener;
};

#endif /* defined(__Loire__CustomMessageBox__) */
