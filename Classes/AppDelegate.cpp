#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"
#include "NonPlayer.h"
#include "Player.h"
#include "Arsenal/Arsenal.h"
#include "GameManageInterface.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
    if (_mainScene) {
        _mainScene->release();
        _mainScene = NULL;
    }
    if (_chatScene) {
        _chatScene->release();
        _chatScene = NULL;
    }
    if (_shipyardScene) {
        _shipyardScene->release();
        _shipyardScene = NULL;
    }
    if (_gameManageScene) {
        _gameManageScene->release();
        _gameManageScene = nullptr;
    }
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }
    
    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // load arsenal
    Arsenal::load_types();

    // load non player characters;
    NonPlayer::load_npcs();
    Player::getInstance();
    
    // create a scene. it's an autorelease object
    _mainScene = HelloWorldScene::create();
    _chatScene = ChatScene::create();
    _mainScene->retain();
    _chatScene->retain();
    _shipyardScene = ShipyardScene::create();
    _shipyardScene->retain();
    _gameManageScene = GameManageInterfaceScene::create();
    _gameManageScene->retain();
    
    // preload audio effects
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("explosion.caf");
    
    // run
    director->runWithScene(_mainScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void AppDelegate::restartGame(std::string filename) {
    _gameScene = GameScene::create();
    if (this->_gameScene->getLayer()->reset(filename)) {
        Director::getInstance()->replaceScene(this->_gameScene);
    }
}

void AppDelegate::startChat() {
    Director::getInstance()->replaceScene(this->_chatScene);
}

void AppDelegate::launchGame() {
    Director::getInstance()->replaceScene(this->_gameManageScene);
}

void AppDelegate::backMain() {
    Director::getInstance()->replaceScene(this->_mainScene);
}

void AppDelegate::goto_shipyard() {
    this->_shipyardScene->getLayer()->reset();
    Director::getInstance()->replaceScene(this->_shipyardScene);
}
