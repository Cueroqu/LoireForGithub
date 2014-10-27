//
//  DialogLayer.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-23.
//
//

#include "DialogLayer.h"
#include "cocos2d.h"
#include "VisibleRect.h"
USING_NS_CC;

DialogLayer::DialogLayer() {
    
}

DialogLayer::~DialogLayer() {

}

bool DialogLayer::init() {
    if (LayerColor::initWithColor(Color4B(0, 0, 0, 125))) {
        this->initiate_dialog();
        return true;
    }
    return false;
}

void DialogLayer::initiate_dialog() {
    this->_label = Label::createWithSystemFont("Are you sure exit?", "", 36);
    _label->setWidth(winSize.width * 2 / 5);
    _label->setPosition(Point(winSize.width / 2, winSize.height / 2));
    this->addChild(_label, 9999);
    
    MenuItemFont *okMenuItem = MenuItemFont::create("OK", CC_CALLBACK_1(DialogLayer::okMenuItemCallback, this));
    okMenuItem->setPosition(Point(-MENUITEM_MARGIN, 0));
    
    MenuItemFont *cancelMenuItem = MenuItemFont::create("Cancel", CC_CALLBACK_1(DialogLayer::cancelMenuItemCallback, this));
    cancelMenuItem->setPosition(Point(MENUITEM_MARGIN, 0));
    
    this->m_pMenu = Menu::create(okMenuItem, cancelMenuItem, NULL);
    this->m_pMenu->setPosition(Point(winSize.width/2, winSize.height/2 - LABEL_MENU_MARGIN));
    this->addChild(m_pMenu, 9999);
    
    onOkMenuItemPressed = nullptr;
    onCancelMenuItemPressed = nullptr;
    
    this->setZOrder(10000);
}

void DialogLayer::okMenuItemCallback(cocos2d::Ref *pSender) {
    if (onOkMenuItemPressed == nullptr) {
        Director::getInstance()->end();
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
    #endif
    } else {
        onOkMenuItemPressed(pSender);
    }
}

void DialogLayer::cancelMenuItemCallback(cocos2d::Ref *pSender) {
    if (onCancelMenuItemPressed == nullptr) {
        this->removeFromParentAndCleanup(false);
    } else {
        onCancelMenuItemPressed(pSender);
    }
}

//void DialogLayer::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) {
//    LayerColor::draw(renderer, transform, flags);
//    _customCommand.init(0);
//    _customCommand.func = CC_CALLBACK_0(DialogLayer::onDraw, this, transform, flags);
//    renderer->addCommand(&_customCommand);
//}
//
//void DialogLayer::onDraw(const cocos2d::Mat4 &transform, uint32_t flags) {
//    Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
//    Director::getInstance()->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
//    
//    float hwidth = MAX(200, _label->getContentSize().width * 0.6);
//    float left = winSize.width / 2 - hwidth;
//    float right = winSize.width / 2 + hwidth;
//    float top = _label->getPositionY() + _label->getContentSize().height / 2 + 50;
//    float bottom = MAX(_label->getPositionY() - _label->getContentSize().height / 2 - 100, 0);
//    Point filledVertices[] = { Point(left,top), Point(right,top), Point(right, bottom), Point(left, bottom)};
//    DrawPrimitives::drawSolidPoly(filledVertices, 4, Color4F(0.5f, 0.5f, 1, 1 ) );
//    
//    Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
//}
