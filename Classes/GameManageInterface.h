//
//  GameManageInterface.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-8-19.
//
//

#ifndef __Loire__GameManageInterface__
#define __Loire__GameManageInterface__

#include <iostream>
#include <string>
#include "cocos2d.h"

class GameManageInterfaceLayer : public cocos2d::Layer {
public:
    static const int MENUITEM_FONTSIZE = 48;
    virtual bool init();
    
    void skipNextCallback(cocos2d::Ref * pSender);
    void nextTaskCallback(cocos2d::Ref * pSender);
    void shipyardCallback(cocos2d::Ref * pSender);
    void backMainsceneCallback(cocos2d::Ref * pSender);
    std::string get_scriptname_from_config(std::string filename);
    CREATE_FUNC(GameManageInterfaceLayer);
    
    cocos2d::Menu * m_pMenu;
    
};

class GameManageInterfaceScene : public cocos2d::Scene {
public:
    CC_SYNTHESIZE_READONLY(GameManageInterfaceLayer * , _layer, Layer);
    CREATE_FUNC(GameManageInterfaceScene);
    virtual bool init();
    GameManageInterfaceScene() : _layer(NULL) {};
    ~GameManageInterfaceScene();
};

#endif /* defined(__Loire__GameManageInterface__) */
