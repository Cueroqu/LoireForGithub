//
//  Path.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-15.
//
//

#include "Path.h"
#include "cocos2d.h"

USING_NS_CC;

Path::Path () {
    size = 0;
    cocos2d::Texture2D * path_texture = Director::getInstance()->getTextureCache()->addImage("bluedot.png");
    for (int i = 0; i < Path::MAX_POINTS; ++i) {
        auto dot = Sprite::createWithTexture(path_texture);
        dot->retain();
        dot->setScale(0.03125);
        dots[i] = dot;
    }
}

Sprite* Path::addPathDot(const Point &position) {
    dots[size++]->setPosition(position);
    return dots[size-1];
}

void Path::clear_dots() {
    size = 0;
}

bool Path::dotsInSize() {
    return size < Path::MAX_POINTS;
}

Sprite* Path::back() {
    if (size == 0) return dots[0];
    return dots[size-1];
}
