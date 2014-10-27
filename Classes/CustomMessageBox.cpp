//
//  CustomMessageBox.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-8-15.
//
//

#include "CustomMessageBox.h"
#include "VisibleRect.h"
#include "cocos2d.h"
USING_NS_CC;

CustomMessageBox::CustomMessageBox(){}

CustomMessageBox::~CustomMessageBox(){}

bool CustomMessageBox::init() {
    if (LayerColor::initWithColor(Color4B(0,0,0,125))) {
        this->initiate_dialog();
        return true;
    }
    return false;
}

void CustomMessageBox::initiate_dialog() {
    this->_label = Label::createWithSystemFont("OK fdsa fs", "", 36);
    _label->setWidth(winSize.width * 0.4);
    _label->setPosition(Point(winSize.width/2, winSize.height/2));
    this->addChild(_label, 9999);
    
    MenuItemFont * okMenuItem = MenuItemFont::create("OK", CC_CALLBACK_1(CustomMessageBox::okMenuItemCallback, this));
    okMenuItem->setPosition(Point(0, 0));
    
    this->m_pMenu = Menu::create(okMenuItem, NULL);
    this->m_pMenu->setPosition(Point(winSize.width/2, winSize.height/2 - LABEL_MENU_MARGIN));
    this->addChild(m_pMenu, 9999);
    
    onOkMenuItemPressed = nullptr;
    
    this->setZOrder(10000);
}

void CustomMessageBox::onEnter() {
    LayerColor::onEnter();
    
    this->listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(CustomMessageBox::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(CustomMessageBox::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(CustomMessageBox::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(CustomMessageBox::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void CustomMessageBox::onExit() {
    LayerColor::onExit();
    _eventDispatcher->removeEventListener(listener);
}

bool CustomMessageBox::onTouchBegan(Touch * touch, Event * event) {
    m_bTouchedMenu = m_pMenu->onTouchBegan(touch, event);
    return true;
}

void CustomMessageBox::onTouchMoved(Touch * touch, Event * event) {
    if (m_bTouchedMenu) {
        m_pMenu->onTouchMoved(touch, event);
    }
}

void CustomMessageBox::onTouchEnded(Touch * touch, Event * event) {
    if (m_bTouchedMenu) {
        m_pMenu->onTouchEnded(touch, event);
        m_bTouchedMenu = false;
    }
}

void CustomMessageBox::onTouchCancelled(Touch * touch, Event * event) {
    if (m_bTouchedMenu) {
        m_pMenu->onTouchCancelled(touch, event);
        m_bTouchedMenu = false;
    }
}

void CustomMessageBox::okMenuItemCallback(Ref * pSender) {
    if (onOkMenuItemPressed != nullptr) {
        onOkMenuItemPressed(pSender);
    } else {
        this->removeFromParentAndCleanup(false);
    }
}

void CustomMessageBox::draw(Renderer * renderer, const Mat4 & transform, uint32_t flags) {
    LayerColor::draw(renderer, transform, flags);
    _customCommand.init(0);
    _customCommand.func = CC_CALLBACK_0(CustomMessageBox::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}

void CustomMessageBox::setString(const std::string &text) {
    this->_label->setString(text);
    m_pMenu->setPosition(Point(winSize.width/2, winSize.height/2-_label->getContentSize().height/2-LABEL_MENU_MARGIN));
}

void CustomMessageBox::onDraw(const Mat4 & transform, uint32_t flags) {
    Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    Director::getInstance()->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
    
    float hwidth = MAX(300, _label->getContentSize().width / 2);
    float left = winSize.width / 2 - hwidth;
    float right = winSize.width / 2 + hwidth;
    float top = _label->getPositionY() + _label->getContentSize().height / 2 + LABEL_TOP_MARGIN;
    float bottom = MAX(m_pMenu->getPositionY() - 50, 0);
    Point filledVertices[] = { Point(left, top), Point(right, top), Point(right, bottom), Point(left, bottom)};
    DrawPrimitives::drawSolidPoly(filledVertices, 4, Color4F(0.5f, 0.5f, 1., 1.));
    
    Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}
