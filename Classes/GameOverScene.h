//
//  GameOverScene.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-29.
//
//

#ifndef __Loire__GameOverScene__
#define __Loire__GameOverScene__

#include <iostream>
#include "cocos2d.h"

class GameOverLayer : public cocos2d::Layer {
public:
    
    static cocos2d::Scene *createScene();
    
    CC_SYNTHESIZE_READONLY(cocos2d::Label *, _label, Label);
    
    CREATE_FUNC(GameOverLayer);
    GameOverLayer() : _label(NULL) {};
    virtual bool init() override;
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) override;
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) override;
};

class GameOverScene : public cocos2d::Scene {
public:
    CC_SYNTHESIZE_READONLY(GameOverLayer *, _layer, Layer);
    CREATE_FUNC(GameOverScene);
    virtual bool init() override;
    
    GameOverScene():_layer(NULL){};
    ~GameOverScene();
};

#endif /* defined(__Loire__GameOverScene__) */
