#include "HelloWorldScene.h"
#include "AppDelegate.h"
#include "Player.h"
#include "DialogLayer.h"
#include "pugixml/pugixml.hpp"
#include "Tools.h"
#include "SystemEvent.h"
#include "SystemEventPool.h"
#include "CustomMessageBox.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
//    auto closeItem = MenuItemImage::create(
//                                           "CloseNormal.png",
//                                           "CloseSelected.png",
//                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
//    
//	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
//                                origin.y + closeItem->getContentSize().height/2));
//    
//    
//    auto startItem = MenuItemImage::create("start_unselected.png",
//                                           "start_selected.png",
//                                           CC_CALLBACK_1(HelloWorld::startMarchCallback, this));
//    startItem->setPosition(Point(winSize.width/2, winSize.height/2));
//    
//    auto shipyardItem = MenuItemImage::create(
//                                              "CloseNormal.png",
//                                              "CloseSelected.png",
//                                              CC_CALLBACK_1(HelloWorld::shipyardCallback, this));
//    shipyardItem->setPosition(Point(origin.x + visibleSize.width - shipyardItem->getContentSize().width / 2 - 100, origin.y + shipyardItem->getContentSize().height / 2));
    
    auto startItem = MenuItemFont::create("Launch", CC_CALLBACK_1(HelloWorld::startMarchCallback, this));
    startItem->setFontSizeObj(MENUITEM_FONTSIZE);
    startItem->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 + 50));
    
    auto closeItem = MenuItemFont::create("Quit Game", CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    closeItem->setFontSizeObj(MENUITEM_FONTSIZE);
    closeItem->setPosition(Point(visibleSize.width/2, visibleSize.height/2 - 50));
    
//
//    auto chatItem = MenuItemFont::create("Chat", CC_CALLBACK_1(HelloWorld::chatItemCallback, this));
//    chatItem->setFontSizeObj(24);
//    chatItem->setFontName("Helvetica");
//    chatItem->setPosition(startItem->getPosition() + Point(0, 100));
//    
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, startItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);


    /////////////////////////////
    // 3. add your codes below...
    
    // add a label shows "Hello World"
    // create and initialize a label
    
//    auto label = LabelTTF::create("Hello World", "Arial", 24);
//    
//    // position the label on the center of the screen
//    label->setPosition(Point(origin.x + visibleSize.width/2,
//                            origin.y + visibleSize.height - label->getContentSize().height));
//
//    // add the label as a child to this layer
//    this->addChild(label, 1);
//
//    // add "HelloWorld" splash screen"
//    auto sprite = Sprite::create("HelloWorld.png");
//
//    // position the sprite on the center of the screen
//    sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
//
//    // add the sprite as a child to this layer
//    this->addChild(sprite, 0);
    
    return true;
}

void HelloWorld::startMarchCallback(cocos2d::Ref *pSender) {
    std::string script_name = get_scriptname_from_config("gameconfig.xml");
    if (script_name.length() > 0) {
        Tools::load_game_events(script_name);
    }
    ((AppDelegate *)Application::getInstance())->launchGame();
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    DialogLayer *dialogLayer = DialogLayer::create();
    this->addChild(dialogLayer);
}

void HelloWorld::startGameCallback(Ref *pSender) {
    ((AppDelegate *)Application::getInstance())->restartGame("Game1Scene.xml");
}

void HelloWorld::chatItemCallback(cocos2d::Ref *pSender) {
    ((AppDelegate *)Application::getInstance())->startChat();
}

//void HelloWorld::newGameCallback(cocos2d::Ref *pSender) {
//    std::string script_name = get_scriptname_from_config("gameconfig.xml");
//    if (script_name.length() > 0) {
//        Tools::load_game_events(script_name);
//    }
//}

void HelloWorld::shipyardCallback(cocos2d::Ref *pSender) {
    ((AppDelegate *)Application::getInstance())->goto_shipyard();
}

std::string HelloWorld::get_scriptname_from_config(std::string filename) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = Tools::load_xml_document(filename, doc);
    if (result) {
        return doc.first_child().first_child().attribute("filename").value();
    }
    return "";
}

bool HelloWorldScene::init() {
    if (Scene::init()) {
        this->_layer = HelloWorld::create();
        this->_layer->retain();
        this->addChild(_layer);
        return true;
    }
    
    return false;
}

HelloWorldScene::~HelloWorldScene() {
    if (_layer) {
        _layer->release();
        _layer = NULL;
    }
    Player::Release();
}
