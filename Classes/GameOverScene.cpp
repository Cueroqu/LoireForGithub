//
//  GameOverScene.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-29.
//
//

#include "AppDelegate.h"
#include "GameOverScene.h"
#include "VisibleRect.h"
#include "SystemEventPool.h"

USING_NS_CC;

Scene* GameOverLayer::createScene() {
    auto scene = Scene::create();
    auto layer = GameOverLayer::create();
    scene->addChild(layer);
    return scene;
}

bool GameOverLayer::init() {
    if (Layer::init()) {
        this->_label = Label::createWithSystemFont("", "Arial", 48);
        this->_label->setPosition(Point(winSize.width / 2, winSize.height / 2));
        this->addChild(_label);
        
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = CC_CALLBACK_2(GameOverLayer::onTouchBegan, this);
        listener->onTouchEnded = CC_CALLBACK_2(GameOverLayer::onTouchEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        return true;
    }
    return false;
}

bool GameOverLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
    return true;
}

void GameOverLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {
    if (!this->_label->getString().compare("Game Over")) {
        this->removeChild(_label);
//        SystemEventPool::getInstance().event_pool.clear();
        if (SystemEventPool::getInstance().current_event != nullptr) {
            SystemEventPool::getInstance().event_pool.push_front(SystemEventPool::getInstance().current_event);
            SystemEventPool::getInstance().current_event = nullptr;
        }
        Director::getInstance()->replaceScene(((AppDelegate *)Application::getInstance())->getGameManageInterfaceScene());
    } else {
        if (SystemEventPool::getInstance().current_event != nullptr) {
            delete SystemEventPool::getInstance().current_event;
            SystemEventPool::getInstance().current_event = nullptr;
        }
        this->removeChild(_label);
        ((AppDelegate *)Application::getInstance())->launchGame();
    }
}

bool GameOverScene::init() {
    if (Scene::init()) {
        this->_layer = GameOverLayer::create();
        this->_layer->retain();
        this->addChild(_layer);
        return true;
    }
    return false;
}

GameOverScene::~GameOverScene() {
    if (_layer) {
        this->removeChild(_layer);
        _layer->release();
        _layer = NULL;
    }
}
