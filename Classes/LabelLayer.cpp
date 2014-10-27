//
//  LabelLayer.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-22.
//
//

#include "AppDelegate.h"
#include "LabelLayer.h"
#include "cocos2d.h"
#include "VisibleRect.h"
USING_NS_CC;

bool LabelLayer::init() {
    if (Layer::init()) {
        this->label_border_origin =
        Vect(winSize.width / 2 - this->WIDTH / 2 - this->BORDERROWPADDING,
             this->HEIGHT + this->YOFFSET + this->BORDERCOLPADDING);
        
        this->label_border_dest =
        Vect(winSize.width / 2 + this->WIDTH / 2 + this->BORDERROWPADDING,
             YOFFSET - this->BORDERCOLPADDING);
        this->_label = Label::createWithSystemFont("ddd", "Arial", 30);
        this->_label->setPosition(Point(winSize.width / 2, this->HEIGHT / 2 + YOFFSET));
        this->_label->setWidth(this->WIDTH);
        this->_label->setHeight(this->HEIGHT);
        this->_label->setColor(Color3B(255,255,255));
        this->_label->retain();
        this->addChild(_label, 1);
        next_mark.set_triangle(Point(_label->getPositionX() + _label->getContentSize().width / 2 - 30,
                                     _label->getPositionY() - _label->getContentSize().height / 2 + 30), 10, 180);
        
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = CC_CALLBACK_2(LabelLayer::onTouchBegan, this);
        listener->onTouchEnded = CC_CALLBACK_2(LabelLayer::onTouchEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

        return true;
    }
    return false;
}

void LabelLayer::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) {
    Layer::draw(renderer, transform, flags);
    _customCommand.init(1);
    _customCommand.func = CC_CALLBACK_0(LabelLayer::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}

void LabelLayer::onDraw(const cocos2d::Mat4 &transform, uint32_t flags) {
    Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    Director::getInstance()->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
    
    DrawPrimitives::setDrawColor4B(0, 0, 255, 255);
    glLineWidth(6);
    DrawPrimitives::drawRect(this->label_border_origin, this->label_border_dest);
    
    DrawPrimitives::drawSolidPoly(next_mark.points, 3, Color4F(1, 1, 1, 1));
    
    Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

bool LabelLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
    return true;
}

void LabelLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {
    ((AppDelegate *)Application::getInstance())->getChatScene()->start_event();
}
