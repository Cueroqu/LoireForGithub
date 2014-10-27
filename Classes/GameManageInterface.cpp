//
//  GameManageInterface.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-8-19.
//
//

#include "GameManageInterface.h"
#include "cocos2d.h"
#include "AppDelegate.h"
#include "SystemEventPool.h"
#include <string>
#include "pugixml/pugixml.hpp"

USING_NS_CC;

bool GameManageInterfaceLayer::init() {
    if (!Layer::init()) {
        return false;
    }
    Size visible_size = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    auto nextTaskItem = MenuItemFont::create("Next Task", CC_CALLBACK_1(GameManageInterfaceLayer::nextTaskCallback, this));
    nextTaskItem->setFontSizeObj(MENUITEM_FONTSIZE);
    nextTaskItem->setPosition(Point(visible_size.width/2, visible_size.height/2+100));
    
    auto shipyardItem = MenuItemFont::create("Shipyard", CC_CALLBACK_1(GameManageInterfaceLayer::shipyardCallback, this));
    shipyardItem->setFontSizeObj(MENUITEM_FONTSIZE);
    shipyardItem->setPosition(Point(visible_size.width/2, visible_size.height/2));
    
    auto backItem = MenuItemFont::create("Back To Main", CC_CALLBACK_1(GameManageInterfaceLayer::backMainsceneCallback, this));
    backItem->setFontSizeObj(MENUITEM_FONTSIZE);
    backItem->setPosition(Point(visible_size.width/2, visible_size.height/2-100));
    
    auto skipNextItem = MenuItemFont::create("Skip Task", CC_CALLBACK_1(GameManageInterfaceLayer::skipNextCallback, this));
    skipNextItem->setFontSizeObj(MENUITEM_FONTSIZE);
    skipNextItem->setPosition(Point(visible_size.width/2, visible_size.height/2-200));
    
    
    m_pMenu = Menu::create(nextTaskItem, shipyardItem, backItem, skipNextItem, nullptr);
    m_pMenu->setPosition(Point::ZERO);
    this->addChild(m_pMenu, 1);
    
    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("mainbg.png");
    // position the sprite on the center of the screen
    sprite->setPosition(Point(winSize.width/2,winSize.height/2));
    this->addChild(sprite, 0);
    
    return true;
}

std::string GameManageInterfaceLayer::get_scriptname_from_config(std::string filename) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = Tools::load_xml_document(filename, doc);
    if (result) {
        return doc.first_child().first_child().attribute("filename").value();
    }
    return "";
}

void GameManageInterfaceLayer::skipNextCallback(cocos2d::Ref * pSender) {
    SystemEventPool::getInstance().skip_event();
    MessageBox("Skip Success", "Information");
}

void GameManageInterfaceLayer::nextTaskCallback(cocos2d::Ref *pSender) {
//    std::string script_name = get_scriptname_from_config("gameconfig.xml");
//    if (script_name.length() > 0) {
//        Tools::load_game_events(script_name);
//    }
    SystemEventPool::getInstance().start_event();
}

void GameManageInterfaceLayer::shipyardCallback(cocos2d::Ref *pSender) {
    ((AppDelegate *)Application::getInstance())->goto_shipyard();
}

void GameManageInterfaceLayer::backMainsceneCallback(cocos2d::Ref *pSender) {
    SystemEventPool::getInstance().clear_event_pool();
    ((AppDelegate *)Application::getInstance())->backMain();
}

bool GameManageInterfaceScene::init() {
    if (Scene::init()) {
        this->_layer = GameManageInterfaceLayer::create();
        this->_layer->retain();
        this->addChild(_layer);
        return true;
    }
    return false;
}

GameManageInterfaceScene::~GameManageInterfaceScene() {
    if (_layer) {
        _layer->release();
        _layer = nullptr;
    }
}
