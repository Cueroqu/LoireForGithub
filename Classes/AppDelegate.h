#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include <string>
#include "cocos2d.h"
#include "HelloWorldScene.h"
#include "GameScene.h"
#include "ChatScene.h"
#include "ShipyardScene.h"
#include "GameManageInterface.h"

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by Director.
*/

#define GET_GAME_SCENE (((AppDelegate *)Application::getInstance())->getGameScene())
#define GET_GAME_LAYER (((AppDelegate *)Application::getInstance())->getGameScene()->getLayer())
#define GET_SHIPYARD_LAER (((AppDelegate *)Application::getInstance())->getShipyardScene()->getLayer())

class  AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    /**
    @brief    Implement Director and Scene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();
    
public:
    void restartGame(std::string filename);
    void startChat();
    void launchGame();
    void backMain();
    void goto_shipyard();
    
    CC_SYNTHESIZE_READONLY(HelloWorldScene *, _mainScene, MainScene);
    CC_SYNTHESIZE_READONLY(GameScene *, _gameScene, GameScene);
    CC_SYNTHESIZE_READONLY(ChatScene *, _chatScene, ChatScene);
    CC_SYNTHESIZE_READONLY(ShipyardScene *, _shipyardScene, ShipyardScene);
    CC_SYNTHESIZE_READONLY(GameManageInterfaceScene *, _gameManageScene, GameManageInterfaceScene);
};

#endif // _APP_DELEGATE_H_

