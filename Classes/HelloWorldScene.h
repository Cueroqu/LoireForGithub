#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include <iostream>
#include <string>
#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static const int MENUITEM_FONTSIZE = 48;
    
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void startMarchCallback(cocos2d::Ref* pSender);
    void menuCloseCallback(cocos2d::Ref* pSender);
    void startGameCallback(cocos2d::Ref* pSender);
    void chatItemCallback(cocos2d::Ref* pSender);
    void shipyardCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    std::string get_scriptname_from_config(std::string filename);
};

class HelloWorldScene : public cocos2d::Scene
{
public:
    HelloWorldScene():_layer(NULL) {};
    ~HelloWorldScene();
    bool init();
    CREATE_FUNC(HelloWorldScene);
    CC_SYNTHESIZE_READONLY(HelloWorld *, _layer, Layer);
};

#endif // __HELLOWORLD_SCENE_H__
