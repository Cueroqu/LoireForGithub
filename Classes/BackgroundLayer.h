//
//  BackgroundLayer.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-8-5.
//
//

#ifndef __Loire__BackgroundLayer__
#define __Loire__BackgroundLayer__

#include <iostream>
#include "cocos2d.h"

class BackgroundLayer : public cocos2d::Layer {
public:
    BackgroundLayer(){};
    ~BackgroundLayer(){};
    CREATE_FUNC(BackgroundLayer);
    void initWithFile(const std::string & filename);
    void schedule_map(float t);
    void schedule_row(float t);
    void schedule_col(float t);
    cocos2d::Sprite * bgs[9];
    cocos2d::Texture2D * texture;
    float width;
    float half_width;
    float height;
    float half_height;
    constexpr static float tolerance = 10;
};

#endif /* defined(__Loire__BackgroundLayer__) */
