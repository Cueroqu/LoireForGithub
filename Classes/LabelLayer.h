//
//  LabelLayer.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-22.
//
//

#ifndef __Loire__LabelLayer__
#define __Loire__LabelLayer__

#include <iostream>
#include "cocos2d.h"
#include "VisibleRect.h"
#include "Triangle.h"

class LabelLayer : public cocos2d::Layer {
public:
    const int HEIGHT = 100;
    const int YOFFSET = 20;
    const int BORDERROWPADDING = 10;
    const int BORDERCOLPADDING = 5;
    const int WIDTH = winSize.width - 300;
    cocos2d::Vect label_border_origin;
    cocos2d::Vect label_border_dest;
    Triangle next_mark;
    CC_SYNTHESIZE_READONLY(cocos2d::Label *, _label, Label);
    
    CREATE_FUNC(LabelLayer);
    
    LabelLayer() : _label(NULL) {};
    virtual bool init() override;
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 & transform, uint32_t flags) override;
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) override;
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) override;

protected:
    void onDraw(const cocos2d::Mat4 &transform, uint32_t flags);
    cocos2d::CustomCommand _customCommand;
};

#endif /* defined(__Loire__LabelLayer__) */
